#pragma once
#include <string>

namespace game
{

	// Contains information about the host's system
	class SystemInfo
	{
	private:
		// Contains information about the host's cpu
		struct CPUInfo
		{
		public:
			unsigned int processorCount = 0;
			//systemInfo.cpuInfo.processorCount = std::thread::hardware_concurrency();
		};
		// Contains information about the host's gpu being used
		struct GPUInfo
		{
			int internalPixelFormat = 0;
			int internalPixelType = 0;
			int totalMemory = 0;
			int freeMemory = 0;
			int glVersionMajor = 0;
			int glVersionMinor = 0;
			int glShaderLanguageVersion = 0;
			int glMultisampleBuffers = 0;
			int glMultisampleSamples = 0;
			//Color frontBufferColorSize;
			//Color backBufferColorSize;
			int depthBufferSize = 0;
			std::string renderer;
			std::string vendor;
		};
	public:
		CPUInfo cpuInfo;
		GPUInfo gpuInfo;
	};

}

