#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include <functional>

class Timer {

    std::thread Thread;
    bool alive = false;
    long callNumber = -1L;
    long repeat_count = -1L;
    std::chrono::milliseconds interval = std::chrono::milliseconds(0);
    std::function<void(void)> func = nullptr;

    void sleepAndRun() {
        std::this_thread::sleep_for(interval);
        if(alive)
            Function()();
    }

    void threadFunc() {
        if(callNumber == infinite)
            while(alive)
                sleepAndRun();
        else
            while(repeat_count--)
                sleepAndRun();
    }
public:
    static const long infinite = -1L;

    Timer() {}
    Timer(const std::function<void(void)> &f) : func(f){}

    Timer(const std::function<void(void)> &f,
            const unsigned long &i,
            const long repeat = Timer::infinite)
            : func(f), interval(std::chrono::milliseconds(i)),
                callNumber(repeat) {}

    void Start(bool Async = true) {
        if(isAlive())
            return;
        alive = true;
        repeat_count = callNumber;
        if(Async)
            Thread = std::thread(threadFunc, this);
        else
            this->threadFunc();
    }
    void Stop() {
        alive = false;
        Thread.join();
    }

    void setFunction(const std::function<void(void)> &f) {
        func = f;
    }

    bool isAlive() const { return alive; }

    void RepeatCount(const long r) {
        if(alive)
            return;
        callNumber = r;
    }

    long getLeftCount() const { return repeat_count; }

    long RepeatCount() const { return callNumber; }

    void setInterval(const unsigned long &i) {
        if(alive)
            return;
        interval = std::chrono::milliseconds(i);
    }

    unsigned long Interval() const { return interval.count(); }

    const std::function<void(void)> &Function() const {
        return func;
    }
};
#endif // TIMER_H
