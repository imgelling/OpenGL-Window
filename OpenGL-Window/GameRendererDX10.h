#if !defined(GAMERENDERERDX10_H)
#define GAMERENDERERDX10_H

#include <d3d10_1.h>

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
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset) {};
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2D& texture);
		bool LoadTexture(std::string fileName, Texture2D& texture);
		void UnLoadTexture(Texture2D& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader);
		void UnLoadShader(Shader& shader);
	protected:
		void _ReadExtensions() {};
	private:
		ID3D10Device* _d3d10Device;
		IDXGISwapChain* _d3d10SwapChain;
		ID3D10RenderTargetView* _d3d10RenderTargetView;
	};

	inline RendererDX10::RendererDX10()
	{
		_d3d10Device = nullptr;
		_d3d10SwapChain = nullptr;
		_d3d10RenderTargetView = nullptr;
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
			uint32_t debug = D3D10_CREATE_DEVICE_DEBUG;
		}

		if (D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, NULL, debug, D3D10_SDK_VERSION, &scd, &_d3d10SwapChain, &_d3d10Device) != S_OK)
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create device." };
			return false;
		}

		// Create the back buffer texture
		if (_d3d10SwapChain->GetBuffer(0, _uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer)) != S_OK)
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create back buffer." };
			if (_d3d10SwapChain)
			{
				_d3d10SwapChain->Release();
				_d3d10SwapChain = nullptr;
			}
			if (_d3d10Device)
			{
				_d3d10Device->Release();
				_d3d10Device = nullptr;
			}
			return false;
		}

		// Create the render target
		if (_d3d10Device->CreateRenderTargetView(backBuffer, 0, &_d3d10RenderTargetView) != S_OK)
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create render target." };
			if (_d3d10SwapChain)
			{
				_d3d10SwapChain->Release();
				_d3d10SwapChain = nullptr;
			}
			if (_d3d10Device)
			{
				_d3d10Device->Release();
				_d3d10Device = nullptr;
			}
			if (backBuffer)
			{
				backBuffer->Release();
				backBuffer = nullptr;
			}
			return false;
		}

		// And release the texture
		if (backBuffer)
		{
			backBuffer->Release();
		}

		// Set the render target
		_d3d10Device->OMSetRenderTargets(1, &_d3d10RenderTargetView, NULL); // last parameter is depth/stencil

		// Set the viewport
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = _attributes.WindowWidth;
		viewPort.Height = _attributes.WindowHeight;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;

		_d3d10Device->RSSetViewports(1, &viewPort);
		
		//test for clear
		_d3d10Device->ClearRenderTargetView(_d3d10RenderTargetView, Colors::Pink.rgba); // rgba

		//_d3d10SwapChain->Present(0, 0);

		return true;
	};

	inline void RendererDX10::DestroyDevice()
	{
		if (_d3d10SwapChain)
		{
			_d3d10SwapChain->Release();
			_d3d10SwapChain = nullptr;
		}
		if (_d3d10RenderTargetView)
		{
			_d3d10RenderTargetView->Release();
			_d3d10RenderTargetView = nullptr;
		}
		if (_d3d10Device)
		{
			_d3d10Device->Release();
			_d3d10Device = nullptr;
		}
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

		if (_d3d10Device->CreateTexture2D(&desc, NULL, &texture.textureInterface10) != S_OK)
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
		desc.MipLevels = desc.ArraySize = 1; // change for mipmapping
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D10_USAGE_DYNAMIC;
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		// Create texture memory
		if (_d3d10Device->CreateTexture2D(&desc, NULL, &texture.textureInterface10) != S_OK)
		{
			lastError = { GameErrors::GameDirectX10Specific, "Could not create texture, \"" + fileName + "\"." };
			return false;
		}

		// Copy texture data to the memory
		if (texture.textureInterface10->Map(D3D10CalcSubresource(0,0,1), D3D10_MAP_WRITE_DISCARD, 0, &lockedRectangle) != S_OK)
		{
			lastError = { GameErrors::GameDirectX10Specific,"Could not map texture, \"" + fileName + "\"." };
			UnLoadTexture(texture);
			return false;
		}
		unsigned char* dest = static_cast<unsigned char*>(lockedRectangle.pData);
		if (dest != NULL)
		{
			memcpy(dest, data, sizeof(unsigned char) * texture.width * texture.height * 4);
		}
		texture.textureInterface10->Unmap(D3D10CalcSubresource(0, 0, 1));
		return true;
	}

	inline void RendererDX10::UnLoadTexture(Texture2D& texture)
	{
		if (texture.textureInterface10)
		{
			texture.textureInterface10->Release();
			texture.textureInterface10 = nullptr;
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

	inline bool RendererDX10::LoadShader(const std::string vertex, const std::string fragment, Shader& shader)
	{
		if (!shader.isPrecompiled)
		{

		}
		else
		{
			// Load compiled vertex shader
			std::ifstream file;
			uint32_t fileSize = 0;
			uint8_t* compiledVertexShader = nullptr;
			uint8_t* compiledPixelShader = nullptr;

			file.open(vertex, std::fstream::in | std::fstream::binary | std::fstream::ate);
			if (file.is_open())
			{
				fileSize = (uint32_t)file.tellg();
				file.seekg(0, file.beg);
				compiledVertexShader = new uint8_t[fileSize];
				file.read((char*)compiledVertexShader, fileSize);
				if (!file.good())
				{
					lastError = { GameErrors::GameDirectX10Specific,"Error reading vertex shader file \"" + vertex + "\".\n" };
					if (compiledVertexShader)
					{
						delete[] compiledVertexShader;
						compiledVertexShader = nullptr;
					}
					file.close();
					return false;
				}
				file.close();

				// Create vertex shader
				if (_d3d10Device->CreateVertexShader((DWORD*)(compiledVertexShader), (SIZE_T)fileSize, &shader.vertexShader10) != S_OK)
				{
					lastError = { GameErrors::GameDirectX10Specific,"Could not create vertex shader from \"" + vertex + "\"." };
					if (compiledVertexShader)
					{
						delete[] compiledVertexShader;
						compiledVertexShader = nullptr;
					}
					return false;
				}
			}
			else
			{
				lastError = { GameErrors::GameDirectX10Specific, "Compiled vertex shader \"" + vertex + "\" does not exsist." };
				if (compiledVertexShader)
				{
					delete[] compiledVertexShader;
					compiledVertexShader = nullptr;
				}
				return false;
			}

			// Reset file size
			fileSize = 0;

			// Load compiled pixel shader
			file.open(fragment, std::fstream::in | std::fstream::binary | std::fstream::ate);
			if (file.is_open())
			{
				fileSize = (uint32_t)file.tellg();
				file.seekg(0, file.beg);
				compiledPixelShader = new uint8_t[fileSize];
				file.read((char*)compiledPixelShader, fileSize);
				if (!file.good())
				{
					lastError = { GameErrors::GameDirectX10Specific,"Error reading pixel shader file \"" + fragment + "\"\n" };
					if (compiledVertexShader)
					{
						delete[] compiledVertexShader;
						compiledVertexShader = nullptr;
					}
					if (compiledPixelShader)
					{
						delete[] compiledPixelShader;
						compiledPixelShader = nullptr;
					}
					if (shader.vertexShader10)
					{
						shader.vertexShader10->Release();
						shader.vertexShader10 = nullptr;
					}
					file.close();

					return false;
				}
				file.close();

				// Create pixel shader
				if (_d3d10Device->CreatePixelShader((DWORD*)(compiledPixelShader), fileSize, &shader.pixelShader10) != S_OK)
				{
					lastError = { GameErrors::GameDirectX10Specific,"Could not create pixel shader from \"" + fragment + "\"." };
					if (compiledVertexShader)
					{
						delete[] compiledVertexShader;
						compiledVertexShader = nullptr;
					}
					if (compiledPixelShader)
					{
						delete[] compiledPixelShader;
						compiledPixelShader = nullptr;
					}
					if (shader.vertexShader10)
					{
						shader.vertexShader10->Release();
						shader.vertexShader10 = nullptr;
					}

					return false;
				}

				// Shaders loaded, unload compiled code
				if (compiledVertexShader)
				{
					delete[] compiledVertexShader;
					compiledVertexShader = nullptr;
				}
				if (compiledPixelShader)
				{
					delete[] compiledPixelShader;
					compiledPixelShader = nullptr;
				}
			} 
			else 
			{
				lastError = { GameErrors::GameDirectX10Specific, "Compiled vertex shader \"" + vertex + "\" does not exsist." };
				if (compiledVertexShader)
				{
					delete[] compiledVertexShader;
					compiledVertexShader = nullptr;
				}
				return false;
			}
		}
		return true;
	}

	inline void RendererDX10::UnLoadShader(Shader& shader)
	{
		if (shader.vertexShader10)
		{
			shader.vertexShader10->Release();
			shader.vertexShader10 = nullptr;
		}
		if (shader.pixelShader10)
		{
			shader.pixelShader10->Release();
			shader.pixelShader10 = nullptr;
		}

	}
	inline void RendererDX10::Swap()
	{
		_d3d10SwapChain->Present(0, 0); // first is vsync, 0 for non, 1-4 interval, second is ???
	}
}

#endif