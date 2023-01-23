#if !defined(GAMEIMAGELOADER_H)
#define GAMEIMAGELOADER_H

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

namespace game
{
	class ImageLoader
	{
	public :
		ImageLoader();
		void* Load(const char* filename, int32_t& width, int32_t& height, int32_t& componentsPerPixel, bool flip);
		void UnLoad();
		~ImageLoader();
	private:
		void* _data = nullptr;
	};

	inline ImageLoader::ImageLoader()
	{
		_data = nullptr;
	}

	inline void* ImageLoader::Load(const char* fileName, int32_t& width, int32_t& height, int32_t& componentsPerPixel, bool flip)
	{
		// Clears data if multiple loads happen
		if (_data != nullptr)
		{
			stbi_image_free(_data);
			_data = nullptr;
		}

		// Invert for OpenGL
		stbi_set_flip_vertically_on_load(flip); 
		_data = stbi_load(fileName, &width, &height, &componentsPerPixel, 4);
		return _data;
	}

	inline void ImageLoader::UnLoad()
	{
		if (_data != nullptr)
		{
			stbi_image_free(_data);
			_data = nullptr;
		}
	}

	inline ImageLoader::~ImageLoader()
	{
		if (_data != nullptr)
		{
			stbi_image_free(_data);
		}
	}
}

#endif