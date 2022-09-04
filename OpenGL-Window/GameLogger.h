#pragma once
#include <fstream>
#include <mutex>
#include "../../../Programming/GameLib2/GameErrors.h"


namespace game
{
    class GameLogger {
    public:
        GameLogger(const std::string filename);
        ~GameLogger();
        void Header(const std::string name, const std::string version);
        void Write(const std::string logline);
        void WriteQuiet(const std::string logline);
        void Error(const std::string logline);
        void Error(const GameError error);
        void Warning(const std::string logline);
        void Line();
        std::mutex streamlock;
    private:
        std::ofstream stream;
        char buffer[256];
        time_t rawtime;
        tm timeinfo = { 0 };
        enum class LogType { NORMAL, WARNING, ERRORS, NONE };
        void _Write(const LogType type, const std::string logline);
        inline void GetTime();
    };

}