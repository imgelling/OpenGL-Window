#pragma once
#include "GameWindow.h"
#include "GameAttributes.h"

namespace game
{
	// --- Base renderer header Start
	class RendererBase
	{
	public:
		virtual bool CreateDevice(Window, bool vsync) = 0;
		virtual void DestroyDevice(void) = 0;
		virtual void Swap(void) = 0;
	protected:
		bool _vSync = false;
		GameAttributes _attributes;
	};
	// --- Base renderer header Stop
}