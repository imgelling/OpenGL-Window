#if !defined(GAMERENDERERDX11_H)
#define GAMERENDERERDX11_H
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShader.h"
#include "GameSystemInfo.h"
#include "GameTexture2D.h"

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = nullptr; } }
#endif

namespace game
{
	extern SystemInfo systemInfo;
	class RendererDX11 : public RendererBase
	{
	public:
		RendererDX11();
		bool CreateDevice(Window& window);
		void DestroyDevice();
		void Swap();
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset);
		void FillOutRendererInfo() {}
		bool CreateTexture(Texture2D& texture);
		bool LoadTexture(std::string fileName, Texture2D& texture) { lastError = { GameErrors::GameDirectX11Specific, "could not load texture" }; return false; }
		void UnLoadTexture(Texture2D& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader);
		bool LoadShader(const std::string vertex, const std::string fragment, const std::string geometry, Shader& shader)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Geometry shaders not implemented yet." };
			return false;
		}
		void UnLoadShader(Shader& shader);
		void GetDevice(ID3D11Device*& device, ID3D11DeviceContext*& context, ID3D11RenderTargetView*& target, ID3D11DepthStencilView*& depth);
	protected:
		void _ReadExtensions() {};
	private:
		ID3D11Device* _d3d11Device;
		ID3D11DeviceContext *_d3d11DeviceContext;
		IDXGISwapChain* _d3d11SwapChain;
		ID3D11RenderTargetView* _d3d11RenderTargetView;
		ID3D11DepthStencilView* _d3d11DepthStencilView;
		ID3D11Texture2D* _d3d11DepthStencilBuffer;
	};

	inline RendererDX11::RendererDX11()
	{
		_d3d11Device = nullptr;
		_d3d11DeviceContext = nullptr;
		_d3d11SwapChain = nullptr;
		_d3d11RenderTargetView = nullptr;
		_d3d11DepthStencilView = nullptr;
		_d3d11DepthStencilBuffer = nullptr;
	}

	inline bool RendererDX11::CreateDevice(Window& window) 
	{ 
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,	// 0xb100
			D3D_FEATURE_LEVEL_11_0, // 0xb00
			D3D_FEATURE_LEVEL_10_1, // 0xa100
			D3D_FEATURE_LEVEL_10_0, // 0xa000
			D3D_FEATURE_LEVEL_9_3,	// 0x9300
			D3D_FEATURE_LEVEL_9_2,	// 0x9200
			D3D_FEATURE_LEVEL_9_1,	// 0x9100
		};
		uint32_t deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D3D_FEATURE_LEVEL featureLevelCreated;
		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		D3D11_VIEWPORT viewPort = { 0 };
		ID3D11Texture2D* backBuffer = nullptr;
		uint32_t debug = 0;
		D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };

		// back buffer stuff
		scd.BufferDesc.Width = _attributes.WindowWidth;
		scd.BufferDesc.Height = _attributes.WindowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 0; // 60
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// multisampling
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;

		scd.OutputWindow = window.GetHandle();
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0; //DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING for variable refresh rate, needs DXGI_SWAP_EFFECT_FLIP_DISCARD

		if (_attributes.DebugMode)
		{
			deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}

		// Create device and swap chain
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &scd, &_d3d11SwapChain, &_d3d11Device, &featureLevelCreated, &_d3d11DeviceContext)))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create device." };
			return false;
		}

		//std::cout << "Feat = " << featureLevelCreated << " in hex " << 0xb100 << ".\n"; // 0xb100 d3d11_1
		//systemInfo.gpuInfo.versionMajor = featureLevelCreated;

		// Describe depth stencil
		depthStencilDesc.Width = _attributes.WindowWidth;
		depthStencilDesc.Height = _attributes.WindowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;	// can change for reading or writing i think
		depthStencilDesc.MiscFlags = 0;
		
		// Create depth stencil buffer texture
		if (FAILED(_d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &_d3d11DepthStencilBuffer)))
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create depth stencil buffer texture." };
			DestroyDevice();
			return false;
		}

		// Second param is states for depth stencil
		if (FAILED(_d3d11Device->CreateDepthStencilView(_d3d11DepthStencilBuffer, NULL, &_d3d11DepthStencilView)))
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create depth stencil view." };
			return false;
		}

		// may be error here may work, if works, need to remove from class and just have it here
		SAFE_RELEASE(_d3d11DepthStencilBuffer);

		//Create our BackBuffer
		ID3D11Texture2D* BackBuffer = nullptr;
		HRESULT hr;
		if (FAILED(_d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer)))
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create create back buffer." };
			return false;
		}

		//Create our Render Target
		hr = _d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &_d3d11RenderTargetView);
		if (FAILED(hr))
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create render target view." };
			return false;
		}
		// same as depthstencilbuffer
		SAFE_RELEASE(BackBuffer);

		//Set our Render Target
		_d3d11DeviceContext->OMSetRenderTargets(1, &_d3d11RenderTargetView, NULL);

		// Set the viewport
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = (float_t)_attributes.WindowWidth;
		viewPort.Height = (float_t)_attributes.WindowHeight;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;

		_d3d11DeviceContext->RSSetViewports(1, &viewPort);

		
		return true; 
	}
	
	inline void RendererDX11::GetDevice(ID3D11Device*& device, ID3D11DeviceContext*& context, ID3D11RenderTargetView*& target, ID3D11DepthStencilView*& depth)
	{
		device = _d3d11Device;
		context = _d3d11DeviceContext;
		target = _d3d11RenderTargetView;
		depth = _d3d11DepthStencilView;
	}

	inline void RendererDX11::DestroyDevice()
	{
		//if (_d3d11Device) _d3d11Device->Release();
		SAFE_RELEASE(_d3d11Device);
		//if (_d3d11Context) _d3d11Context->Release();
		SAFE_RELEASE(_d3d11DeviceContext);
		//if (_d3d11SwapChain) _d3d11SwapChain->Release();
		SAFE_RELEASE(_d3d11SwapChain);
		//if (_d3d11RenderTarget) _d3d11RenderTarget->Release();
		SAFE_RELEASE(_d3d11RenderTargetView);
		SAFE_RELEASE(_d3d11DepthStencilView);
	}

	inline void RendererDX11::Swap()
	{
		_d3d11SwapChain->Present(0, 0);
	}

	inline void RendererDX11::HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset)
	{
		float color[4] = { 0 };
		D3D11_VIEWPORT viewport = { 0 };
		
		if (!width || !height) return;

		//_d3d11Context->ClearRenderTargetView(_d3d11RenderTarget, color);

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float_t)width;
		viewport.Height = (float_t)height;

		_d3d11DeviceContext->RSSetViewports(1, &viewport);
	}

	inline bool RendererDX11::CreateTexture(Texture2D& texture)
	{
		texture.oneOverWidth = 1.0f / (float_t)texture.width;
		texture.oneOverHeight = 1.0f / (float_t)texture.height;
		texture.isCopy = false;

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = texture.width;
		desc.Height = texture.height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;


		if (FAILED(_d3d11Device->CreateTexture2D(&desc, NULL, &texture.textureInterface11)))
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create texture." };
			return false;
		}
		

		return true;
	}

	inline void RendererDX11::UnLoadTexture(Texture2D& texture)
	{
		SAFE_RELEASE(texture.textureInterface11);
		//if (texture.textureInterface11)
		//{
		//	texture.textureInterface11->Release();
		//	texture.textureInterface11 = nullptr;
		//}
		texture.width = 0;
		texture.height = 0;
		texture.oneOverWidth = 0.0f;
		texture.oneOverHeight = 0.0f;
		texture.componentsPerPixel = 0;
		texture.isCopy = false;
		texture.name = "NULL";
		// Attributes of texture filtering
		texture.isMipMapped = true;
		texture.filterType = TextureFilterType::Trilinear;
		texture.anisotropyLevel = 1;
	}

	inline bool RendererDX11::LoadShader(const std::string vertex, const std::string fragment, Shader& shader)
	{
		if (!shader.isPrecompiled)
		{
			DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
			if (_attributes.DebugMode)
			{
				flags |= D3DCOMPILE_DEBUG;
			}
			ID3DBlob* compiledVertexShader = nullptr;
			ID3DBlob* compiledPixelShader = nullptr;
			ID3DBlob* compilationMsgs = nullptr;

			// Compile the vertex shader
			if (FAILED(D3DCompileFromFile(ConvertToWide(vertex).c_str(), NULL, NULL, "main", "vs_4_0", flags, NULL, &compiledVertexShader, &compilationMsgs)))
			{
				SIZE_T size = compilationMsgs->GetBufferSize();
				uint8_t* p = reinterpret_cast<unsigned char*>(compilationMsgs->GetBufferPointer());
				lastError = { GameErrors::GameDirectX11Specific,"Could not compile shader \"" + vertex + "\".\n" };
				for (uint32_t bytes = 0; bytes < size; bytes++)
				{
					lastError.lastErrorString += p[bytes];
				}
				SAFE_RELEASE(compilationMsgs);
				SAFE_RELEASE(compiledVertexShader);
				return false;
			}

			// Compile the pixel shader
			if (FAILED(D3DCompileFromFile(ConvertToWide(fragment).c_str(), NULL, NULL, "main", "ps_4_0", flags, NULL, &compiledPixelShader, &compilationMsgs)))
			{
				SIZE_T size = compilationMsgs->GetBufferSize();
				auto* p = reinterpret_cast<unsigned char*>(compilationMsgs->GetBufferPointer());
				lastError = { GameErrors::GameDirectX11Specific,"Could not compile shader \"" + fragment + "\".\n" };
				for (uint32_t bytes = 0; bytes < size; bytes++)
				{
					lastError.lastErrorString += p[bytes];
				}
				SAFE_RELEASE(compilationMsgs);
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(compiledPixelShader);
				return false;
			}

			// Free up any messages from compilation if any
			SAFE_RELEASE(compilationMsgs);

			// Create vertex shader
			if (FAILED(_d3d11Device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &shader.vertexShader11)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not create vertex shader from \"" + vertex + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(compiledPixelShader);
				SAFE_RELEASE(shader.pixelShader11);

				return false;
			}

			// Create pixel shader
			if (FAILED(_d3d11Device->CreatePixelShader((DWORD*)(compiledPixelShader->GetBufferPointer()), compiledPixelShader->GetBufferSize(), NULL, &shader.pixelShader11)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not create pixel shader from \"" + fragment + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(compiledPixelShader);
				SAFE_RELEASE(shader.vertexShader11);
				return false;
			}

			// Shaders created, save a reference and release this one
			compiledVertexShader->AddRef();
			shader.compiledVertexShader11 = compiledVertexShader;
			SAFE_RELEASE(compiledVertexShader);

			compiledPixelShader->AddRef();
			shader.compiledPixelShader11 = compiledPixelShader;
			SAFE_RELEASE(compiledPixelShader);

			return true;
		}
		else
		{
			// Load compiled vertex shader
			ID3DBlob* compiledPixelShader = nullptr;
			ID3DBlob* compiledVertexShader = nullptr;

			// Create vertex shader
			if (FAILED(D3DReadFileToBlob((ConvertToWide(vertex).c_str()), &compiledVertexShader)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not read vertex file \"" + vertex + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				return false;
			}
			HRESULT hr = _d3d11Device->CreateVertexShader((DWORD*)compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &shader.vertexShader11);
			if (FAILED(hr))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not create vertex shader from \"" + vertex + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				return false;
			}

			// Create pixel shader
			if (FAILED(D3DReadFileToBlob((ConvertToWide(fragment).c_str()), &compiledPixelShader)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not read pixel file \"" + fragment + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(shader.vertexShader11);
				SAFE_RELEASE(compiledPixelShader);
			}

			if (FAILED(_d3d11Device->CreatePixelShader((DWORD*)compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &shader.pixelShader11)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not create pixel shader from \"" + fragment + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(shader.vertexShader11);
				SAFE_RELEASE(compiledPixelShader);
				SAFE_RELEASE(shader.pixelShader11);
				return false;
			}

			// Shaders created, save a reference and release this one
			compiledVertexShader->AddRef();
			shader.compiledVertexShader11 = compiledVertexShader;
			SAFE_RELEASE(compiledVertexShader);

			compiledPixelShader->AddRef();
			shader.compiledPixelShader11 = compiledPixelShader;
			SAFE_RELEASE(compiledPixelShader);
		}
		return true;
	}

	inline void RendererDX11::UnLoadShader(Shader& shader)
	{
		SAFE_RELEASE(shader.compiledVertexShader11);
		SAFE_RELEASE(shader.vertexShader11);
		SAFE_RELEASE(shader.compiledPixelShader11);
		SAFE_RELEASE(shader.pixelShader11);
		SAFE_RELEASE(shader.compiledGeometryShader11);
		SAFE_RELEASE(shader.geometryShader11);
	}
}

#endif