#pragma once
#include <string>
#include <sstream>
#include <thread>
#include <intrin.h>

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
			uint32_t processorCount = 0;
		};
		// Contains information about the host's renderer
		struct GPUInfo
		{
			int32_t internalPixelFormat = 0;
			int32_t internalPixelType = 0;
			int32_t totalMemory = 0;
			int32_t freeMemory = 0;
			std::string version;
			int32_t versionMajor = 0;
			int32_t versionMinor = 0;
			std::string maxShaderLanguageVersion;
			int32_t maxAnisotropy = 1;
			int32_t maxMultisamples = 0;
			int32_t multisampleSamples = 0;
			int32_t frontBufferColorSize = 0;
			int32_t backBufferColorSize = 0;
			int32_t depthBufferSize = 0;
			std::string renderer;
			std::string vendor;
		};
	public:
		CPUInfo cpuInfo;
		GPUInfo gpuInfo;
		void GetCPUInfo();
	};

	inline void SystemInfo::GetCPUInfo()
	{
		cpuInfo.processorCount = std::thread::hardware_concurrency();

		// cpu vendor
		int32_t regs[4] = { 0 };
		int8_t vendor[13] = { 0 };
		__cpuidex(regs, 0,0);              // mov eax,0; cpuid
		memcpy(vendor, &regs[1], 4);   // copy EBX
		memcpy(vendor + 4, &regs[3], 4); // copy EDX
		memcpy(vendor + 8, &regs[2], 4); // copy ECX
		vendor[12] = '\0';
		std::cout << "My CPU is a " << vendor << "\n";

		
		__cpuidex(regs, 1,0);              // mov eax,0; cpuid
		memcpy(vendor, &regs[1], 4);   // copy EBX
		memcpy(vendor + 4, &regs[3], 4); // copy EDX
		memcpy(vendor + 8, &regs[2], 4); // copy ECX
		vendor[12] = '\0';
		std::cout << "My CPU is a " << vendor << "\n";
	}

}

