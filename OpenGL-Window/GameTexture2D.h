#pragma once

namespace game
{
	enum class TextureFilterType
	{
		Point,
		Bilinear,
		Trilinear
	};

	class Texture2dGL
	{
	public:
		uint32_t bind;
		std::string name;
		uint32_t width;
		uint32_t height;
		int32_t componentsPerPixel;
		float_t oneOverWidth;
		float_t oneOverHeight;
		bool isCopy;

		// Attributes of the texture filtering
		bool isMipMapped;
		TextureFilterType filterType;
		uint8_t anisotropyLevel;		// 1 is off, then powers of 2 up to max

		Texture2dGL();
	private:
	};

	//  GL_LINEAR will give you bilinear filtering. GL_LINEAR_MIPMAP_LINEAR should be trilinear.
	//  GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
	//	GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap leveland samples that level using linear interpolation.
	// GL_NEAREST_MIPMAP_LINEAR : linearly interpolates between the two mipmaps that most closely match the size of a pixeland samples the interpolated level via nearest neighbor interpolation.
	// GL_LINEAR_MIPMAP_LINEAR : linearly interpolates between the two closest mipmapsand samples the interpolated level via linear interpolation.
	// anistropic, mipmaps

	inline Texture2dGL::Texture2dGL()
	{
		bind = 0;
		width = 0;
		height = 0;
		oneOverWidth = 0.0f;
		oneOverHeight = 0.0f;
		componentsPerPixel = 0;
		isCopy = false;
		name = "NULL";

		// Attributes of texture filtering
		isMipMapped = true;
		filterType = TextureFilterType::Trilinear;
		anisotropyLevel = 1;
	}
}