#if !defined(GAMERENDERERDX10_H)
#define GAMERENDERERDX10_H

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = nullptr; } }
#endif

#include <d3d10.h>
#include <d3dcompiler.h>

#include "GameColor.h"
#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShader.h"
#include "GameSystemInfo.h"
#include "GameTexture2D.h"


namespace game
{
	extern SystemInfo systemInfo;
	class RendererDX10 : public RendererBase
	{
	public:
		RendererDX10();
		bool CreateDevice(Window& window);
		void DestroyDevice();
		void Swap();
		void HandleWindowResize(const uint32_t width, const uint32_t height);
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2D& texture);
		bool LoadTexture(std::string fileName, Texture2D& texture);
		void UnLoadTexture(Texture2D& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader);
		bool LoadShader(const std::string vertex, const std::string fragment, const std::string geometry, Shader& shader);
		bool LoadTextShader(const std::string shaderText, const std::string vertexEntryPoint, const std::string fragmentEntryPoint, Shader& shader);
		void UnLoadShader(Shader& shader);
		void GetDevice(Microsoft::WRL::ComPtr<ID3D10Device>& device, Microsoft::WRL::ComPtr<IDXGISwapChain>& swapChain, Microsoft::WRL::ComPtr<ID3D10RenderTargetView>& renderTargetView, ID3D10DepthStencilView*& depthStencilView);
		void Clear(const uint32_t bufferFlags, const Color color);
	protected:
		void _ReadExtensions() {};
	private:
		Microsoft::WRL::ComPtr<ID3D10Device> _d3d10Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> _d3d10SwapChain;
		Microsoft::WRL::ComPtr<ID3D10RenderTargetView> _d3d10RenderTargetView;
		ID3D10DepthStencilView* _d3d10DepthStencilView;
		ID3D10Texture2D* _d3d10DepthStencilBuffer;
	};

	inline void RendererDX10::HandleWindowResize(const uint32_t width, const uint32_t height)
	{
		D3D10_VIEWPORT viewPort = { 0 };
		ID3D10Texture2D* backBuffer = nullptr;
		D3D10_TEXTURE2D_DESC depthStencilDesc = { 0 };

		if (!width || !height) return;

		//_d3d10Device->ClearState();

		// Save new size
		_attributes.WindowWidth = width;
		_attributes.WindowHeight = height;

		// Destory old buffers
		_d3d10Device->OMSetRenderTargets(NULL, NULL, NULL);
		//SAFE_RELEASE(_d3d10RenderTargetView);
		_d3d10RenderTargetView.Reset();
		SAFE_RELEASE(_d3d10DepthStencilView);
		SAFE_RELEASE(_d3d10DepthStencilBuffer);
		_d3d10Device->Flush();

		// Resize the new buffers
		_d3d10SwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		// Create depth and stencil buffer
		depthStencilDesc.Width = _attributes.WindowWidth;
		depthStencilDesc.Height = _attributes.WindowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;	// can change for reading or writing i think
		depthStencilDesc.MiscFlags = 0;

		// Create depth stencil buffer texture
		if (FAILED(_d3d10Device->CreateTexture2D(&depthStencilDesc, NULL, &_d3d10DepthStencilBuffer)))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create depth stencil buffer texture on resize." };
			DestroyDevice();
			return;
		}

		// Second param is states for depth stencil

		if (FAILED(_d3d10Device->CreateDepthStencilView(_d3d10DepthStencilBuffer, NULL, &_d3d10DepthStencilView)))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create depth stencil view on resize." };
			DestroyDevice();
			return;
		}

		// Create the back buffer texture
		if (FAILED(_d3d10SwapChain->GetBuffer(0, _uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer))))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create back buffer on resize." };
			DestroyDevice();
			return;
		}

		// Create the render target
		if (FAILED(_d3d10Device->CreateRenderTargetView(backBuffer, 0, _d3d10RenderTargetView.GetAddressOf())))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create render target on resize." };
			DestroyDevice();
			return;
		}

		// And release the texture
		SAFE_RELEASE(backBuffer);

		// Set the render target
		_d3d10Device->OMSetRenderTargets(1, _d3d10RenderTargetView.GetAddressOf(), _d3d10DepthStencilView);

		// Set the viewport
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = _attributes.WindowWidth;
		viewPort.Height = _attributes.WindowHeight;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		_d3d10Device->RSSetViewports(1, &viewPort);
	}

	inline RendererDX10::RendererDX10()
	{
		_d3d10DepthStencilView = nullptr;
		_d3d10DepthStencilBuffer = nullptr;
	}

	inline void RendererDX10::GetDevice(Microsoft::WRL::ComPtr<ID3D10Device>&device, Microsoft::WRL::ComPtr<IDXGISwapChain>& swapChain, Microsoft::WRL::ComPtr<ID3D10RenderTargetView>& renderTargetView, ID3D10DepthStencilView*& depthStencilView)
	{
		device = _d3d10Device;
		swapChain = _d3d10SwapChain;
		renderTargetView = _d3d10RenderTargetView;
		depthStencilView = _d3d10DepthStencilView;
	}

	inline bool RendererDX10::CreateDevice(Window& window)
	{
		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		D3D10_VIEWPORT viewPort = { 0 };
		ID3D10Texture2D* backBuffer = nullptr;
		uint32_t debug = 0;

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
			debug = D3D10_CREATE_DEVICE_DEBUG;
		}

		try
		{
			if (FAILED(D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, NULL, debug, D3D10_SDK_VERSION, &scd, _d3d10SwapChain.GetAddressOf(), _d3d10Device.GetAddressOf())))
			{
				lastError = { GameErrors::GameDirectX10Specific, "Could not create device." };
				return false;
			}
		}
		catch (...)
		{
			std::cout << "exception after createDeviceandswapchain.\n";
		}

		// Create depth and stencil buffer
		D3D10_TEXTURE2D_DESC depthStencilDesc = { 0 };

		depthStencilDesc.Width = _attributes.WindowWidth;
		depthStencilDesc.Height = _attributes.WindowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;	// can change for reading or writing i think
		depthStencilDesc.MiscFlags = 0;

		// Create depth stencil buffer texture
		if (FAILED(_d3d10Device->CreateTexture2D(&depthStencilDesc, NULL, &_d3d10DepthStencilBuffer)))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create depth stencil buffer texture." };
			DestroyDevice();
			return false;
		}


		// Second param is states for depth stencil
		if (FAILED(_d3d10Device->CreateDepthStencilView(_d3d10DepthStencilBuffer, NULL, &_d3d10DepthStencilView)))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create depth stencil view." };
			DestroyDevice();
			return false;
		}

		// Create the back buffer texture
		if (FAILED(_d3d10SwapChain->GetBuffer(0, _uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer))))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create back buffer." };
			DestroyDevice();
			return false;
		}

		// Create the render target
		if (FAILED(_d3d10Device->CreateRenderTargetView(backBuffer, 0, _d3d10RenderTargetView.GetAddressOf())))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create render target." };
			DestroyDevice();
			return false;
		}

		// And release the texture
		SAFE_RELEASE(backBuffer);

		// Set the render target
		_d3d10Device->OMSetRenderTargets(1, _d3d10RenderTargetView.GetAddressOf(), _d3d10DepthStencilView);


		// Set the viewport
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = _attributes.WindowWidth;
		viewPort.Height = _attributes.WindowHeight;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;

		_d3d10Device->RSSetViewports(1, &viewPort);

		return true;
	};

	inline void RendererDX10::DestroyDevice()
	{
		_d3d10Device.Reset();
		_d3d10SwapChain.Reset();
		_d3d10RenderTargetView.Reset();
		SAFE_RELEASE(_d3d10DepthStencilBuffer);
		SAFE_RELEASE(_d3d10DepthStencilView);
	}

	inline void RendererDX10::Clear(const uint32_t bufferFlags, const Color color)
	{
		_d3d10Device->ClearRenderTargetView(_d3d10RenderTargetView.Get(), color.rgba);
		_d3d10Device->ClearDepthStencilView(_d3d10DepthStencilView, bufferFlags, 1.0f, 0);
	}

	inline bool RendererDX10::CreateTexture(Texture2D& texture)
	{
		D3D10_TEXTURE2D_DESC desc = { 0 };
		desc.Width = texture.width;
		desc.Height = texture.height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D10_USAGE_DYNAMIC;
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		texture.oneOverWidth = 1.0f / (float_t)texture.width;
		texture.oneOverHeight = 1.0f / (float_t)texture.height;
		texture.isCopy = false;
		//texture.name = "Created";

		if (FAILED(_d3d10Device->CreateTexture2D(&desc, NULL, &texture.textureInterface10)))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create the texture." };
			return false;
		}

		return true;
	};

	inline bool RendererDX10::LoadTexture(std::string fileName, Texture2D& texture)
	{
		ImageLoader loader;
		void* data = nullptr;
		int32_t width = 0;
		int32_t height = 0;
		int32_t componentsPerPixel = 0;
		D3D10_MAPPED_TEXTURE2D  lockedRectangle = { 0 };

		data = loader.Load(fileName.c_str(), width, height, componentsPerPixel, false);
		if (data == nullptr)
		{
			lastError = { GameErrors::GameContent, "Failed to load texture : " + fileName };
			return false;
		}

		texture.width = width;
		texture.height = height;
		texture.oneOverWidth = 1.0f / (float_t)texture.width;
		texture.oneOverHeight = 1.0f / (float_t)texture.height;
		texture.isCopy = false;
		texture.name = fileName;

		D3D10_TEXTURE2D_DESC desc = { 0 };
		desc.Width = texture.width;
		desc.Height = texture.height;
		desc.MipLevels = texture.isMipMapped ? 0 : 1;// 0 for mipmaps, 1 for not
		desc.ArraySize = 1; 
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D10_USAGE_DEFAULT;// D3D10_USAGE_DYNAMIC;
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;// D3D10_CPU_ACCESS_WRITE;
		if (texture.isMipMapped)
		{ 
			desc.BindFlags |= D3D10_BIND_RENDER_TARGET;
			desc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
		}		

		D3D10_SUBRESOURCE_DATA tex = {};
		tex.pSysMem = data;
		tex.SysMemPitch = static_cast<unsigned long long>(texture.width) * (uint32_t)4 * sizeof(unsigned char);
		tex.SysMemSlicePitch = 0;// tex.SysMemPitch* texture.height;

		// Create texture memory
		if (FAILED(_d3d10Device->CreateTexture2D(&desc, NULL, &texture.textureInterface10)))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create texture, \"" + fileName + "\"." };
			return false;
		}

		// Copy data into memory
		_d3d10Device->UpdateSubresource(texture.textureInterface10.Get(), 0, NULL, data, tex.SysMemPitch, 0);

		D3D10_SHADER_RESOURCE_VIEW_DESC srDesc = {};
		srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		srDesc.Texture2D.MostDetailedMip = 0;
		srDesc.Texture2D.MipLevels = texture.isMipMapped ? -1 : 1; // -1 for mipmaps
		if (FAILED(_d3d10Device->CreateShaderResourceView(texture.textureInterface10.Get(), &srDesc, texture.textureSRV10.GetAddressOf())))
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create texture SRV, \"" + fileName + "\"." };
			return false;
		}

		_d3d10Device->GenerateMips(texture.textureSRV10.Get());


		//// Copy texture data to the memory
		//if (FAILED(texture.textureInterface10->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &lockedRectangle)))
		//{
		//	lastError = { GameErrors::GameDirectX10Specific,"Could not map texture, \"" + fileName + "\"." };
		//	UnLoadTexture(texture);
		//	return false;
		//}
		//unsigned char* dest = static_cast<unsigned char*>(lockedRectangle.pData);
		//if (dest != NULL)
		//{
		//	memcpy(dest, data, sizeof(unsigned char) * texture.width * texture.height * 4);
		//}
		//texture.textureInterface10->Unmap(D3D10CalcSubresource(0, 0, 1));
		return true;
	}

	inline void RendererDX10::UnLoadTexture(Texture2D& texture)
	{
		texture.textureInterface10.Reset();
		texture.textureSRV10.Reset();
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

	inline bool RendererDX10::LoadShader(const std::string vertex, const std::string fragment, Shader& shader)
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
				lastError = { GameErrors::GameDirectX10Specific,"Could not compile shader \"" + vertex + "\".\n" };
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
				lastError = { GameErrors::GameDirectX10Specific,"Could not compile shader \"" + fragment + "\".\n" };
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
			if (FAILED(_d3d10Device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &shader.vertexShader10)))
			{
				lastError = { GameErrors::GameDirectX9Specific,"Could not create vertex shader from \"" + vertex + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(compiledPixelShader);
				SAFE_RELEASE(shader.pixelShader10);

				return false;
			}

			// Create pixel shader
			if (FAILED(_d3d10Device->CreatePixelShader((DWORD*)(compiledPixelShader->GetBufferPointer()), compiledPixelShader->GetBufferSize(), &shader.pixelShader10)))
			{
				lastError = { GameErrors::GameDirectX9Specific,"Could not create pixel shader from \"" + fragment + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(compiledPixelShader);
				SAFE_RELEASE(shader.vertexShader10);
				return false;
			}

			// Shaders created, save a reference and release this one
			compiledVertexShader->AddRef();
			shader.compiledVertexShader10 = compiledVertexShader;
			SAFE_RELEASE(compiledVertexShader);

			compiledPixelShader->AddRef();
			shader.compiledPixelShader10 = compiledPixelShader;
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
				lastError = { GameErrors::GameDirectX10Specific,"Could not read vertex file \"" + vertex + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				return false;
			}
			HRESULT hr = _d3d10Device->CreateVertexShader((DWORD*)compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &shader.vertexShader10);
			if (FAILED(hr))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create vertex shader from \"" + vertex + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				return false;
			}

			// Create pixel shader
			if (FAILED(D3DReadFileToBlob((ConvertToWide(fragment).c_str()), &compiledPixelShader)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not read pixel file \"" + fragment + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(shader.vertexShader10);
				SAFE_RELEASE(compiledPixelShader);
			}

			if (FAILED(_d3d10Device->CreatePixelShader((DWORD*)compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), &shader.pixelShader10)))
			{
				lastError = { GameErrors::GameDirectX10Specific,"Could not create pixel shader from \"" + fragment + "\"." };
				SAFE_RELEASE(compiledVertexShader);
				SAFE_RELEASE(shader.vertexShader10);
				SAFE_RELEASE(compiledPixelShader);
				SAFE_RELEASE(shader.pixelShader10);
				return false;
			}

			// Shaders created, save a reference and release this one
			compiledVertexShader->AddRef();
			shader.compiledVertexShader10 = compiledVertexShader;
			SAFE_RELEASE(compiledVertexShader);

			compiledPixelShader->AddRef();
			shader.compiledPixelShader10 = compiledPixelShader;
			SAFE_RELEASE(compiledPixelShader);
		}
		return true;
	}

	inline bool RendererDX10::LoadShader(const std::string vertex, const std::string fragment, const std::string geometry, Shader& shader)
	{
		if (!LoadShader(vertex, fragment, shader))
		{
			return false;
		}

		ID3D10Blob* pCompiledShader = nullptr;
		ID3D10Blob* pErrors = nullptr;

		HRESULT hr = D3DCompileFromFile(ConvertToWide(geometry).c_str(), nullptr, nullptr, "main", "gs_4_0", 0, 0, &shader.compiledGeometryShader10, &pErrors);

		if (FAILED(hr))
		{
			if (pErrors)
			{
				std::cout << (char*)pErrors->GetBufferPointer() << "\n";
				pErrors->Release();
			}
			lastError = { GameErrors::GameDirectX10Specific, "Could not compile geometry shader." };
			return false;
		}

		hr = _d3d10Device->CreateGeometryShader(shader.compiledGeometryShader10->GetBufferPointer(), shader.compiledGeometryShader10->GetBufferSize(), &shader.geometryShader10);

		if (FAILED(hr))
		{
			SAFE_RELEASE(shader.compiledGeometryShader10);// pCompiledShader->Release();
			lastError = { GameErrors::GameDirectX10Specific, "Could not create geometry shader." };
			return false;
		}

		//pCompiledShader->Release();
		return true;
	}

	inline bool RendererDX10::LoadTextShader(const std::string shaderText, const std::string vertexEntryPoint, const std::string fragmentEntryPoint, Shader& shader)
	{
		//bool LoadTextShader(const std::string shaderText, const std::string vertexEntryPoint, const std::string fragmentEntryPoint, Shader & shader)
		//{
			//if (FAILED(D3DCompile2(shaderText.c_str(), shaderText.length(), NULL, NULL, NULL, vertexEntryPoint.c_str(), "vs_5_0", flags, NULL, NULL, NULL, NULL, compiledVertexShader.GetAddressOf(), compilationMsgs.GetAddressOf())))
			//return false;
		//}
		return false;
	}

	inline void RendererDX10::UnLoadShader(Shader& shader)
	{
		SAFE_RELEASE(shader.compiledVertexShader10);
		SAFE_RELEASE(shader.vertexShader10);
		SAFE_RELEASE(shader.compiledPixelShader10);
		SAFE_RELEASE(shader.pixelShader10);
		SAFE_RELEASE(shader.compiledGeometryShader10);
		SAFE_RELEASE(shader.geometryShader10);
	}

	inline void RendererDX10::Swap()
	{
		if (_attributes.VsyncOn)
		{
			_d3d10SwapChain->Present(1, 0); // first is vsync, 0 for non, 1-4 interval, second is ???
		}
		else
		{
			_d3d10SwapChain->Present(0, 0);
		}
	}
}

#endif