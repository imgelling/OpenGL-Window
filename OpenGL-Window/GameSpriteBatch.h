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
		bool Initialize();
		void Begin();
		void Render();
		void Draw(const Texture2D &texture, const uint32_t x, const uint32_t y);
		void End();
	private:
		static constexpr uint32_t _maxSprites = 6; // low for testing
		uint32_t _numberOfSpritesUsed;
		Texture2D _currentTexture;
		void _Enable2D();
		void _Disable2D();
#if defined(GAME_OPENGL)
		float_t orthogonalMatrix[4][4] = { 0.0f };
#endif
#if defined(GAME_DIRECTX9)
		//(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
		struct _spriteVertex
		{
			float_t x, y, z, rhw;
			uint32_t color;
			float_t u, v;
		};
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
		_spriteVertex* _spriteVertices;
#endif
#if defined (GAME_DIRECTX11)
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
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_vertexBuffer = nullptr;
			_spriteVertices = nullptr;
		}
#endif
#if defined (GAME_DIRECTX11)
#endif

		_numberOfSpritesUsed = 0;
	}

	inline SpriteBatch::~SpriteBatch()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{

		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (_vertexBuffer)
			{
				_vertexBuffer->Release();
				_vertexBuffer = nullptr;
			}
			if (_spriteVertices)
			{
				delete[] _spriteVertices;
				_spriteVertices = nullptr;
			}
		}
#endif
#if defined (GAME_DIRECTX11)
#endif
	}

	inline bool SpriteBatch::Initialize()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{

		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_spriteVertices = new _spriteVertex[_maxSprites * 6];
			for (uint32_t vertex = 0; vertex < _maxSprites * 6; vertex++)
			{
				_spriteVertices[vertex].rhw = 1.0f;
				_spriteVertices[vertex].z = 0.0f;
				_spriteVertices[vertex].color = D3DCOLOR_ARGB(255, 255, 255, 255);
			}

			enginePointer->d3d9Device->CreateVertexBuffer(_maxSprites * (uint32_t)6 * (uint32_t)sizeof(_spriteVertex), 0, (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), D3DPOOL_MANAGED, &_vertexBuffer, NULL);
			if (_vertexBuffer == nullptr)
			{
				lastError = { GameErrors::GameDirectX9Specific, "Could not create vertex buffer for SpriteBatch." };
				return false;
			}
		}

		return true;
#endif
#if defined (GAME_DIRECTX11)
#endif
	}

	inline void SpriteBatch::Begin()
	{

	}

	inline void SpriteBatch::End()
	{
		if (_numberOfSpritesUsed > 0)
		{
			Render();
		}
	}

	inline void SpriteBatch::Render()
	{
		if (_numberOfSpritesUsed == 0)
		{
			return;
		}

#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			VOID* pVoid = nullptr;

			// Send sprite vertices to gpu
			_vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
			memcpy(pVoid, _spriteVertices, sizeof(_spriteVertex) * 6 * _maxSprites);
			_vertexBuffer->Unlock();

			// Draw the sprites
			DWORD oldFVF = 0;
			IDirect3DBaseTexture9* activeTexture = 0;
			enginePointer->d3d9Device->BeginScene();
			// Save current state
			enginePointer->d3d9Device->GetFVF(&oldFVF);
			enginePointer->d3d9Device->GetTexture(0, &activeTexture);

			// Disable multisampling if enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
			}

			enginePointer->d3d9Device->SetTexture(0, _currentTexture.textureInterface9);
			enginePointer->d3d9Device->SetFVF((D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
			enginePointer->d3d9Device->SetStreamSource(0, _vertexBuffer, 0, sizeof(_spriteVertex));
			enginePointer->d3d9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _numberOfSpritesUsed * 2);

			// Restore previous state
			enginePointer->d3d9Device->SetFVF(oldFVF);
			enginePointer->d3d9Device->SetTexture(0, activeTexture);
			if (activeTexture)
			{
				activeTexture->Release();
			}

			// Renable multisampling if it was enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			}

			enginePointer->d3d9Device->EndScene();
		}

		_numberOfSpritesUsed = 0;
#endif
	}

	inline void SpriteBatch::Draw(const Texture2D& texture, const uint32_t x, const uint32_t y)
	{
		if (_numberOfSpritesUsed + 1 > _maxSprites)
		{
			Render();
		}

		if (texture.textureInterface9 != _currentTexture.textureInterface9)
		{
			Render();
			_currentTexture = texture;
		}

#if defined(GAME_DIRECTX9)
		_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed];
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			// Top left
			access->x = (float_t)x;
			access->y = (float_t)y;
			access->u = 0.0f;
			access->v = 0.0f;
			access++;

			// Top right
			access->x = (float_t)x+(float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access++;

			// Bottom left
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
			access++;

			// Top right
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access++;

			// Bottom right
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 1.0f;
			access->v = 1.0f;
			access++;

			// Bottom left
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
		}
#endif

		_numberOfSpritesUsed++;
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
#if defined (GAME_DIRECTX9)
#endif
#if defined (GAME_DIRECTX11)
#endif
	}

	inline void SpriteBatch::_Disable2D()
	{
#if defined(GAME_DIRECTX9)

#endif
	}
}