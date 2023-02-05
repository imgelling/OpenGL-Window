#pragma once
#if defined(GAME_DIRECTX10)
#include <d3d10_1.h>
#endif
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
		IDirect3DVertexShader9* vertexShader9;
		IDirect3DPixelShader9* pixelShader9;
#endif
#if defined(GAME_DIRECTX10)
		ID3D10VertexShader* vertexShader10;
		ID3D10PixelShader* pixelShader10;
		ID3DBlob* compiledPixelShader10 = nullptr;
		ID3DBlob* compiledVertexShader10 = nullptr;
#endif
#if defined(GAME_DIRECTX9) || defined(GAME_DIRECTX10)
		bool isPrecompiled;
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
		vertexShader9 = nullptr;
		pixelShader9 = nullptr;
#endif
#if defined(GAME_DIRECTX10)
		vertexShader10 = nullptr;
		pixelShader10 = nullptr;
		compiledPixelShader10 = nullptr;
		compiledVertexShader10 = nullptr;
#endif
#if defined(GAME_DIRECTX9) || defined(GAME_DIRECTX10)
		isPrecompiled = false;
#endif
	}

	Shader::~Shader()
	{

	}
}