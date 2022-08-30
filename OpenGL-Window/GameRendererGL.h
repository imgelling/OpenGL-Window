#pragma once

#include "GameRendererBase.h"


namespace game
{
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096
#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format 1
#endif
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NEED_PALETTE_ARB 0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_NUMBER_OVERLAYS_ARB 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define WGL_TRANSPARENT_ARB 0x200A
#define WGL_SHARE_DEPTH_ARB 0x200C
#define WGL_SHARE_STENCIL_ARB 0x200D
#define WGL_SHARE_ACCUM_ARB 0x200E
#define WGL_SUPPORT_GDI_ARB 0x200F
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201A
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_ALPHA_SHIFT_ARB 0x201C
#define WGL_ACCUM_BITS_ARB 0x201D
#define WGL_ACCUM_RED_BITS_ARB 0x201E
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B

	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	static wglSwapInterval_t* wglSwapInterval = nullptr;
	
	typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
	static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
	
	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hshareContext, const int* attribList);
	static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	
	class RendererGL : public RendererBase
	{
	public:
		typedef HDC glDeviceContext_t;
		typedef HGLRC glRenderContext_t;

		int pixelform = 0;
		glDeviceContext_t glDeviceContext = NULL;
		glRenderContext_t glRenderContext = NULL;

		RendererGL()
		{
			glDeviceContext_t glTempDevice = NULL;

			glRenderContext_t glTempRender = NULL;
			game::Window tempWindow;
			if (!tempWindow.CreateTheWindow())
			{
				std::cout << "Temp window not created.\n";
			}

			// Create Device Context
			glTempDevice = GetDC(tempWindow.GetHandle());
			PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};

			uint32_t pixelFormat = 0;
			pixelFormat = ChoosePixelFormat(glTempDevice, &pixelFormatDescriptor);
			if (!pixelFormat)
			{
				lastError = { GameErrors::GameWindowsSpecific, "Temp ChoosePixelFormat Failed." };
				return;
			}
			SetPixelFormat(glTempDevice, pixelFormat, &pixelFormatDescriptor);

			// Create the OpenGL rendering context
			glTempRender = wglCreateContext(glTempDevice);
			if (!glTempRender)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "Temporary wglCreateContext Failed." };
				return;
			}
			wglMakeCurrent(glTempDevice, glTempRender);

			// load some extensions

			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
			if (wglChoosePixelFormatARB == nullptr)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "wglChoosePixelFormatARM failed to load." };
				return;
			}

			wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			if (wglCreateContextAttribsARB == nullptr)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContextAttribsARB failed to load." };
				return;
			}

			//uint32_t pixelFormat[1];




			int pixfmt[32] = { 0 };
			unsigned int numpf;

			int piAttribIList[] =
			{
				WGL_SUPPORT_OPENGL_ARB, 1,
				WGL_DRAW_TO_WINDOW_ARB, 1,
				WGL_ACCELERATION_ARB, 1,
				WGL_COLOR_BITS_ARB, 32,
				WGL_RED_BITS_ARB, 8,
				WGL_GREEN_BITS_ARB, 8,
				WGL_BLUE_BITS_ARB, 8,
				WGL_ALPHA_BITS_ARB, 8,
				WGL_DEPTH_BITS_ARB, 24,
				//WGL_STENCIL_BITS_ARB, 8,
				WGL_DOUBLE_BUFFER_ARB, 1,
				//WGL_STEREO_ARB, 0,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				0
			};
			float pfAttribFList[] = { 0, 0 };


			wglChoosePixelFormatARB(glTempDevice, piAttribIList, pfAttribFList, 1, &pixfmt[0], &numpf);

			if (!numpf)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "No compatible pixel formats found." };
				return;
			}

			pixelform = pixfmt[0];
			wglMakeCurrent(NULL, NULL);
			if (glTempRender) wglDeleteContext(glTempRender);
			// need to close temp window
			PostMessage(tempWindow.GetHandle(), WM_DESTROY, 0, 0);
			tempWindow.DoMessagePump();
		}

		bool CreateDevice(Window window) override
		{

			const int cca_list[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB,  _attributes.ContextMajor,
				WGL_CONTEXT_MINOR_VERSION_ARB,  _attributes.ContextMinor,
				WGL_CONTEXT_FLAGS_ARB,
				_attributes.isDebugMode ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,              
				WGL_CONTEXT_PROFILE_MASK_ARB,   
				_attributes.glBackwardsCompatible ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};
			PIXELFORMATDESCRIPTOR pd;
			pd = { 0 };
			glDeviceContext = GetDC(window.GetHandle());



			int attributeListInt[19] = { 0 };

			if (!DescribePixelFormat(glDeviceContext, pixelform, sizeof(PIXELFORMATDESCRIPTOR), &pd))
			{
				lastError = { GameErrors::GameWindowsSpecific, "DescribePixelFormat failed." };
			}

			if (!SetPixelFormat(glDeviceContext, pixelform, &pd))
			{
				lastError = { GameErrors::GameWindowsSpecific, "SetPixelFormat failed. Windows Error " + std::to_string(GetLastError())};
				return false;
			}


			glRenderContext = wglCreateContextAttribsARB(glDeviceContext, 0, cca_list);
			if (glRenderContext == nullptr)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContextAttribsARB failed" };
				return false;
			}

			wglMakeCurrent(glDeviceContext, glRenderContext);
	

			// Set Vertical Sync
			wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
			if (wglSwapInterval == NULL)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "Loading SwapInterval Failed" };
				return false;
			}
			if (_attributes.isVsync)
				wglSwapInterval(1);
			else
				wglSwapInterval(0);

			enginePointer->logger->Write("OpenGL Device created!");

			// Engine is now running
			enginePointer->isRunning = true;

			return true;
		}
		void DestroyDevice() override
		{
			wglMakeCurrent(NULL, NULL);
			if (glRenderContext) wglDeleteContext(glRenderContext);
			glRenderContext = NULL;
		}
		void Swap() override
		{
			SwapBuffers(glDeviceContext);
		};
	private:
	};
}
