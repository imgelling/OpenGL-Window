#pragma once
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif
#if defined(GAME_DIRECTX10)
#include <d3d10.h>
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
		bool precompiled;
#endif
#if defined(GAME_DIRECTX10)
		ID3D10VertexShader* vertexShader;
		ID3D10PixelShader* pixelShader;
		bool precompiled;
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
		precompiled = false;
#endif
#if defined(GAME_DIRECTX10)
		vertexShader = nullptr;
		pixelShader = nullptr;
		precompiled = false;
#endif
	}

	Shader::~Shader()
	{

	}
}