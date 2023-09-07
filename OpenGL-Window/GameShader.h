#pragma once
#if defined(GAME_DIRECTX11)
#include <d3d11.h>
#endif
#if defined(GAME_DIRECTX10)
#include <d3d10.h>
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
		ID3D10GeometryShader* geometryShader10;
		ID3DBlob* compiledPixelShader10;
		ID3DBlob* compiledVertexShader10;
		ID3DBlob* compiledGeometryShader10;
#endif
#if defined(GAME_DIRECTX11)
		ID3D11VertexShader* vertexShader11;
		ID3D11PixelShader* pixelShader11;
		ID3D11GeometryShader* geometryShader11;
		ID3DBlob* compiledPixelShader11;
		ID3DBlob* compiledVertexShader11;
		ID3DBlob* compiledGeometryShader11;
#endif
#if defined(GAME_DIRECTX12)
		D3D12_SHADER_BYTECODE vertexShader12;
		D3D12_SHADER_BYTECODE pixelShader12;
		D3D12_SHADER_BYTECODE geometryShader12;
		//ID3DBlob* compiledPixelShader12;
		//ID3DBlob* compiledVertexShader12;
		//ID3DBlob* compiledGeometryShader12;
#endif
#if defined(GAME_DIRECTX9) | defined(GAME_DIRECTX10) | defined(GAME_DIRECTX11) | defined(GAME_DIRECTX12)
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
		geometryShader10 = nullptr;
		compiledPixelShader10 = nullptr;
		compiledVertexShader10 = nullptr;
		compiledGeometryShader10 = nullptr;
#endif
#if defined(GAME_DIRECTX11)
		vertexShader11 = nullptr;
		pixelShader11 = nullptr;
		geometryShader11 = nullptr;
		compiledPixelShader11 = nullptr;
		compiledVertexShader11 = nullptr;
		compiledGeometryShader11 = nullptr;
#endif
#if defined(GAME_DIRECTX12)
		vertexShader12 = {};
		pixelShader12 = {};
		geometryShader12 = {};
#endif
#if defined(GAME_DIRECTX9) | defined(GAME_DIRECTX10) | defined(GAME_DIRECTX11) | defined(GAME_DIRECTX12)
		isPrecompiled = false;
#endif
	}

	Shader::~Shader()
	{

	}
}