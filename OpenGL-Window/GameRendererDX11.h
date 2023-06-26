#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShader.h"
#include "GameSystemInfo.h"
#include "GameTexture2D.h"

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
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader) { return false; };
		bool LoadShader(const std::string vertex, const std::string fragment, const std::string geometry, Shader& shader)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Geometry shaders not implemented yet." };
			return false;
		}
		void UnLoadShader(Shader& shader) {};
		void GetDevice(ID3D11Device*& device, ID3D11DeviceContext*& context, ID3D11RenderTargetView*& target);
	protected:
		void _ReadExtensions() {};
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> _d3d11Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _d3d11Context;
	};

	inline RendererDX11::RendererDX11()
	{

	}

	inline bool RendererDX11::CreateDevice(Window& window) 
	{ 
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_9_1,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1
		};
		
		return true; 
	}
	
	inline void RendererDX11::GetDevice(ID3D11Device*& device, ID3D11DeviceContext*& context, ID3D11RenderTargetView*& target)
	{
		//_d3d11Device->AddRef();
		//_d3d11Context->AddRef();
		//_d3d11RenderTarget->AddRef();
		//device = _d3d11Device;
		//context = _d3d11Context;
		//target = _d3d11RenderTarget;
	}

	inline void RendererDX11::DestroyDevice()
	{
		//if (_d3d11RenderTarget) _d3d11RenderTarget->Release();
		//if (_d3d11Device) _d3d11Device->Release();
		//if (_d3d11SwapChain) _d3d11SwapChain->Release();
		//if (_d3d11Context) _d3d11Context->Release();
	}

	inline void RendererDX11::Swap()
	{
		//_d3d11SwapChain->Present(0, 0);
	}

	inline void RendererDX11::HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset)
	{
		float color[4] = { 0 };
		D3D11_VIEWPORT viewport = { 0 };
		
		if (!width || !height) return;

		//_d3d11Context->ClearRenderTargetView(_d3d11RenderTarget, color);

		//viewport.TopLeftX = 0;
		//viewport.TopLeftY = 0;
		//viewport.Width = (float_t)width;
		//viewport.Height = (float_t)height;

		//_d3d11Context->RSSetViewports(1, &viewport);
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


		//if (_d3d11Device->CreateTexture2D(&desc, NULL, &texture.textureInterface11) != S_OK)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create texture." };
			return false;
		}
		

		return true;
	}

	inline void RendererDX11::UnLoadTexture(Texture2D& texture)
	{
		if (texture.textureInterface11)
		{
			texture.textureInterface11->Release();
			texture.textureInterface11 = nullptr;
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