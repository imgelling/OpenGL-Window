#if !defined(GAMEPIXELMODE_H)
#define GAMEPIXELMODE_H

#if defined(GAME_OPENGL)
#include <GL/gl.h>
#endif
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif
#include "GameErrors.h"
#include "GameEngine.h"
#include "GameMath.h"
#include "GameTexture2D.h"

// Just a test to make sure defines are working
#if defined(GAME_ENABLE_SHADERS)
#define USING_SHADERS
#endif

namespace game
{
	extern GameError lastError;
	extern Engine* enginePointer;
	class PixelMode
	{
	public:
		PixelMode();
		~PixelMode();

		bool Initialize(const Vector2i& sizeOfScreen);
		void Render();
		void Clear(const Color &color) noexcept;
		void Pixel(const int32_t x, const int32_t y, const game::Color& color) noexcept;
		void PixelClip(const int32_t x, const int32_t y, const game::Color& color) noexcept;
		void Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color) noexcept;
		void LineClip(int32_t x1, int32_t y1,  int32_t x2,  int32_t y2, const Color& color) noexcept;
		void Circle(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void CircleClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void CircleFilled(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void CircleFilledClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void Rect(const Recti& rectangle, const Color& color) noexcept;
		void RectClip(const Recti& rectangle, const Color& color) noexcept;
		void HorizontalPillClip(const int32_t x, const int32_t y, const int32_t length, const int32_t radius, const game::Color& color) noexcept;
		void VerticalPillClip(const int32_t x, const int32_t y, const int32_t height, const int32_t radius, const game::Color& color) noexcept;

		Pointi GetScaledMousePosition() noexcept;
		Pointi GetPixelFrameBufferSize() noexcept;
	private:
		Texture2D _frameBuffer[2];
		Vector2f _oneOverScale;
		Vector2f _savedPositionOfScaledTexture;
		uint32_t* _video;
		Vector2i _bufferSize;
		Vector2i _windowSize;
		uint32_t _currentBuffer;
		void _UpdateFrameBuffer();
		void _ScaleQuadToWindow();
#if defined(GAME_OPENGL) & !defined(GAME_ENABLE_SHADERS)
		uint32_t _compiledQuad;
#endif
#if defined(GAME_DIRECTX9)
		struct _vertex9
		{
			float_t x, y, z, rhw;    
			uint32_t color;    
			float_t u, v;
		};
		_vertex9 _quadVertices9[6] =
		{
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 1.0f},

			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 1.0f}
		};
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer9;
#endif
#if defined(GAME_DIRECTX10)
		struct _vertex10
		{
			float_t x, y, z;
			float_t r, g, b, a;
			float_t u, v;
			// D3DXColor is just a float for rgba
			// D3DXColor color
		};
		_vertex10 _quadVertices10[4] =
		{
			// tl
			{0.0f, 1.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
			// tr
			{0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, .0f},
			// bl
			{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},
			// br
			{0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		};
		ID3D10Buffer* _vertexBuffer10;
		ID3D10Buffer* _indexBuffer;
		Shader _pixelModeShader;
		ID3D10InputLayout* _vertexLayout10;
		ID3D10ShaderResourceView* _textureShaderResourceView0;
		ID3D10ShaderResourceView* _textureShaderResourceView1;
		ID3D10SamplerState* _textureSamplerState10;
#endif
#if defined(GAME_DIRECTX11)

#endif
	};

	inline PixelMode::PixelMode()
	{
		_video = nullptr;
		_currentBuffer = 0;
#if defined(GAME_OPENGL) & !defined(GAME_ENABLE_SHADERS)
		_compiledQuad = 0;
#endif
#if defined(GAME_DIRECTX9)
		_vertexBuffer9 = nullptr;
#endif
#if defined(GAME_DIRECTX10)
		_vertexBuffer10 = nullptr;
		_vertexLayout10 = nullptr;
		_indexBuffer = nullptr;
		_textureShaderResourceView0 = nullptr;
		_textureShaderResourceView1 = nullptr;
		_textureSamplerState10 = nullptr;
#endif
#if defined(GAME_DIRECTX11)

#endif
	}

	inline PixelMode::~PixelMode()
	{
		if (_video != nullptr) delete[] _video;
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (_vertexBuffer9)
			{
				_vertexBuffer9->Release();
				_vertexBuffer9 = nullptr;
			}
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			if (_vertexBuffer10)
			{
				_vertexBuffer10->Release();
				_vertexBuffer10 = nullptr;
			}
			if (_vertexLayout10)
			{
				_vertexLayout10->Release();
				_vertexLayout10 = nullptr;
			}
			if (_indexBuffer)
			{
				_indexBuffer->Release();
				_indexBuffer = nullptr;
			}
			enginePointer->geUnLoadShader(_pixelModeShader);
			SAFE_RELEASE(_textureShaderResourceView0);
			SAFE_RELEASE(_textureShaderResourceView1);
			SAFE_RELEASE(_textureSamplerState10);
		}
#endif
		enginePointer->geUnLoadTexture(_frameBuffer[0]);
		enginePointer->geUnLoadTexture(_frameBuffer[1]);
	}

	inline bool PixelMode::Initialize(const Vector2i& sizeOfScreen)
	{
		_bufferSize = sizeOfScreen;

		// Save window size
		_windowSize = enginePointer->geGetWindowSize();

		// Create video buffer
		_video = new uint32_t[((size_t)_bufferSize.width) * ((size_t)_bufferSize.height)];
		if (_video == nullptr)
		{
			lastError = { GameErrors::GameRenderer, "Could not allocate RAM for PixelMode video buffer." };
			return false;
		}

		Clear(Colors::Black);

		// Create frame buffer textures
		for (uint32_t loop = 0; loop < 2; loop++)
		{
			_frameBuffer[loop].width = _bufferSize.width;
			_frameBuffer[loop].height = _bufferSize.height;
			_frameBuffer[loop].componentsPerPixel = 4;
			_frameBuffer[loop].filterType = game::TextureFilterType::Point;
			_frameBuffer[loop].isMipMapped = false;
			_frameBuffer[loop].name = "PixelMode FrameBuffer" + std::to_string(loop);
			if (!enginePointer->geCreateTexture(_frameBuffer[loop]))
			{
				lastError = { GameErrors::GameRenderer, "Could not create textures for PixelMode frame buffers." };
				return false;
			}
		}

#if defined(GAME_OPENGL) & !defined(GAME_ENABLE_SHADERS)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_compiledQuad = glGenLists(1);
		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			enginePointer->d3d9Device->CreateVertexBuffer(6 * sizeof(_vertex9), 0, (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), D3DPOOL_MANAGED, &_vertexBuffer9, NULL);
			if (_vertexBuffer9 == nullptr)
			{
				lastError = { GameErrors::GameDirectX9Specific, "Could not create vertex buffer for PixelMode." };
				return false;
			}
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			D3D10_BUFFER_DESC vertexBufferDescription = { 0 };
			D3D10_BUFFER_DESC indexBufferDescription = { 0 };
			D3D10_SUBRESOURCE_DATA vertexInitialData = { 0 };
			D3D10_SUBRESOURCE_DATA indexInitialData = { 0 };
			DWORD indices[] = { 0, 1, 2, 1, 3, 2, };
			D3D10_INPUT_ELEMENT_DESC inputLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0},
			};
			
			// Load shaders for sprite mode
			if (!enginePointer->geLoadShader("Content/VertexShader.hlsl", "Content/PixelShader.hlsl", _pixelModeShader))
			{
				return false;
			}
			
			// Create the vertex buffer
			vertexBufferDescription.ByteWidth = sizeof(_vertex10) * 4;
			vertexBufferDescription.Usage = D3D10_USAGE_DYNAMIC;// D3D10_USAGE_DEFAULT;
			vertexBufferDescription.BindFlags = D3D10_BIND_VERTEX_BUFFER;
			vertexBufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; //0;
			vertexBufferDescription.MiscFlags = 0;
			vertexInitialData.pSysMem = _quadVertices10;
			if (FAILED(enginePointer->d3d10Device->CreateBuffer(&vertexBufferDescription, &vertexInitialData, &_vertexBuffer10)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create vertex buffer for PixelMode." };
				enginePointer->geUnLoadShader(_pixelModeShader);
				return false;
			}

			// Create index buffer
			indexBufferDescription.Usage = D3D10_USAGE_IMMUTABLE;
			indexBufferDescription.ByteWidth = sizeof(DWORD) * 2 * 3;
			indexBufferDescription.BindFlags = D3D10_BIND_INDEX_BUFFER;
			indexBufferDescription.CPUAccessFlags = 0;
			indexBufferDescription.MiscFlags = 0;
			indexInitialData.pSysMem = indices;
			if (FAILED(enginePointer->d3d10Device->CreateBuffer(&indexBufferDescription, &indexInitialData, &_indexBuffer)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create index buffer for PixelMode." };
				_vertexBuffer10->Release();
				_vertexBuffer10 = nullptr;
				enginePointer->geUnLoadShader(_pixelModeShader);
				return false;
			}

			// Create input layout for shaders
			if (FAILED(enginePointer->d3d10Device->CreateInputLayout(inputLayout, 3, _pixelModeShader.compiledVertexShader10->GetBufferPointer(), _pixelModeShader.compiledVertexShader10->GetBufferSize(), &_vertexLayout10)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create input layout for PixelMode." };
				_indexBuffer->Release();
				_indexBuffer = nullptr;
				_vertexBuffer10->Release();
				_vertexBuffer10 = nullptr;
				enginePointer->geUnLoadShader(_pixelModeShader);
				return false;
			}

			D3D10_SAMPLER_DESC samplerDesc = { };
			//ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

			if (FAILED(enginePointer->d3d10Device->CreateSamplerState(&samplerDesc, &_textureSamplerState10)))
			{
				std::cout << "Create sampler failed!\n";
			}

			D3D10_SHADER_RESOURCE_VIEW_DESC srDesc = {};
			srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
			srDesc.Texture2D.MostDetailedMip = 0;
			srDesc.Texture2D.MipLevels = 1;
			if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(_frameBuffer[0].textureInterface10, &srDesc, &_textureShaderResourceView0)))
			{
				std::cout << "CreateSRV0 failed!\n";
			}
			if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(_frameBuffer[1].textureInterface10, &srDesc, &_textureShaderResourceView1)))
			{
				std::cout << "CreateSRV1 failed!\n";
			}
		}
#endif
#if defined (GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			//enginePointer->d3d11Device->CreateBuffer()
		}
#endif

		// Scale the texture to window size
		_ScaleQuadToWindow();
		return true;
	}

	inline void PixelMode::_UpdateFrameBuffer()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _frameBuffer[_currentBuffer].width, _frameBuffer[_currentBuffer].height, GL_RGBA, game::systemInfo.gpuInfo.internalPixelType, (GLvoid*)_video);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			D3DLOCKED_RECT rect;
			unsigned char* test = (unsigned char*)_video;
			_frameBuffer[_currentBuffer].textureInterface9->LockRect(0, &rect, 0, 0);
			unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
			memcpy(dest, test, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			_frameBuffer[_currentBuffer].textureInterface9->UnlockRect(0);
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			D3D10_MAPPED_TEXTURE2D mappedTex = { 0 };
			if (FAILED(_frameBuffer[_currentBuffer].textureInterface10->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex)))
			{
				std::cout << "Could not map texture\n";
				return;
			}
			unsigned char* dest = (unsigned char*)mappedTex.pData;
			memcpy(dest, (unsigned char*)_video, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);

			_frameBuffer[_currentBuffer].textureInterface10->Unmap(D3D10CalcSubresource(0, 0, 1));
		}
#endif
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			//D3D11_MAPPED_SUBRESOURCE data;
			//HRESULT hr;
			//hr = enginePointer->d3d11Context->Map(_frameBuffer[_currentBuffer].textureInterface11, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			//memcpy(data.pData, (unsigned char*)_video, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			//enginePointer->d3d11Context->Unmap(_frameBuffer[_currentBuffer].textureInterface11, 0);
		}
#endif
	}

	inline void PixelMode::_ScaleQuadToWindow()
	{
		float_t tempScale = 0.0f;
		Vector2f _scale;
		Vector2f _sizeOfScaledTexture;
		Vector2f _positionOfScaledTexture;

		if (_windowSize.height < _windowSize.width)
		{
			_scale.y = (float_t)_windowSize.height * _frameBuffer[_currentBuffer].oneOverHeight;
			tempScale = (float_t)_windowSize.width * _frameBuffer[_currentBuffer].oneOverWidth;
			if (tempScale > _scale.y)
			{
				_scale.x = _scale.y;
			}
			else
			{
				_scale.x = _scale.y = tempScale;
				_positionOfScaledTexture.y = ((_windowSize.height >> 1) - ((float_t)_frameBuffer[_currentBuffer].height * _scale.y / 2.0f));
			}
			_positionOfScaledTexture.x = ((_windowSize.width >> 1) - ((float_t)_frameBuffer[_currentBuffer].width * _scale.x / 2.0f));
		}
		else if (_windowSize.height > _windowSize.width)
		{
			_scale.x = (float_t)_windowSize.width * _frameBuffer[_currentBuffer].oneOverWidth;
			_scale.y = _scale.x;
			_positionOfScaledTexture.y = ((_windowSize.height >> 1) - ((float_t)_frameBuffer[_currentBuffer].height * _scale.y / 2.0f));
		}
		else
		{
			_scale = { 1.0f, 1.0f };
		}

		_oneOverScale.x = 1.0f / _scale.x;
		_oneOverScale.y = 1.0f / _scale.y;

		// Set the size of the scaled texture
		_sizeOfScaledTexture.width = _positionOfScaledTexture.x + (_frameBuffer[_currentBuffer].width * _scale.x);
		_sizeOfScaledTexture.height = _positionOfScaledTexture.y + (_frameBuffer[_currentBuffer].height * _scale.y);

		// Pixel offset fix
		_positionOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
		_positionOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
		_sizeOfScaledTexture.width -= _frameBuffer[_currentBuffer].oneOverWidth;
		_sizeOfScaledTexture.height -= _frameBuffer[_currentBuffer].oneOverHeight;

		_savedPositionOfScaledTexture = _positionOfScaledTexture;

#if defined(GAME_OPENGL) & !defined(GAME_ENABLE_SHADERS)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{

			// Homoginize the scaled rect to -1 to 1 range using
			_positionOfScaledTexture.x = (_positionOfScaledTexture.x * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_positionOfScaledTexture.y = (_positionOfScaledTexture.y * 2.0f / (float_t)_windowSize.height) - 1.0f;
			_sizeOfScaledTexture.width = ((float_t)_sizeOfScaledTexture.width * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_sizeOfScaledTexture.height = ((float_t)_sizeOfScaledTexture.height * 2.0f / (float_t)_windowSize.height) - 1.0f;

			glNewList(_compiledQuad, GL_COMPILE);
			{
				glBegin(GL_QUADS);
				//bl
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0, 1);
				glVertex2f(_positionOfScaledTexture.x, -_sizeOfScaledTexture.height);
				//br
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1, 1);
				glVertex2f(_sizeOfScaledTexture.width, -_sizeOfScaledTexture.height);
				//tr
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(_sizeOfScaledTexture.width, -_positionOfScaledTexture.y);
				// tl
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0, 0);
				glVertex2f(_positionOfScaledTexture.x, -_positionOfScaledTexture.y);

				glEnd();
			}
			glEndList();
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			VOID* pVoid = nullptr;  

			// tl
			_quadVertices9[0].x = _positionOfScaledTexture.x;
			_quadVertices9[0].y = _positionOfScaledTexture.y;
			// tr
			_quadVertices9[1].x = _sizeOfScaledTexture.width;
			_quadVertices9[1].y = _positionOfScaledTexture.y;
			// bl
			_quadVertices9[2].x = _positionOfScaledTexture.x;
			_quadVertices9[2].y = _sizeOfScaledTexture.height;

			// tr
			_quadVertices9[3].x = _sizeOfScaledTexture.width;
			_quadVertices9[3].y = _positionOfScaledTexture.y;
			// br
			_quadVertices9[4].x = _sizeOfScaledTexture.width;
			_quadVertices9[4].y = _sizeOfScaledTexture.height;
			// bl
			_quadVertices9[5].x = _positionOfScaledTexture.x;
			_quadVertices9[5].y = _sizeOfScaledTexture.height;

			// Copy vertices to the vertex buffer
			_vertexBuffer9->Lock(0, 0, (void**)&pVoid, 0);
			memcpy(pVoid, _quadVertices9, sizeof(_quadVertices9));
			_vertexBuffer9->Unlock();
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			// Homoginize the scaled rect to -1 to 1 range using
			_positionOfScaledTexture.x = (_positionOfScaledTexture.x * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_positionOfScaledTexture.y = (_positionOfScaledTexture.y * 2.0f / (float_t)_windowSize.height) - 1.0f;
			_sizeOfScaledTexture.width = ((float_t)_sizeOfScaledTexture.width * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_sizeOfScaledTexture.height = ((float_t)_sizeOfScaledTexture.height * 2.0f / (float_t)_windowSize.height) - 1.0f;
			_positionOfScaledTexture.y = -_positionOfScaledTexture.y;
			_sizeOfScaledTexture.height = -_sizeOfScaledTexture.height;

			// tl
			_quadVertices10[0].x = _positionOfScaledTexture.x;
			_quadVertices10[0].y = _positionOfScaledTexture.y;
			// tr
			_quadVertices10[1].x = _sizeOfScaledTexture.width;
			_quadVertices10[1].y = _positionOfScaledTexture.y;
			// bl
			_quadVertices10[2].x = _positionOfScaledTexture.x;
			_quadVertices10[2].y = _sizeOfScaledTexture.height;

			// br
			_quadVertices10[3].x = _sizeOfScaledTexture.width;
			_quadVertices10[3].y = _sizeOfScaledTexture.height;

			VOID* pVoid = nullptr;
			_vertexBuffer10->Map(D3D10_MAP_WRITE_DISCARD, 0, &pVoid);
			memcpy(pVoid, _quadVertices10, sizeof(_quadVertices10));
			_vertexBuffer10->Unmap();
		}
#endif

	}

	inline void PixelMode::Render()
	{
		Vector2i currentWindowSize = enginePointer->geGetWindowSize();

		// If window size has changed
		if ((currentWindowSize.width != _windowSize.width) || (currentWindowSize.height != _windowSize.height))
		{
			_windowSize = currentWindowSize;

			// Rescale the quad to fit new window size
			_ScaleQuadToWindow();
		}

		// Copy video buffer to gpu
		_UpdateFrameBuffer();

		// Draw the quad
#if defined(GAME_OPENGL) & !defined(GAME_ENABLE_SHADERS)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				glDisable(0x809D); // 0x809D is GL_MULTISAMPLE
			}
			glCallList(_compiledQuad);
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				glEnable(0x809D);
			}
			glDisable(GL_TEXTURE_2D);
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			DWORD oldFVF = 0;
			IDirect3DBaseTexture9* activeTexture = 0;
			//enginePointer->d3d9Device->BeginScene();
			// Save current state
			enginePointer->d3d9Device->GetFVF(&oldFVF);
			enginePointer->d3d9Device->GetTexture(0, &activeTexture);

			// Disable multisampling if enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
			}

			enginePointer->d3d9Device->SetTexture(0, _frameBuffer[_currentBuffer].textureInterface9);
			enginePointer->d3d9Device->SetFVF((D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
			enginePointer->d3d9Device->SetStreamSource(0, _vertexBuffer9, 0, sizeof(_vertex9));
			enginePointer->d3d9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			// Restore previous state
			enginePointer->d3d9Device->SetFVF(oldFVF);
			enginePointer->d3d9Device->SetTexture(0, activeTexture);
			if (activeTexture)
			{
				activeTexture->Release();
			}

			// Renable multisampling if it was enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			}

			//enginePointer->d3d9Device->EndScene();
		}
		
#endif

#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			uint32_t stride = sizeof(_vertex10);
			uint32_t oldStride = 0;
			uint32_t offset = 0;
			uint32_t oldOffset = 0;
			ID3D10Buffer* oldVertexBuffer = nullptr;
			ID3D10Buffer* oldIndexBuffer = nullptr;
			DXGI_FORMAT oldIndexFormat = {};
			uint32_t oldIndexOffset = 0;
			ID3D10InputLayout* oldInputLayout = nullptr;
			ID3D10VertexShader* oldVertexShader = nullptr;
			ID3D10PixelShader* oldPixelShader = nullptr;
			ID3D10SamplerState* oldTextureSamplerState = nullptr;
			D3D10_PRIMITIVE_TOPOLOGY oldPrimitiveTopology = {};


			// Save everything we modify
			enginePointer->d3d10Device->IAGetIndexBuffer(&oldIndexBuffer, &oldIndexFormat, &oldIndexOffset);
			enginePointer->d3d10Device->IAGetVertexBuffers(0, 1, &oldVertexBuffer, &oldStride, &oldOffset);
			enginePointer->d3d10Device->IAGetInputLayout(&oldInputLayout);
			enginePointer->d3d10Device->VSGetShader(&oldVertexShader);
			enginePointer->d3d10Device->PSGetShader(&oldPixelShader);
			enginePointer->d3d10Device->PSGetSamplers(0, 1, &oldTextureSamplerState);
			enginePointer->d3d10Device->IAGetPrimitiveTopology(&oldPrimitiveTopology);


			// Change what we need
			enginePointer->d3d10Device->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			enginePointer->d3d10Device->IASetVertexBuffers(0, 1, &_vertexBuffer10, &stride, &offset);
			enginePointer->d3d10Device->IASetInputLayout(_vertexLayout10);
			enginePointer->d3d10Device->VSSetShader(_pixelModeShader.vertexShader10);
			enginePointer->d3d10Device->PSSetShader(_pixelModeShader.pixelShader10);
			enginePointer->d3d10Device->PSSetSamplers(0, 1, &_textureSamplerState10);
			enginePointer->d3d10Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (!_currentBuffer)
			{
				enginePointer->d3d10Device->PSSetShaderResources(0, 1, &_textureShaderResourceView0);
			}
			else
			{
				enginePointer->d3d10Device->PSSetShaderResources(0, 1, &_textureShaderResourceView1);
			}

			enginePointer->d3d10Device->DrawIndexed(6, 0, 0);
			
			// Restore old states
			enginePointer->d3d10Device->IASetIndexBuffer(oldIndexBuffer, oldIndexFormat, oldIndexOffset);
			enginePointer->d3d10Device->IASetVertexBuffers(0, 1, &oldVertexBuffer, &oldStride, &oldOffset);
			enginePointer->d3d10Device->IASetInputLayout(oldInputLayout);
			enginePointer->d3d10Device->VSSetShader(oldVertexShader);
			enginePointer->d3d10Device->PSSetShader(oldPixelShader);
			enginePointer->d3d10Device->PSSetSamplers(0, 1, &oldTextureSamplerState);
			enginePointer->d3d10Device->IASetPrimitiveTopology(oldPrimitiveTopology);
		}
#endif

		_currentBuffer++;
		if (_currentBuffer > 1) _currentBuffer = 0;

	}

	inline void PixelMode::Clear(const Color &color) noexcept
	{
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packedARGB);
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packedABGR);
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packedABGR);
		}
#endif

	}

	inline void PixelMode::Pixel(const int32_t x, const int32_t y, const game::Color& color) noexcept
	{
#if defined(_DEBUG)
		if (x < 0)
		{
			std::cout << "X < 0\n";
			return;
		}
		if (y < 0)
		{
			std::cout << "Y < 0\n";
			return;
		}
		if (x > _bufferSize.width - 1)
		{
			std::cout << "X > _bufferSize - 1\n";
			return;
		}
		if (y > _bufferSize.height - 1)
		{
			std::cout << "Y > _bufferSize - 1\n";
			return;
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_video[y * _bufferSize.width + x] = color.packedARGB;
			return;
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			_video[y * _bufferSize.width + x] = color.packedABGR;
			return;
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_video[y * _bufferSize.width + x] = color.packedABGR;
			return;
		}
#endif
	}

	inline void PixelMode::PixelClip(const int32_t x, const int32_t y, const game::Color& color) noexcept
	{
		if (x < 0 || y < 0) return;
		if (x > _bufferSize.width-1 || y > _bufferSize.height - 1) return;
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_video[y * _bufferSize.width + x] = color.packedARGB;
		}
#endif
#if defined(GAME_DIRECTX10)
		if (enginePointer->geIsUsing(GAME_DIRECTX10))
		{
			_video[y * _bufferSize.width + x] = color.packedABGR;
			return;
	}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_video[y * _bufferSize.width + x] = color.packedABGR;
		}
#endif
	}

	inline void PixelMode::Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color) noexcept
	{
		int32_t delta_x(x2 - x1);
		int32_t delta_y(y2 - y1);

		// if x1 == x2, then it does not matter what we set here
		const int8_t ix((delta_x > 0) - (delta_x < 0));
		delta_x = abs(delta_x) << 1;

		// if y1 == y2, then it does not matter what we set here
		const int8_t iy((delta_y > 0) - (delta_y < 0));
		delta_y = abs(delta_y) << 1;

		Pixel(x1, y1, color);

		if (delta_x >= delta_y)
		{
			// error may go below zero
			int32_t error(delta_y - (delta_x >> 1));

			while (x1 != x2)
			{
				if ((error >= 0) && (error || (ix > 0)))
				{
					error -= delta_x;
					y1 += iy;
				}
				// else do nothing

				error += delta_y;
				x1 += ix;

				Pixel(x1, y1, color);
			}
		}
		else
		{
			// error may go below zero
			int32_t error(delta_x - (delta_y >> 1));

			while (y1 != y2)
			{
				if ((error >= 0) && (error || (iy > 0)))
				{
					error -= delta_y;
					x1 += ix;
				}
				// else do nothing

				error += delta_x;
				y1 += iy;

				Pixel(x1, y1, color);
			}
		}
	}

	inline void PixelMode::LineClip(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color) noexcept
	{
		//Liang - Barsky Algorithm

		struct _Clip
		{
			inline int clipTest(float_t p, float_t q, float_t* u1, float_t* u2) noexcept
			{
				float_t r(0.0f);

				if (p < 0.0) {
					r = q / p;
					if (r > *u2)
						return false;
					else
						if (r > *u1)
							*u1 = r;
				}
				else
					if (p > 0.0) {
						r = q / p;
						if (r < *u1)
							return false;
						else if (r < *u2)
							*u2 = r;
					}
					else
						if (q < 0.0)
							return false;

				return true;
			}
		};

		_Clip _clip;
		float_t u1(0.0f);
		float_t u2(1.0f);
		float_t dx((float_t)(x2 - x1));
		float_t dy(0);
		float_t x1f((float_t)x1);
		float_t x2f((float_t)x2);
		float_t y1f((float_t)y1);
		float_t y2f((float_t)y2);

		if (_clip.clipTest(-dx, x1f - 0, &u1, &u2))
			if (_clip.clipTest(dx, _bufferSize.width - 1 - x1f, &u1, &u2)) {
				dy = y2f - y1f;
				if (_clip.clipTest(-dy, y1f, &u1, &u2))
					if (_clip.clipTest(dy, _bufferSize.height - 1 - y1f, &u1, &u2)) {
						if (u2 < 1.0) {
							x2f = x1f + u2 * dx;
							y2f = y1f + u2 * dy;
						}
						if (u1 > 0.0) {
							x1f += u1 * dx;
							y1f += u1 * dy;
						}
						Line((int32_t)std::round(x1f), (int32_t)std::round(y1f), (int32_t)std::round(x2f), (int32_t)std::round(y2f), color);
					}
			}
	}

	inline void PixelMode::Circle(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;

			while (y0 >= x0) // only formulate 1/8 of circle
			{
				// Draw even octants
				Pixel(x + x0, y - y0, color);// Q6 - upper right right
				Pixel(x + y0, y + x0, color);// Q4 - lower lower right
				Pixel(x - x0, y + y0, color);// Q2 - lower left left
				Pixel(x - y0, y - x0, color);// Q0 - upper upper left
				if (x0 != 0 && x0 != y0)
				{
					Pixel(x + y0, y - x0, color);// Q7 - upper upper right
					Pixel(x + x0, y + y0, color);// Q5 - lower right right
					Pixel(x - y0, y + x0, color);// Q3 - lower lower left
					Pixel(x - x0, y - y0, color);// Q1 - upper left left
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
					d += 4 * (x0++ - y0--) + 10;
			}
		}
		else
			Pixel(x, y, color);
	}

	inline void PixelMode::CircleClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0(0);
			int32_t y0(radius);
			int32_t d(3 - 2 * radius);

			while (y0 >= x0) // only formulate 1/8 of circle
			{
				// Draw even octants
				PixelClip(x + x0, y - y0, color);// Q6 - upper right right
				PixelClip(x + y0, y + x0, color);// Q4 - lower lower right
				PixelClip(x - x0, y + y0, color);// Q2 - lower left left
				PixelClip(x - y0, y - x0, color);// Q0 - upper upper left
				if (x0 != 0 && x0 != y0)
				{
					PixelClip(x + y0, y - x0, color);// Q7 - upper upper right
					PixelClip(x + x0, y + y0, color);// Q5 - lower right right
					PixelClip(x - y0, y + x0, color);// Q3 - lower lower left
					PixelClip(x - x0, y - y0, color);// Q1 - upper left left
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
					d += 4 * (x0++ - y0--) + 10;
			}
		}
		else
			PixelClip(x, y, color);
	}

	inline void PixelMode::CircleFilled(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			auto drawline = [&](int32_t sx, int32_t ex, int32_t y)
			{
				for (int32_t x = sx; x <= ex; x++)
					Pixel(x, y, color);
			};

			while (y0 >= x0)
			{
				drawline(x - y0, x + y0, y - x0);
				if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						drawline(x - x0, x + x0, y - y0);
						drawline(x - x0, x + x0, y + y0);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Pixel(x, y, color);
	}

	inline void PixelMode::CircleFilledClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			auto drawline = [&](int32_t sx, int32_t ex, int32_t y)
			{
				if (sx < 0) sx = 0;
				if (ex > _bufferSize.width - 1) ex = _bufferSize.width - 1;
				if (y < 0) y = 0;
				if (y > _bufferSize.height - 1) y = _bufferSize.height - 1;
				for (int x = sx; x <= ex; x++)
					Pixel(x, y, color);
			};

			while (y0 >= x0)
			{
				drawline(x - y0, x + y0, y - x0);
				if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						drawline(x - x0, x + x0, y - y0);
						drawline(x - x0, x + x0, y + y0);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Pixel(x, y, color);
	}

	inline void PixelMode::Rect(const Recti& rectangle, const Color& color) noexcept
	{
		// Top
		Line(rectangle.x, rectangle.y, rectangle.right, rectangle.y, color);
		// Bottom
		Line(rectangle.x, rectangle.bottom, rectangle.right, rectangle.bottom, color);
		// Left
		Line(rectangle.x, rectangle.y, rectangle.x, rectangle.bottom, color);
		// Right
		Line(rectangle.right, rectangle.y, rectangle.right, rectangle.bottom, color);
	}

	inline void PixelMode::RectClip(const Recti& rectangle, const Color& color) noexcept
	{
		// Top
		LineClip(rectangle.x, rectangle.y, rectangle.right, rectangle.y, color);
		// Bottom
		LineClip(rectangle.x, rectangle.bottom, rectangle.right, rectangle.bottom, color);
		// Left
		LineClip(rectangle.x, rectangle.y, rectangle.x, rectangle.bottom, color);
		// Right
		LineClip(rectangle.right, rectangle.y, rectangle.right, rectangle.bottom, color);
	}

	inline void PixelMode::HorizontalPillClip(const int32_t x, const int32_t y, const int32_t length, const int32_t radius, const game::Color& color) noexcept
	{
		int32_t calculatedLength = length - (radius * 2);

		if (radius < 0 || x < -length || y < -radius || x - _bufferSize.width > length || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			while (y0 >= x0)
			{
				LineClip(x - y0, y - x0, x + y0 + calculatedLength, y - x0, color);
				if (x0 > 0)
				{
					LineClip(x - y0, y + x0, x + y0 + calculatedLength, y + x0, color);
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						LineClip(x - x0, y - y0, x + x0 + calculatedLength, y - y0, color);
						LineClip(x - x0, y + y0, x + x0 + calculatedLength, y + y0, color);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Pixel(x, y, color);
	}

	inline void PixelMode::VerticalPillClip(const int32_t x, const int32_t y, const int32_t height, const int32_t radius, const game::Color& color) noexcept
	{
		int32_t calculatedHeight = y + (height - (radius * 2));

		if (radius < 0 || x < -radius || y < -height || x - _bufferSize.width > radius || y - _bufferSize.height > height)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			while (y0 >= x0)
			{
				// Top half
				LineClip(x - y0, y - x0, x + y0, y - x0, color);
				if (x0 > 0)
				{
					// Bottom half
					LineClip(x - y0, calculatedHeight + x0, x + y0, calculatedHeight + x0, color);
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						// Top half
						LineClip(x - x0, y - y0, x + x0, y - y0, color);
						// Bottom half
						LineClip(x - x0, calculatedHeight + y0, x + x0, calculatedHeight + y0, color);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Pixel(x, y, color);

		for (int32_t width = -radius; width <= radius; width++)
		{
			LineClip(x + width, y, x + width, calculatedHeight, color);
		}
	}

	inline Pointi PixelMode::GetScaledMousePosition() noexcept
	{
		Pointi scaledMouseCoords = enginePointer->geMouse.GetPosition();

		scaledMouseCoords.x = (int32_t)(scaledMouseCoords.x - _savedPositionOfScaledTexture.x);
		scaledMouseCoords.y = (int32_t)(scaledMouseCoords.y - _savedPositionOfScaledTexture.y);
		scaledMouseCoords.x = (int32_t)(scaledMouseCoords.x * _oneOverScale.x);
		scaledMouseCoords.y = (int32_t)(scaledMouseCoords.y * _oneOverScale.y);
		return scaledMouseCoords;
	}

	inline Pointi PixelMode::GetPixelFrameBufferSize() noexcept
	{
		return { _bufferSize.width, _bufferSize.height };
	}
}

#undef PIXELMODEFVF

#endif