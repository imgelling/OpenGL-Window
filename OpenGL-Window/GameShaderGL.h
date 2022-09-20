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
		// Below will be handled by engine > renderer
		//std::string Load(const std::string vertex, const std::string fragment);
		//void Bind();
		//void UnBind();
		//void UnLoad();
		uint32_t iD;  // not sure what for

	private:
		uint32_t _shaderId;
		uint32_t _vertexId;
		uint32_t _fragmentId;
		bool _loaded; // needed?

	};

	ShaderGL::ShaderGL()
	{
		_shaderId = 0;
		_vertexId = 0;
		_fragmentId = 0;
		_loaded = false;
		iD = 0;
	}

	ShaderGL::~ShaderGL()
	{

	}
}