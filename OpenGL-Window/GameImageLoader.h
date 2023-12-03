#if !defined(GAMEIMAGELOADER_H)
#define GAMEIMAGELOADER_H

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

/*
#include <windows.h>
#include <wincodec.h>

int main(int argc, char* argv[]) {
    IWICImagingFactory* factory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        // Handle error
    }

    IWICBitmapDecoder* decoder = nullptr;
    hr = factory->CreateDecoderFromFilename(L"image.png", nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr)) {
        factory->Release();
        // Handle error
    }

    IWICBitmapFrameDecode* frame = nullptr;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) {
        decoder->Release();
        factory->Release();
        // Handle error
    }

    UINT width, height;
    hr = frame->GetSize(&width, &height);
    if (FAILED(hr)) {
        frame->Release();
        decoder->Release();
        factory->Release();
        // Handle error
    }

    UINT stride = width * 4;
    UINT bufferSize = stride * height;
    BYTE* buffer = new BYTE[bufferSize];

    hr = frame->CopyPixels(nullptr, stride, bufferSize, buffer);
    if (FAILED(hr)) {
        delete[] buffer;
        frame->Release();
        decoder->Release();
        factory->Release();
        // Handle error
    }

    // Use the image data

    delete[] buffer;
    frame->Release();
    decoder->Release();
    factory->Release();

    return 0;
}
*/

namespace game
{
	class ImageLoader
	{
	public :
		ImageLoader();
		void* Load(const char* filename, uint32_t& width, uint32_t& height, uint32_t& componentsPerPixel, bool flip);
		void UnLoad();
		~ImageLoader();
	private:
		void* _data = nullptr;
	};

	inline ImageLoader::ImageLoader()
	{
		_data = nullptr;
	}

	inline void* ImageLoader::Load(const char* fileName, uint32_t& width, uint32_t& height, uint32_t& componentsPerPixel, bool flip)
	{
		// Clears data if multiple loads happen
		if (_data != nullptr)
		{
			stbi_image_free(_data);
			_data = nullptr;
		}

		// Invert for OpenGL
		stbi_set_flip_vertically_on_load(flip); 
        int w, h, c;
		_data = stbi_load(fileName, &w, &h, &c, 4);
        width = (uint32_t)w;
        height = (uint32_t)h;
        componentsPerPixel = (uint32_t)c;
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