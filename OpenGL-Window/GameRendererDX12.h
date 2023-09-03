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
		uint32_t _frameIndex; // current rtv we are on
		ID3D12DescriptorHeap* _rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets
		uint32_t _rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
		ID3D12Resource* _renderTargets[frameBufferCount]; // number of render targets equal to buffer count
		ID3D12CommandAllocator* _commandAllocator[frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)
		ID3D12GraphicsCommandList* _commandList; // a command list we can record commands into, then execute them to render the frame
		ID3D12Fence* _fence[frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
//as we have allocators (more if we want to know when the gpu is finished with an asset)
		HANDLE _fenceEvent; // a handle to an event when our fence is unlocked by the gpu
		UINT64 _fenceValue[frameBufferCount]; // this value is incremented each frame. each fence will have its own value




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
		_rtvDescriptorHeap = nullptr;
		for (uint32_t buffer = 0; buffer < frameBufferCount; buffer++)
		{
			_renderTargets[buffer] = nullptr;
			_commandAllocator[buffer] = nullptr;
			_fence[buffer] = nullptr;
		}
		_commandList = nullptr;
	}

	inline void RendererDX12::DestroyDevice()
	{
		SAFE_RELEASE(_d3d12Device);
		SAFE_RELEASE(_commandQueue);
		SAFE_RELEASE(_swapChain);
		SAFE_RELEASE(_rtvDescriptorHeap);
		for (uint32_t buffer = 0; buffer < frameBufferCount; buffer++)
		{
			SAFE_RELEASE(_renderTargets[buffer]);
			SAFE_RELEASE(_commandAllocator[buffer]);
			SAFE_RELEASE(_fence[buffer]);
		}
		SAFE_RELEASE(_commandList);
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

		DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
		backBufferDesc.Width = _attributes.WindowWidth; // buffer width
		backBufferDesc.Height = _attributes.WindowHeight; // buffer height
		backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)

		// describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

		// Describe and create the swap chain.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = frameBufferCount; // number of buffers we have
		swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
		swapChainDesc.OutputWindow = window.GetHandle(); // handle to our window
		swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
		swapChainDesc.Windowed = !_attributes.WindowFullscreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

		IDXGISwapChain* tempSwapChain;

		if (FAILED(dxgiFactory->CreateSwapChain(
			_commandQueue, // the queue will be flushed once the swap chain is created
			&swapChainDesc, // give it the swap chain description we created above
			&tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
		)))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create swap chain." };
			return false;
		}

		// need to cast to IDXGISwapChain3 for GetCurrentBackBufferIndex
		_swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
		_frameIndex = _swapChain->GetCurrentBackBufferIndex();

		// -- Create the Back Buffers (render target views) Descriptor Heap -- //

		// describe an rtv descriptor heap and create
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = frameBufferCount; // number of descriptors for this heap.
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

		// This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
		// otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		if (FAILED(_d3d12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvDescriptorHeap))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create descriptor heap." };
			return false;
		}

		// get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
		// descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
		// device to give us the size. we will use this size to increment a descriptor handle offset
		_rtvDescriptorSize = _d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
		// but we cannot literally use it like a c++ pointer.
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = _rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		// Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
		for (int i = 0; i < frameBufferCount; i++)
		{
			// first we get the n'th buffer in the swap chain and store it in the n'th
			// position of our ID3D12Resource array
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not get buffer for RTV " + std::to_string(i) };
				return false;
			}

			// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
			_d3d12Device->CreateRenderTargetView(_renderTargets[i], nullptr, rtvHandle);

			// we increment the rtv handle by the rtv descriptor size we got above
			rtvHandle.ptr += (SIZE_T)i * _rtvDescriptorSize;
		}

		// -- Create the Command Allocators -- //
		for (int i = 0; i < frameBufferCount; i++)
		{
			if (FAILED(_d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator[i]))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not create command allocator " + std::to_string(i) };
				return false;
			}
		}

		// create the command list with the first allocator
		if (FAILED(_d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator[0], NULL, IID_PPV_ARGS(&_commandList))))
		{
			lastError = { GameErrors::GameDirectX12Specific,"Could not create command list." };
			return false;
		}

		// -- Create a Fence & Fence Event -- //

// create the fences
		for (int i = 0; i < frameBufferCount; i++)
		{
			if (FAILED(_d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence[i]))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not create fence " + std::to_string(i) };
				return false;
			}
			_fenceValue[i] = 0; // set the initial fence value to 0
		}

		// create a handle to a fence event
		_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (_fenceEvent == nullptr)
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create fence event" };
			return false;
		}

		return true;
	};

}

#undef frameBufferCount