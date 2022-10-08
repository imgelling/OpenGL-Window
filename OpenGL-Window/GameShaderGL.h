#pragma once
#include <string>

namespace game
{
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