#pragma once

#include "GameRendererBase.h"
#include <gl/GL.h>


namespace game
{
constexpr auto WGL_CONTEXT_DEBUG_BIT_ARB = 0x0001;
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

//opengl 3.0 stuff
#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
#define GL_CLIP_DISTANCE1 GL_CLIP_PLANE1
#define GL_CLIP_DISTANCE2 GL_CLIP_PLANE2
#define GL_CLIP_DISTANCE3 GL_CLIP_PLANE3
#define GL_CLIP_DISTANCE4 GL_CLIP_PLANE4
#define GL_CLIP_DISTANCE5 GL_CLIP_PLANE5
#define GL_COMPARE_REF_TO_TEXTURE GL_COMPARE_R_TO_TEXTURE_ARB
#define GL_MAX_CLIP_DISTANCES GL_MAX_CLIP_PLANES
#define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_FLOATS
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_DEPTH_BUFFER 0x8223
#define GL_STENCIL_BUFFER 0x8224
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_ALPHA_INTEGER 0x8D97
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16

	//typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	//static wglSwapInterval_t* wglSwapInterval = nullptr;
	
	typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
	static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
	
	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hshareContext, const int* attribList);
	static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

	typedef const GLubyte* (WINAPI* PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
	static PFNGLGETSTRINGIPROC glGetStringi = nullptr;

	typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);
	static PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapInterval = nullptr;

	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);
	static PFNWGLSWAPINTERVALEXTPROC wglSwapInterval = nullptr;

	
	class RendererGL : public RendererBase
	{
	public:
		typedef HDC glDeviceContext_t;
		typedef HGLRC glRenderContext_t;


		glDeviceContext_t glDeviceContext = NULL;
		glRenderContext_t glRenderContext = NULL;

		RendererGL()
		{
			glDeviceContext_t glTempDevice = NULL;
			glRenderContext_t glTempRender = NULL;
			game::Window tempWindow;
			GameAttributes tempAttrib;
			PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};

			// Create a temporary invisible window
			tempAttrib.WindowHeight = 0;
			tempAttrib.WindowWidth = 0;
			tempWindow.SetAttributes(tempAttrib);
			if (!tempWindow.CreateTheWindow())
			{
				std::cout << "Temporary window creation failed.\n";
				return;
			}

			// Get the device context
			glTempDevice = GetDC(tempWindow.GetHandle());

			// Choose a generic pixel format
			uint32_t tempPixelFormat = 0;
			tempPixelFormat = ChoosePixelFormat(glTempDevice, &pixelFormatDescriptor);
			if (!tempPixelFormat)
			{
				std::cout << "Temporary Window ChoosePixelFormat Failed.\n";
				return;
			}

			// Set the generic pixel format
			if (!SetPixelFormat(glTempDevice, tempPixelFormat, &pixelFormatDescriptor))
			{
				std::cout << "Temporary Window SetPixelFormat Failed.\n";
			}

			// Create the OpenGL rendering context
			glTempRender = wglCreateContext(glTempDevice);
			if (!glTempRender)
			{
				std::cout << "Temporary Window wglCreateContext Failed.\n";
				return;
			}

			// Use new rendering context
			wglMakeCurrent(glTempDevice, glTempRender);

			
			// Load necessary extensions for actual OpenGl render context
			wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
			if (wglChoosePixelFormatARB == nullptr)
			{
				std::cout << "wglChoosePixelFormatARM failed to load.\n";
				return;
			}

			wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			if (wglCreateContextAttribsARB == nullptr)
			{
				std::cout << "wglCreateContextAttribsARB failed to load.\n";
				return;
			}

			// Clean up OpenGL stuff
			wglMakeCurrent(NULL, NULL);
			if (glTempRender) wglDeleteContext(glTempRender);

			// Clean up temporary window stuff
			PostMessage(tempWindow.GetHandle(), WM_DESTROY, 0, 0);
			tempWindow.DoMessagePump();
		}

		bool CreateDevice(Window window) override
		{

			const int glContextAttributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB,  _attributes.ContextMajor,
				WGL_CONTEXT_MINOR_VERSION_ARB,  _attributes.ContextMinor,
				WGL_CONTEXT_FLAGS_ARB,
				_attributes.isDebugMode ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,              
				WGL_CONTEXT_PROFILE_MASK_ARB,   
				_attributes.glBackwardsCompatible ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};
			int32_t pixelFormatsChosen[1] = { 0 };
			uint32_t numberOfPixelFormatsChosen = 0;

			int pixelAttributeList[] =
			{
				WGL_SUPPORT_OPENGL_ARB, 1,
				WGL_DRAW_TO_WINDOW_ARB, 1,
				WGL_COLOR_BITS_ARB, 32,
				WGL_RED_BITS_ARB, 8,
				WGL_GREEN_BITS_ARB, 8,
				WGL_BLUE_BITS_ARB, 8,
				WGL_ALPHA_BITS_ARB, 8,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 0,
				WGL_DOUBLE_BUFFER_ARB, 1,
				WGL_STEREO_ARB, 0,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				0
			}; // Nvidia does not like WGL_ACCELERATED_ARB or whatever

			float pixelAttribFloatList[] = { 0, 0 };
			PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };

			// Get the window handle as the device we are using
			glDeviceContext = GetDC(window.GetHandle());


			// Choose a suitable pixel format for what attributes we want for real window
			wglChoosePixelFormatARB(glDeviceContext, pixelAttributeList, pixelAttribFloatList, 1, &pixelFormatsChosen[0], &numberOfPixelFormatsChosen);
			if (!numberOfPixelFormatsChosen)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "No compatible pixel formats found." };
				return false;
			}

			// Fills out some of the pixelformatedescriptor
			if (!DescribePixelFormat(glDeviceContext, pixelFormatsChosen[0], sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor))
			{
				lastError = { GameErrors::GameWindowsSpecific, "DescribePixelFormat failed. Window Error " + std::to_string(GetLastError())};
				return false;
			}

			// Set our pixel format that we got from the temporary window
			if (!SetPixelFormat(glDeviceContext, pixelFormatsChosen[0], &pixelFormatDescriptor))
			{
				lastError = { GameErrors::GameWindowsSpecific, "SetPixelFormat failed. Windows Error " + std::to_string(GetLastError())};
				return false;
			}

			// Create the rendering context
			glRenderContext = wglCreateContextAttribsARB(glDeviceContext, 0, glContextAttributes);
			if (glRenderContext == nullptr)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContextAttribsARB failed" };
				return false;
			}

			// Use the rendering context we just created
			wglMakeCurrent(glDeviceContext, glRenderContext);
	

			// Load wglSwapInterval
			wglSwapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			if (wglSwapInterval == NULL)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "Loading SwapInterval Failed" };
				return false;
			}

			// Load wglGetSwapInterval
			wglGetSwapInterval = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
			if (wglGetSwapInterval == nullptr)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "Loading GetSwapInterval Failed" };
				return false;
			}

			// Set vertical sync
			if (_attributes.isVsync)
				wglSwapInterval(1);
			else
				wglSwapInterval(0);

			// Renderer is good to go
			enginePointer->logger->Write("OpenGL Device created!");

			// Engine is now running
			enginePointer->isRunning = true;

			// temp to write out extensions
		
			// Load glGetStringi
			glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
			if (glGetStringi == nullptr)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "Extension glGetStringi not available." };
				return false;
			}

			// Get the number of OpenGL extensions available
			GLint numberOfExtensions = 0;
			glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);
			enginePointer->logger->WriteQuiet("Listing " + std::to_string(numberOfExtensions) + " OpenGL Extensions Available.");

			// Write all extensions out to the log file, not stdout
			std::string extensionName;
			for (GLint extensionNumber = 0; extensionNumber < numberOfExtensions; extensionNumber++)
			{
				extensionName = (char*)glGetStringi(GL_EXTENSIONS, extensionNumber);
				enginePointer->logger->WriteQuiet(extensionName);
			}

			return true;
		}

		void DestroyDevice() override
		{
			// Clean up OpenGL stuff
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
