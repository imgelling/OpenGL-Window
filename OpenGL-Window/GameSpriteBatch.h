#pragma once
#if defined(GAME_OPENGL)
#include <gl/GL.h>
#endif
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif

#include "GameDefines.h"
#include "GameEngine.h"
#include "GameErrors.h"
#include "GameMath.h"
#include "GameTexture2D.h"

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
#if defined(GAME_OPENGL)
		float_t orthogonalMatrix[4][4] = { 0.0f };
#endif
#if defined(GAME_DIRECTX9)
			
#endif
	};

	inline SpriteBatch::SpriteBatch()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			// Set identity
			orthogonalMatrix[0][0] = 1.0f;
			orthogonalMatrix[1][1] = 1.0f;
			orthogonalMatrix[2][2] = 1.0f;
			orthogonalMatrix[3][3] = 1.0f;
		}
#endif
#if defined(GAME_DIRECTX9)
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
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			float Right = (float)enginePointer->geGetWindowSize().width;
			float Left = 0.0;
			float Top = 0.0f;
			float Bottom = (float)enginePointer->geGetWindowSize().height;
			float Near = -1.0f;
			float Far = 1.0f;

			orthogonalMatrix[0][0] = 2.0f / (Right - Left);

			orthogonalMatrix[1][1] = 2.0f / (Top - Bottom);
			orthogonalMatrix[2][2] = 2.0f / (Near - Far);

			orthogonalMatrix[3][0] = (Left + Right) / (Left - Right);
			orthogonalMatrix[3][1] = (Bottom + Top) / (Bottom - Top);

			orthogonalMatrix[3][2] = (Far + Near) / (Near - Far);
		}
#endif
	}

	inline void SpriteBatch::_Disable2D()
	{

	}
}