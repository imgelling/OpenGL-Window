#if !defined(GAMEIMAGELOADER_H)
#define GAMEIMAGELOADER_H
#include <wrl.h>
#include <wincodec.h>

#include "GameHelpers.h"

namespace game
{
	class ImageLoader
	{
	public :
		ImageLoader();
		void* Load(const char* filename, uint32_t& width, uint32_t& height, uint32_t& componentsPerPixel);
		void UnLoad();
		~ImageLoader();
	private:
		void* _data;
	};

	inline ImageLoader::ImageLoader()
	{
		_data = nullptr;
	}

	inline void* ImageLoader::Load(const char* fileName, uint32_t& width, uint32_t& height, uint32_t& componentsPerPixel)
	{
		// Clears data if multiple loads happen
		if (_data != nullptr)
		{
            delete[] _data;
			_data = nullptr;
		}

        Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(factory.GetAddressOf()));
        if (FAILED(hr)) {
            return nullptr;
        }

        Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
        hr = factory->CreateDecoderFromFilename(ConvertToWide(fileName).c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());
        if (FAILED(hr)) {
            return nullptr;
        }

        Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, frame.GetAddressOf());
        if (FAILED(hr)) {
            return nullptr;
        }

        hr = frame->GetSize(&width, &height);
        if (FAILED(hr)) {
            return nullptr;
        }
        componentsPerPixel = 4;

        _data = new uint8_t[width * 4 * height];

        hr = frame->CopyPixels(nullptr, width * 4, width * 4 * height , static_cast<uint8_t*>(_data));
        if (FAILED(hr)) {
            delete[] _data;
            _data = nullptr;
            return nullptr;
        }
		return _data;
	}

	inline void ImageLoader::UnLoad()
	{
		if (_data != nullptr)
		{
            delete[] _data;
			_data = nullptr;
		}
	}

	inline ImageLoader::~ImageLoader()
	{
		if (_data != nullptr)
		{
            delete[] _data;
		}
	}
}

#endif