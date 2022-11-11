#pragma once
#include <string>
#include <vector>

#include "GameAttributes.h"
#include "GameColor.h"
#include "GameLogger.h"
#include "GameShaderGL.h"
#include "GameSystemInfo.h"
#include "GameTexture2D.h"
#include "GameWindow.h"


namespace game
{
	class RendererBase
	{
	public:
		void SetAttributes(const Attributes& attrib, Logger* logger);
		virtual bool CreateDevice(Window& window) = 0;
		virtual void DestroyDevice() = 0;
		virtual void Swap() = 0;
		virtual void HandleWindowResize(const uint32_t width, const uint32_t height) = 0;
		virtual void FillOutRendererInfo() = 0;
		virtual bool CreateTexture(Texture2D& texture) = 0;
		virtual bool LoadTexture(std::string fileName, Texture2D& texture) = 0;
		virtual void UnLoadTexture(Texture2D& texture) = 0;
		virtual bool LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader) = 0;
		virtual void UnLoadShader(ShaderGL& shader) = 0;
	protected:
		Attributes _attributes;
		std::vector<std::string> _extensionsAvailable;
		virtual void _ReadExtensions() {};
		Logger* _logger = nullptr;
	};

	inline void RendererBase::SetAttributes(const Attributes& attrib, Logger* logger)
	{
		_attributes = attrib;
		_logger = logger;
	}

}