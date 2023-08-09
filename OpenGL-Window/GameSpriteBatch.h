#pragma once
#if defined(GAME_OPENGL)
#include <GL/gl.h>
#endif
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif
#if defined(GAME_DIRECTX10)
#include <d3d10.h>
#endif

#include "GameEngine.h"
#include "GameErrors.h"
#include "GameMath.h"
#include "GameSpriteFont.h"
#include "GameTexture2D.h"

namespace game
{
	extern GameError lastError;
	extern Engine* enginePointer;

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		bool Initialize();
		void Begin();
		void Render();
		// Will draw entire texture to location x,y
		void Draw(const Texture2D &texture, const uint32_t x, const uint32_t y, const Color color);
		void Draw(const Texture2D& texture, const Pointi& position, const Color color);
		// Will draw a specified rectangle portion of a texture to location x,y
		void Draw(const Texture2D& texture, const Recti& destination, const Recti& source, const Color& color);
		void DrawString(const SpriteFont &font, const std::string &Str, const int x, const int y, const Color& color);
		void End();
	private:
		static constexpr uint32_t _maxSprites = 200;
		uint32_t _numberOfSpritesUsed;
		Texture2D _currentTexture;
		void _Enable2D();
		void _Disable2D();
#if defined(GAME_OPENGL) | defined(GAME_DIRECTX9) //| defined(GAME_DIRECTX10)
		struct _spriteVertex
		{
			float_t x, y, z, rhw;
			uint32_t color;
			float_t u, v;
		};
		_spriteVertex* _spriteVertices;
#endif
#if defined(GAME_DIRECTX9)
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
		DWORD _savedFVF;
		DWORD _savedBlending;
		IDirect3DBaseTexture9* _savedTexture;
#endif
#if defined (GAME_DIRECTX10)
		struct _spriteVertex
		{
			float_t x, y, z;
			float_t r, g, b, a;
			float_t u, v;
		};
		_spriteVertex* _spriteVertices;
		ID3D10Buffer* _vertexBuffer10;
		Shader _spriteBatchShader;
		ID3D10InputLayout* _vertexLayout10;
		ID3D10SamplerState* _textureSamplerState10;
		ID3D10Buffer* _indexBuffer;
		// not sure on how to handle the textures
		ID3D10ShaderResourceView* _currentTextureResourceView;

		// saves state of dx10
		uint32_t _oldStride = 0;
		uint32_t _oldOffset = 0;
		ID3D10Buffer* _oldVertexBuffer = nullptr;
		ID3D10Buffer* _oldIndexBuffer = nullptr;
		DXGI_FORMAT _oldIndexFormat = {};
		uint32_t _oldIndexOffset = 0;
		ID3D10InputLayout* _oldInputLayout = nullptr;
		ID3D10VertexShader* _oldVertexShader = nullptr;
		ID3D10PixelShader* _oldPixelShader = nullptr;
		ID3D10SamplerState* _oldTextureSamplerState = nullptr;
		D3D10_PRIMITIVE_TOPOLOGY _oldPrimitiveTopology = {};
#endif
#if defined (GAME_DIRECTX11)
#endif
	};

	// Notes for gs for dx10
	// not using shaders, just index buffers, will be 6 floats to define a vertex (x,y,z) (c) (u,v)
	// and 4 vertices per sprite, total of 24 floats
	// for using shaders, position (x,y,z), color(r,g,b,a), width(w), height(h), top left uv (u,v), bottom right uv(u,v) per
	// sprite, total of 13 floats per sprite.
	// In old spritebatch I had a max of 32,768 sprites, no shaders that is 3.1 million bytes (3.072MB)
	// Using GS, 32,768 sprites will use 1.7 million bytes (1.664MB)
	// So about half the data, so maybe 2x the sprite limit? x4? Maybe



	inline SpriteBatch::SpriteBatch()
	{
#if defined(GAME_OPENGL)
		//if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_spriteVertices = nullptr;
		}
#endif
#if defined(GAME_DIRECTX9)  
		//if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_spriteVertices = nullptr;

			_vertexBuffer = nullptr;
			_savedFVF = 0;
			_savedBlending = 0;
			_savedTexture = nullptr;
		}
#endif
#if defined (GAME_DIRECTX10)
		//if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			_spriteVertices = nullptr;
			_indexBuffer = nullptr;
			_vertexBuffer10 = nullptr;
			_vertexLayout10 = nullptr;
			_textureSamplerState10 = nullptr;
			_currentTextureResourceView = nullptr;
		}
#endif
#if defined (GAME_DIRECTX11)
#endif

		_numberOfSpritesUsed = 0;
	}

	inline SpriteBatch::~SpriteBatch()
	{
		if (_spriteVertices)
		{
			delete[] _spriteVertices;
			_spriteVertices = nullptr;
		}
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			if (_spriteVertices)
			{
				delete[] _spriteVertices;
				_spriteVertices = nullptr;
			}
		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (_vertexBuffer)
			{
				_vertexBuffer->Release();
				_vertexBuffer = nullptr;
			}
			if (_spriteVertices)
			{
				delete[] _spriteVertices;
				_spriteVertices = nullptr;
			}
		}
#endif
#if defined (GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			SAFE_RELEASE(_vertexBuffer10);
			SAFE_RELEASE(_vertexLayout10);
			SAFE_RELEASE(_textureSamplerState10);
			SAFE_RELEASE(_currentTextureResourceView);
			SAFE_RELEASE(_indexBuffer);
			enginePointer->geUnLoadShader(_spriteBatchShader);
		}
#endif
#if defined (GAME_DIRECTX11)

#endif
	}

	inline bool SpriteBatch::Initialize()
	{
		// OpenGL and DX9 implementation of vertices
#if defined(GAME_OPENGL) || defined (GAME_DIRECTX9) || defined(GAME_DIRECTX10)
		_spriteVertices = new _spriteVertex[_maxSprites * 6];
		for (uint32_t vertex = 0; vertex < _maxSprites * 6; vertex++)
		{
			_spriteVertices[vertex].x = 0.0f;
			_spriteVertices[vertex].y = 0.0f;
			_spriteVertices[vertex].z = 0.0f;
#if defined(GAME_OPENGL) || defined(GAME_DIRECTX9)
			_spriteVertices[vertex].rhw = 1.0f;
#endif
			_spriteVertices[vertex].u = 0.0f;
			_spriteVertices[vertex].v = 0.0f;
#if defined(GAME_DIRECTX9)
			if (enginePointer->geIsUsing(GAME_DIRECTX9))
			{
				_spriteVertices[vertex].color = Colors::White.packedARGB;
			}
#endif
#if defined(GAME_DIRECTX10)
			if (enginePointer->geIsUsing(GAME_DIRECTX10))
			{
				_spriteVertices[vertex].r = Colors::White.rf;
				_spriteVertices[vertex].g = Colors::White.gf;
				_spriteVertices[vertex].b = Colors::White.bf;
				_spriteVertices[vertex].a = Colors::White.af;

			}
#endif
#if defined(GAME_OPENGL)
			if (enginePointer->geIsUsing(GAME_OPENGL))
			{
				_spriteVertices[vertex].color = Colors::White.packedABGR;
			}
#endif
		}
#endif

		// DX10 impementation of vertices
#if defined(GAME_DIRECTX10)

#endif

		// Initialization of API methods used
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{

		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			enginePointer->d3d9Device->CreateVertexBuffer(_maxSprites * (uint32_t)6 * (uint32_t)sizeof(_spriteVertex), 0, (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), D3DPOOL_MANAGED, &_vertexBuffer, NULL);
			if (_vertexBuffer == nullptr)
			{
				lastError = { GameErrors::GameDirectX9Specific, "Could not create vertex buffer for SpriteBatch." };
				return false;
			}
		}
#endif
#if defined (GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			D3D10_BUFFER_DESC vertexBufferDescription = { 0 };
			D3D10_BUFFER_DESC indexBufferDescription = { 0 }; 
			D3D10_SUBRESOURCE_DATA vertexInitialData = { 0 };
			D3D10_SUBRESOURCE_DATA indexInitialData = { 0 };  
			//DWORD indices[] = { 0, 1, 2, 1, 3, 2, }; 

			D3D10_INPUT_ELEMENT_DESC inputLayout[] = // seems wrong
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
			};
			D3D10_SAMPLER_DESC samplerDesc = { };

			// Load shaders for spriteBatch
			if (!enginePointer->geLoadShader("Content/VertexShader.hlsl", "Content/PixelShader.hlsl", _spriteBatchShader))
			{
				return false;
			}

			// Create the vertex buffer
			vertexBufferDescription.ByteWidth = _maxSprites * (uint32_t)6 * (uint32_t)sizeof(_spriteVertex);
			//std::cout << "SpriteBatch VertexBuffer size : " << sizeof(_spriteVertex) * _maxSprites / 1024 << "kB\n";
			vertexBufferDescription.Usage = D3D10_USAGE_DYNAMIC;
			vertexBufferDescription.BindFlags = D3D10_BIND_VERTEX_BUFFER;
			vertexBufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
			vertexBufferDescription.MiscFlags = 0;
			if (FAILED(enginePointer->d3d10Device->CreateBuffer(&vertexBufferDescription, NULL, &_vertexBuffer10)))
			{
				lastError = { GameErrors::GameDirectX10Specific, "Could not create vertex buffer for SpriteBatch." };
				return false;
			}

			// Create index buffer
			//  0, 1, 2, 1, 3, 2
			std::vector<DWORD> indices;
			for (uint32_t index = 0; index < _maxSprites; index += 1)
			{
				indices.push_back(0 + (index * 6));
				indices.push_back(1 + (index * 6));
				indices.push_back(2 + (index * 6));
				indices.push_back(1 + (index * 6));
				indices.push_back(3 + (index * 6));
				indices.push_back(2 + (index * 6));
			}
			indexBufferDescription.Usage = D3D10_USAGE_IMMUTABLE;
			indexBufferDescription.ByteWidth = sizeof(DWORD) * 6 * _maxSprites;
			indexBufferDescription.BindFlags = D3D10_BIND_INDEX_BUFFER;
			indexBufferDescription.CPUAccessFlags = 0;
			indexBufferDescription.MiscFlags = 0;
			indexInitialData.pSysMem = indices.data();
			if (FAILED(enginePointer->d3d10Device->CreateBuffer(&indexBufferDescription, &indexInitialData, &_indexBuffer)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create index buffer for SpriteBatch." };
				_vertexBuffer10->Release();
				_vertexBuffer10 = nullptr;
				enginePointer->geUnLoadShader(_spriteBatchShader);
				return false;
			}


			// Create input layout for shaders
			if (FAILED(enginePointer->d3d10Device->CreateInputLayout(inputLayout, 3, _spriteBatchShader.compiledVertexShader10->GetBufferPointer(), _spriteBatchShader.compiledVertexShader10->GetBufferSize(), &_vertexLayout10)))
			{
				lastError = { GameErrors::GameDirectX10Specific, "Could not create input layout for SpriteBatch." };
				return false;
			}

			// Create texture sampler 
			samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;
			if (FAILED(enginePointer->d3d10Device->CreateSamplerState(&samplerDesc, &_textureSamplerState10)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create sampler state for SpriteBatch." };
				return false;
			}

			// shader resource view, need one for each texture??
			//D3D10_SHADER_RESOURCE_VIEW_DESC srDesc = {};
			//srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
			//srDesc.Texture2D.MostDetailedMip = 0;
			//srDesc.Texture2D.MipLevels = 1;
			//if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(_frameBuffer[0].textureInterface10, &srDesc, &_currentTextureResourceView)))
			//{
			//	std::cout << "CreateSRV0 failed!\n";
			//}
		}
#endif
#if defined (GAME_DIRECTX11)
#endif 
		return true;
	}

	inline void SpriteBatch::Begin()
	{
		// needs to save and retore ALL changed states
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			// Save current state
			enginePointer->d3d9Device->GetFVF(&_savedFVF);
			enginePointer->d3d9Device->GetTexture(0, &_savedTexture);
			enginePointer->d3d9Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &_savedBlending);

			// Disable multisampling if enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
			}

			enginePointer->d3d9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			enginePointer->d3d9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			uint32_t stride = sizeof(_spriteVertex);
			uint32_t offset = 0;

			// need to save blend state check Render

			// Save everything we modify
			enginePointer->d3d10Device->IAGetIndexBuffer(&_oldIndexBuffer, &_oldIndexFormat, &_oldIndexOffset);
			enginePointer->d3d10Device->IAGetVertexBuffers(0, 1, &_oldVertexBuffer, &_oldStride, &_oldOffset);
			enginePointer->d3d10Device->IAGetInputLayout(&_oldInputLayout);
			enginePointer->d3d10Device->VSGetShader(&_oldVertexShader);
			enginePointer->d3d10Device->PSGetShader(&_oldPixelShader);
			enginePointer->d3d10Device->PSGetSamplers(0, 1, &_oldTextureSamplerState);
			enginePointer->d3d10Device->IAGetPrimitiveTopology(&_oldPrimitiveTopology);

			// Change what we need
			enginePointer->d3d10Device->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			enginePointer->d3d10Device->IASetVertexBuffers(0, 1, &_vertexBuffer10, &stride, &offset);
			enginePointer->d3d10Device->IASetInputLayout(_vertexLayout10);
			enginePointer->d3d10Device->VSSetShader(_spriteBatchShader.vertexShader10);
			enginePointer->d3d10Device->PSSetShader(_spriteBatchShader.pixelShader10);
			enginePointer->d3d10Device->PSSetSamplers(0, 1, &_textureSamplerState10);
			enginePointer->d3d10Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			enginePointer->d3d10Device->PSSetShaderResources(0, 1, &_currentTextureResourceView);


		// Disable multisampling
		// not now
		}
#endif
#if defined(GAME_DIRECTX11)
#endif


#if defined (GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			// Save states? thist
			glEnable(GL_TEXTURE_2D);
			// and texture bound to it
		}
#endif

	}

	inline void SpriteBatch::End()
	{
		if (_numberOfSpritesUsed > 0)
		{
			Render();
		}


#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{

			// Restore previous state
			enginePointer->d3d9Device->SetFVF(_savedFVF);
			enginePointer->d3d9Device->SetTexture(0, _savedTexture);
			enginePointer->d3d9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, _savedBlending);
			if (_savedTexture)
			{
				_savedTexture->Release();
				_savedTexture = nullptr;
			}

			// Renable multisampling if it was enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			}
		}
#endif
#if defined (GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			// restore everything
			enginePointer->d3d10Device->IASetIndexBuffer(_oldIndexBuffer, _oldIndexFormat, _oldIndexOffset);
			enginePointer->d3d10Device->IASetVertexBuffers(0, 1, &_oldVertexBuffer, &_oldStride, &_oldOffset);
			enginePointer->d3d10Device->IASetInputLayout(_oldInputLayout);
			enginePointer->d3d10Device->VSSetShader(_oldVertexShader);
			enginePointer->d3d10Device->PSSetShader(_oldPixelShader);
			enginePointer->d3d10Device->PSSetSamplers(0, 1, &_oldTextureSamplerState);
			enginePointer->d3d10Device->IASetPrimitiveTopology(_oldPrimitiveTopology);
		}
#endif
#if defined (GAME_DIRECTX11)
#endif
#if defined (GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			// restore saved this stuff
			glBindTexture(GL_TEXTURE_2D,0);
			glDisable(GL_TEXTURE_2D);
		}
#endif
	}

	inline void SpriteBatch::Render()
	{
		if (_numberOfSpritesUsed == 0)
		{
			return;
		}

#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			VOID* pVoid = nullptr;


			// Send sprite vertices to gpu
			_vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
			memcpy(pVoid, &_spriteVertices[0], sizeof(_spriteVertex) * 6 * _maxSprites);
			_vertexBuffer->Unlock();

			// Draw the sprites
			enginePointer->d3d9Device->SetTexture(0, _currentTexture.textureInterface9);
			enginePointer->d3d9Device->SetFVF((D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
			enginePointer->d3d9Device->SetStreamSource(0, _vertexBuffer, 0, sizeof(_spriteVertex));
			enginePointer->d3d9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _numberOfSpritesUsed * 2);
		}
#endif
#if defined (GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			// Send vertices to card
			VOID* pVoid = nullptr;
			if (FAILED(_vertexBuffer10->Map(D3D10_MAP_WRITE_DISCARD, 0, &pVoid)))
			{
				std::cout << "Could not map vertex buffer\n";
			}
			else
			{
				memcpy(pVoid, &_spriteVertices[0], sizeof(_spriteVertex) * 6 * _numberOfSpritesUsed);
				_vertexBuffer10->Unmap();
			}

			D3D10_BLEND_DESC BlendStateDesc;
			ZeroMemory(&BlendStateDesc, sizeof(D3D10_BLEND_DESC));
			BlendStateDesc.AlphaToCoverageEnable = FALSE;
			BlendStateDesc.BlendEnable[0] = TRUE;
			BlendStateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
			BlendStateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
			BlendStateDesc.BlendOp = D3D10_BLEND_OP_ADD;
			BlendStateDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
			BlendStateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
			BlendStateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
			BlendStateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

			ID3D10BlendState* pBlendState = NULL;
			HRESULT hr = enginePointer->d3d10Device->CreateBlendState(&BlendStateDesc, &pBlendState);
			float b[] = {1.0f, 1.0f, 1.0f, 1.0f};
			enginePointer->d3d10Device->OMSetBlendState(pBlendState, b, 0xffffffff);

			enginePointer->d3d10Device->DrawIndexed(_numberOfSpritesUsed * 6, 0, 0);

			// need to save whatever was there
			enginePointer->d3d10Device->OMSetBlendState(NULL, b, 0xffffffff);
			SAFE_RELEASE(pBlendState);


		}
#endif
#if defined (GAME_DIRECTX11)
#endif

#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			float_t r, g, b, a = 0.0f;
			Vector2i windowSize = enginePointer->geGetWindowSize();
			
			glBindTexture(GL_TEXTURE_2D, _currentTexture.bind);
			
			_spriteVertex *access = &_spriteVertices[0];

			float pixelOffsetFixX = 1.0f + (1.0f / windowSize.width);
			float pixelOffsetFixY = 1.0f + (1.0f / windowSize.height);
			
			glBegin(GL_QUADS);


			for (uint32_t i = 0; i < _numberOfSpritesUsed; i++, access++)
			{
				r = (access->color & 0xff) / 255.0f;
				g = (access->color >> 8 & 0xff) / 255.0f;
				b = (access->color >> 16 & 0xff) / 255.0f;
				a = (access->color >> 24 & 0xff) / 255.0f;

				// Bottom left
				glTexCoord2f(access->u, 1.0f-access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - pixelOffsetFixX;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - pixelOffsetFixY;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				access++;

				// Bottom right
				glTexCoord2f(access->u, 1.0f - access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - pixelOffsetFixX;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - pixelOffsetFixY;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				access++;

				// Top right
				glTexCoord2f(access->u, 1.0f - access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - pixelOffsetFixX;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - pixelOffsetFixY;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				access++;

				// Bottom left
				glTexCoord2f(access->u, 1.0f - access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - pixelOffsetFixX;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - pixelOffsetFixY;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				//access++;
			}


			glEnd();
			//glBindTexture(GL_TEXTURE_2D, 0);

		}
#endif
		_numberOfSpritesUsed = 0;
	}

	inline void SpriteBatch::Draw(const Texture2D& texture, const Pointi& position, const Color color)
	{
		Draw(texture, position.x, position.y, color);
	}

	inline void SpriteBatch::Draw(const Texture2D& texture, const uint32_t x, const uint32_t y, const Color color)
	{
		if (_numberOfSpritesUsed + 1 > _maxSprites)
		{
			Render();
		}

#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			if (texture.bind != _currentTexture.bind)
			{
				Render();
				_currentTexture = texture;
			}
			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 4];

			// bl
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
			access->color = color.packedABGR;
			access++;

			// br
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 1.0f;
			access->v = 1.0f;
			access->color = color.packedABGR;
			access++;

			// tr
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access->color = color.packedABGR;
			access++;

			// tl
			access->x = (float_t)x;
			access->y = (float_t)y;
			access->u = 0.0f;
			access->v = 0.0f;
			access->color = color.packedABGR;
			access++;

		}
#endif

#if defined(GAME_DIRECTX9)

		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (texture.textureInterface9 != _currentTexture.textureInterface9)
			{
				Render();
				_currentTexture = texture;
			}
			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 6];
			// Top left
			access->x = (float_t)x;
			access->y = (float_t)y;
			access->u = 0.0f;
			access->v = 0.0f;
			access->color = color.packedARGB;
			access++;

			// Top right
			access->x = (float_t)x+(float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access->color = color.packedARGB;
			access++;

			// Bottom left
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
			access->color = color.packedARGB;
			access++;

			// Top right
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access->color = color.packedARGB;
			access++;

			// Bottom right
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 1.0f;
			access->v = 1.0f;
			access->color = color.packedARGB;
			access++;

			// Bottom left
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
			access->color = color.packedARGB;
			access++;
		}
#endif
#if defined (GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			_spriteVertex* access = nullptr;
			Vector2i window;
			Rectf scaledpos;

			// If texture changed, render and change texture/SRV
			if (texture.textureInterface10 != _currentTexture.textureInterface10)
			{
				Render();
				_currentTexture = texture;
				// Change shader texture to new one
				//enginePointer->d3d10Device->PSSetShaderResources(0, 1, &_textureShaderResourceView0);
				D3D10_SHADER_RESOURCE_VIEW_DESC srDesc = {};
				srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
				srDesc.Texture2D.MostDetailedMip = 0;
				srDesc.Texture2D.MipLevels = 1;
				if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(texture.textureInterface10, &srDesc, &_currentTextureResourceView)))
				{
					std::cout << "CreateSRV spritebatch failed!\n";
				}
				//enginePointer->d3d10Device->PSSetShaderResources(0, 1, &_currentTextureResourceView);
			}

			access = &_spriteVertices[_numberOfSpritesUsed * 6];
			window = enginePointer->geGetWindowSize();
			// Homogenise coordinates to -1.0f to 1.0f
			scaledpos.left = ((float_t)x * 2.0f / (float_t)window.width) - 1.0f;
			scaledpos.top = ((float_t)y * 2.0f / (float_t)window.height) - 1.0f;
			scaledpos.right = (((float_t)x + (float_t)texture.width) * 2.0f / (float)window.width) - 1.0f;
			scaledpos.bottom = (((float_t)y + (float_t)texture.height) * 2.0f / (float)window.height) - 1.0f;
			// Flip the y axis
			scaledpos.top = -scaledpos.top;
			scaledpos.bottom = -scaledpos.bottom;
			
			// Fill vertices

			// Top left
			access->x = scaledpos.left;
			access->y = scaledpos.top;
			access->u = 0.0f;
			access->v = 0.0f;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;

			access++;

			// Top right
			access->x = scaledpos.right;
			access->y = scaledpos.top;
			access->u = 1.0f;
			access->v = 0.0f;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;
			access++;

			// Bottom left
			access->x = scaledpos.left;
			access->y = scaledpos.bottom;
			access->u = 0.0f;
			access->v = 1.0f;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;
			access++;

			// Bottom right
			access->x = scaledpos.right;
			access->y = scaledpos.bottom;
			access->u = 1.0f;
			access->v = 1.0f;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;
			access++;
		}
#endif
#if defined (GAME_DIRECTX11)
#endif

		_numberOfSpritesUsed++;
	}

	inline void SpriteBatch::Draw(const Texture2D& texture, const Recti& destination, const Recti& portion, const Color& color)
	{
		if (_numberOfSpritesUsed + 1 > _maxSprites)
		{
			Render();
		}
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			Vector2i windowSize = enginePointer->geGetWindowSize();
			if (texture.bind != _currentTexture.bind)
			{
				Render();
				_currentTexture = texture;
			}
			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 4];

			// Bottom left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = color.packedABGR;
			access++;

			// Bottom right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = color.packedABGR;
			access++;

			// Top right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = color.packedABGR;
			access++;

			// Top left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = color.packedABGR;
			access++;
		}
#endif


#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (texture.textureInterface9 != _currentTexture.textureInterface9)
			{
				Render();
				_currentTexture = texture;
			}

			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 6];
			// Top left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = color.packedARGB;
			access++;

			// Top right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;// 1.0f;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = color.packedARGB;
			access++;

			// Bottom left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = color.packedARGB;
			access++;

			// Top right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;// 1.0f;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = color.packedARGB;
			access++;

			// Bottom right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = color.packedARGB;
			access++;

			// Bottom left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = color.packedARGB;

		}
#endif
#if defined (GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
		}
#endif
#if defined (GAME_DIRECTX11)
#endif

		_numberOfSpritesUsed++;
	}
	
	void SpriteBatch::DrawString(const SpriteFont &font, const std::string& Str, const int x, const int y, const Color& color)
	{
		int CurX = x;
		int CurY = y;
		int Width, Height;
		Recti src, dest;
		short ch;

		for (unsigned int i = 0; i < Str.size(); ++i)
		{
			ch = Str[i];
			Width = font._characterSet.chars[ch].width;
			Height = font._characterSet.chars[ch].height;

			src.left = font._characterSet.chars[ch].x;
			src.top = font._characterSet.chars[ch].y;
			src.right = src.left + Width;
			src.bottom = (src.top + Height);

			dest.left = CurX + font._characterSet.chars[ch].xOffset;
			dest.top = CurY + font._characterSet.chars[ch].yOffset;
			dest.right = Width + dest.left;
			dest.bottom = Height + dest.top;

			Draw(font.Texture(), dest, src, color);

			CurX += font._characterSet.chars[ch].xAdvance;
	}
}

	inline void SpriteBatch::_Enable2D()
	{
#if defined(GAME_OPENGL)
#endif
#if defined (GAME_DIRECTX9)
#endif
#if defined (GAME_DIRECTX10)
#endif
#if defined (GAME_DIRECTX11)
#endif
	}

	inline void SpriteBatch::_Disable2D()
	{
#if defined (GAME_OPENGL)
#endif
#if defined(GAME_DIRECTX9)
#endif
#if defined (GAME_DIRECTX10)
#endif
#if defined (GAME_DIRECTX11)
#endif
	}
}