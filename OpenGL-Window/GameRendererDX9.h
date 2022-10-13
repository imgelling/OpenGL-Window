#pragma once

namespace game
{
	class RendererDX9 : public RendererBase
	{
	public:
		RendererDX9() {};
		bool CreateDevice(Window window) { return false; };
		void DestroyDevice() {};
		void Swap() {};
		void HandleWindowResize(const uint32_t width, const uint32_t height) {};
		void FillOutRendererInfo() {};
		bool CreateTexture(Texture2dGL& texture) { return false; };
		bool LoadTexture(std::string fileName, Texture2dGL& texture) { return false; };
		void UnLoadTexture(Texture2dGL& texture) {};
		bool LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader) { return false; };
		void UnLoadShader(ShaderGL& shader) {};
		void SetClearColor(const Color& color) noexcept {};
		void Clear(const bool color, const bool depth, const bool stencil) noexcept {};


	protected:
		void _ReadExtensions() {};
	private:

	};
}