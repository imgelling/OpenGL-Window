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
	friend Window;
	public:
		void SetAttributes(const GameAttributes attrib)
		{
			_attributes = attrib;
		}
		virtual bool CreateDevice(Window window) { return false; };
		virtual void DestroyDevice() {};
		virtual void Swap() {};
	protected:
		GameAttributes _attributes;
		std::vector<std::string> _extensionsAvailable;
		virtual void _ReadExtensions() {};
	};
}