#if !defined(GAMEIMAGELOADER_H)
#define GAMEIMAGELOADER_H

//#define STB_IMAGE_IMPLEMENTATION
//#define STBI_ONLY_PNG
//#include "stb_image.h"
#include <wincodec.h>
#include "GameHelpers.h"

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
            delete[] _data;// stbi_image_free(_data);
			_data = nullptr;
		}

        CoInitialize(NULL);
        IWICImagingFactory* factory = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
        if (FAILED(hr)) {
            // Handle error
            return nullptr;
        }

        IWICBitmapDecoder* decoder = nullptr;
        hr = factory->CreateDecoderFromFilename(ConvertToWide(fileName).c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
        if (FAILED(hr)) {
            factory->Release();
            // Handle error
            return nullptr;
        }

        IWICBitmapFrameDecode* frame = nullptr;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) {
            decoder->Release();
            factory->Release();
            // Handle error
            return nullptr;
        }

        UINT w2, h2;
        hr = frame->GetSize(&w2, &h2);
        if (FAILED(hr)) {
            frame->Release();
            decoder->Release();
            factory->Release();
            // Handle error
            return nullptr;
        }
        width = w2;
        height = h2;
        componentsPerPixel = 4;

        UINT stride = width * 4;
        UINT bufferSize = stride * height;
        //BYTE* buffer = new BYTE[bufferSize];
        _data = new uint8_t[bufferSize];

        hr = frame->CopyPixels(nullptr, stride, bufferSize, static_cast<uint8_t*>(_data));
        if (FAILED(hr)) {
            //delete[] buffer;
            frame->Release();
            decoder->Release();
            factory->Release();
            return nullptr;
            // Handle error
        }

        // Use the image data

        //delete[] buffer;
        frame->Release();
        decoder->Release();
        factory->Release();

        CoUninitialize();

		// Invert for OpenGL
//		stbi_set_flip_vertically_on_load(flip); 

		return _data;
	}

	inline void ImageLoader::UnLoad()
	{
		if (_data != nullptr)
		{
            delete[] _data;// stbi_image_free(_data);
			_data = nullptr;
		}
	}

	inline ImageLoader::~ImageLoader()
	{
		if (_data != nullptr)
		{
            delete[] _data;// stbi_image_free(_data);
		}
	}
}

#endif