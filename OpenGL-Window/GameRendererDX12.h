#pragma once

//#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
//#include <DirectXMath.h>
#include <vector>

#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameRendererBase.h"
#include "GameShader.h"
#include "GameSystemInfo.h"
#include "GameTexture2D.h"

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = nullptr; } }
#endif

#define frameBufferCount  3 

namespace game
{
	extern SystemInfo systemInfo;
	class RendererDX12 : public RendererBase
	{
	public:
		RendererDX12();
		bool CreateDevice(Window& window);
		void DestroyDevice();
		void Swap() {};
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset) {};
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2D& texture) {
			lastError = { GameErrors::GameDirectX12Specific,"Texture not implemented " }; return false;
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

		ID3D12Device2* _d3d12Device; // direct3d device
		ID3D12CommandQueue* _commandQueue; // container for command lists
		IDXGISwapChain3* _swapChain; // swapchain used to switch between render targets

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

	inline RendererDX12::RendererDX12()
	{
		_d3d12Device = nullptr;
		_commandQueue = nullptr;
		_swapChain = nullptr;
	}

	inline void RendererDX12::DestroyDevice()
	{
		SAFE_RELEASE(_d3d12Device);
		SAFE_RELEASE(_commandQueue);
		SAFE_RELEASE(_swapChain);
	}

	inline bool RendererDX12::CreateDevice(Window& window)
	{
		uint32_t createFactoryFlags = 0;
		std::vector<IDXGIAdapter1*> adapterList;  // needs in class
		IDXGIAdapter1* adapter = nullptr;
		uint32_t adapterIndex = 0;
		
		
		if (_attributes.DebugMode)
		{
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
		}

		IDXGIFactory4* dxgiFactory;
		if (FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create DXGIFactory." };
			return false;
		}

		// Enable debug mode if needed
		if (_attributes.DebugMode)
		{
			Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;  // Do I need to hold onto this?
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not get debug layer." };
				return false;
			}
			debugInterface->EnableDebugLayer();
		}


		// Enumerate all adapters for dx12 support
		while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			// Ignore software adapters
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				adapterIndex++;
				continue;
			}
			std::cout << "description : ";
			std::wcout << desc.Description;
			std::cout << "\n";
			std::cout << "RAM : " << desc.DedicatedVideoMemory / 1024 / 1024 << "MB\n";
			std::cout << "sys RAM : " << desc.DedicatedSystemMemory / 1024 / 1024 << "MB\n";
			std::cout << "Vendor id : " << desc.VendorId << "\n";

			// Check for dx12 support on current adapter
			if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				// Save the adapter
				adapterList.emplace_back(adapter);
			}

			// Check the next adapter
			adapterIndex++;
		}

		// If list is empty, no suitable adapter found
		if (adapterList.empty())
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not find a DirectX 12 adapter." };
			return false;
		}




		// Create the device
		if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_d3d12Device))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create device." };
			return false;
		}

		// Filter debug messages 
		if (_attributes.DebugMode)
		{
			ID3D12InfoQueue* infoQueue = nullptr;
			if (FAILED(_d3d12Device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not get info queue." };
				return false;
			}
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
			// Suppress whole categories of messages
			//D3D12_MESSAGE_CATEGORY Categories[] = {};

			// Suppress messages based on their severity level
			D3D12_MESSAGE_SEVERITY severities[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO
			};

			// Suppress individual messages by their ID
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
			};

			D3D12_INFO_QUEUE_FILTER newFilter = {};
			//NewFilter.DenyList.NumCategories = _countof(Categories);
			//NewFilter.DenyList.pCategoryList = Categories;
			newFilter.DenyList.NumSeverities = _countof(severities);
			newFilter.DenyList.pSeverityList = severities;
			newFilter.DenyList.NumIDs = _countof(denyIds);
			newFilter.DenyList.pIDList = denyIds;

			if (FAILED(infoQueue->PushStorageFilter(&newFilter)))
			{
				lastError = { GameErrors::GameDirectX12Specific,"Could not update debug filter." };
				return false;
			}
		}

		// Create the command queue
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = { }; // Use defaults
		if (FAILED(_d3d12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&_commandQueue))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create command queue." };
			return false;
		}

		// Create the swap chain




		return true;
	};

}

#undef frameBufferCount