#pragma once
#include <string>

//friend class GameContent;
//friend class GameSpriteBatch;
//	public:
//		Shader();
//		~Shader();
//		void Bind();
//		void UnBind();
//		void UnLoad();
//		unsigned int Id();
//
//	private:
//		std::string Load(std::string vertex, std::string fragment);
//		std::string validateShader(unsigned int shader, const char* file = 0);
//		std::string validateProgram(unsigned int program);
//		char* LoadTextFile(const char* fileName);
//		unsigned int shaderId;
//		unsigned int vertexId;
//		unsigned int fragmentId;
//		bool loaded;
namespace game
{
	// This might not work out well, cyclic dependencies #include RendererGL and #include ShaderGL
	class ShaderGL
	{
	public:
		ShaderGL();
		~ShaderGL();
		uint32_t shaderId;
		uint32_t vertexId;
		uint32_t fragmentId;
	private:
	};

	ShaderGL::ShaderGL()
	{
		shaderId = 0;
		vertexId = 0;
		fragmentId = 0;
	}

	ShaderGL::~ShaderGL()
	{

	}
}