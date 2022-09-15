#include "GameRendererVK.h"
#include "GameEngine.h"

namespace game
{
	extern Engine* enginePointer;
	bool RendererVK::CreateDevice(Window window)
	{
		// Enable Validation Layers if debug mode was wanted
		if (_attributes.DebugMode)
		{
			_enableValidationLayers = true;
		}

		// Create the instance
		if (!CreateInstance())
		{
			lastError = { GameErrors::GameVulkanSpecific, "Create Instance failed." };
			return false;
		};
		enginePointer->logger->Write("Create Vulkan instance successful!");


		// Engine is now running
		enginePointer->isRunning = true;

		return true;
	}

	void RendererVK::DestroyDevice()
	{
		if (_vkInstance) vkDestroyInstance(_vkInstance, nullptr);
		_vkInstance = nullptr;
	}

	void RendererVK::Swap() 
	{

	};

	void RendererVK::_ReadExtensions()
	{
		// do read extensions
	};

	bool RendererVK::CreateInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = _attributes.WindowTitle.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Game Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Check extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		// Enumerate extensions
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		enginePointer->logger->WriteQuiet("Listing " + std::to_string(extensionCount) + " Vulkan Extensions Available.");

		// Convert extension names to char*
		std::vector<const char*> extenChar;
		for (const auto& extension : extensions) {
			extenChar.push_back(extension.extensionName);
			_extensionsAvailable.emplace_back(extension.extensionName);
			enginePointer->logger->WriteQuiet(extension.extensionName);
		}

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extenChar.data();
		createInfo.enabledLayerCount = 0;

		// Create an instance
		VkResult res = vkCreateInstance(&createInfo, nullptr, &_vkInstance);
		if (res != VK_SUCCESS)
		{
			lastError = { GameErrors::GameVulkanSpecific, "vkCreateInstance failed." };
			return false;
		}

		//vkCreateDebugUtilsMessengerEXT()

		return true;
	}

	void RendererVK::HandleWindowResize(const uint32_t width, const uint32_t height)
	{

	}

	void RendererVK::FillOutRendererInfo(SystemInfo& info)
	{

	}

	bool RendererVK::LoadTexture(std::string fileName)
	{
		return true;
	}
}