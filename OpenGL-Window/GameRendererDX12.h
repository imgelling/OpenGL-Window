#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShader.h"
#include "GameSystemInfo.h"
#include "GameTexture2D.h"

namespace game
{
	extern SystemInfo systemInfo;
	class RendererDX12 : public RendererBase
	{
	public:
		bool CreateDevice(Window& window);
		void DestroyDevice() {};
		void Swap() {};
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset) {};
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2D& texture) { lastError = { GameErrors::GameDirectX12Specific,"Texture not implemented " }; return false;
		};
		bool LoadTexture(std::string fileName, Texture2D& texture) {
			lastError = { GameErrors::GameDirectX12Specific,"Texture not implemented " }; return false;
		};
		void UnLoadTexture(Texture2D& texture) {};
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader) { lastError = { GameErrors::GameDirectX12Specific,"shader not implemented " }; return false; };
		bool LoadShader(const std::string vertex, const std::string fragment, const std::string geometry, Shader& shader)
		{
			lastError = { GameErrors::GameDirectX12Specific, "Geometry shaders not implemented yet." };
			return false;
		}
		void UnLoadShader(Shader& shader) {};
	protected:
		void _ReadExtensions() {};
		// direct3d stuff
		#define frameBufferCount  3 // number of buffers we want, 2 for double buffering, 3 for tripple buffering;

		ID3D12Device* device; // direct3d device

		IDXGISwapChain3* swapChain; // swapchain used to switch between render targets

		ID3D12CommandQueue* commandQueue; // container for command lists

		ID3D12DescriptorHeap* rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

		ID3D12Resource* renderTargets[frameBufferCount]; // number of render targets equal to buffer count

		ID3D12CommandAllocator* commandAllocator[frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)

		ID3D12GraphicsCommandList* commandList; // a command list we can record commands into, then execute them to render the frame

		ID3D12Fence* fence[frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
		//as we have allocators (more if we want to know when the gpu is finished with an asset)

		HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu

		UINT64 fenceValue[frameBufferCount]; // this value is incremented each frame. each fence will have its own value

		int frameIndex; // current rtv we are on

		int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)

		//// function declarations
		//bool InitD3D(); // initializes direct3d 12

		//void Update(); // update the game logic

		//void UpdatePipeline(); // update the direct3d pipeline (update command lists)

		//void Render(); // execute the command list

		//void Cleanup(); // release com ojects and clean up memory

		//void WaitForPreviousFrame(); // wait until gpu is finished with command list
	};

	inline bool RendererDX12::CreateDevice(Window& window) 
	{
		// -- Create the Device -- //

		IDXGIFactory4* dxgiFactory;
		HRESULT hr;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
		if (FAILED(hr))
		{
			return false;
		}

		IDXGIAdapter1* adapter; // adapters are the graphics card (this includes the embedded graphics on the motherboard)

		int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

		bool adapterFound = false; // set this to true when a good one was found

		
		// find first hardware gpu that supports d3d 12
		while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// we dont want a software device
				adapterIndex++; // add this line here. Its not currently in the downloadable project
				continue;
			}
			std::cout << "description : ";
			std::wcout << desc.Description;
			std::cout << "\n";
			std::cout << "RAM : " << desc.DedicatedVideoMemory / 1024 / 1024 << "MB\n";
			std::cout << "sys RAM : " << desc.DedicatedSystemMemory / 1024 / 1024 << "MB\n";

			// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
			hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
			{
				adapterFound = true;
				break;
			}

			adapterIndex++;
		}

		if (!adapterFound)
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not find a DirectX 12 adapter." };
			return false;
		}
		
		return true;
	};
}