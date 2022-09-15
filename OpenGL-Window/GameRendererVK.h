#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "GameRendererBase.h"



namespace game
{

	class RendererVK : public RendererBase
	{
	public:
		bool CreateDevice(Window window);

		void DestroyDevice();

		void Swap();

		bool CreateInstance();

		void HandleWindowResize(const uint32_t width, const uint32_t height);
		void FillOutRendererInfo(SystemInfo& info);
		bool LoadTexture(std::string fileName);
	protected:
		void _ReadExtensions();

	private:
		VkInstance _vkInstance = nullptr;
		bool _enableValidationLayers = false;
	};
}