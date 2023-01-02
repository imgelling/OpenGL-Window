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
		bool CreateDevice(Window& window) {
			lastError = { GameErrors::GameDirectX12Specific, "Not implemented" }; return false;
			
	};
		void DestroyDevice() {};
		void Swap() {};
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset) {};
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2D& texture) { return false; };
		bool LoadTexture(std::string fileName, Texture2D& texture) { return false; };
		void UnLoadTexture(Texture2D& texture) {};
		bool LoadShader(const std::string vertex, const std::string fragment, Shader& shader) { return false; };
		void UnLoadShader(Shader& shader) {};
	protected:
		void _ReadExtensions() {};
	};
}