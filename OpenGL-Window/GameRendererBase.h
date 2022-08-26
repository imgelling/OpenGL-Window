#pragma once
#include "GameWindow.h"
#include "GameAttributes.h"

namespace game
{
	extern GameError lastError;
	class RendererBase
	{
	public:
		virtual bool CreateDevice(Window, bool vsync) 
		{ 
			return false;
		};
		virtual void DestroyDevice() {};
		virtual void Swap() {};
	protected:
		bool _vSync = false;
		GameAttributes _attributes;
	};
}