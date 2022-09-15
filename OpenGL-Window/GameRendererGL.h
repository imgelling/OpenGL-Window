#pragma once

#include "GameRendererBase.h"
#include <gl/GL.h>


namespace game
{
// OpenGL defines (removed at end of RendererGL.cpp)

#define WGL_CONTEXT_DEBUG_BIT_ARB  0x0001

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
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_UNSIGNED_BYTE 0x1401
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER 0x8D40
#define GL_MAX_SAMPLES 0x8D57
#define WGL_SAMPLE_BUFFERS_ARB 0x2041
#define WGL_SAMPLES_ARB 0x2042
#define GL_MULTISAMPLE 0x809D
	
	class RendererGL : public RendererBase
	{
	public:
		RendererGL();
		bool CreateDevice(Window window);
		void DestroyDevice();
		void Swap();
		void HandleWindowResize(const uint32_t width, const uint32_t height);
		void FillOutRendererInfo(SystemInfo& info);
		bool LoadTexture(std::string fileName);
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

		typedef void (WINAPI* _PFNGLGENERATEMIPMAPPROC) (GLenum target);
		_PFNGLGENERATEMIPMAPPROC _glGenerateMipmap = nullptr;

		typedef void (WINAPI* _PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
		_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC _glGetFramebufferAttachmentParameteriv = nullptr;
	};
}
