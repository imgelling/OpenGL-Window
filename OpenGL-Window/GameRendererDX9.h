#pragma once

#include <d3d9.h>
//#include <dxgi.h>
//#pragma comment(lib, "dxgi.lib") // for gpu memory

#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShader.h"
#include "GameSystemInfo.h"
#include "GameTexture2D.h"

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
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset);
		void FillOutRendererInfo();
		bool CreateTexture(Texture2D& texture);
		bool LoadTexture(std::string fileName, Texture2D& texture);
		void UnLoadTexture(Texture2D& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader);
		void UnLoadShader(Shader& shader) {};
		void GetDevice(LPDIRECT3DDEVICE9& device);
	protected:
		void _ReadExtensions() {};
	private:
		LPDIRECT3D9 _d3d9;
		LPDIRECT3DDEVICE9 _d3d9Device;
		D3DPRESENT_PARAMETERS _d3dpp = { 0 };


	};

	inline RendererDX9::RendererDX9()
	{
		_d3d9 = nullptr;
		_d3d9Device = nullptr;

	}

	inline void RendererDX9::GetDevice(LPDIRECT3DDEVICE9& device)
	{
		_d3d9Device->AddRef();
		device = _d3d9Device;
	}

	inline void RendererDX9::HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset)
	{
		D3DVIEWPORT9 view { 0, 0, width, height, 0.0f, 1.0f};
		
		if (!width || !height) return;

		_d3d9Device->Clear(0, NULL, D3DCLEAR_TARGET, Colors::Black.packedARGB, 1.0f, 0);
		_d3dpp.BackBufferWidth = width;
		_d3dpp.BackBufferHeight = height;
		if (doReset) _d3d9Device->Reset(&_d3dpp);
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
		//D3DFMT_D16_LOCKABLE = 70,
		//	D3DFMT_D32 = 71,
		//	D3DFMT_D15S1 = 73,
		//	D3DFMT_D24S8 = 75,
		//	D3DFMT_D24X8 = 77,
		//	D3DFMT_D24X4S4 = 79,
		//	D3DFMT_D16 = 80,

		//	D3DFMT_D32F_LOCKABLE = 82,
		//	D3DFMT_D24FS8 = 83,
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
				lastError = { GameErrors::GameDirectX9Specific, "Device does not support " + std::to_string(_attributes.MultiSamples) + "x multisampling."};
				return false;
			}
			d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)_attributes.MultiSamples;
			d3dpp.MultiSampleQuality = multiSamplingQuality - 1;
		}
		_d3dpp = d3dpp;
		// Create the device
		if (FAILED(_d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window.GetHandle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &_d3d9Device)))
		{
			lastError = { GameErrors::GameDirectX9Specific, "Create Device failed." };
			return false;
		}
		D3DVIEWPORT9 view{ 0, 0, _attributes.WindowWidth, _attributes.WindowHeight, 0.0f, 1.0f };

		_d3d9Device->SetViewport(&view);

		// If device was created then the settings must have worked, log them
		systemInfo.gpuInfo.depthBufferSize = 24; //temp
		systemInfo.gpuInfo.backBufferColorSize = 32; // temp
		systemInfo.gpuInfo.frontBufferColorSize = 32; //temp
		systemInfo.gpuInfo.multisampleSamples = _attributes.MultiSamples;
		systemInfo.gpuInfo.maxMultisamples = 0; // temp till I can fix
		systemInfo.gpuInfo.internalPixelFormat = (int32_t)D3DFMT_A8R8G8B8;// figure out from somewhere
		systemInfo.gpuInfo.internalPixelType = 0; // unkown
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

		D3DCAPS9 caps;
		_d3d9Device->GetDeviceCaps(&caps);

		// Log shader versions
		systemInfo.gpuInfo.maxShaderLanguageVersion = "VS 3.0, ";
		if (caps.VertexShaderVersion < D3DVS_VERSION(3, 0))
		{
			systemInfo.gpuInfo.maxShaderLanguageVersion = "VS 2.x, ";
			if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
			{
				systemInfo.gpuInfo.maxShaderLanguageVersion = "VS 1.1, ";
			}
		}
		systemInfo.gpuInfo.maxShaderLanguageVersion += "PS 3.0";
		if (caps.PixelShaderVersion < D3DVS_VERSION(3, 0))
		{
			systemInfo.gpuInfo.maxShaderLanguageVersion += "PS 2.x";
			if (caps.PixelShaderVersion < D3DVS_VERSION(2, 0))
			{
				systemInfo.gpuInfo.maxShaderLanguageVersion += "PS 1.1";
			}
		}
		sStream << "Max supported shader versions : " << systemInfo.gpuInfo.maxShaderLanguageVersion;
		LOG(sStream);

		// Log video memory
		systemInfo.gpuInfo.freeMemory = _d3d9Device->GetAvailableTextureMem() / 1024 / 1024;
		sStream << "GPU available memory : " << systemInfo.gpuInfo.freeMemory << "MB";
		LOG(sStream);

		// Log internal pixel format
		sStream << "Internal Pixel Format : " << systemInfo.gpuInfo.internalPixelFormat << " (";
		switch (systemInfo.gpuInfo.internalPixelFormat)
		{
		case 0: sStream << "Unknown)"; break;
		case 21: sStream << "ARGB)"; break;
		default: sStream << "Unknown)";
		}
		LOG(sStream);

		// Log internal pixel type
		sStream << "Internal Pixel Type : " << systemInfo.gpuInfo.internalPixelType << " (";
		switch (systemInfo.gpuInfo.internalPixelType)
		{
		case 0: sStream << "Unknown)"; break;
		default: sStream << "Unknown)";
		}
		LOG(sStream);

		// Log multisampling 
		sStream << "Multisampling samples : ";
		if (systemInfo.gpuInfo.multisampleSamples > 1)
		{
			sStream << systemInfo.gpuInfo.multisampleSamples << " (";
			sStream << systemInfo.gpuInfo.maxMultisamples << " max)";
		}
		else
		{
			sStream << "Disabled";
		}
		LOG(sStream);

		// Log front buffer
		sStream << "Front buffer : " << systemInfo.gpuInfo.frontBufferColorSize << " bits";
		LOG(sStream);

		// Log back buffer
		sStream << "Back buffer : " << systemInfo.gpuInfo.backBufferColorSize << " bits";
		LOG(sStream);

		// Log depth buffer
		sStream << "Depth buffer : " << systemInfo.gpuInfo.depthBufferSize << " bits";
		LOG(sStream);

		// Log max anisotropy
		systemInfo.gpuInfo.maxAnisotropy = caps.MaxAnisotropy;
		sStream << "Max anisotropy : " << systemInfo.gpuInfo.maxAnisotropy << "x";
		LOG(sStream);
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
		_d3d9Device->CreateTexture(texture.width, texture.height, texture.isMipMapped ? 0 : 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture.textureInterface9, NULL);
		if (!texture.textureInterface9)
		{
			lastError = { GameErrors::GameDirectX9Specific, "Could not create texture." };
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
		hResult = _d3d9Device->CreateTexture(texture.width, texture.height, texture.isMipMapped ? 0 : 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture.textureInterface9, NULL);
		if (hResult != D3D_OK)
		{
			lastError = { GameErrors::GameDirectX9Specific, "Could not create texture, \"" + fileName + "\""};
		}

		// Copy texture data to the memory
		texture.textureInterface9->LockRect(0, &lockedRectangle, 0, 0);
		unsigned char* dest = static_cast<unsigned char*>(lockedRectangle.pBits);
		if (dest != NULL)
		{
			memcpy(dest, data, sizeof(unsigned char) * texture.width * texture.height * 4);
		}
		texture.textureInterface9->UnlockRect(0);

		return true;
	}

	inline void RendererDX9::UnLoadTexture(Texture2D& texture)
	{
		if (texture.textureInterface9)
		{
			texture.textureInterface9->Release();
			texture.textureInterface9 = nullptr;
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

	inline bool RendererDX9::LoadShader(const std::string vertex, const std::string fragment, Shader& shader)
	{

		return false;
	}
}