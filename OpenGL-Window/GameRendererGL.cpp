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
			_attributes.glBackwardsCompatible ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};
		int32_t pixelFormatsChosen[1] = { 0 };
		uint32_t numberOfPixelFormatsChosen = 0;

		int glPixelAttributeList[] =
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
		GL::wglChoosePixelFormatARB(glDeviceContext, glPixelAttributeList, pixelAttribFloatList, 1, &pixelFormatsChosen[0], &numberOfPixelFormatsChosen);
		if (!numberOfPixelFormatsChosen)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "No compatible pixel formats found." };
			return false;
		}

		// Fills out some of the pixelformatedescriptor
		if (!DescribePixelFormat(glDeviceContext, pixelFormatsChosen[0], sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor))
		{
			lastError = { GameErrors::GameWindowsSpecific, "DescribePixelFormat failed. Window Error " + std::to_string(GetLastError()) };
			return false;
		}

		// Set our pixel format that we got from the temporary window
		if (!SetPixelFormat(glDeviceContext, pixelFormatsChosen[0], &pixelFormatDescriptor))
		{
			lastError = { GameErrors::GameWindowsSpecific, "SetPixelFormat failed. Windows Error " + std::to_string(GetLastError()) };
			return false;
		}

		// Create the rendering context
		glRenderContext = GL::wglCreateContextAttribsARB(glDeviceContext, 0, glContextAttributes);
		if (glRenderContext == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContextAttribsARB failed" };
			return false;
		}

		// Use the rendering context we just created
		wglMakeCurrent(glDeviceContext, glRenderContext);


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

		// Set vertical sync
		if (_attributes.isVsync)
			GL::wglSwapInterval(1);
		else
			GL::wglSwapInterval(0);

		// Renderer is good to go
		enginePointer->logger->Write("OpenGL Device created!");

		// Engine is now running
		enginePointer->isRunning = true;

		// temp to write out extensions

		// Load glGetStringi
		GL::glGetStringi = (GL::PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
		if (GL::glGetStringi == nullptr)
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
			extensionName = (char*)GL::glGetStringi(GL_EXTENSIONS, extensionNumber);
			enginePointer->logger->WriteQuiet(extensionName);
		}

		return true;
	}

	void RendererGL::DestroyDevice() 
	{
		// Clean up OpenGL stuff
		wglMakeCurrent(NULL, NULL);
		if (glRenderContext) wglDeleteContext(glRenderContext);
		glRenderContext = NULL;
	}

	void RendererGL::Swap() 
	{
		SwapBuffers(glDeviceContext);
	};
}