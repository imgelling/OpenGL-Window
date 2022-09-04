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
		virtual bool CreateDevice(Window window) { return false; };
		virtual void DestroyDevice() {};
		virtual void Swap() {};
		virtual void HandleWindowResize(const uint32_t width, const uint32_t height) {};
	protected:
		Attributes _attributes;
		std::vector<std::string> _extensionsAvailable;
		virtual void _ReadExtensions() {};
	};
}