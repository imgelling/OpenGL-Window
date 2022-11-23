#pragma once

#include "GameEngine.h"
#include "GameErrors.h"
#include "GameImageLoader.h"
#include "GameTexture2D.h"

namespace game
{
	extern Engine* enginePointer;
	struct FontDescriptor
	{
		unsigned short x, y;
		unsigned short width, height;
		short xOffset, yOffset;
		unsigned short xAdvance;
		unsigned short page;

		FontDescriptor()
		{
			y = x = 0;
			width = height = 0;
			xOffset = yOffset = 0;
			xAdvance = 0;
			page = 0;
		}
	};

	struct Charset
	{
		unsigned short lineHeight = 0;
		unsigned short base = 0;
		unsigned short width = 0, height = 0;
		unsigned short pages = 0;
		FontDescriptor chars[256];
	};

	class SpriteFont
	{
	public:
		SpriteFont();
		~SpriteFont();
		uint32_t Length(std::string text);
		bool Load(const std::string filename, const std::string& texture);
		void UnLoad();
		Texture2D Texture();
		Charset _characterSet;


	private:
		Texture2D _texture;
	};

	inline SpriteFont::SpriteFont()
	{

	}

	inline SpriteFont::~SpriteFont()
	{
		UnLoad();
	}

	inline bool SpriteFont::Load(const std::string filename, const std::string& texture)
	{
		if (!enginePointer->geLoadTexture(texture, _texture))
		{
			return false;
		}

		std::string Line;
		std::string Read, Key, Value;
		std::size_t i;
		std::ifstream Stream;

		std::string file = filename;

		Stream.open(file.c_str());
		while (!Stream.eof())
		{
			std::stringstream LineStream;
			getline(Stream, Line);
			LineStream << Line;

			//read the line's type
			LineStream >> Read;
			if (Read == "common")
			{
				//this holds common data
				while (!LineStream.eof())
				{
					std::stringstream Converter;
					LineStream >> Read;
					i = Read.find('=');
					Key = Read.substr(0, i);
					Value = Read.substr(i + 1);

					//assign the correct value
					Converter << Value;
					if (Key == "lineHeight")
						Converter >> _characterSet.lineHeight;
					else if (Key == "base")
						Converter >> _characterSet.base;
					else if (Key == "scaleW")
						Converter >> _characterSet.width;
					else if (Key == "scaleH")
						Converter >> _characterSet.height;
					else if (Key == "pages")
						Converter >> _characterSet.pages;
				}
			}
			else if (Read == "char")
			{
				//this is data for a specific char
				unsigned short CharID = 0;

				while (!LineStream.eof())
				{
					if (CharID > 255)
					{
						Stream.close();
						return false;
					}
					std::stringstream Converter;
					LineStream >> Read;
					i = Read.find('=');
					Key = Read.substr(0, i);
					Value = Read.substr(i + 1);

					//assign the correct value
					Converter << Value;
					if (Key == "id")
						Converter >> CharID;
					else if (Key == "x")
						Converter >> _characterSet.chars[CharID].x;
					else if (Key == "y")
						Converter >> _characterSet.chars[CharID].y;
					else if (Key == "width")
						Converter >> _characterSet.chars[CharID].width;
					else if (Key == "height")
						Converter >> _characterSet.chars[CharID].height;
					else if (Key == "xoffset")
						Converter >> _characterSet.chars[CharID].xOffset;
					else if (Key == "yoffset")
						Converter >> _characterSet.chars[CharID].yOffset;
					else if (Key == "xadvance")
						Converter >> _characterSet.chars[CharID].xAdvance;
					else if (Key == "page")
						Converter >> _characterSet.chars[CharID].page;
				}
			}
		}
		Stream.close();

		return true;
	}

	inline void SpriteFont::UnLoad()
	{
		enginePointer->geUnLoadTexture(_texture);
	}

	inline uint32_t SpriteFont::Length(std::string text)
	{

	}

	inline Texture2D SpriteFont::Texture()
	{
		return _texture;
	}
}
