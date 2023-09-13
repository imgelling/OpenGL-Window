#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>

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
		void Swap();
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset) {};
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2D& texture) {
			lastError = { GameErrors::GameDirectX12Specific,"Texture not implemented " }; return false;
		};
		bool LoadTexture(std::string fileName, Texture2D& texture) {
			lastError = { GameErrors::GameDirectX12Specific,"Texture not implemented " }; return false;
		};
		void UnLoadTexture(Texture2D& texture) {};
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader);
		bool LoadShader(const std::string vertex, const std::string fragment, const std::string geometry, Shader& shader)
		{
			lastError = { GameErrors::GameDirectX12Specific, "Geometry shaders not implemented yet." };
			return false;
		}
		void UnLoadShader(Shader& shader);
		void StartFrame();
		void EndFrame();
		void GetDevice(Microsoft::WRL::ComPtr<ID3D12Device2> &d3d12Device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList);

		void Clear();
		D3D12_CPU_DESCRIPTOR_HANDLE currentFrameBuffer;
	protected:
		void _ReadExtensions() {};
		void _WaitForPreviousFrame(bool getcurrent);

		//Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
		Microsoft::WRL::ComPtr<ID3D12Device2> _d3d12Device; // direct3d device
		Microsoft::WRL::ComPtr <ID3D12CommandQueue> _commandQueue; // container for command lists
		Microsoft::WRL::ComPtr <IDXGISwapChain3> _swapChain; // swapchain used to switch between render targets
		uint32_t _frameIndex; // current rtv we are on
		Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> _rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets
		uint32_t _rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
		Microsoft::WRL::ComPtr <ID3D12Resource> _renderTargets[frameBufferCount]; // number of render targets equal to buffer count
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator[frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList; // a command list we can record commands into, then execute them to render the frame
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence[frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
//as we have allocators (more if we want to know when the gpu is finished with an asset)
		HANDLE _fenceEvent; // a handle to an event when our fence is unlocked by the gpu
		uint64_t _fenceValue[frameBufferCount]; // this value is incremented each frame. each fence will have its own value
	};

	inline void RendererDX12::GetDevice(Microsoft::WRL::ComPtr<ID3D12Device2> &d3d12Device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList)
	{
		d3d12Device = _d3d12Device;
		commandList = _commandList;
	}

	inline RendererDX12::RendererDX12()
	{
		_d3d12Device = nullptr;
		_commandQueue = nullptr;
		_swapChain = nullptr;
		_rtvDescriptorHeap = nullptr;
		_frameIndex = 0;
		_rtvDescriptorSize = 0;
		for (uint32_t buffer = 0; buffer < frameBufferCount; buffer++)
		{
			_renderTargets[buffer] = nullptr;
			_commandAllocator[buffer] = nullptr;
			_fence[buffer] = nullptr;
			_fenceValue[buffer] = 0;
		}
		_commandList = nullptr;
		_fenceEvent = nullptr;
	}

	inline void RendererDX12::DestroyDevice()
	{
		// get swapchain out of full screen before exiting
		BOOL fs = false;
		if (_swapChain)
		{
			_swapChain->GetFullscreenState(&fs, NULL);
			if (fs)
				_swapChain->SetFullscreenState(false, NULL);

			// wait for the gpu to finish all frames
			for (int i = 0; i < frameBufferCount; ++i)
			{
				_frameIndex = i;
				_WaitForPreviousFrame(false);
			}
		}

		//SAFE_RELEASE(_d3d12Device);
		//SAFE_RELEASE(_commandQueue);
		//SAFE_RELEASE(_swapChain);
		//SAFE_RELEASE(_rtvDescriptorHeap);
		//for (uint32_t buffer = 0; buffer < frameBufferCount; buffer++)
		//{
			//SAFE_RELEASE(_renderTargets[buffer]);
			//SAFE_RELEASE(_commandAllocator[buffer]);
			//SAFE_RELEASE(_fence[buffer]);
		//}
		//SAFE_RELEASE(_commandList);
	}

	inline bool RendererDX12::CreateDevice(Window& window)
	{
		uint32_t createFactoryFlags = 0;
		std::vector<IDXGIAdapter1*> adapterList;  // needs in class
		IDXGIAdapter1* adapter = nullptr;
		uint32_t adapterIndex = 0;
		
		//
		//if (_attributes.DebugMode)
		//{
		//	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
		//}

		Microsoft::WRL::ComPtr <IDXGIFactory4> dxgiFactory;
		if (FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create DXGIFactory." };
			return false;
		}

		//// Enable debug mode if needed
		//if (_attributes.DebugMode)
		//{
		//	/*Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;*/  // Do I need to hold onto this?
		//	if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
		//	{
		//		lastError = { GameErrors::GameDirectX12Specific, "Could not get debug layer." };
		//		return false;
		//	}
		//	debugInterface->EnableDebugLayer();
		//}


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
		//if (_attributes.DebugMode)
		//{
		//	Microsoft::WRL::ComPtr <ID3D12InfoQueue> infoQueue = nullptr;
		//	if (FAILED(_d3d12Device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
		//	{
		//		lastError = { GameErrors::GameDirectX12Specific, "Could not get info queue." };
		//		return false;
		//	}
		//	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		//	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		//	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
		//	// Suppress whole categories of messages
		//	//D3D12_MESSAGE_CATEGORY Categories[] = {};

		//	// Suppress messages based on their severity level
		//	D3D12_MESSAGE_SEVERITY severities[] =
		//	{
		//		D3D12_MESSAGE_SEVERITY_INFO
		//	};

		//	// Suppress individual messages by their ID
		//	D3D12_MESSAGE_ID denyIds[] = {
		//		D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
		//		D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
		//		D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
		//	};

		//	D3D12_INFO_QUEUE_FILTER newFilter = {};
		//	newFilter.DenyList.NumCategories = 0;// _countof(Categories);
		//	newFilter.DenyList.pCategoryList = NULL;// Categories;
		//	newFilter.DenyList.NumSeverities = 0;// _countof(severities);
		//	newFilter.DenyList.pSeverityList = NULL;// severities;
		//	newFilter.DenyList.NumIDs = _countof(denyIds);
		//	newFilter.DenyList.pIDList = denyIds;

		//	if (FAILED(infoQueue->PushStorageFilter(&newFilter)))
		//	{
		//		lastError = { GameErrors::GameDirectX12Specific,"Could not update debug filter." };
		//		return false;
		//	}
		//}

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
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		Microsoft::WRL::ComPtr <IDXGISwapChain> tempSwapChain;

		if (FAILED(dxgiFactory->CreateSwapChain(
			_commandQueue.Get(), // the queue will be flushed once the swap chain is created
			&swapChainDesc, // give it the swap chain description we created above
			&tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
		)))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create swap chain." };
			return false;
		}

		// need to cast to IDXGISwapChain3 for GetCurrentBackBufferIndex
		tempSwapChain.As(&_swapChain);
		_frameIndex = _swapChain->GetCurrentBackBufferIndex();

		// -- Create the Back Buffers (render target views) Descriptor Heap -- //

		// describe an rtv descriptor heap and create
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = frameBufferCount; // number of descriptors for this heap.
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

		// This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
		// otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		if (FAILED(_d3d12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvDescriptorHeap))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create descriptor heap." };
			return false;
		}
		_rtvDescriptorHeap->SetName(L"RTV descriptor heap");

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
			_renderTargets[i]->SetName(L"FrameBuffer");

			// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
			_d3d12Device->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);

			// we increment the rtv handle by the rtv descriptor size we got above
			rtvHandle.ptr += _rtvDescriptorSize;
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
		if (FAILED(_d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator[0].Get(), NULL, IID_PPV_ARGS(&_commandList))))
		{
			lastError = { GameErrors::GameDirectX12Specific,"Could not create command list." };
			return false;
		}

		_commandList->Close();
		_commandList->SetName(L"CommandList");

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
			lastError = { GameErrors::GameDirectX12Specific, "Could not create fence event handle" };
			return false;
		}

		return true;
	};

	inline void RendererDX12::_WaitForPreviousFrame(bool getcurrent)
	{
		HRESULT hr;

		// swap the current rtv buffer index so we draw on the correct buffer
		if (getcurrent)
			_frameIndex = _swapChain->GetCurrentBackBufferIndex();

		// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
		// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
		if (_fence[_frameIndex]->GetCompletedValue() < _fenceValue[_frameIndex])
		{
			// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
			hr = _fence[_frameIndex]->SetEventOnCompletion(_fenceValue[_frameIndex], _fenceEvent);
			if (FAILED(hr))
			{
				lastError = { GameErrors::GameDirectX12Specific,"Fence event completion failed." };
				//enginePointer->geLogLastError();// Running = false;
			}

			// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
			// has reached "fenceValue", we know the command queue has finished executing
			WaitForSingleObject(_fenceEvent, INFINITE);
		}

		// increment fenceValue for next frame
		_fenceValue[_frameIndex]++;
		if (getcurrent)
			_frameIndex = _swapChain->GetCurrentBackBufferIndex();
	}

	inline void RendererDX12::Clear()
	{

		// Clear the render target by using the ClearRenderTargetView command
		_commandList->ClearRenderTargetView(currentFrameBuffer, Colors::DarkGray.rgba, 0, nullptr);

	}

	inline void RendererDX12::StartFrame()
	{

		// We have to wait for the gpu to finish with the command allocator before we reset it
		_WaitForPreviousFrame(true);

		// we can only reset an allocator once the gpu is done with it
		// resetting an allocator frees the memory that the command list was stored in
		if (FAILED(_commandAllocator[_frameIndex]->Reset()))
		{
			//Running = false;
			std::cout << "Command allocator reset failed\n";
		}

		// reset the command list. by resetting the command list we are putting it into
		// a recording state so we can start recording commands into the command allocator.
		// the command allocator that we reference here may have multiple command lists
		// associated with it, but only one can be recording at any time. Make sure
		// that any other command lists associated to this command allocator are in
		// the closed state (not recording).
		// Here you will pass an initial pipeline state object as the second parameter,
		// but in this tutorial we are only clearing the rtv, and do not actually need
		// anything but an initial default pipeline, which is what we get by setting
		// the second parameter to NULL
		if (FAILED(_commandList->Reset(_commandAllocator[_frameIndex].Get(), NULL)))
		{
			std::cout << "command list reset failed\n";
			//Running = false;
		}

		// here we start recording commands into the commandList (which all the commands will be stored in the commandAllocator)

		// transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here

		// start frame
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = _renderTargets[_frameIndex].Get();// pResource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		_commandList->ResourceBarrier(1, &barrier);

		currentFrameBuffer = _rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		currentFrameBuffer.ptr += ((SIZE_T)_frameIndex * _rtvDescriptorSize);
		// set the render target for the output merger stage (the output of the pipeline)
		_commandList->OMSetRenderTargets(1, &currentFrameBuffer, FALSE, nullptr);
	}

	inline void RendererDX12::EndFrame()
	{
		// end frame
// transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
// warning if present is called on the render target when it's not in the present state
		D3D12_RESOURCE_BARRIER barrier2 = {};
		barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier2.Transition.pResource = _renderTargets[_frameIndex].Get();// pResource;
		barrier2.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barrier2.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		_commandList->ResourceBarrier(1, &barrier2);//a &CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		if (FAILED(_commandList->Close()))
		{
			//Running = false;
			std::cout << "Commandlist close failed\n";
		}
	}

	inline void RendererDX12::Swap()
	{
		HRESULT hr;

		//Clear(); // update the pipeline by sending commands to the commandqueue

		// create an array of command lists (only one command list here)
		ID3D12CommandList* ppCommandLists[] = { _commandList.Get() };

		// execute the array of command lists
		_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// this command goes in at the end of our command queue. we will know when our command queue 
		// has finished because the fence value will be set to "fenceValue" from the GPU since the command
		// queue is being executed on the GPU
		hr = _commandQueue->Signal(_fence[_frameIndex].Get(), _fenceValue[_frameIndex]);
		if (FAILED(hr))
		{
			std::cout << "Signal failed in DX12 swap\n";
			//Running = false;
		}

		// present the current backbuffer
		hr = _swapChain->Present(_attributes.VsyncOn ? 1 : 0, DXGI_PRESENT_ALLOW_TEARING);
		if (FAILED(hr))
		{
			//Running = false;
		}
	}

	inline bool RendererDX12::LoadShader(const std::string vertex, const std::string fragment, Shader& shader)
	{
		if (!shader.isPrecompiled)
		{
			DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
			//if (_attributes.DebugMode)
			//{
			//	flags |= D3DCOMPILE_DEBUG;
			//	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
			//}
			Microsoft::WRL::ComPtr<ID3DBlob> compiledVertexShader = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> compiledPixelShader = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> compilationMsgs = nullptr;

			// Compile the vertex shader
			if (FAILED(D3DCompileFromFile(ConvertToWide(vertex).c_str(), NULL, NULL, "main", "vs_5_0", flags, NULL, &compiledVertexShader, &compilationMsgs)))
			{
				SIZE_T size = compilationMsgs->GetBufferSize();
				uint8_t* p = reinterpret_cast<unsigned char*>(compilationMsgs->GetBufferPointer());
				lastError = { GameErrors::GameDirectX12Specific,"Could not compile shader \"" + vertex + "\".\n" };
				for (uint32_t bytes = 0; bytes < size; bytes++)
				{
					lastError.lastErrorString += p[bytes];
				}
				//SAFE_RELEASE(compilationMsgs);
				//SAFE_RELEASE(compiledVertexShader);
				return false;
			}

			// Compile the pixel shader
			if (FAILED(D3DCompileFromFile(ConvertToWide(fragment).c_str(), NULL, NULL, "main", "ps_5_0", flags, NULL, &compiledPixelShader, &compilationMsgs)))
			{
				SIZE_T size = compilationMsgs->GetBufferSize();
				uint8_t* p = reinterpret_cast<unsigned char*>(compilationMsgs->GetBufferPointer());
				lastError = { GameErrors::GameDirectX12Specific,"Could not compile shader \"" + fragment + "\".\n" };
				for (uint32_t bytes = 0; bytes < size; bytes++)
				{
					lastError.lastErrorString += p[bytes];
				}
				//SAFE_RELEASE(compilationMsgs);
				//SAFE_RELEASE(compiledVertexShader);
				//SAFE_RELEASE(compiledPixelShader);
				return false;
			}

			// Free up any messages from compilation if any
			//SAFE_RELEASE(compilationMsgs);
			shader.vertexShader12.BytecodeLength = compiledVertexShader.Get()->GetBufferSize();
			shader.vertexShader12.pShaderBytecode = compiledVertexShader.Get()->GetBufferPointer();
			shader.compiledVertexShader12 = compiledVertexShader;
			shader.pixelShader12.BytecodeLength = compiledPixelShader.Get()->GetBufferSize();
			shader.pixelShader12.pShaderBytecode = compiledPixelShader.Get()->GetBufferPointer();
			shader.compiledVertexShader12 = compiledPixelShader;


			//bytecode stuff

			return true;
		}
		else
		{
			// Load compiled vertex shader
			Microsoft::WRL::ComPtr<ID3DBlob> compiledPixelShader = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> compiledVertexShader = nullptr;

			// Load vertex shader
			if (FAILED(D3DReadFileToBlob((ConvertToWide(vertex).c_str()), &compiledVertexShader)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not read vertex file \"" + vertex + "\"." };
				//SAFE_RELEASE(compiledVertexShader);
				return false;
			}

			// Load pixel shader
			if (FAILED(D3DReadFileToBlob((ConvertToWide(fragment).c_str()), &compiledPixelShader)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not read pixel file \"" + fragment + "\"." };
				//SAFE_RELEASE(compiledVertexShader);
				//SAFE_RELEASE(shader.vertexShader11);
				SAFE_RELEASE(compiledPixelShader);
			}

			// byte code stuff
			shader.vertexShader12.BytecodeLength = compiledVertexShader.Get()->GetBufferSize();
			shader.vertexShader12.pShaderBytecode = compiledVertexShader.Get()->GetBufferPointer();
			shader.compiledVertexShader12 = compiledVertexShader;
			shader.pixelShader12.BytecodeLength = compiledPixelShader.Get()->GetBufferSize();
			shader.pixelShader12.pShaderBytecode = compiledPixelShader.Get()->GetBufferPointer();
			shader.compiledVertexShader12 = compiledPixelShader;

		}
		return true;
	}

	inline void RendererDX12::UnLoadShader(Shader& shader)
	{

	}
}

#undef frameBufferCount