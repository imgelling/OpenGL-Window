#pragma once
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
#include <d3d9.h>
#endif

namespace game
{
	enum class TextureFilterType
	{
		Point,
		Bilinear,
		Trilinear
	};

	class Texture2D
	{
	public:
#if defined(GAME_OPENGL)
		uint32_t bind;
#endif
#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL) // this breaks shit if I shorten it, don't know why
		LPDIRECT3DTEXTURE9 textureInterface;
#endif
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

		Texture2D();
	private:
	};

	//  GL_LINEAR will give you bilinear filtering. GL_LINEAR_MIPMAP_LINEAR should be trilinear.
	//  GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
	//	GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap leveland samples that level using linear interpolation.
	// GL_NEAREST_MIPMAP_LINEAR : linearly interpolates between the two mipmaps that most closely match the size of a pixeland samples the interpolated level via nearest neighbor interpolation.
	// GL_LINEAR_MIPMAP_LINEAR : linearly interpolates between the two closest mipmapsand samples the interpolated level via linear interpolation.
	// anistropic, mipmaps

	inline Texture2D::Texture2D()
	{
#if defined(GAME_OPENGL)
		bind = 0;
#endif
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