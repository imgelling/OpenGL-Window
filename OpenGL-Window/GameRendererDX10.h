#if !defined(GAMERENDERERDX10_H)
#define GAMERENDERERDX10_H

#include <d3d10.h>

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
		void Swap() {};
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset) {};
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2D& texture) { return false; };
		bool LoadTexture(std::string fileName, Texture2D& texture) { lastError = { GameErrors::GameDirectX10Specific, "Can't load textures." }; return false; };
		void UnLoadTexture(Texture2D& texture) {};
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader) { return false; };
		void UnLoadShader(Shader& shader) {};
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
		float black[4] = {0.0f, 0.0f, 1.0f, 1.0f};
		_d3d10Device->ClearRenderTargetView(_d3d10RenderTargetView, black); // rgba

		_d3d10SwapChain->Present(0, 0);

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
}

#endif