#include "GameRendererGL.h"
#include "GameEngine.h"
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

	extern uint32_t bindTexture;
namespace game
{
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
		tempAttrib.WindowVisible = false;
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

		// Need to know max multisamples the card can do before we create the real window
		glGetIntegerv(GL_MAX_SAMPLES, &enginePointer->systemInfo.gpuInfo.maxMultisamples);
		if (enginePointer->systemInfo.gpuInfo.maxMultisamples == 32)
		{
			// Nvidia reports 32 samples, but doesn't work for frame buffer
			enginePointer->systemInfo.gpuInfo.maxMultisamples = 16;
		}

		// Clean up OpenGL stuff
		wglMakeCurrent(NULL, NULL);
		if (glTempRender) wglDeleteContext(glTempRender);

		// Clean up temporary window stuff
		PostMessage(tempWindow.GetHandle(), WM_DESTROY, 0, 0);
	}

	bool RendererGL::CreateDevice(Window window) 
	{

		const int glContextAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,  _attributes.ContextMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB,  _attributes.ContextMinor,
			WGL_CONTEXT_FLAGS_ARB,
			_attributes.DebugMode ? WGL_CONTEXT_DEBUG_BIT_ARB : 0,
			WGL_CONTEXT_PROFILE_MASK_ARB,
			_attributes.GLBackwardsCompatible ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};
		int32_t pixelFormatsChosen[1] = { 0 };
		uint32_t numberOfPixelFormatsChosen = 0;

		int32_t colorBits = _attributes.RedSize + _attributes.BlueSize + _attributes.GreenSize + _attributes.AlphaSize;

		// Make sure we don't go above limits
		if (_attributes.MultiSamples > enginePointer->systemInfo.gpuInfo.maxMultisamples)
		{
			_attributes.MultiSamples = enginePointer->systemInfo.gpuInfo.maxMultisamples;
		}
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
			WGL_SAMPLE_BUFFERS_ARB, _attributes.MultiSamples <= 1 ? 0 : 1,
			WGL_SAMPLES_ARB, _attributes.MultiSamples,
			0
		}; 

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

		_glGenerateMipmap = (_PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
		if (_glGetInternalformativ == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Extension glGenerateMipmap not available." };
			return false;
		}

		_glGetFramebufferAttachmentParameteriv = (_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
		if (_glGetFramebufferAttachmentParameteriv == nullptr)
		{
			lastError = { GameErrors::GameOpenGLSpecific, "Extension glGetFramebufferAttachmentParameteriv not available " };
			return false;
		}

		// Set vertical sync
		if (_attributes.VsyncOn)
			_wglSwapInterval(1);
		else
			_wglSwapInterval(0);

		// If multisampling is desired enable it
		if (_attributes.MultiSamples > 1)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}


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
		int32_t value = 0;
		std::stringstream sStream;

		// Lambda to log easier
		auto LOG = [&](std::stringstream & stream)
		{
			enginePointer->logger->Write(stream.str());
			stream.str("");
		};

		// Log the version of OpenGL context
		info.gpuInfo.renderer = (char *)glGetString(GL_VERSION);
		sStream << "OpenGL renderer : " << info.gpuInfo.renderer;
		LOG(sStream);

		// Extract numerical major and minor version numbers
		info.gpuInfo.versionMajor = std::strtol(info.gpuInfo.renderer.c_str(), NULL, 10);
		info.gpuInfo.renderer.erase(0, 2);
		info.gpuInfo.versionMinor = std::strtol(info.gpuInfo.renderer.c_str(), NULL, 10);

		// Log the vender of the renderer
		info.gpuInfo.vendor = (char*)glGetString(GL_VENDOR);
		sStream << "Vendor : " << info.gpuInfo.vendor;
		LOG(sStream);

		// Log the max shader language version
		info.gpuInfo.maxShaderLanguageVersion = (char *)_glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0);
		sStream << "Max supported GLSL version : " << info.gpuInfo.maxShaderLanguageVersion;
		LOG(sStream);

		// Log video memory
		if (info.gpuInfo.vendor.find("NVIDIA") != std::string::npos)
		{
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
			glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &info.gpuInfo.totalMemory);
			glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &info.gpuInfo.freeMemory);
			info.gpuInfo.totalMemory /= 1024;
			info.gpuInfo.freeMemory /= 1024;
			sStream << "GPU total memory : " << info.gpuInfo.totalMemory << "MB";
			LOG(sStream);
			sStream << "GPU available memory : " << info.gpuInfo.freeMemory << "MB";
			LOG(sStream);
#undef GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX
#undef GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 
		}
		else
		{
			sStream << "GPU total memory : Can not retrieve video memory on AMD.";
			LOG(sStream);
			sStream << "GPU available memory : Can not retrieve video memory on AMD.";
			LOG(sStream);
		}

		// Log internal pixel format
		_glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &info.gpuInfo.internalPixelFormat);
		// Convert to hexidecimal
		sStream << std::hex << info.gpuInfo.internalPixelFormat;
		std::string hex(sStream.str());
		sStream.str("");
		info.gpuInfo.internalPixelFormat = std::strtol(hex.c_str(), NULL, 16);
		sStream << "Internal pixel format : " << info.gpuInfo.internalPixelFormat;
		std::string pixelFormat = (info.gpuInfo.internalPixelFormat == 0x1908) ? " (RGBA)" : " (Unknown)";
		sStream << pixelFormat;
		LOG(sStream);

		// Log internal pixel type
		_glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_TYPE, 1, &info.gpuInfo.internalPixelType);
		// Convert to hex
		sStream << std::hex << info.gpuInfo.internalPixelType;
		hex = sStream.str();
		sStream.str("");
		info.gpuInfo.internalPixelType = std::strtol(hex.c_str(), NULL, 16);
		sStream << "Internal pixel type : " << info.gpuInfo.internalPixelType;
		std::string pixelType;
		if (info.gpuInfo.internalPixelType == 0x8367)
			pixelType = " (GL_UNSIGNED_INT_8_8_8_8_REV)";
		else if (info.gpuInfo.internalPixelType == 0x1401)
			pixelType = " (GL_UNSIGNED_BYTE)";
		else
			pixelType = " (Unknown)";
		sStream << pixelType;
		LOG(sStream);
		// The difference is endianness, both pixel formats are RGBA

		// Log multisampling
		if (_attributes.MultiSamples > 1)
		{
			info.gpuInfo.multisampleSamples = _attributes.MultiSamples;
			sStream << "Multisampling samples : " << std::dec << info.gpuInfo.multisampleSamples;
			sStream << " (" << info.gpuInfo.maxMultisamples << " max)";
			LOG(sStream);
		}
		else
		{
			sStream << "Multisampling samples : disabled";
			LOG(sStream);
		}
		 

		// Log front buffer
		int32_t r, g, b, a = 0;
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &r);
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &g);
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &b);
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &a);
		// hex to dec
		info.gpuInfo.frontBufferColorSize = r + b + g + a;
		sStream << std::dec << info.gpuInfo.frontBufferColorSize;
		hex = sStream.str();
		sStream.str("");
		info.gpuInfo.frontBufferColorSize = std::strtol(hex.c_str(), NULL, 10);
		sStream << "Front buffer : " << info.gpuInfo.frontBufferColorSize << " bits";
		LOG(sStream);

		// Log back buffer
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &r);
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &g);
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &b);
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &a);
		enginePointer->systemInfo.gpuInfo.backBufferColorSize = r + b + g + a;
		// hex to dec
		sStream << std::dec << info.gpuInfo.backBufferColorSize;
		hex = sStream.str();
		sStream.str("");
		info.gpuInfo.backBufferColorSize = std::strtol(hex.c_str(), NULL, 10);
		sStream << "Back buffer : " << info.gpuInfo.backBufferColorSize << " bits";
		LOG(sStream);

		// Log depth buffer
		_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &info.gpuInfo.depthBufferSize);
		sStream << "Depth buffer : " << info.gpuInfo.depthBufferSize << "bits";
		LOG(sStream);
		
		// Get the max anisotropy
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &info.gpuInfo.maxAnisotropy);
		// Convert hex to dec
		sStream << std::dec << info.gpuInfo.maxAnisotropy;
		hex = sStream.str();
		sStream.str("");
		info.gpuInfo.maxAnisotropy = std::strtol(hex.c_str(), NULL, 10);
		sStream << "Max anisotropy : " << info.gpuInfo.maxAnisotropy << "x";
		LOG(sStream);

	}
	bool  RendererGL::LoadTexture(std::string fileName)
	{
		//Content content;
		void* data = nullptr;
		int32_t width = 0;
		int32_t height = 0;
		int32_t bytesPerPixel = 0;

		// Read data
		data = stbi_load(fileName.c_str(), &width, &height, &bytesPerPixel, 0);
		if (data == NULL)
		{
			lastError = { GameErrors::GameContent, "Failed to load texture : " + fileName };
			if (data) stbi_image_free(data);
			return false;
		}
		// Not already loaded so we create it
		
		glGenTextures(1, &bindTexture);
		glBindTexture(GL_TEXTURE_2D, bindTexture);
		//tex.name = filename;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// LINEAR_MIPMAP_LINEAR); // min
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);// GL_LINEAR); //max
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Anisotropy
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, enginePointer->systemInfo.gpuInfo.maxAnisotropy);
		// When adding gfx options, just div by 2 down to 2x; so 16, 8, 4, 2, 0

		//tex.width = width;
		//tex.height = height;
		//tex.widthDiv = 1.0f / (float)tex.width;
		//tex.heightDiv = 1.0f / (float)tex.height;


		// needs to be adjusted for amd (GL_UNSIGNED_BYTE amd or whatever) GL_UNSIGNED_INT_8_8_8_8_REV nvidia
		if (bytesPerPixel == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, enginePointer->systemInfo.gpuInfo.internalPixelType, data);
		}
		else if (bytesPerPixel == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, enginePointer->systemInfo.gpuInfo.internalPixelType, data);
		}
		
		if (true)
			_glGenerateMipmap(GL_TEXTURE_2D);
		//Textures[filename] = tex;
		glBindTexture(GL_TEXTURE_2D, 0);




		stbi_image_free(data);
		return true;
	}
}


// Undefine what we have done, if someone uses an extension loader 
#undef GL_MULTISAMPLE
#undef WGL_SAMPLE_BUFFERS_ARB 
#undef WGL_SAMPLES_ARB
#undef GL_MAX_SAMPLES
#undef GL_FRAMEBUFFER 
#undef GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE
#undef GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 
#undef GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 
#undef GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 
#undef GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 
#undef GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 
#undef GL_CLAMP_TO_EDGE
#undef GL_TEXTURE_MAX_ANISOTROPY_EXT
#undef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#undef GL_UNSIGNED_INT_8_8_8_8_REV
#undef GL_UNSIGNED_BYTE
#undef GL_NUM_SHADING_LANGUAGE_VERSIONS 
#undef GL_SHADING_LANGUAGE_VERSION
#undef GL_TEXTURE_IMAGE_FORMAT
#undef GL_TEXTURE_IMAGE_TYPE 
#undef WGL_CONTEXT_DEBUG_BIT_ARB
#undef WGL_CONTEXT_MAJOR_VERSION_ARB 
#undef WGL_CONTEXT_MINOR_VERSION_ARB 
#undef WGL_CONTEXT_FLAGS_ARB 
#undef WGL_CONTEXT_CORE_PROFILE_BIT_ARB 
#undef WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 
#undef WGL_CONTEXT_PROFILE_MASK_ARB 
#undef WGL_DRAW_TO_WINDOW_ARB 
#undef WGL_ACCELERATION_ARB 
#undef WGL_SWAP_METHOD_ARB 
#undef WGL_SUPPORT_OPENGL_ARB 
#undef WGL_DOUBLE_BUFFER_ARB 
#undef WGL_STEREO_ARB 
#undef WGL_PIXEL_TYPE_ARB 
#undef WGL_COLOR_BITS_ARB 
#undef WGL_RED_BITS_ARB 
#undef WGL_GREEN_BITS_ARB 
#undef WGL_BLUE_BITS_ARB 
#undef WGL_ALPHA_BITS_ARB 
#undef WGL_STENCIL_BITS_ARB 
#undef WGL_FULL_ACCELERATION_ARB 
#undef WGL_SWAP_EXCHANGE_ARB 
//#define WGL_SWAP_COPY_ARB 0x2029
#undef WGL_TYPE_RGBA_ARB 
#undef GL_NUM_EXTENSIONS 
