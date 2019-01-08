#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <string>
#include <thread>
#include <iostream>

namespace EdenClass
{
    class StopWatch
    {
        #define StopWatchGetTimeNow std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
    public:
        StopWatch();
        void Start();
        void Stop();
        void Reset();
        long long GetTime();
        void Delay(long long Time, bool ActiveWork);
        long long TimeStamp();
        std::string TimeToString(long long T);
    private:
        bool IsRunning;
        long long Time1;
        long long Time2;
    };
}

#endif // STOPWATCH_H
