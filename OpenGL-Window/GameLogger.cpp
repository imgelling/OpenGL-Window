#include "GameLogger.h"
#include <iostream>
#include <sstream>

namespace game
{

	void GameLogger::Line()
	{
		//#ifdef _DEBUG
		streamlock.lock();
		std::cout << "--------------------------------------------------------------------------------" << std::endl;
		streamlock.unlock();
		//#endif
		streamlock.lock();
		stream << "<HR>";
		streamlock.unlock();
	}

	GameLogger::GameLogger(const std::string filename)
	{
		for (uint16_t buff = 0; buff < 256; buff++)
		{
			buffer[buff] = 0;
		}
		rawtime = 0;
		streamlock.lock();
		stream.open(filename.c_str(), std::ios::out);
		streamlock.unlock();
	}

	void GameLogger::Write(const std::string logline)
	{
		Write(LogType::NORMAL, logline);
	}

	void GameLogger::Error(const std::string logline)
	{
		Write(LogType::ERRORS, logline);
	}

	void GameLogger::Error(const GameError error)
	{
		std::stringstream str;
		str << error;
		Write(LogType::ERRORS, str.str());
	}

	void GameLogger::Warning(const std::string logline)
	{
		Write(LogType::WARNING, logline);
	}

	void GameLogger::Write(const LogType type, const std::string logline)
	{
		std::string color;
		std::string temp;
		std::string pre;
		std::string time;

		GetTime();
		time = std::string(buffer);
		time.pop_back();
		pre = "[" + time + "] ";
		color = "FFFFFF";

		if (type == LogType::ERRORS)
		{
			pre += "ERROR : ";
			color = "FF0000";
		}
		else if (type == LogType::WARNING)
		{
			pre += "WARNING : ";
			color = "FFFF00";
		}
		//#ifdef _DEBUG
		streamlock.lock();
		std::cout << pre << logline << std::endl;
		streamlock.unlock();
		//#endif
		temp += "<FONT style=\"background-color:#" + color + "\">";
		temp += pre;
		temp += logline + "</FONT><BR>";
		streamlock.lock();
		stream << temp << std::endl;
		streamlock.unlock();
	}

	void GameLogger::WriteQuiet(const std::string logline)
	{
		std::string color;
		std::string temp;
		std::string pre;
		std::string time;

		GetTime();
		time = std::string(buffer);
		time.pop_back();
		pre = "[" + time + "] ";
		color = "FFFFFF";

		temp += "<FONT style=\"background-color:#" + color + "\">";
		temp += pre;
		temp += logline + "</FONT><BR>";
		streamlock.lock();
		stream << temp << std::endl;
		streamlock.unlock();
	}

	inline void GameLogger::GetTime()
	{
#ifdef __linux__

#else
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		asctime_s(buffer, &timeinfo);
#endif
	}

	void GameLogger::Header(const std::string name, const std::string version)
	{
		Write(name + ", version " + version);
		Write("Built on " + std::string(__DATE__) + " at " + std::string(__TIME__) + ".");
		Line();
	}

	GameLogger::~GameLogger()
	{
		streamlock.lock();
		stream.close();
		streamlock.unlock();
	}

}