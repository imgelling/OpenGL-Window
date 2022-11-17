#pragma once

#include "GameMath.h"
#include "GameTexture2D.h"

namespace game
{
	class SpriteSheet
	{
	public:
		SpriteSheet();
		void Initialize(const Texture2D texure, const int width, const int height) noexcept;
		void Initialize(const Texture2D texure, const Pointi &size) noexcept;
		void SetTexture(Texture2D& texure) {};
		Recti GetRectFromId(int id) noexcept;
	private:
		uint32_t _tileWidth;
		uint32_t _tileHeight;
		uint32_t _tilesPerRow;
		Texture2D* _texture;
	};

	inline SpriteSheet::SpriteSheet()
	{
		_tileWidth = 0;
		_tileHeight = 0;
		_tilesPerRow = 0;
		_texture = nullptr;
	}

	inline void SpriteSheet::Initialize(Texture2D texture, const int width, const int height) noexcept
	{
		_texture = &texture;
		_tileWidth = width;
		_tileHeight = height;
		_tilesPerRow = _texture->width / _tileWidth;
	}

	inline void SpriteSheet::Initialize(const Texture2D texture, const Pointi &size) noexcept
	{
		Initialize(texture, size.width, size.height);
	}

	inline Recti SpriteSheet::GetRectFromId(int id) noexcept
	{
		Recti rectangle;
		if (!_texture)
		{
			return rectangle;
		}

		rectangle.left = id % _tilesPerRow * _tileWidth;
		rectangle.top = id / _tilesPerRow * _tileHeight;
		rectangle.right = rectangle.left + _tileWidth;
		rectangle.bottom = rectangle.top + _tileHeight;

		return rectangle;
	}
}