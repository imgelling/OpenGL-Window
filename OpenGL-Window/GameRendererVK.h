#pragma once

#include <vulkan/vulkan.h>
#include <vector>
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
			appInfo.pApplicationName = "Spinning Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Game Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = NULL;
			createInfo.ppEnabledExtensionNames = NULL;
			createInfo.enabledLayerCount = 0;

			// Create an instance
			VkResult res = vkCreateInstance(&createInfo, nullptr, &_vkInstance);
			if (res != VK_SUCCESS)
			{
				lastError = { GameErrors::GameVulkanSpecific, "vkCreateInstance failed." };
				return false;
			}

			// Check extensions
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			
			// Enumerate extensions
			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			// Print them out
			std::cout << "available extensions:\n";

			for (const auto& extension : extensions) {
				std::cout << '\t' << extension.extensionName << '\n';
			}

			// Engine is now running
			enginePointer->isRunning = true;

			return true;
		}

		void DestroyDevice() override
		{
			vkDestroyInstance(_vkInstance, nullptr);
		}

		void Swap() override
		{

		};
	private:
		VkInstance _vkInstance;
	};
}