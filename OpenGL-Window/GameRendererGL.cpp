#include "GameRendererGL.h"
#include "GameEngine.h"

namespace game
{
	//extern Engine;
	extern Engine* enginePointer;

	namespace GL
	{
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
		PFNGLGETSTRINGIPROC glGetStringi = nullptr;
		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapInterval = nullptr;
		PFNWGLSWAPINTERVALEXTPROC wglSwapInterval = nullptr;
	}

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
		GL::wglChoosePixelFormatARB = (GL::PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
		if (GL::wglChoosePixelFormatARB == nullptr)
		{
			std::cout << "wglChoosePixelFormatARM failed to load.\n";
			return;
		}

		GL::wglCreateContextAttribsARB = (GL::PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (GL::wglCreateContextAttribsARB == nullptr)
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

		float pixelAttribFloatList[] = { 0, 0 };
		PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };

		// Get the window handle as the device we are using
		_glDeviceContext = GetDC(window.GetHandle());


		// Choose a suitable pixel format for what attributes we want for real window
		GL::wglChoosePixelFormatARB(_glDeviceContext, glPixelAttributeList, pixelAttribFloatList, 1, &pixelFormatsChosen[0], &numberOfPixelFormatsChosen);
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
		_glRenderContext = GL::wglCreateContextAttribsARB(_glDeviceContext, 0, glContextAttributes);
		if (_glRenderContext == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContextAttribsARB failed" };
			return false;
		}

		// Use the rendering context we just created
		wglMakeCurrent(_glDeviceContext, _glRenderContext);


		// Load wglSwapInterval
		GL::wglSwapInterval = (GL::PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (GL::wglSwapInterval == NULL)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Loading SwapInterval Failed" };
			return false;
		}

		// Load wglGetSwapInterval
		GL::wglGetSwapInterval = (GL::PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		if (GL::wglGetSwapInterval == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Loading GetSwapInterval Failed" };
			return false;
		}

		// Load glGetStringi
		GL::glGetStringi = (GL::PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
		if (GL::glGetStringi == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Extension glGetStringi not available." };
			return false;
		}

		// Set vertical sync
		if (_attributes.isVsyncOn)
			GL::wglSwapInterval(1);
		else
			GL::wglSwapInterval(0);

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
			extensionName = (char*)GL::glGetStringi(GL_EXTENSIONS, extensionNumber);
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
}