#pragma once
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
		void HandleWindowResize(const uint32_t width, const uint32_t height);
		void FillOutRendererInfo() {}
		bool CreateTexture(Texture2D& texture);
		bool LoadTexture(std::string fileName, Texture2D& texture) { lastError = { GameErrors::GameDirectX11Specific, "could not load texture" }; return false; }
		void UnLoadTexture(Texture2D& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader) { return false; };
		void UnLoadShader(Shader& shader) {};
		void GetDevice(ID3D11Device*& device, ID3D11DeviceContext*& context, ID3D11RenderTargetView*& target);
	protected:
		void _ReadExtensions() {};
	private:
		IDXGISwapChain* _d3d11SwapChain;
		ID3D11Device* _d3d11Device;
		ID3D11DeviceContext* _d3d11Context;
		DXGI_SWAP_CHAIN_DESC _d3d11SCD;
		ID3D11RenderTargetView* _d3d11RenderTarget;
	};

	inline RendererDX11::RendererDX11()
	{
		_d3d11SwapChain = nullptr;
		_d3d11Device = nullptr;
		_d3d11Context = nullptr;
		_d3d11RenderTarget = nullptr;
		_d3d11SCD = { 0 };
	}

	inline bool RendererDX11::CreateDevice(Window& window) 
	{ 

		// fill the swap chain description struct
		_d3d11SCD.BufferCount = 1;
		_d3d11SCD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
		_d3d11SCD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_d3d11SCD.OutputWindow = window.GetHandle();
		_d3d11SCD.SampleDesc.Count = _attributes.MultiSamples;                               // how many multisamples
		_d3d11SCD.Windowed = !_attributes.WindowFullscreen;
		_d3d11SCD.Flags = _attributes.DebugMode ? D3D11_CREATE_DEVICE_DEBUG : 0;

		HRESULT hr = 0;

		if (D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&_d3d11SCD,
			&_d3d11SwapChain,
			&_d3d11Device,
			NULL,
			&_d3d11Context) != S_OK)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create device." };
			return false;
		}

		ID3D11Debug* d3dDebug = nullptr;
		_d3d11Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug);
		if (d3dDebug)
		{
			ID3D11InfoQueue* d3dInfoQueue = nullptr;
			if (SUCCEEDED(d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
			{
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
				d3dInfoQueue->Release();
			}
			else
			{
				lastError = { GameErrors::GameDirectX11Specific, "Could not set breaking on error for DirectX11." };
				d3dDebug->Release();
				return false;
			}
			d3dDebug->Release();
		}

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = { 0 };
		if (_d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer) != S_OK)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not get address of back buffer" };
			return false;
		}

		// use the back buffer address to create the render target
		if (_d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &_d3d11RenderTarget) != S_OK)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create render target." };
			return false;
		}
		pBackBuffer->Release();

		// set the render target as the back buffer
		_d3d11Context->OMSetRenderTargets(1, &_d3d11RenderTarget, NULL);

		D3D11_VIEWPORT viewport = { 0 };

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float_t)_attributes.WindowWidth;
		viewport.Height = (float_t)_attributes.WindowHeight;

		_d3d11Context->RSSetViewports(1, &viewport);
		return true; 
	}
	
	inline void RendererDX11::GetDevice(ID3D11Device*& device, ID3D11DeviceContext*& context, ID3D11RenderTargetView*& target)
	{
		_d3d11Device->AddRef();
		_d3d11Context->AddRef();
		_d3d11RenderTarget->AddRef();
		device = _d3d11Device;
		context = _d3d11Context;
		target = _d3d11RenderTarget;
	}

	inline void RendererDX11::DestroyDevice()
	{
		if (_d3d11RenderTarget) _d3d11RenderTarget->Release();
		if (_d3d11Device) _d3d11Device->Release();
		if (_d3d11SwapChain) _d3d11SwapChain->Release();
		if (_d3d11Context) _d3d11Context->Release();
	}

	inline void RendererDX11::Swap()
	{
		_d3d11SwapChain->Present(0, 0);
	}

	inline void RendererDX11::HandleWindowResize(const uint32_t width, const uint32_t height)
	{
		float color[4] = { 0 };
		D3D11_VIEWPORT viewport = { 0 };
		
		if (!width || !height) return;

		_d3d11Context->ClearRenderTargetView(_d3d11RenderTarget, color);

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float_t)width;
		viewport.Height = (float_t)height;

		_d3d11Context->RSSetViewports(1, &viewport);
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


		if (_d3d11Device->CreateTexture2D(&desc, NULL, &texture.textureInterface11) != S_OK)
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