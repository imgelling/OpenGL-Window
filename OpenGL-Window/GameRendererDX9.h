#pragma once

#include <d3d9.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShaderGL.h"
#include "GameTexture2D.h"
#include "GameErrors.h"
#include "GameSystemInfo.h"
#include "GameImageLoader.h"

namespace game
{
	extern SystemInfo systemInfo;
	class RendererDX9 : public RendererBase
	{
	public:
		RendererDX9();
		bool CreateDevice(Window& window);
		void DestroyDevice();
		void Swap();
		void HandleWindowResize(const uint32_t width, const uint32_t height);
		void FillOutRendererInfo();
		bool CreateTexture(Texture2D& texture);
		bool LoadTexture(std::string fileName, Texture2D& texture);
		void UnLoadTexture(Texture2D& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader) { return false; };
		void UnLoadShader(ShaderGL& shader) {};
		void SetClearColor(const Color& color) noexcept;
		void Clear(const bool color, const bool depth, const bool stencil) noexcept;
		void Enable(const uint32_t capability) noexcept {};
		void Disable(const uint32_t capability) noexcept {};
		void BindTexture(const uint32_t capability, const Texture2D& texture) noexcept {};


	protected:
		void _ReadExtensions() {};
	private:
		LPDIRECT3D9 _d3d9;
		LPDIRECT3DDEVICE9 _d3d9Device;
		Color _clearColor;
		D3DPRESENT_PARAMETERS _d3dpp = { 0 };


	};

	inline RendererDX9::RendererDX9()
	{
		_d3d9 = nullptr;
		_d3d9Device = nullptr;

	}

	inline void RendererDX9::HandleWindowResize(const uint32_t width, const uint32_t height)
	{
		D3DVIEWPORT9 view { 0, 0, width, height, 0.0f, 1.0f};
		Clear(true, true, true);
		_d3dpp.BackBufferWidth = width;
		_d3dpp.BackBufferHeight = height;
		_d3d9Device->Reset(&_d3dpp);
		if (_d3d9Device->SetViewport(&view) != D3D_OK)
		{
			std::cout << "viewport";
		}
	}

	inline void RendererDX9::DestroyDevice()
	{
		if (_d3d9Device)
		{
			_d3d9Device->Release();
		}
		if (_d3d9)
		{
			_d3d9->Release();
		}
	}

	inline bool RendererDX9::CreateDevice(Window& window)
	{
		D3DPRESENT_PARAMETERS d3dpp = { 0 };   
		DWORD multiSamplingQuality = 0;
		
		_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);    

		// Setup present parameters
		d3dpp.Windowed = !_attributes.WindowFullscreen;    
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    
		d3dpp.hDeviceWindow = window.GetHandle();   
		d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8; // 24 bit, no stencil
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		if (_attributes.VsyncOn)
		{
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}
		else
		{
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}

		// Enables multisampling
		if (_attributes.MultiSamples > 1)
		{
			if (FAILED(_d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, !_attributes.WindowFullscreen, (D3DMULTISAMPLE_TYPE)_attributes.MultiSamples, &multiSamplingQuality)))
			{
				lastError = { GameErrors::DirectXSpecific, "Device does not support " + std::to_string(_attributes.MultiSamples) + "x multisampling."};
				return false;
			}
			d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)_attributes.MultiSamples;
			d3dpp.MultiSampleQuality = multiSamplingQuality - 1;
		}
		_d3dpp = d3dpp;
		// Create the device
		if (FAILED(_d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window.GetHandle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &_d3d9Device)))
		{
			lastError = { GameErrors::DirectXSpecific, "Create Device failed." };
			return false;
		}
		D3DVIEWPORT9 view{ 0, 0, _attributes.WindowWidth, _attributes.WindowHeight, 0.0f, 1.0f };

		_d3d9Device->SetViewport(&view);
		return true;
	}

	inline void RendererDX9::FillOutRendererInfo()
	{
		std::stringstream sStream;
		std::string tempValue;
		D3DADAPTER_IDENTIFIER9 adapterIdentifier = { 0 };

		// Lambda to log easier
		auto LOG = [&](std::stringstream& stream)
		{
			_logger->Write(stream.str());
			stream.str("");
		};

		// Log the OpenGL rendering device
		_d3d9->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapterIdentifier);
		systemInfo.gpuInfo.renderer = adapterIdentifier.Description;
		sStream << "DirectX9 renderer : " << systemInfo.gpuInfo.renderer;
		LOG(sStream);

		// Log the version of DirectX context
		systemInfo.gpuInfo.version = "DirectX 9.0c";
		sStream << "DirectX version : " << systemInfo.gpuInfo.version;
		LOG(sStream);

		// Log video memory
		systemInfo.gpuInfo.freeMemory = _d3d9Device->GetAvailableTextureMem() / 1024 / 1024;
		sStream << "GPU available memory : " << systemInfo.gpuInfo.freeMemory << "MB";
		LOG(sStream);

		// This should be done with all renderers
		HINSTANCE hDXGI = LoadLibrary(L"dxgi.dll");
		if (hDXGI != nullptr)
		{
			typedef HRESULT(WINAPI* LPCREATEDXGIFACTORY)(REFIID, void**);

			LPCREATEDXGIFACTORY pCreateDXGIFactory = nullptr;
			IDXGIFactory* pDXGIFactory = nullptr;

			// We prefer the use of DXGI 1.1
			pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory1");

			if (!pCreateDXGIFactory)
			{
				pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory");

				if (!pCreateDXGIFactory)
				{
					FreeLibrary(hDXGI);

					return;
				}
			}

			HRESULT hr = pCreateDXGIFactory(__uuidof(IDXGIFactory), (LPVOID*)&pDXGIFactory);
			IDXGIAdapter* pAdapter = nullptr;
			hr = pDXGIFactory->EnumAdapters(0, &pAdapter);
			DXGI_ADAPTER_DESC desc{ 0 };
			pAdapter->GetDesc(&desc);

			std::cout << "DedicatedVideoMemory = " << desc.DedicatedVideoMemory / 1024 / 1024 << "MB\n";

			pAdapter->Release();
			pAdapter = nullptr;
			pDXGIFactory->Release();
			pDXGIFactory = nullptr;
			FreeLibrary(hDXGI);
		}

		D3DCAPS9 caps;
		_d3d9Device->GetDeviceCaps(&caps);
	}

	inline void RendererDX9::Clear(const bool color, const bool depth, const bool stencil) noexcept
	{
		if (color)
		{
			_d3d9Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(_clearColor.r, _clearColor.g, _clearColor.b), 1.0f, 0);
		}
	}

	inline void RendererDX9::SetClearColor(const Color& color) noexcept
	{
		_clearColor.Set(color.rf, color.gf, color.bf, color.af);
	}

	inline void RendererDX9::Swap()
	{
		_d3d9Device->Present(NULL, NULL, NULL, NULL);
	}

	inline bool RendererDX9::CreateTexture(Texture2D& texture)
	{
		texture.oneOverWidth = 1.0f / (float_t)texture.width;
		texture.oneOverHeight = 1.0f / (float_t)texture.height;
		texture.isCopy = false;

		// does mipmapping, need to add format also
		_d3d9Device->CreateTexture(texture.width, texture.height, texture.isMipMapped ? 0 : 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture.textureInterface, NULL);
		if (!texture.textureInterface)
		{
			lastError = { GameErrors::DirectXSpecific, "Could not create texture." };
			return false;
		}

		return true;
	}

	inline bool RendererDX9::LoadTexture(std::string fileName, Texture2D& texture)
	{
		ImageLoader loader;
		void* data = nullptr;
		int32_t width = 0;
		int32_t height = 0;
		int32_t componentsPerPixel = 0;
		D3DLOCKED_RECT lockedRectangle = { 0 };
		HRESULT hResult;
		
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

		// Create texture memory
		hResult = _d3d9Device->CreateTexture(texture.width, texture.height, texture.isMipMapped ? 0 : 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture.textureInterface, NULL);
		if (hResult != D3D_OK)
		{
			lastError = { GameErrors::DirectXSpecific, "Could not create texture, \"" + fileName + "\""};
		}

		// Copy texture data to the memory
		texture.textureInterface->LockRect(0, &lockedRectangle, 0, 0);
		unsigned char* dest = static_cast<unsigned char*>(lockedRectangle.pBits);
		memcpy(dest, data, sizeof(unsigned char) * texture.width * texture.height * 4);
		texture.textureInterface->UnlockRect(0);

		return true;
	}

	inline void RendererDX9::UnLoadTexture(Texture2D& texture)
	{
		if (texture.textureInterface)
		{
			texture.textureInterface->Release();
			texture.textureInterface = nullptr;
		}
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
}