#pragma once
#include "GameWindow.h"
#include "GameAttributes.h"
#include "GameSystemInfo.h"
#include "GameLogger.h"
#include "GameTexture2D.h"
#include "GameShaderGL.h"
#include "GameColor.h"

#include <string>
#include <vector>

namespace game
{
	extern GameError lastError;
	class RendererBase
	{
	public:
		void SetAttributes(const Attributes attrib, Logger *logger)
		{
			_attributes = attrib;
			_logger = logger;
		}
		virtual bool CreateDevice(Window& window) = 0;
		virtual void DestroyDevice() = 0;
		virtual void Swap() = 0;
		virtual void HandleWindowResize(const uint32_t width, const uint32_t height) = 0;
		virtual void FillOutRendererInfo() = 0;
		virtual bool CreateTexture(Texture2dGL& texture) = 0;
		virtual bool LoadTexture(std::string fileName, Texture2dGL &texture) = 0;
		virtual void UnLoadTexture(Texture2dGL& texture) = 0;
		virtual bool LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader) = 0;
		virtual void UnLoadShader(ShaderGL& shader) = 0;
		virtual void SetClearColor(const Color& color) noexcept = 0;
		virtual void Clear(const bool color, const bool depth, const bool stencil) noexcept = 0;
	protected:
		Attributes _attributes;
		std::vector<std::string> _extensionsAvailable;
		virtual void _ReadExtensions() {};
		Logger* _logger = nullptr;
	};
}