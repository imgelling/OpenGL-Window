#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "GameRendererBase.h"
#include "GameEngine.h"
#include "GameTexture2D.h"

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
		void FillOutRendererInfo();
		bool CreateTexture(Texture2D& texture);
		bool LoadTexture(std::string fileName, Texture2D &texture);
		void UnLoadTexture(Texture2D& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader);
		void UnLoadShader(ShaderGL& shader);
		void SetClearColor(const Color& color) noexcept {};
		void Clear(const bool color, const bool depth, const bool stencil) noexcept {};
		void Enable(const uint32_t capability) noexcept {};
		void Disable(const uint32_t capability) noexcept {};
		void geBindTexture(const uint32_t capability, const Texture2D& texture) noexcept {};
	protected:
		void _ReadExtensions();

	private:
		VkInstance _vkInstance = nullptr;
		bool _enableValidationLayers = false;
	};

	inline bool RendererVK::CreateDevice(Window window)
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
		_logger->Write("Create Vulkan instance successful!");

		return true;
	}

	inline void RendererVK::DestroyDevice()
	{
		if (_vkInstance) vkDestroyInstance(_vkInstance, nullptr);
		_vkInstance = nullptr;
	}

	inline void RendererVK::Swap()
	{

	};

	inline void RendererVK::_ReadExtensions()
	{
		// do read extensions
	};

	inline bool RendererVK::CreateInstance()
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

		_logger->WriteQuiet("Listing " + std::to_string(extensionCount) + " Vulkan Extensions Available.");

		// Convert extension names to char*
		std::vector<const char*> extenChar;
		for (const auto& extension : extensions) {
			extenChar.push_back(extension.extensionName);
			_extensionsAvailable.emplace_back(extension.extensionName);
			_logger->WriteQuiet(extension.extensionName);
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

	inline void RendererVK::HandleWindowResize(const uint32_t width, const uint32_t height)
	{

	}

	inline void RendererVK::FillOutRendererInfo()
	{

	}

	inline bool RendererVK::CreateTexture(Texture2D& texture)
	{
		return false;
	}

	inline bool RendererVK::LoadTexture(std::string fileName, Texture2D &texture)
	{
		return false;
	}

	inline void RendererVK::UnLoadTexture(Texture2D& texture)
	{

	}

	inline bool RendererVK::LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader)
	{
		return false;
	}
	
	inline void RendererVK::UnLoadShader(ShaderGL& shader)
	{

	}
}