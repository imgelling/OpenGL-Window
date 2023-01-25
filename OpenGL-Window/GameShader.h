#pragma once
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif
namespace game
{
	class Shader
	{
	public:
		Shader();
		~Shader();
#if defined(GAME_OPENGL)
		uint32_t shaderId;
		uint32_t vertexId;
		uint32_t fragmentId;
#endif
#if defined(GAME_DIRECTX9)
		IDirect3DVertexShader9* vertexShader;
		IDirect3DPixelShader9* pixelShader;
#endif
	private:
	};

	Shader::Shader()
	{
#if defined(GAME_OPENGL)
		shaderId = 0;
		vertexId = 0;
		fragmentId = 0;
#endif

#if defined(GAME_DIRECTX9)
		vertexShader = nullptr;
		pixelShader = nullptr;
#endif
	}

	Shader::~Shader()
	{

	}
}