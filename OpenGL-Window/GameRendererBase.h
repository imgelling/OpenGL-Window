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
		virtual void DestroyDevice(void) {};
		virtual void Swap(void) {};
	protected:
		bool _vSync = false;
		GameAttributes _attributes;
	};
}