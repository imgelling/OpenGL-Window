#pragma once
#include "GameWindow.h"
#include "GameAttributes.h"

#include <string>
#include <vector>

namespace game
{
	extern GameError lastError;
	class RendererBase
	{
	//friend Window;
	public:
		void SetAttributes(const Attributes attrib)
		{
			_attributes = attrib;
		}
		virtual bool CreateDevice(Window window) = 0;
		virtual void DestroyDevice() = 0;
		virtual void Swap() = 0;
		virtual void HandleWindowResize(const uint32_t width, const uint32_t height) = 0;
	protected:
		Attributes _attributes;
		std::vector<std::string> _extensionsAvailable;
		virtual void _ReadExtensions() {};
	};
}