#pragma once
#include <fstream>
#include <mutex>
#include "../../../Programming/GameLib2/GameErrors.h"


namespace game
{
    class Logger {
    public:
        Logger(const std::string filename);
        ~Logger();
        void Header(const std::string name, const std::string version);
        void Error(const std::string logline);
        void Error(const GameError error);
        void Line();
        void Warning(const std::string logline);
        void Write(const std::string logline);
        void WriteQuiet(const std::string logline);
    private:
        char _buffer[256];
        enum class _LogType { NORMAL, WARNING, ERRORS, NONE };
        time_t _rawtime;
        std::ofstream _stream;
        std::mutex _streamlock;
        tm _timeinfo = { 0 };
        void _Write(const _LogType type, const std::string logline);
        inline void _GetTime();
    };

}