#include "GameRendererGL.h"
#include "GameEngine.h"
#include <sstream>

namespace game
{
	//extern Engine;
	extern Engine* enginePointer;

	RendererGL::RendererGL()
	{
		_glDeviceContext_t glTempDevice = NULL;
		_glRenderContext_t glTempRender = NULL;
		game::Window tempWindow;
		Attributes tempAttrib;
		PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
		{
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			PFD_MAIN_PLANE, 0, 0, 0, 0
		};

		// Create a temporary invisible window
		tempAttrib.isWindowVisible = false;
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
		_wglChoosePixelFormatARB = (_PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
		if (_wglChoosePixelFormatARB == nullptr)
		{
			std::cout << "wglChoosePixelFormatARM failed to load.\n";
			return;
		}

		_wglCreateContextAttribsARB = (_PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (_wglCreateContextAttribsARB == nullptr)
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

	bool RendererGL::CreateDevice(Window window) 
	{

		const int glContextAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,  _attributes.ContextMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB,  _attributes.ContextMinor,
			WGL_CONTEXT_FLAGS_ARB,
			_attributes.isDebugMode ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
			WGL_CONTEXT_PROFILE_MASK_ARB,
			_attributes.isGLBackwardsCompatible ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};
		int32_t pixelFormatsChosen[1] = { 0 };
		uint32_t numberOfPixelFormatsChosen = 0;

		int32_t colorBits = _attributes.RedSize + _attributes.BlueSize + _attributes.GreenSize + _attributes.AlphaSize;

		int32_t glPixelAttributeList[] =
		{
			WGL_SUPPORT_OPENGL_ARB, 1,
			WGL_DRAW_TO_WINDOW_ARB, 1,
			WGL_COLOR_BITS_ARB, colorBits,
			WGL_RED_BITS_ARB, _attributes.RedSize,
			WGL_GREEN_BITS_ARB, _attributes.GreenSize,
			WGL_BLUE_BITS_ARB, _attributes.BlueSize,
			WGL_ALPHA_BITS_ARB, _attributes.AlphaSize,
			WGL_DEPTH_BITS_ARB, _attributes.DepthSize,
			WGL_STENCIL_BITS_ARB, _attributes.StencilSize,
			WGL_DOUBLE_BUFFER_ARB, _attributes.DoubleBuffer,
			WGL_STEREO_ARB, 0,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
			0
		}; 
		// 	WGL_SWAP_EXCHANGE_ARB vs WGL_SWAP_COPY_ARB.

		float_t pixelAttribFloatList[] = { 0, 0 };
		PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };

		// Get the window handle as the device we are using
		_glDeviceContext = GetDC(window.GetHandle());


		// Choose a suitable pixel format for what attributes we want for real window
		_wglChoosePixelFormatARB(_glDeviceContext, glPixelAttributeList, pixelAttribFloatList, 1, &pixelFormatsChosen[0], &numberOfPixelFormatsChosen);
		if (!numberOfPixelFormatsChosen)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "No compatible pixel formats found." };
			return false;
		}

		// Fills out some of the pixelformatedescriptor
		if (!DescribePixelFormat(_glDeviceContext, pixelFormatsChosen[0], sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor))
		{
			lastError = { GameErrors::GameWindowsSpecific, "DescribePixelFormat failed. Window Error " + std::to_string(GetLastError()) };
			return false;
		}

		// Set our pixel format that we got from the temporary window
		if (!SetPixelFormat(_glDeviceContext, pixelFormatsChosen[0], &pixelFormatDescriptor))
		{
			lastError = { GameErrors::GameWindowsSpecific, "SetPixelFormat failed. Windows Error " + std::to_string(GetLastError()) };
			return false;
		}

		// Create the rendering context
		_glRenderContext = _wglCreateContextAttribsARB(_glDeviceContext, 0, glContextAttributes);
		if (_glRenderContext == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContextAttribsARB failed" };
			return false;
		}

		// Use the rendering context we just created
		wglMakeCurrent(_glDeviceContext, _glRenderContext);


		// Load wglSwapInterval
		_wglSwapInterval = (_PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (_wglSwapInterval == NULL)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Loading SwapInterval Failed" };
			return false;
		}

		// Load wglGetSwapInterval
		_wglGetSwapInterval = (_PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		if (_wglGetSwapInterval == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Loading GetSwapInterval Failed" };
			return false;
		}

		// Load glGetStringi
		_glGetStringi = (_PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
		if (_glGetStringi == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Extension glGetStringi not available." };
			return false;
		}

		// Load glGetInternalformativ
		_glGetInternalformativ = (_PFNGLGETINTERNALFORMATIVPROC)wglGetProcAddress("glGetInternalformativ");
		if (_glGetInternalformativ == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Extension glGetInternalformativ not available." };
			return false;
		}

		// Set vertical sync
		if (_attributes.isVsyncOn)
			_wglSwapInterval(1);
		else
			_wglSwapInterval(0);

		// Renderer is good to go
		enginePointer->logger->Write("OpenGL Device created!");

		// Engine is now running
		enginePointer->isRunning = true;

		// Read all available extensions
		_ReadExtensions();


		return true;
	}

	void RendererGL::_ReadExtensions()
	{
		GLint numberOfExtensions = 0;
		std::string extensionName;

		// Get the number of OpenGL extensions available
		glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);
		enginePointer->logger->WriteQuiet("Listing " + std::to_string(numberOfExtensions) + " OpenGL Extensions Available.");
		
		// Write all extensions out to the log file, not stdout 
		// and store in a vector
		for (GLint extensionNumber = 0; extensionNumber < numberOfExtensions; extensionNumber++)
		{
			extensionName = (char*)_glGetStringi(GL_EXTENSIONS, extensionNumber);
			_extensionsAvailable.emplace_back(extensionName);
			enginePointer->logger->WriteQuiet(extensionName);
		}
	}

	void RendererGL::DestroyDevice() 
	{
		// Clean up OpenGL stuff
		wglMakeCurrent(NULL, NULL);
		if (_glRenderContext) wglDeleteContext(_glRenderContext);
		_glRenderContext = NULL;
	}

	void RendererGL::Swap() 
	{
		SwapBuffers(_glDeviceContext);
	};

	void RendererGL::HandleWindowResize(const uint32_t width, const uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void RendererGL::FillOutRendererInfo(SystemInfo& info)
	{
		auto LOG = [&](std::stringstream & s)
		{
			enginePointer->logger->Write(s.str());
			s.str("");
		};

		int32_t value = 0;
		std::stringstream sStream;

		glGetIntegerv(WGL_CONTEXT_MAJOR_VERSION_ARB, &info.gpuInfo.glVersionMajor);
		glGetIntegerv(WGL_CONTEXT_MINOR_VERSION_ARB, &info.gpuInfo.glVersionMinor);
		info.gpuInfo.renderer = (char *)glGetString(GL_VERSION);
		sStream << "OpenGL renderer : " << info.gpuInfo.renderer;
		LOG(sStream);

		// Extract numerical major and minor version numbers
		info.gpuInfo.glVersionMajor = std::strtol(info.gpuInfo.renderer.c_str(), NULL, 10);
		info.gpuInfo.renderer.erase(0, 2);
		info.gpuInfo.glVersionMinor = std::strtol(info.gpuInfo.renderer.c_str(), NULL, 10);

		// Get the vender of the renderer
		info.gpuInfo.vendor = (char*)glGetString(GL_VENDOR);
		sStream << "Vendor : " << info.gpuInfo.vendor;
		LOG(sStream);

		// Get the shader language version
		//int32_t numberOfVersions = 0;
		//glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numberOfVersions);
		//std::cout << numberOfVersions << "\n";
		//for (int32_t version = 0; version < numberOfVersions; version++)
		//{
		//	std::cout << _glGetStringi(GL_SHADING_LANGUAGE_VERSION, version) << "\n";
		//}
		info.gpuInfo.glMaxShaderLanguageVersion = (char *)_glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0);
		sStream << "Max supported GLSL version : " << info.gpuInfo.glMaxShaderLanguageVersion;
		LOG(sStream);

		// Log video memory
		GLint total_mem_kb[4] = { 0 };
		GLint cur_avail_mem_kb[4] = { 0 };
		if (info.gpuInfo.vendor.find("NVIDIA") != std::string::npos)
		{
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

			glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb[0]);


			glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb[0]);
			info.gpuInfo.totalMemory = total_mem_kb[0] / 1024;
			info.gpuInfo.freeMemory = cur_avail_mem_kb[0] / 1024;
			sStream << "GPU total memory is " << total_mem_kb[0] / 1024.0f << "MB and has " << cur_avail_mem_kb[0] / 1024.0f << "MB available.";
			LOG(sStream);
#undef GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX
#undef GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 
		}
		else
		{
			sStream << "Can not retrieve video RAM on AMD.";
			LOG(sStream);
		}

		// Get texture formats
		_glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &info.gpuInfo.internalPixelFormat);
		// Convert to hexidecimal
		sStream << std::hex << info.gpuInfo.internalPixelFormat;
		std::string hex(sStream.str());
		sStream.str("");
		info.gpuInfo.internalPixelFormat = std::strtol(hex.c_str(), NULL, 16);

		_glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &info.gpuInfo.internalPixelType);
		// Convert to hexidecimal
		sStream << std::hex << info.gpuInfo.internalPixelType;
		hex = sStream.str();
		sStream.str("");
		info.gpuInfo.internalPixelType = std::strtol(hex.c_str(), NULL, 16);

		//nvidia type is GL_UNSIGNED_INT_8_8_8_8_REV (0x8367) difference is endianness
		//amd type is GL_UNSIGNED_BYTE (0x1401)
		// both formats are RGBA
		sStream << "Internal texture format is " << info.gpuInfo.internalPixelFormat << " and is type " << info.gpuInfo.internalPixelType;
		LOG(sStream);
		 
	}
}


// Undefine what we have done, if someone uses an extension loader 

// OpenGL context stuff
#undef GL_NUM_SHADING_LANGUAGE_VERSIONS 
#undef GL_SHADING_LANGUAGE_VERSION
#undef GL_TEXTURE_IMAGE_FORMAT
#undef GL_TEXTURE_IMAGE_TYPE 
#undef WGL_CONTEXT_DEBUG_BIT_ARB
//#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#undef WGL_CONTEXT_MAJOR_VERSION_ARB 
#undef WGL_CONTEXT_MINOR_VERSION_ARB 
//#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#undef WGL_CONTEXT_FLAGS_ARB 
//#define ERROR_INVALID_VERSION_ARB 0x2095
//#define ERROR_INVALID_PROFILE_ARB 0x2096
//#ifndef WGL_ARB_pixel_format
//#define WGL_ARB_pixel_format 1
//#endif
#undef WGL_CONTEXT_CORE_PROFILE_BIT_ARB 
#undef WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 
#undef WGL_CONTEXT_PROFILE_MASK_ARB 
//
//#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#undef WGL_DRAW_TO_WINDOW_ARB 
//#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#undef WGL_ACCELERATION_ARB 
//#define WGL_NEED_PALETTE_ARB 0x2004
//#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
//#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#undef WGL_SWAP_METHOD_ARB 
//#define WGL_NUMBER_OVERLAYS_ARB 0x2008
//#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
//#define WGL_TRANSPARENT_ARB 0x200A
//#define WGL_SHARE_DEPTH_ARB 0x200C
//#define WGL_SHARE_STENCIL_ARB 0x200D
//#define WGL_SHARE_ACCUM_ARB 0x200E
//#define WGL_SUPPORT_GDI_ARB 0x200F
#undef WGL_SUPPORT_OPENGL_ARB 
#undef WGL_DOUBLE_BUFFER_ARB 
#undef WGL_STEREO_ARB 
#undef WGL_PIXEL_TYPE_ARB 
#undef WGL_COLOR_BITS_ARB 
#undef WGL_RED_BITS_ARB 
//#define WGL_RED_SHIFT_ARB 0x2016
#undef WGL_GREEN_BITS_ARB 
//#define WGL_GREEN_SHIFT_ARB 0x2018
#undef WGL_BLUE_BITS_ARB 
//#define WGL_BLUE_SHIFT_ARB 0x201A
#undef WGL_ALPHA_BITS_ARB 
//#define WGL_ALPHA_SHIFT_ARB 0x201C
//#define WGL_ACCUM_BITS_ARB 0x201D
//#define WGL_ACCUM_RED_BITS_ARB 0x201E
//#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
//#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
//#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#undef WGL_DEPTH_BITS_ARB 
#undef WGL_STENCIL_BITS_ARB 
//#define WGL_AUX_BUFFERS_ARB 0x2024
//#define WGL_NO_ACCELERATION_ARB 0x2025
//#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#undef WGL_FULL_ACCELERATION_ARB 
#undef WGL_SWAP_EXCHANGE_ARB 
//#define WGL_SWAP_COPY_ARB 0x2029
//#define WGL_SWAP_UNDEFINED_ARB 0x202A
#undef WGL_TYPE_RGBA_ARB 
//#define WGL_TYPE_COLORINDEX_ARB 0x202C
//#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
//#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
//#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
//#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
//#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B

//opengl 3.0 stuff
//#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
//#define GL_CLIP_DISTANCE1 GL_CLIP_PLANE1
//#define GL_CLIP_DISTANCE2 GL_CLIP_PLANE2
//#define GL_CLIP_DISTANCE3 GL_CLIP_PLANE3
//#define GL_CLIP_DISTANCE4 GL_CLIP_PLANE4
//#define GL_CLIP_DISTANCE5 GL_CLIP_PLANE5
//#define GL_COMPARE_REF_TO_TEXTURE GL_COMPARE_R_TO_TEXTURE_ARB
//#define GL_MAX_CLIP_DISTANCES GL_MAX_CLIP_PLANES
//#define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_FLOATS
//#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
//#define GL_MAJOR_VERSION 0x821B
//#define GL_MINOR_VERSION 0x821C
#undef GL_NUM_EXTENSIONS 
//#define GL_CONTEXT_FLAGS 0x821E
//#define GL_DEPTH_BUFFER 0x8223
//#define GL_STENCIL_BUFFER 0x8224
//#define GL_RGBA32F 0x8814
//#define GL_RGB32F 0x8815
//#define GL_RGBA16F 0x881A
//#define GL_RGB16F 0x881B
//#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
//#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
//#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
//#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
//#define GL_CLAMP_VERTEX_COLOR 0x891A
//#define GL_CLAMP_FRAGMENT_COLOR 0x891B
//#define GL_CLAMP_READ_COLOR 0x891C
//#define GL_FIXED_ONLY 0x891D
//#define GL_TEXTURE_RED_TYPE 0x8C10
//#define GL_TEXTURE_GREEN_TYPE 0x8C11
//#define GL_TEXTURE_BLUE_TYPE 0x8C12
//#define GL_TEXTURE_ALPHA_TYPE 0x8C13
//#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
//#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
//#define GL_TEXTURE_DEPTH_TYPE 0x8C16
//#define GL_TEXTURE_1D_ARRAY 0x8C18
//#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
//#define GL_TEXTURE_2D_ARRAY 0x8C1A
//#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
//#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
//#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
//#define GL_R11F_G11F_B10F 0x8C3A
//#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
//#define GL_RGB9_E5 0x8C3D
//#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
//#define GL_TEXTURE_SHARED_SIZE 0x8C3F
//#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
//#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
//#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
//#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
//#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
//#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
//#define GL_PRIMITIVES_GENERATED 0x8C87
//#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
//#define GL_RASTERIZER_DISCARD 0x8C89
//#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
//#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
//#define GL_INTERLEAVED_ATTRIBS 0x8C8C
//#define GL_SEPARATE_ATTRIBS 0x8C8D
//#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
//#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
//#define GL_RGBA32UI 0x8D70
//#define GL_RGB32UI 0x8D71
//#define GL_RGBA16UI 0x8D76
//#define GL_RGB16UI 0x8D77
//#define GL_RGBA8UI 0x8D7C
//#define GL_RGB8UI 0x8D7D
//#define GL_RGBA32I 0x8D82
//#define GL_RGB32I 0x8D83
//#define GL_RGBA16I 0x8D88
//#define GL_RGB16I 0x8D89
//#define GL_RGBA8I 0x8D8E
//#define GL_RGB8I 0x8D8F
//#define GL_RED_INTEGER 0x8D94
//#define GL_GREEN_INTEGER 0x8D95
//#define GL_BLUE_INTEGER 0x8D96
//#define GL_ALPHA_INTEGER 0x8D97
//#define GL_RGB_INTEGER 0x8D98
//#define GL_RGBA_INTEGER 0x8D99
//#define GL_BGR_INTEGER 0x8D9A
//#define GL_BGRA_INTEGER 0x8D9B
//#define GL_SAMPLER_1D_ARRAY 0x8DC0
//#define GL_SAMPLER_2D_ARRAY 0x8DC1
//#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
//#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
//#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
//#define GL_UNSIGNED_INT_VEC2 0x8DC6
//#define GL_UNSIGNED_INT_VEC3 0x8DC7
//#define GL_UNSIGNED_INT_VEC4 0x8DC8
//#define GL_INT_SAMPLER_1D 0x8DC9
//#define GL_INT_SAMPLER_2D 0x8DCA
//#define GL_INT_SAMPLER_3D 0x8DCB
//#define GL_INT_SAMPLER_CUBE 0x8DCC
//#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
//#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
//#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
//#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
//#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
//#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
//#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
//#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
//#define GL_QUERY_WAIT 0x8E13
//#define GL_QUERY_NO_WAIT 0x8E14
//#define GL_QUERY_BY_REGION_WAIT 0x8E15
//#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16