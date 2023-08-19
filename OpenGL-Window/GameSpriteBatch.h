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

#include <unordered_map>
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
		// Sets up states for sprite rendering and saves old states
		void Begin();
		// Cleans up sprite rendering and restores previous states
		void End();
		// Render a complete batch of sprites
		void Render();
		// Will draw entire texture to location x,y
		void Draw(const Texture2D &texture, const uint32_t x, const uint32_t y, const Color color);
		void Draw(const Texture2D& texture, const Pointi& position, const Color color);
		// Will draw a specified rectangle portion of a texture to location x,y
		void Draw(const Texture2D& texture, const Recti& destination, const Recti& source, const Color& color);
		void DrawString(const SpriteFont &font, const std::string &Str, const int x, const int y, const Color& color);
	private:
		const uint32_t _maxSprites = 200000;
		uint32_t _numberOfSpritesUsed;
		Texture2D _currentTexture;
		void _Enable2D();  // May not need
		void _Disable2D();	// May not need
#if defined(GAME_OPENGL) | defined(GAME_DIRECTX9)
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
		struct _spriteVertex10
		{
			float_t x, y, z;
			float_t r, g, b, a;
			float_t u, v;
		};
		_spriteVertex10* _spriteVertices10;
		ID3D10Buffer* _vertexBuffer10;
		Shader _spriteBatchShader;
		ID3D10InputLayout* _vertexLayout10;
		ID3D10SamplerState* _textureSamplerState10;
		ID3D10Buffer* _indexBuffer;
		ID3D10BlendState* _spriteBatchBlendState;
		// not sure how to store it, maybe name?
		std::unordered_map<std::string, ID3D10ShaderResourceView*> _knownTextures;

		// saves state of dx10 states we change to restore
		uint32_t _oldStride;
		uint32_t _oldOffset;
		ID3D10Buffer* _oldVertexBuffer;
		ID3D10Buffer* _oldIndexBuffer;
		DXGI_FORMAT _oldIndexFormat;
		uint32_t _oldIndexOffset;
		ID3D10InputLayout* _oldInputLayout;
		ID3D10VertexShader* _oldVertexShader;
		ID3D10PixelShader* _oldPixelShader;
		ID3D10SamplerState* _oldTextureSamplerState;
		D3D10_PRIMITIVE_TOPOLOGY _oldPrimitiveTopology;
		ID3D10BlendState* _oldBlendState;
		float_t _oldBlendFactor[4];
		uint32_t _oldSampleMask;
#endif
#if defined (GAME_DIRECTX11)
#endif
	};

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
			_spriteVertices10 = nullptr;
			_indexBuffer = nullptr;
			_vertexBuffer10 = nullptr;
			_vertexLayout10 = nullptr;
			_textureSamplerState10 = nullptr;
			_spriteBatchBlendState = nullptr;
			_oldStride = 0;
			_oldOffset = 0;
			_oldVertexBuffer = nullptr;
			_oldIndexBuffer = nullptr;
			_oldIndexFormat = {};
			_oldIndexOffset = 0;
			_oldInputLayout = nullptr;
			_oldVertexShader = nullptr;
			_oldPixelShader = nullptr;
			_oldTextureSamplerState = nullptr;
			_oldPrimitiveTopology = {};
			_oldBlendState = nullptr;
			_oldBlendFactor[4] = { 0 };
			_oldSampleMask = 0;
		}
#endif
#if defined (GAME_DIRECTX11)
#endif
		_numberOfSpritesUsed = 0;
	}

	inline SpriteBatch::~SpriteBatch()
	{

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
			if (_spriteVertices10)
			{
				delete[] _spriteVertices10;
				_spriteVertices10 = nullptr;
			}
			SAFE_RELEASE(_vertexBuffer10);
			SAFE_RELEASE(_vertexLayout10);
			SAFE_RELEASE(_textureSamplerState10);
			SAFE_RELEASE(_indexBuffer);
			SAFE_RELEASE(_spriteBatchBlendState);
			enginePointer->geUnLoadShader(_spriteBatchShader);
			for (auto &textureIterator : _knownTextures)
			{
				SAFE_RELEASE(textureIterator.second);
			}
		}
#endif
#if defined (GAME_DIRECTX11)

#endif
	}

	inline bool SpriteBatch::Initialize()
	{
		// OpenGL and DX9 implementation of vertices
#if defined(GAME_OPENGL) || defined (GAME_DIRECTX9)
		_spriteVertices = new _spriteVertex[_maxSprites * 6];
		for (uint32_t vertex = 0; vertex < _maxSprites * 6; vertex++)
		{
			_spriteVertices[vertex].x = 0.0f;
			_spriteVertices[vertex].y = 0.0f;
			_spriteVertices[vertex].z = 0.0f;
			_spriteVertices[vertex].rhw = 1.0f;
			_spriteVertices[vertex].u = 0.0f;
			_spriteVertices[vertex].v = 0.0f;
#if defined(GAME_DIRECTX9)
			if (enginePointer->geIsUsing(GAME_DIRECTX9))
			{
				_spriteVertices[vertex].color = Colors::White.packedARGB;
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
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			_spriteVertices10 = new _spriteVertex10[_maxSprites * 6];
			for (uint32_t vertex = 0; vertex < _maxSprites * 6; vertex++)
			{
				_spriteVertices10[vertex].x = 0.0f;
				_spriteVertices10[vertex].y = 0.0f;
				_spriteVertices10[vertex].z = 0.0f;
				_spriteVertices10[vertex].u = 0.0f;
				_spriteVertices10[vertex].v = 0.0f;
				_spriteVertices10[vertex].r = Colors::White.rf;
				_spriteVertices10[vertex].g = Colors::White.gf;
				_spriteVertices10[vertex].b = Colors::White.bf;
				_spriteVertices10[vertex].a = Colors::White.af;
			}
		}
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
			std::vector<DWORD> indices;

			D3D10_INPUT_ELEMENT_DESC inputLayout[] =
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
			vertexBufferDescription.ByteWidth = _maxSprites * (uint32_t)6 * (uint32_t)sizeof(_spriteVertex10);
			std::cout << "SpriteBatch VertexBuffer size : " << sizeof(_spriteVertex10) * _maxSprites * 6 / 1024 << "kB\n";
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
			for (uint32_t index = 0; index < _maxSprites; index += 1)
			{
				indices.emplace_back(0 + (index * 6));
				indices.emplace_back(1 + (index * 6));
				indices.emplace_back(2 + (index * 6));
				indices.emplace_back(1 + (index * 6));
				indices.emplace_back(3 + (index * 6));
				indices.emplace_back(2 + (index * 6));
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

			// Create blend state
			D3D10_BLEND_DESC blendStateDesc = { 0 };
			blendStateDesc.AlphaToCoverageEnable = FALSE;
			blendStateDesc.BlendEnable[0] = TRUE;
			blendStateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
			blendStateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
			blendStateDesc.BlendOp = D3D10_BLEND_OP_ADD;
			blendStateDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
			blendStateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
			blendStateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
			blendStateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
			if (FAILED(enginePointer->d3d10Device->CreateBlendState(&blendStateDesc, &_spriteBatchBlendState)))
			{
				lastError = { GameErrors::GameDirectX10Specific, "Could not create blend state for SpriteBatch." };
				return false;
			}
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
			uint32_t stride = sizeof(_spriteVertex10);
			uint32_t offset = 0;
			float sampleMask[] = { 1.0f, 1.0f, 1.0f, 1.0f };

			// need to save blend state check Render

			// Save everything we modify
			enginePointer->d3d10Device->IAGetIndexBuffer(&_oldIndexBuffer, &_oldIndexFormat, &_oldIndexOffset);
			enginePointer->d3d10Device->IAGetVertexBuffers(0, 1, &_oldVertexBuffer, &_oldStride, &_oldOffset);
			enginePointer->d3d10Device->IAGetInputLayout(&_oldInputLayout);
			enginePointer->d3d10Device->VSGetShader(&_oldVertexShader);
			enginePointer->d3d10Device->PSGetShader(&_oldPixelShader);
			enginePointer->d3d10Device->PSGetSamplers(0, 1, &_oldTextureSamplerState);
			enginePointer->d3d10Device->IAGetPrimitiveTopology(&_oldPrimitiveTopology);
			enginePointer->d3d10Device->OMGetBlendState(&_oldBlendState, _oldBlendFactor, &_oldSampleMask);

			// Change what we need
			enginePointer->d3d10Device->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			enginePointer->d3d10Device->IASetVertexBuffers(0, 1, &_vertexBuffer10, &stride, &offset);
			enginePointer->d3d10Device->IASetInputLayout(_vertexLayout10);
			enginePointer->d3d10Device->VSSetShader(_spriteBatchShader.vertexShader10);
			enginePointer->d3d10Device->PSSetShader(_spriteBatchShader.pixelShader10);
			enginePointer->d3d10Device->PSSetSamplers(0, 1, &_textureSamplerState10);
			enginePointer->d3d10Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			enginePointer->d3d10Device->OMSetBlendState(_spriteBatchBlendState, sampleMask, 0xffffffff);


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
			if (_oldPrimitiveTopology !=  D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED)
			{
				enginePointer->d3d10Device->IASetPrimitiveTopology(_oldPrimitiveTopology);
			}
			enginePointer->d3d10Device->OMSetBlendState(_oldBlendState, _oldBlendFactor, _oldSampleMask);
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
				std::cout << "Could not map vertex buffer in SpriteBatch\n";
			}
			else
			{
				memcpy(pVoid, &_spriteVertices10[0], sizeof(_spriteVertex10) * 6 * _numberOfSpritesUsed);
				_vertexBuffer10->Unmap();
			}

			enginePointer->d3d10Device->DrawIndexed(_numberOfSpritesUsed * 6, 0, 0);
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
			_spriteVertex10* access = nullptr;
			Vector2i windowSize;
			Rectf scaledPos;

			// If texture changed, render and change SRV
			if (texture.textureInterface10 != _currentTexture.textureInterface10)
			{
				Render();
				_currentTexture = texture;
				// Change shader texture to new one
				auto foundTexture = _knownTextures.find(texture.name);
				// Texture is known to us, so use the saved SRV
				if (foundTexture != _knownTextures.end())
				{
					// SRV has been created before
					// So use it
					enginePointer->d3d10Device->PSSetShaderResources(0, 1, &foundTexture->second);
				}
				else
				{
					// New to us texture,so create a SRV for it and save it

					ID3D10ShaderResourceView* newTextureSRV;
					D3D10_SHADER_RESOURCE_VIEW_DESC srDesc = {};
					srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
					srDesc.Texture2D.MostDetailedMip = 0;
					srDesc.Texture2D.MipLevels = 1;
					if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(texture.textureInterface10, &srDesc, &newTextureSRV)))
					{
						std::cout << "CreateSRV spritebatch failed!\n";
					}
					_knownTextures[texture.name] = newTextureSRV;
					enginePointer->d3d10Device->PSSetShaderResources(0, 1, &newTextureSRV);
				}
			}

			access = &_spriteVertices10[_numberOfSpritesUsed * 6];
			windowSize = enginePointer->geGetWindowSize();
			// Homogenise coordinates to -1.0f to 1.0f
			scaledPos.left = ((float_t)x * 2.0f / (float_t)windowSize.width) - 1.0f;
			scaledPos.top = ((float_t)y * 2.0f / (float_t)windowSize.height) - 1.0f;
			scaledPos.right = (((float_t)x + (float_t)texture.width) * 2.0f / (float)windowSize.width) - 1.0f;
			scaledPos.bottom = (((float_t)y + (float_t)texture.height) * 2.0f / (float)windowSize.height) - 1.0f;
			// Flip the y axis
			scaledPos.top = -scaledPos.top;
			scaledPos.bottom = -scaledPos.bottom;
			
			// Fill vertices

			// Top left
			access->x = scaledPos.left;
			access->y = scaledPos.top;
			access->u = 0.0f;
			access->v = 0.0f;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;

			access++;

			// Top right
			access->x = scaledPos.right;
			access->y = scaledPos.top;
			access->u = 1.0f;
			access->v = 0.0f;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;
			access++;

			// Bottom left
			access->x = scaledPos.left;
			access->y = scaledPos.bottom;
			access->u = 0.0f;
			access->v = 1.0f;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;
			access++;

			// Bottom right
			access->x = scaledPos.right;
			access->y = scaledPos.bottom;
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
			_spriteVertex10* access = nullptr;
			Vector2i window;
			Rectf scaledPosition;
			Rectf scaledUV;

			// If texture changed, render and change texture/SRV
			if (texture.textureInterface10 != _currentTexture.textureInterface10)
			{
				Render();
				_currentTexture = texture;
				// Change shader texture to new one
				auto foundTexture = _knownTextures.find(texture.name);
				// Texture is known to us, so use the saved SRV
				if (foundTexture != _knownTextures.end())
				{
					// Resource view has been created
					// So use it
					enginePointer->d3d10Device->PSSetShaderResources(0, 1, &foundTexture->second);
				}
				else
				{
					// New to us texture,so create a SRV for it and save it

					ID3D10ShaderResourceView* newTextureSRV;
					D3D10_SHADER_RESOURCE_VIEW_DESC srDesc = {};
					srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
					srDesc.Texture2D.MostDetailedMip = 0;
					srDesc.Texture2D.MipLevels = 1;
					if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(texture.textureInterface10, &srDesc, &newTextureSRV)))
					{
						std::cout << "CreateSRV spritebatch failed!\n";
					}
					_knownTextures[texture.name] = newTextureSRV;
					enginePointer->d3d10Device->PSSetShaderResources(0, 1, &newTextureSRV);
				}
			}

			access = &_spriteVertices10[_numberOfSpritesUsed * 6];
			window = enginePointer->geGetWindowSize();
			// Homogenise coordinates to -1.0f to 1.0f
			scaledPosition.left = ((float_t)destination.left * 2.0f / (float_t)window.width) - 1.0f;
			scaledPosition.top = ((float_t)destination.top * 2.0f / (float_t)window.height) - 1.0f;
			scaledPosition.right = (((float_t)destination.right) * 2.0f / (float)window.width) - 1.0f;
			scaledPosition.bottom = (((float_t)destination.bottom) * 2.0f / (float)window.height) - 1.0f;
			// Flip the y axis
			scaledPosition.top = -scaledPosition.top;
			scaledPosition.bottom = -scaledPosition.bottom;
			// Homogenise UV coords to 0.0f - 1.0f
			scaledUV.left = portion.left * texture.oneOverWidth;
			scaledUV.top = portion.top * texture.oneOverHeight;
			scaledUV.right = portion.right * texture.oneOverWidth;
			scaledUV.bottom = portion.bottom * texture.oneOverHeight;

			// Fill vertices

			// Top left
			access->x = scaledPosition.left;
			access->y = scaledPosition.top;
			access->u = (float_t)scaledUV.left;
			access->v = (float_t)scaledUV.top;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;

			access++;

			// Top right
			access->x = scaledPosition.right;
			access->y = scaledPosition.top;
			access->u = scaledUV.right;
			access->v = scaledUV.top;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;
			access++;

			// Bottom left
			access->x = scaledPosition.left;
			access->y = scaledPosition.bottom;
			access->u = scaledUV.left;
			access->v = scaledUV.bottom;
			access->r = color.rf;
			access->g = color.gf;
			access->b = color.bf;
			access->a = color.af;
			access++;

			// Bottom right
			access->x = scaledPosition.right;
			access->y = scaledPosition.bottom;
			access->u = scaledUV.right;
			access->v = scaledUV.bottom;
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
	
	void SpriteBatch::DrawString(const SpriteFont &font, const std::string& Str, const int x, const int y, const Color& color)
	{
		int32_t currentX = x;
		int32_t currentY = y;
		uint32_t widthOfLetter = 0;
		uint32_t heightOfLetter = 0;
		Recti source, destination;
		int16_t letter;

		for (unsigned int i = 0; i < Str.size(); ++i)
		{
			letter = Str[i];
			widthOfLetter = font._characterSet.letters[letter].width;
			heightOfLetter = font._characterSet.letters[letter].height;

			source.left = font._characterSet.letters[letter].x;
			source.top = font._characterSet.letters[letter].y;
			source.right = source.left + widthOfLetter;
			source.bottom = source.top + heightOfLetter;

			destination.left = currentX + font._characterSet.letters[letter].xOffset;
			destination.top = currentY + font._characterSet.letters[letter].yOffset;
			destination.right = widthOfLetter + destination.left;
			destination.bottom = heightOfLetter + destination.top;

			Draw(font.Texture(), destination, source, color);

			currentX += font._characterSet.letters[letter].xAdvance;
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