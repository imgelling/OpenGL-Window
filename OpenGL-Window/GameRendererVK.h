#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "GameRendererBase.h"



namespace game
{

	class RendererVK : public RendererBase
	{
	public:
		bool CreateDevice(Window window) override;

		void DestroyDevice() override;

		void Swap() override;

		bool CreateInstance();

	protected:
		void _ReadExtensions() override;

	private:
		VkInstance _vkInstance = nullptr;
		bool _enableValidationLayers = false;
	};
}