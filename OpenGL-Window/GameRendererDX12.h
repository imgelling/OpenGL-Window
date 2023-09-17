#pragma once

#include "d3dx12.h"
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
		void GetDevice(Microsoft::WRL::ComPtr<ID3D12Device2> &d3d12Device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList, Microsoft::WRL::ComPtr <ID3D12CommandQueue> &commandQueue);

		void Clear();
		D3D12_CPU_DESCRIPTOR_HANDLE currentFrameBuffer;

		Microsoft::WRL::ComPtr<ID3D12Fence> _fence[frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
		//as we have allocators (more if we want to know when the gpu is finished with an asset)
		HANDLE _fenceEvent; // a handle to an event when our fence is unlocked by the gpu
		uint64_t _fenceValue[frameBufferCount]; // this value is incremented each frame. each fence will have its own value
		uint32_t _frameIndex; // current rtv we are on
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator[frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)
		void _WaitForPreviousFrame(bool getcurrent);
	protected:
		void _ReadExtensions() {};

		Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
		Microsoft::WRL::ComPtr<ID3D12Device2> _d3d12Device; // direct3d device
		Microsoft::WRL::ComPtr <ID3D12CommandQueue> _commandQueue; // container for command lists
		Microsoft::WRL::ComPtr <IDXGISwapChain3> _swapChain; // swapchain used to switch between render targets
		Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> _rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets
		uint32_t _rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
		Microsoft::WRL::ComPtr <ID3D12Resource> _renderTargets[frameBufferCount]; // number of render targets equal to buffer count
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList; // a command list we can record commands into, then execute them to render the frame

	};

	inline void RendererDX12::GetDevice(Microsoft::WRL::ComPtr<ID3D12Device2> &d3d12Device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList, Microsoft::WRL::ComPtr <ID3D12CommandQueue> &commandQueue)
	{
		d3d12Device = _d3d12Device;
		commandList = _commandList;
		commandQueue = _commandQueue;
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
		CloseHandle(_fenceEvent);

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
		
		
		if (_attributes.DebugMode)
		{
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
		}

		// Enable debug mode if needed
		if (_attributes.DebugMode)
		{
			/*Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;*/  // Do I need to hold onto this?
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not get debug layer." };
				return false;
			}
			debugInterface->EnableDebugLayer();
		}

		Microsoft::WRL::ComPtr <IDXGIFactory4> dxgiFactory;
		if (FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create DXGIFactory." };
			return false;
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
			if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
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
		if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&_d3d12Device))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create device." };
			return false;
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
		// Describe and create the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = frameBufferCount;
		swapChainDesc.Width = _attributes.WindowWidth;
		swapChainDesc.Height = _attributes.WindowHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain;
		if (FAILED(dxgiFactory->CreateSwapChainForHwnd(
			_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
			window.GetHandle(),
			&swapChainDesc,
			nullptr,
			nullptr,
			&tempSwapChain
		)))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create swap chain." };
			return false;
		}

		// Need to cast to IDXGISwapChain3 for GetCurrentBackBufferIndex
		if (FAILED(tempSwapChain.As(&_swapChain)))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not cast to IDXGISwapChain3." };
			return false;
		}
		_frameIndex = _swapChain->GetCurrentBackBufferIndex();

		// Create RTV Descriptor Heap -- //

		// describe an rtv descriptor heap and create
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = frameBufferCount; 
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; 
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		if (FAILED(_d3d12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvDescriptorHeap))))
		{
			lastError = { GameErrors::GameDirectX12Specific, "Could not create RTV descriptor heap." };
			return false;
		}
		_rtvDescriptorHeap->SetName(L"RTV descriptor heap");
		_rtvDescriptorSize = _d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Handle to start of RTV heap
		D3D12_CPU_DESCRIPTOR_HANDLE  rtvHandle = _rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		// Create a RTV for each buffer
		for (int i = 0; i < frameBufferCount; i++)
		{
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]))))
			{
				lastError = { GameErrors::GameDirectX12Specific, "Could not get buffer for RTV " + std::to_string(i) };
				return false;
			}
			_renderTargets[i]->SetName(L"FrameBuffer");

			// Create RTV
			_d3d12Device->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);

			// Increment the rtv handle by the rtv descriptor size
			rtvHandle.ptr += _rtvDescriptorSize;
		}

		// MS only makes 1
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
		_commandList->SetName(L"CommandList");

		_commandList->Close();

		// MS creates only 1
		// Create the fences
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
// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
// sample illustrates how to use fences for efficient resource usage and to
// maximize GPU utilization.

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

		// reset the command list. 
		if (FAILED(_commandList->Reset(_commandAllocator[_frameIndex].Get(), NULL)))
		{
			std::cout << "command list reset failed\n";
			//Running = false;
		}

		// Transition current rendertarget to render target state
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		_commandList->ResourceBarrier(1, &barrier);

		// Set the current render target
		currentFrameBuffer = _rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		currentFrameBuffer.ptr += ((SIZE_T)_frameIndex * _rtvDescriptorSize);
		_commandList->OMSetRenderTargets(1, &currentFrameBuffer, FALSE, nullptr);
	}

	inline void RendererDX12::EndFrame()
	{
		// transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a

		CD3DX12_RESOURCE_BARRIER t = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		_commandList->ResourceBarrier(1, &t);

		if (FAILED(_commandList->Close()))
		{
			//Running = false;
			std::cout << "Commandlist close failed\n";
		}
	}

	inline void RendererDX12::Swap()
	{
		HRESULT hr;

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
			//Running = false;
		}

		// present the current backbuffer
		hr = _swapChain->Present(_attributes.VsyncOn ? 1 : 0, DXGI_PRESENT_ALLOW_TEARING);
		if (FAILED(hr))
		{
			//Running = false;
		}
		//_WaitForPreviousFrame(true);
	}

	inline bool RendererDX12::LoadShader(const std::string vertex, const std::string fragment, Shader& shader)
	{
		if (!shader.isPrecompiled)
		{
			DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
			if (_attributes.DebugMode)
			{
				flags |= D3DCOMPILE_DEBUG;
				flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
			}
			else
			{
				//flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
				flags = 0;
			}
			Microsoft::WRL::ComPtr<ID3DBlob> compiledVertexShader;// = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> compiledPixelShader;// = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> compilationMsgs;// = nullptr;

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
				return false;
			}
			
			// Cache the compiled code
			shader.compiledVertexShader12 = compiledVertexShader;
			shader.compiledPixelShader12 = compiledPixelShader;


			return true;
		}
		else
		{
			// Load compiled vertex shader
			Microsoft::WRL::ComPtr<ID3DBlob> compiledPixelShader;// = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> compiledVertexShader;// = nullptr;

			// Load vertex shader
			if (FAILED(D3DReadFileToBlob((ConvertToWide(vertex).c_str()), &compiledVertexShader)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not read vertex file \"" + vertex + "\"." };
				return false;
			}

			// Load pixel shader
			if (FAILED(D3DReadFileToBlob((ConvertToWide(fragment).c_str()), &compiledPixelShader)))
			{
				lastError = { GameErrors::GameDirectX11Specific,"Could not read pixel file \"" + fragment + "\"." };
				return false;
			}

			// Cache the compiled code
			shader.compiledVertexShader12 = compiledVertexShader;
			shader.compiledPixelShader12 = compiledPixelShader;

		}
		return true;
	}

	inline void RendererDX12::UnLoadShader(Shader& shader)
	{

	}
}

#undef frameBufferCount