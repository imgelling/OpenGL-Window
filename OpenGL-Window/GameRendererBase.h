#pragma once
#include "GameWindow.h"
#include "GameAttributes.h"
#include "GameSystemInfo.h"

#include <string>
#include <vector>

namespace game
{
	extern GameError lastError;
	class RendererBase
	{
	public:
		void SetAttributes(const Attributes attrib)
		{
			_attributes = attrib;
		}
		virtual bool CreateDevice(Window window) = 0;
		virtual void DestroyDevice() = 0;
		virtual void Swap() = 0;
		virtual void HandleWindowResize(const uint32_t width, const uint32_t height) = 0;
		virtual void FillOutRendererInfo(SystemInfo& info) = 0;
		virtual bool LoadTexture(std::string fileName) = 0;
	protected:
		Attributes _attributes;
		std::vector<std::string> _extensionsAvailable;
		virtual void _ReadExtensions() {};
	};
}