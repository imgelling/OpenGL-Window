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
			int32_t internalPixelFormat = 0;
			int32_t internalPixelType = 0;
			int32_t totalMemory = 0;
			int32_t freeMemory = 0;
			int32_t glVersionMajor = 0;
			int32_t glVersionMinor = 0;
			std::string glMaxShaderLanguageVersion;
			int32_t glMultisampleBuffers = 0;
			int32_t glMultisampleSamples = 0;
			int32_t frontBufferColorSize;
			int32_t backBufferColorSize;
			int32_t depthBufferSize = 0;
			std::string renderer;
			std::string vendor;
		};
	public:
		CPUInfo cpuInfo;
		GPUInfo gpuInfo;
	};

}

