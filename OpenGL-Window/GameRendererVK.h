#pragma once

#include <vulkan/vulkan.h>
#include "GameRendererBase.h"


namespace game
{

	class RendererVK : public RendererBase
	{
	public:



		bool CreateDevice(Window window, const bool vsync) override
		{
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = NULL;
			createInfo.ppEnabledExtensionNames = NULL;
			createInfo.enabledLayerCount = 0;



			



			//VkResult result = vkCreateInstance(&createInfo, nullptr, &_vkInstance);
			//if (result != VK_SUCCESS)
			//{
			//	lastError = { GameErrors::GameRenderer, "vkCreateInstance failed" };
			//	return false;
			//}


			// Engine is now running
			enginePointer->isRunning = true;

			return true;
		}
		void DestroyDevice() override
		{


		}
		void Swap() override
		{

		};
	private:
		VkInstance _vkInstance;
	};
}