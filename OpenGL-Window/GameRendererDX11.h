#pragma once
#include <d3d11.h>

#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShaderGL.h"
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
		void Swap() {}
		void HandleWindowResize(const uint32_t width, const uint32_t height) {}
		void FillOutRendererInfo() {}
		bool CreateTexture(Texture2D& texture) { return false; }
		bool LoadTexture(std::string fileName, Texture2D& texture) { return false; }
		void UnLoadTexture(Texture2D& texture) {};
		bool LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader) { return false; };
		void UnLoadShader(ShaderGL& shader) {};
		//void GetDevice(LPDIRECT3DDEVICE9& device) {};
	protected:
		void _ReadExtensions() {};
	private:
		IDXGISwapChain* _d3d11SwapChain;
		ID3D11Device* _d3d11Device;
		ID3D11DeviceContext* _d3d11Context;
		DXGI_SWAP_CHAIN_DESC _d3d11SCD;
		ID3D11RenderTargetView* _d3d11BackBuffer;
		//Color _clearColor;
		//D3DPRESENT_PARAMETERS _d3dpp = { 0 };

	};

	inline RendererDX11::RendererDX11()
	{
		_d3d11SwapChain = nullptr;
		_d3d11Device = nullptr;
		_d3d11Context = nullptr;
		_d3d11BackBuffer = nullptr;
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

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = { 0 };
		if (_d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer) != S_OK)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not get address of back buffer" };
			return false;
		}

		// use the back buffer address to create the render target
		if (_d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &_d3d11BackBuffer) != S_OK)
		{
			lastError = { GameErrors::GameDirectX11Specific, "Could not create render target." };
			return false;
		}
		pBackBuffer->Release();

		// set the render target as the back buffer
		_d3d11Context->OMSetRenderTargets(1, &_d3d11BackBuffer, NULL);



		return true; 
	}

	inline void RendererDX11::DestroyDevice()
	{
		if (_d3d11BackBuffer) _d3d11BackBuffer->Release();
		if (_d3d11Device) _d3d11Device->Release();
		if (_d3d11SwapChain) _d3d11SwapChain->Release();
		if (_d3d11Context) _d3d11Context->Release();
	}
}