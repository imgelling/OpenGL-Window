#pragma once

namespace game
{
	class Texture2d
	{
	public:
		uint32_t bind;
		std::string name;
		uint32_t width;
		uint32_t height;
		float_t oneOverWidth;
		float_t oneOverHeight;
		bool isCopy;
		Texture2d();
	private:
	};

	inline Texture2d::Texture2d()
	{
		bind = 0;
		width = 0;
		height = 0;
		oneOverWidth = 0.0f;
		oneOverHeight = 0.0f;
		isCopy = false;
		name = "NULL";
	}
}