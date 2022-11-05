#pragma once
#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
#include <gl/GL.h>
#endif
#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
#include <d3d9.h>
#endif
#include "GameDefines.h"
#include "GameErrors.h"
#include "GameMath.h"
#include "GameTexture2D.h"
#include "GameEngine.h"

namespace game
{
	extern GameError lastError;
	extern Engine* enginePointer;
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		void Initialize();
		void Begin();
		void End();
	private:
		void _Enable2D();
		void _Disable2D();
#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
		float_t orthoganalMatrix[4][4] = { 0.0f };
#endif
#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
			
#endif
	};

	inline SpriteBatch::SpriteBatch()
	{
#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			// Set identity
			orthoganalMatrix[0][0] = 1.0f;
			orthoganalMatrix[1][1] = 1.0f;
			orthoganalMatrix[2][2] = 1.0f;
			orthoganalMatrix[3][3] = 1.0f;
		}
#endif
#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{

		}
#endif
	}

	inline SpriteBatch::~SpriteBatch()
	{

	}

	inline void SpriteBatch::Initialize()
	{

	}

	inline void SpriteBatch::Begin()
	{

	}

	inline void SpriteBatch::End()
	{

	}

	inline void SpriteBatch::_Enable2D()
	{
#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			float Right = (float)enginePointer->geGetWindowSize().width;
			float Left = 0.0;
			float Top = 0.0f;
			float Bottom = (float)enginePointer->geGetWindowSize().height;
			float Near = -1.0f;
			float Far = 1.0f;

			orthoganalMatrix[0][0] = 2.0f / (Right - Left);

			orthoganalMatrix[1][1] = 2.0f / (Top - Bottom);
			orthoganalMatrix[2][2] = 2.0f / (Near - Far);

			orthoganalMatrix[3][0] = (Left + Right) / (Left - Right);
			orthoganalMatrix[3][1] = (Bottom + Top) / (Bottom - Top);

			orthoganalMatrix[3][2] = (Far + Near) / (Near - Far);
		}
#endif
	}

	inline void SpriteBatch::_Disable2D()
	{

	}
}