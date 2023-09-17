#if !defined(GAMEPIXELMODE_H)
#define GAMEPIXELMODE_H

#if defined(GAME_OPENGL)
#include <GL/gl.h>
#endif
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif
#if defined(GAME_DIRECTX12)
#include "d3dx12.h"
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
		void LineClip(const int32_t x1, const int32_t y1, const int32_t x2,  int32_t y2, const Color& color) noexcept;
		void Circle(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept;
		void CircleClip(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept;
		void CircleFilled(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept;
		void CircleFilledClip(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept;
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
			{0.0f, 1.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
			// tr
			{0.5f, -0.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, .0f},
			// bl
			{-0.5f, -0.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
			// br
			{0.5f, 0.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		};
		ID3D10Buffer* _vertexBuffer10;
		ID3D10Buffer* _indexBuffer10;
		Shader _pixelModeShader10;
		ID3D10InputLayout* _vertexLayout10;
		ID3D10ShaderResourceView* _textureShaderResourceView0_10;
		ID3D10ShaderResourceView* _textureShaderResourceView1_10;
		ID3D10SamplerState* _textureSamplerState10;
#endif
#if defined(GAME_DIRECTX11)
		struct _vertex11
		{
			float_t x, y, z;
			float_t r, g, b, a;
			float_t u, v;
		};
		_vertex11 _quadVertices11[4] =
		{
			// tl
			{0.0f, 1.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
			// tr
			{0.5f, -0.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, .0f},
			// bl
			{-0.5f, -0.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
			// br
			{0.5f, 0.5f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		};
		ID3D11Buffer* _vertexBuffer11;
		ID3D11Buffer* _indexBuffer11;
		Shader _pixelModeShader11;
		ID3D11InputLayout* _vertexLayout11;
		ID3D11ShaderResourceView* _textureShaderResourceView0_11;
		ID3D11ShaderResourceView* _textureShaderResourceView1_11;
		ID3D11SamplerState* _textureSamplerState11;
#endif
#if defined(GAME_DIRECTX12)
		struct _vertex12
		{
			float_t x, y, z;
			float_t r, g, b, a;
			float_t u, v;
		};
		Shader _pixelModeShader12;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineStateObject; // pso containing a pipeline state
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature; // root signature defines data shaders will access
		Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer; // a default buffer in GPU memory that we will load vertex data for our triangle into
		D3D12_VIEWPORT _viewport; // area that output from rasterizer will be stretched to.
		D3D12_RECT _scissorRect; // the area to draw in. pixels outside that area will not be drawn onto


		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {}; // a structure containing a pointer to the vertex data in gpu memory
		// the total size of the buffer, and the size of each element (vertex)

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
		_indexBuffer10 = nullptr;
		_textureShaderResourceView0_10 = nullptr;
		_textureShaderResourceView1_10 = nullptr;
		_textureSamplerState10 = nullptr;
#endif
#if defined(GAME_DIRECTX11)
		_vertexBuffer11 = nullptr;
		_vertexLayout11 = nullptr;
		_indexBuffer11 = nullptr;
		_textureShaderResourceView0_11 = nullptr;
		_textureShaderResourceView1_11 = nullptr;
		_textureSamplerState11 = nullptr;
#endif
#if defined(GAME_DIRECTX12)
		_scissorRect = {};
		_viewport = {};
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
			SAFE_RELEASE(_vertexBuffer10);
			SAFE_RELEASE(_vertexLayout10);
			SAFE_RELEASE(_indexBuffer10);
			//if (_vertexBuffer10)
			//{
			//	_vertexBuffer10->Release();
			//	_vertexBuffer10 = nullptr;
			//}
			//if (_vertexLayout10)
			//{
			//	_vertexLayout10->Release();
			//	_vertexLayout10 = nullptr;
			//}
			//if (_indexBuffer)
			//{
			//	_indexBuffer->Release();
			//	_indexBuffer = nullptr;
			//}
			enginePointer->geUnLoadShader(_pixelModeShader10);
			SAFE_RELEASE(_textureShaderResourceView0_10);
			SAFE_RELEASE(_textureShaderResourceView1_10);
			SAFE_RELEASE(_textureSamplerState10);
		}
#endif
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			SAFE_RELEASE(_vertexBuffer11);
			SAFE_RELEASE(_vertexLayout11);
			SAFE_RELEASE(_indexBuffer11);
			//if (_vertexBuffer11)
			//{
			//	_vertexBuffer11->Release();
			//	_vertexBuffer11 = nullptr;
			//}
			//if (_vertexLayout11)
			//{
			//	_vertexLayout11->Release();
			//	_vertexLayout11 = nullptr;
			//}
			//if (_indexBuffer11)
			//{
			//	_indexBuffer11->Release();
			//	_indexBuffer11 = nullptr;
			//}
			enginePointer->geUnLoadShader(_pixelModeShader11);
			SAFE_RELEASE(_textureShaderResourceView0_11);
			SAFE_RELEASE(_textureShaderResourceView1_11);
			SAFE_RELEASE(_textureSamplerState11);
		}
#endif
#if defined(GAME_DIRECTX12)
		if (enginePointer->geIsUsing(GAME_DIRECTX12))
		{
			enginePointer->geUnLoadShader(_pixelModeShader12);
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
				//return false;
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
			if (!enginePointer->geLoadShader("Content/VertexShader.hlsl", "Content/PixelShader.hlsl", _pixelModeShader10))
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
				enginePointer->geUnLoadShader(_pixelModeShader10);
				return false;
			}

			// Create index buffer
			indexBufferDescription.Usage = D3D10_USAGE_IMMUTABLE;
			indexBufferDescription.ByteWidth = sizeof(DWORD) * 2 * 3;
			indexBufferDescription.BindFlags = D3D10_BIND_INDEX_BUFFER;
			indexBufferDescription.CPUAccessFlags = 0;
			indexBufferDescription.MiscFlags = 0;
			indexInitialData.pSysMem = indices;
			if (FAILED(enginePointer->d3d10Device->CreateBuffer(&indexBufferDescription, &indexInitialData, &_indexBuffer10)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create index buffer for PixelMode." };
				_vertexBuffer10->Release();
				_vertexBuffer10 = nullptr;
				enginePointer->geUnLoadShader(_pixelModeShader10);
				return false;
			}

			// Create input layout for shaders
			if (FAILED(enginePointer->d3d10Device->CreateInputLayout(inputLayout, 3, _pixelModeShader10.compiledVertexShader10->GetBufferPointer(), _pixelModeShader10.compiledVertexShader10->GetBufferSize(), &_vertexLayout10)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create input layout for PixelMode." };
				_indexBuffer10->Release();
				_indexBuffer10 = nullptr;
				_vertexBuffer10->Release();
				_vertexBuffer10 = nullptr;
				enginePointer->geUnLoadShader(_pixelModeShader10);
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
			if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(_frameBuffer[0].textureInterface10, &srDesc, &_textureShaderResourceView0_10)))
			{
				std::cout << "CreateSRV0 failed!\n";
			}
			if (FAILED(enginePointer->d3d10Device->CreateShaderResourceView(_frameBuffer[1].textureInterface10, &srDesc, &_textureShaderResourceView1_10)))
			{
				std::cout << "CreateSRV1 failed!\n";
			}
		}
#endif
#if defined (GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			D3D11_BUFFER_DESC vertexBufferDescription = { 0 };
			D3D11_BUFFER_DESC indexBufferDescription = { 0 };
			D3D11_SUBRESOURCE_DATA vertexInitialData = { 0 };
			D3D11_SUBRESOURCE_DATA indexInitialData = { 0 };
			DWORD indices[] = { 0, 1, 2, 1, 3, 2, };
			D3D11_INPUT_ELEMENT_DESC inputLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			// Load shaders for sprite mode
			if (!enginePointer->geLoadShader("Content/VertexShader.hlsl", "Content/PixelShader.hlsl", _pixelModeShader11))
			{
				return false;
			}

			// Create the vertex buffer
			vertexBufferDescription.ByteWidth = sizeof(_vertex11) * 4;
			vertexBufferDescription.Usage = D3D11_USAGE_DYNAMIC;// D3D10_USAGE_DEFAULT;
			vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //0;
			vertexBufferDescription.MiscFlags = 0;
			vertexInitialData.pSysMem = _quadVertices11;
			if (FAILED(enginePointer->d3d11Device->CreateBuffer(&vertexBufferDescription, &vertexInitialData, &_vertexBuffer11)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create vertex buffer for PixelMode." };
				enginePointer->geUnLoadShader(_pixelModeShader11);
				return false;
			}

			// Create index buffer
			indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDescription.ByteWidth = sizeof(DWORD) * 2 * 3;
			indexBufferDescription.BindFlags = D3D10_BIND_INDEX_BUFFER;
			indexBufferDescription.CPUAccessFlags = 0;
			indexBufferDescription.MiscFlags = 0;
			indexInitialData.pSysMem = indices;
			if (FAILED(enginePointer->d3d11Device->CreateBuffer(&indexBufferDescription, &indexInitialData, &_indexBuffer11)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not create index buffer for PixelMode." };
				SAFE_RELEASE(_vertexBuffer11);
				//_vertexBuffer11->Release();
				//_vertexBuffer11 = nullptr;
				enginePointer->geUnLoadShader(_pixelModeShader11);
				return false;
			}

			// Create input layout for shaders
			if (FAILED(enginePointer->d3d11Device->CreateInputLayout(inputLayout, 3, _pixelModeShader11.compiledVertexShader11->GetBufferPointer(), _pixelModeShader11.compiledVertexShader11->GetBufferSize(), &_vertexLayout11)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not create input layout for PixelMode." };
				SAFE_RELEASE(_indexBuffer11);
				SAFE_RELEASE(_vertexBuffer11);
				//_indexBuffer11->Release();
				//_indexBuffer11 = nullptr;
				//_vertexBuffer11->Release();
				//_vertexBuffer11 = nullptr;
				enginePointer->geUnLoadShader(_pixelModeShader11);
				return false;
			}

			D3D11_SAMPLER_DESC samplerDesc = { };
			//ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			if (FAILED(enginePointer->d3d11Device->CreateSamplerState(&samplerDesc, &_textureSamplerState11)))
			{
				std::cout << "Create sampler failed!\n";
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC srDesc = {};
			srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
			srDesc.Texture2D.MostDetailedMip = 0;
			srDesc.Texture2D.MipLevels = 1;
			if (FAILED(enginePointer->d3d11Device->CreateShaderResourceView(_frameBuffer[0].textureInterface11, &srDesc, &_textureShaderResourceView0_11)))
			{
				std::cout << "CreateSRV0 failed!\n";
			}
			if (FAILED(enginePointer->d3d11Device->CreateShaderResourceView(_frameBuffer[1].textureInterface11, &srDesc, &_textureShaderResourceView1_11)))
			{
				std::cout << "CreateSRV1 failed!\n";
			}
		}
#endif
#if defined (GAME_DIRECTX12)
		if (enginePointer->geIsUsing(GAME_DIRECTX12))
		{
			// create root signature

			D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
			rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			rootSignatureDesc.NumParameters = 0;
			rootSignatureDesc.pParameters = nullptr;
			rootSignatureDesc.NumStaticSamplers = 0;
			rootSignatureDesc.pStaticSamplers = nullptr;
			

			Microsoft::WRL::ComPtr<ID3DBlob> signature;
			if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr)))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not serialize root signature in PixleMode." };
				//std::cout << "Serialize of root signature failed in PixelMode!\n";
				return false;
			}

			if (FAILED(enginePointer->d3d12Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not create root signature in PixelMode." };
				return false;
			}
			_rootSignature->SetName(L"PixelMode Root Signature");

			// Load shaders for sprite mode
			if (!enginePointer->geLoadShader("Content/VertexShader.hlsl", "Content/PixelShader.hlsl", _pixelModeShader12))
			{
				return false;
			}

			// create input layout
			//D3D12_INPUT_ELEMENT_DESC inputLayout[] =
			//{
			//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			//	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			//};
			D3D12_INPUT_ELEMENT_DESC inputLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			// fill out an input layout description structure
			D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};

			// we can get the number of elements in an array by "sizeof(array) / sizeof(arrayElementType)"
			inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
			inputLayoutDesc.pInputElementDescs = inputLayout;

			
			
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; // a structure to define a pso
			psoDesc.InputLayout = inputLayoutDesc; // the structure describing our input layout
			psoDesc.pRootSignature = _rootSignature.Get(); // the root signature that describes the input data this pso needs
			psoDesc.VS = _pixelModeShader12.vertexShader12; // structure describing where to find the vertex shader bytecode and how large it is
			psoDesc.PS = _pixelModeShader12.pixelShader12; // same as VS but for pixel shader
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the render target
			DXGI_SAMPLE_DESC sampleDesc = {};
			sampleDesc.Count = 1;
			psoDesc.SampleDesc = sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
			psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
			D3D12_RASTERIZER_DESC rasterDesc = {};
			rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
			rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
			rasterDesc.FrontCounterClockwise = FALSE;
			rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			rasterDesc.DepthClipEnable = TRUE;
			rasterDesc.MultisampleEnable = FALSE;
			rasterDesc.AntialiasedLineEnable = FALSE;
			rasterDesc.ForcedSampleCount = 0;
			rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			psoDesc.RasterizerState = rasterDesc;
			D3D12_BLEND_DESC blendDesc = {};
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
			{
				FALSE,FALSE,
				D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP_NOOP,
				D3D12_COLOR_WRITE_ENABLE_ALL,
			};
			for (uint32_t target = 0; target < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; target++)
				blendDesc.RenderTarget[target] = defaultRenderTargetBlendDesc;
			psoDesc.BlendState = blendDesc;
			psoDesc.NumRenderTargets = 1; // we are only binding one render target
			psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;// D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG; only works with warp

			// create the pso
			HRESULT hr = 0;
			hr = enginePointer->d3d12Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_pipelineStateObject));
			if (FAILED(hr))
			{
				lastError = { GameErrors::GameDirectX12Specific,"Could not create pipline state for PixelMode." };
				// lastError.string += the hr error
				if (hr == D3D12_ERROR_ADAPTER_NOT_FOUND)
					lastError.lastErrorString  += ": D3D12_ERROR_ADAPTER_NOT_FOUND";
				else if (hr == D3D12_ERROR_DRIVER_VERSION_MISMATCH)
					lastError.lastErrorString += ": D3D12_ERROR_DRIVER_VERSION_MISMATCH";
				else if (hr == DXGI_ERROR_INVALID_CALL)
					lastError.lastErrorString += ": DXGI_ERROR_INVALID_CALL";
				else if (hr == DXGI_ERROR_WAS_STILL_DRAWING)
					lastError.lastErrorString += ": DXGI_ERROR_WAS_STILL_DRAWING";
				else if (hr == E_FAIL)
					lastError.lastErrorString += ": E_FAIL";
				else if (hr == E_INVALIDARG)
					lastError.lastErrorString += ": E_INVALIDARG";
				else if (hr == E_OUTOFMEMORY)
					lastError.lastErrorString += ": E_OUTOFMEMORY";
				else if (hr == E_NOTIMPL)
					lastError.lastErrorString += ": E_NOTIMPL";
				//lastError = { GameErrors::GameDirectX12Specific, "Could not create graphics pipeline state." };
				return false;
			}
			_pipelineStateObject->SetName(L"PixelMode PSO");

			// Create vertex buffer

			// a triangle
			struct Vertextemp
			{
				float_t x;
				float_t y;
				float_t z;
				float_t r;
				float_t g;
				float_t b;
				float_t a;
			};
			Vertextemp vList[3] =
			{
				{ 0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
				{0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
				{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f }
			};

			int vBufferSize = sizeof(vList);

			// create default heap
			// default heap is memory on the GPU. Only the GPU has access to this memory
			// To get data into this heap, we will have to upload the data using
			// an upload heap
			D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(vBufferSize);
			hr = enginePointer->d3d12Device->CreateCommittedResource(
				&heapProp, // a default heap
				D3D12_HEAP_FLAG_NONE, // no flags
				&resDesc, // resource description for a buffer
				D3D12_RESOURCE_STATE_COMMON,//D3D12_RESOURCE_STATE_COPY_DEST, // we will start this heap in the copy destination state since we will copy data
				// from the upload heap to this heap
				nullptr, // optimized clear value must be null for this type of resource. used for render targets and depth/stencil buffers
				IID_PPV_ARGS(&_vertexBuffer));
			if (FAILED(hr))
			{
				lastError = { GameErrors::GameDirectX12Specific,"Could not create vertex buffer resource heap for PixelMode." };
				// lastError.string += the hr error
				if (hr == D3D12_ERROR_ADAPTER_NOT_FOUND)
					lastError.lastErrorString += ": D3D12_ERROR_ADAPTER_NOT_FOUND";
				else if (hr == D3D12_ERROR_DRIVER_VERSION_MISMATCH)
					lastError.lastErrorString += ": D3D12_ERROR_DRIVER_VERSION_MISMATCH";
				else if (hr == DXGI_ERROR_INVALID_CALL)
					lastError.lastErrorString += ": DXGI_ERROR_INVALID_CALL";
				else if (hr == DXGI_ERROR_WAS_STILL_DRAWING)
					lastError.lastErrorString += ": DXGI_ERROR_WAS_STILL_DRAWING";
				else if (hr == E_FAIL)
					lastError.lastErrorString += ": E_FAIL";
				else if (hr == E_INVALIDARG)
					lastError.lastErrorString += ": E_INVALIDARG";
				else if (hr == E_OUTOFMEMORY)
					lastError.lastErrorString += ": E_OUTOFMEMORY";
				else if (hr == E_NOTIMPL)
					lastError.lastErrorString += ": E_NOTIMPL";
				//lastError = { GameErrors::GameDirectX12Specific, "Could not create graphics pipeline state." };
				return false;
			}
			// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
			_vertexBuffer->SetName(L"Vertex Buffer Resource Heap");

			// PROBABLY NEED TO KEEP THIS
			// create upload heap
			// upload heaps are used to upload data to the GPU. CPU can write to it, GPU can read from it
			// We will upload the vertex buffer using this heap to the default heap
			heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			resDesc = CD3DX12_RESOURCE_DESC::Buffer(vBufferSize);
			ID3D12Resource* vBufferUploadHeap;
			hr = enginePointer->d3d12Device->CreateCommittedResource(
				&heapProp, // upload heap
				D3D12_HEAP_FLAG_NONE, // no flags
				&resDesc, // resource description for a buffer
				D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
				nullptr,
				IID_PPV_ARGS(&vBufferUploadHeap));
			if (FAILED(hr))
			{
				lastError = { GameErrors::GameDirectX12Specific,"Could not create vertex buffer upload heap for PixelMode." };
				// lastError.string += the hr error
				if (hr == D3D12_ERROR_ADAPTER_NOT_FOUND)
					lastError.lastErrorString += ": D3D12_ERROR_ADAPTER_NOT_FOUND";
				else if (hr == D3D12_ERROR_DRIVER_VERSION_MISMATCH)
					lastError.lastErrorString += ": D3D12_ERROR_DRIVER_VERSION_MISMATCH";
				else if (hr == DXGI_ERROR_INVALID_CALL)
					lastError.lastErrorString += ": DXGI_ERROR_INVALID_CALL";
				else if (hr == DXGI_ERROR_WAS_STILL_DRAWING)
					lastError.lastErrorString += ": DXGI_ERROR_WAS_STILL_DRAWING";
				else if (hr == E_FAIL)
					lastError.lastErrorString += ": E_FAIL";
				else if (hr == E_INVALIDARG)
					lastError.lastErrorString += ": E_INVALIDARG";
				else if (hr == E_OUTOFMEMORY)
					lastError.lastErrorString += ": E_OUTOFMEMORY";
				else if (hr == E_NOTIMPL)
					lastError.lastErrorString += ": E_NOTIMPL";
				//lastError = { GameErrors::GameDirectX12Specific, "Could not create graphics pipeline state." };
				return false;
			}
			vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

			RendererDX12* temp = enginePointer->geGetRenderer();


			// resets the command list -----------------------------
			if (FAILED(temp->_commandAllocator[temp->_frameIndex]->Reset()))
			{
				//Running = false;
				std::cout << "Command allocator reset failed\n";
			}

			// reset the command list. by resetting the command list we are putting it into
			// a recording state so we can start recording commands into the command allocator.
			// the command allocator that we reference here may have multiple command lists
			// associated with it, but only one can be recording at any time. Make sure
			// that any other command lists associated to this command allocator are in
			// the closed state (not recording).
			// Here you will pass an initial pipeline state object as the second parameter,
			// but in this tutorial we are only clearing the rtv, and do not actually need
			// anything but an initial default pipeline, which is what we get by setting
			// the second parameter to NULL
			if (FAILED(enginePointer->commandList->Reset(temp->_commandAllocator[temp->_frameIndex].Get(), _pipelineStateObject.Get())))
			{
				std::cout << "command list reset failed\n";
				//Running = false;
			}

			// end of command list reset ---------------------------------------
			
			// store vertex buffer in upload heap
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = reinterpret_cast<BYTE*>(vList); // pointer to our vertex array
			vertexData.RowPitch = vBufferSize; // size of all our triangle vertex data
			vertexData.SlicePitch = vBufferSize; // also the size of our triangle vertex data

			// we are now creating a command with the command list to copy the data from
			// the upload heap to the default heap
			UpdateSubresources(enginePointer->commandList.Get(), _vertexBuffer.Get(), vBufferUploadHeap, 0, 0, 1, &vertexData);

			// transition the vertex buffer data from copy destination state to vertex buffer state
			D3D12_RESOURCE_BARRIER resBar = CD3DX12_RESOURCE_BARRIER::Transition(_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			enginePointer->commandList->ResourceBarrier(1, &resBar);

			// Now we execute the command list to upload the initial assets (triangle data)
			enginePointer->commandList->Close();
			ID3D12CommandList* ppCommandLists[] = { enginePointer->commandList.Get() };
			enginePointer->commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			// increment the fence value now, otherwise the buffer might not be uploaded by the time we start drawing

			temp->_fenceValue[temp->_frameIndex]++;
			hr = enginePointer->commandQueue->Signal(temp->_fence[temp->_frameIndex].Get(), temp->_fenceValue[temp->_frameIndex]);
			if (FAILED(hr))
			{
				lastError = { GameErrors::GameDirectX12Specific,"Pixel mode signal failed." };
				// lastError.string += the hr error
				if (hr == D3D12_ERROR_ADAPTER_NOT_FOUND)
					lastError.lastErrorString += ": D3D12_ERROR_ADAPTER_NOT_FOUND";
				else if (hr == D3D12_ERROR_DRIVER_VERSION_MISMATCH)
					lastError.lastErrorString += ": D3D12_ERROR_DRIVER_VERSION_MISMATCH";
				else if (hr == DXGI_ERROR_INVALID_CALL)
					lastError.lastErrorString += ": DXGI_ERROR_INVALID_CALL";
				else if (hr == DXGI_ERROR_WAS_STILL_DRAWING)
					lastError.lastErrorString += ": DXGI_ERROR_WAS_STILL_DRAWING";
				else if (hr == E_FAIL)
					lastError.lastErrorString += ": E_FAIL";
				else if (hr == E_INVALIDARG)
					lastError.lastErrorString += ": E_INVALIDARG";
				else if (hr == E_OUTOFMEMORY)
					lastError.lastErrorString += ": E_OUTOFMEMORY";
				else if (hr == E_NOTIMPL)
					lastError.lastErrorString += ": E_NOTIMPL";
				//lastError = { GameErrors::GameDirectX12Specific, "Could not create graphics pipeline state." };
				return false;
			}

			// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
			vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
			vertexBufferView.StrideInBytes = sizeof(Vertextemp);
			vertexBufferView.SizeInBytes = vBufferSize;

			// Fill out the Viewport
			_viewport.TopLeftX = 0;
			_viewport.TopLeftY = 0;
			Vector2i t = enginePointer->geGetWindowSize();
			_viewport.Width = (float_t)t.width;
			_viewport.Height = (float_t)t.height;
			_viewport.MinDepth = 0.0f;
			_viewport.MaxDepth = 1.0f;

			// Fill out a scissor rect
			_scissorRect.left = 0;
			_scissorRect.top = 0;
			_scissorRect.right = t.width;
			_scissorRect.bottom = t.height;



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
			D3D11_MAPPED_SUBRESOURCE data;
			if (FAILED(enginePointer->d3d11DeviceContext->Map(_frameBuffer[_currentBuffer].textureInterface11, 0, D3D11_MAP_WRITE_DISCARD, 0, &data)))
			{
				std::cout << "Could not map framebuffer in spritebatch\n.";
			}
			memcpy(data.pData, (unsigned char*)_video, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			enginePointer->d3d11DeviceContext->Unmap(_frameBuffer[_currentBuffer].textureInterface11, 0);
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
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			// Homoginize the scaled rect to -1 to 1 range using
			_positionOfScaledTexture.x = (_positionOfScaledTexture.x * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_positionOfScaledTexture.y = (_positionOfScaledTexture.y * 2.0f / (float_t)_windowSize.height) - 1.0f;
			_sizeOfScaledTexture.width = ((float_t)_sizeOfScaledTexture.width * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_sizeOfScaledTexture.height = ((float_t)_sizeOfScaledTexture.height * 2.0f / (float_t)_windowSize.height) - 1.0f;
			_positionOfScaledTexture.y = -_positionOfScaledTexture.y;
			_sizeOfScaledTexture.height = -_sizeOfScaledTexture.height;

			// tl
			_quadVertices11[0].x = _positionOfScaledTexture.x;
			_quadVertices11[0].y = _positionOfScaledTexture.y;
			// tr
			_quadVertices11[1].x = _sizeOfScaledTexture.width;
			_quadVertices11[1].y = _positionOfScaledTexture.y;
			// bl
			_quadVertices11[2].x = _positionOfScaledTexture.x;
			_quadVertices11[2].y = _sizeOfScaledTexture.height;

			// br
			_quadVertices11[3].x = _sizeOfScaledTexture.width;
			_quadVertices11[3].y = _sizeOfScaledTexture.height;

			//VOID* pVoid = nullptr;
			//_vertexBuffer11->Map(D3D10_MAP_WRITE_DISCARD, 0, &pVoid);
			//memcpy(pVoid, _quadVertices10, sizeof(_quadVertices10));
			//_vertexBuffer11->Unmap();

			D3D11_MAPPED_SUBRESOURCE data;
			if (FAILED(enginePointer->d3d11DeviceContext->Map(_vertexBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &data)))
			{
				std::cout << "Could not map vertexbuffer in spritebatch\n.";
			}
			memcpy(data.pData, _quadVertices11, sizeof(_quadVertices11));
			enginePointer->d3d11DeviceContext->Unmap(_vertexBuffer11, 0);
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
			enginePointer->d3d10Device->IASetIndexBuffer(_indexBuffer10, DXGI_FORMAT_R32_UINT, 0);
			enginePointer->d3d10Device->IASetVertexBuffers(0, 1, &_vertexBuffer10, &stride, &offset);
			enginePointer->d3d10Device->IASetInputLayout(_vertexLayout10);
			enginePointer->d3d10Device->VSSetShader(_pixelModeShader10.vertexShader10);
			enginePointer->d3d10Device->PSSetShader(_pixelModeShader10.pixelShader10);
			enginePointer->d3d10Device->PSSetSamplers(0, 1, &_textureSamplerState10);
			enginePointer->d3d10Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (!_currentBuffer)
			{
				enginePointer->d3d10Device->PSSetShaderResources(0, 1, &_textureShaderResourceView0_10);
			}
			else
			{
				enginePointer->d3d10Device->PSSetShaderResources(0, 1, &_textureShaderResourceView1_10);
			}

			enginePointer->d3d10Device->DrawIndexed(6, 0, 0);
			
			// Restore old states
			enginePointer->d3d10Device->IASetIndexBuffer(oldIndexBuffer, oldIndexFormat, oldIndexOffset);
			enginePointer->d3d10Device->IASetVertexBuffers(0, 1, &oldVertexBuffer, &oldStride, &oldOffset);
			enginePointer->d3d10Device->IASetInputLayout(oldInputLayout);
			enginePointer->d3d10Device->VSSetShader(oldVertexShader);
			enginePointer->d3d10Device->PSSetShader(oldPixelShader);
			enginePointer->d3d10Device->PSSetSamplers(0, 1, &oldTextureSamplerState);
			if (oldPrimitiveTopology != D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED)
			{
				enginePointer->d3d10Device->IASetPrimitiveTopology(oldPrimitiveTopology);
			}
		}
#endif
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			uint32_t stride = sizeof(_vertex11);
			uint32_t oldStride = 0;
			uint32_t offset = 0;
			uint32_t oldOffset = 0;
			ID3D11Buffer* oldVertexBuffer = nullptr;
			ID3D11Buffer* oldIndexBuffer = nullptr;
			DXGI_FORMAT oldIndexFormat = {};
			uint32_t oldIndexOffset = 0;
			ID3D11InputLayout* oldInputLayout = nullptr;
			ID3D11VertexShader* oldVertexShader = nullptr;
			ID3D11PixelShader* oldPixelShader = nullptr;
			ID3D11SamplerState* oldTextureSamplerState = nullptr;
			D3D11_PRIMITIVE_TOPOLOGY oldPrimitiveTopology = {};


			// Save everything we modify
			enginePointer->d3d11DeviceContext->IAGetIndexBuffer(&oldIndexBuffer, &oldIndexFormat, &oldIndexOffset);
			enginePointer->d3d11DeviceContext->IAGetVertexBuffers(0, 1, &oldVertexBuffer, &oldStride, &oldOffset);
			enginePointer->d3d11DeviceContext->IAGetInputLayout(&oldInputLayout);
			enginePointer->d3d11DeviceContext->VSGetShader(&oldVertexShader,NULL,NULL);
			enginePointer->d3d11DeviceContext->PSGetShader(&oldPixelShader,NULL,NULL);
			enginePointer->d3d11DeviceContext->PSGetSamplers(0, 1, &oldTextureSamplerState);
			enginePointer->d3d11DeviceContext->IAGetPrimitiveTopology(&oldPrimitiveTopology);


			// Change what we need
			enginePointer->d3d11DeviceContext->IASetIndexBuffer(_indexBuffer11, DXGI_FORMAT_R32_UINT, 0);
			enginePointer->d3d11DeviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer11, &stride, &offset);
			enginePointer->d3d11DeviceContext->IASetInputLayout(_vertexLayout11);
			enginePointer->d3d11DeviceContext->VSSetShader(_pixelModeShader11.vertexShader11,NULL,NULL);
			enginePointer->d3d11DeviceContext->PSSetShader(_pixelModeShader11.pixelShader11,NULL,NULL);
			enginePointer->d3d11DeviceContext->PSSetSamplers(0, 1, &_textureSamplerState11);
			enginePointer->d3d11DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (!_currentBuffer)
			{
				enginePointer->d3d11DeviceContext->PSSetShaderResources(0, 1, &_textureShaderResourceView0_11);
			}
			else
			{
				enginePointer->d3d11DeviceContext->PSSetShaderResources(0, 1, &_textureShaderResourceView1_11);
			}

			enginePointer->d3d11DeviceContext->DrawIndexed(6, 0, 0);

			// Restore old states
			enginePointer->d3d11DeviceContext->IASetIndexBuffer(oldIndexBuffer, oldIndexFormat, oldIndexOffset);
			enginePointer->d3d11DeviceContext->IASetVertexBuffers(0, 1, &oldVertexBuffer, &oldStride, &oldOffset);
			enginePointer->d3d11DeviceContext->IASetInputLayout(oldInputLayout);
			enginePointer->d3d11DeviceContext->VSSetShader(oldVertexShader,NULL,NULL);
			enginePointer->d3d11DeviceContext->PSSetShader(oldPixelShader,NULL,NULL);
			enginePointer->d3d11DeviceContext->PSSetSamplers(0, 1, &oldTextureSamplerState);
			if (oldPrimitiveTopology != D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
			{
				enginePointer->d3d11DeviceContext->IASetPrimitiveTopology(oldPrimitiveTopology);
			}
		}
#endif
#if defined(GAME_DIRECTX12)
		if (enginePointer->geIsUsing(GAME_DIRECTX12))
		{
			// draw triangle
			enginePointer->commandList->SetPipelineState(_pipelineStateObject.Get());
			enginePointer->commandList->SetGraphicsRootSignature(_rootSignature.Get()); // set the root signature
			enginePointer->commandList->RSSetViewports(1, &_viewport); // set the viewports
			enginePointer->commandList->RSSetScissorRects(1, &_scissorRect); // set the scissor rects
			enginePointer->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // set the primitive topology
			enginePointer->commandList->IASetVertexBuffers(0, 1, &vertexBufferView); // set the vertex buffer (using the vertex buffer view)
			enginePointer->commandList->DrawInstanced(3, 1, 0, 0); // finally draw 3 vertices (draw the triangle)
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
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
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
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
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
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
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

	inline void PixelMode::Line(int32_t x1, int32_t y1, const int32_t x2, const int32_t y2, const Color& color) noexcept
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

	inline void PixelMode::LineClip(const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2, const Color& color) noexcept
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

	inline void PixelMode::Circle(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

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

	inline void PixelMode::CircleClip(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept
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

	inline void PixelMode::CircleFilled(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept
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

	inline void PixelMode::CircleFilledClip(const int32_t x, const int32_t y, const int32_t radius, const Color& color) noexcept
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