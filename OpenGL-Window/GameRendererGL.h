#pragma once

#include "GameRendererBase.h"
#include <gl/GL.h>


namespace game
{
// OpenGL context stuff
#define WGL_CONTEXT_DEBUG_BIT_ARB  0x0001
//#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_TYPE_RGBA_ARB 0x202B
#define GL_TEXTURE_IMAGE_FORMAT 0x828F
#define GL_TEXTURE_IMAGE_TYPE 0x8290
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_NUM_SHADING_LANGUAGE_VERSIONS 0x82E9
#define GL_NUM_EXTENSIONS 0x821D
	
	class RendererGL : public RendererBase
	{
	public:
		RendererGL();
		bool CreateDevice(Window window);
		void DestroyDevice();
		void Swap();
		void HandleWindowResize(const uint32_t width, const uint32_t height);
		void FillOutRendererInfo(SystemInfo& info);
	protected:
		void _ReadExtensions();

	private:
		typedef HDC _glDeviceContext_t;
		typedef HGLRC _glRenderContext_t;

		_glDeviceContext_t _glDeviceContext = NULL;
		_glRenderContext_t _glRenderContext = NULL;
		typedef BOOL(WINAPI* _PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
		_PFNWGLCHOOSEPIXELFORMATARBPROC _wglChoosePixelFormatARB = nullptr;

		typedef HGLRC(WINAPI* _PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hshareContext, const int* attribList);
		_PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB = nullptr;

		typedef const GLubyte* (WINAPI* _PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
		_PFNGLGETSTRINGIPROC _glGetStringi = nullptr;

		typedef int (WINAPI* _PFNWGLGETSWAPINTERVALEXTPROC) ();
		_PFNWGLGETSWAPINTERVALEXTPROC _wglGetSwapInterval = nullptr;

		typedef BOOL(WINAPI* _PFNWGLSWAPINTERVALEXTPROC) (int interval);
		_PFNWGLSWAPINTERVALEXTPROC _wglSwapInterval = nullptr;

		typedef void (WINAPI* _PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);
		_PFNGLGETINTERNALFORMATIVPROC _glGetInternalformativ = nullptr;
	};
}
