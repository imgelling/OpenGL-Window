#pragma once
#include "GameWindow.h"
#include "GameAttributes.h"

namespace game
{
	extern GameError lastError;
	class RendererBase
	{
	public:
		void SetAttributes(const GameAttributes attrib)
		{
			_attributes = attrib;
		}
		virtual bool CreateDevice(Window window) 
		{ 
			return false;
		};
		virtual void DestroyDevice() {};
		virtual void Swap() {};
	protected:
		GameAttributes _attributes;
	};
}