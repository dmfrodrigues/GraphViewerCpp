#ifndef FPSMONITOR_H_INCLUDED
#define FPSMONITOR_H_INCLUDED

#include <chrono>
#include <queue>

class FPSMonitor {
private:
    std::queue<std::chrono::high_resolution_clock::time_point> q;
    std::chrono::high_resolution_clock::duration Dt;
    std::chrono::high_resolution_clock::time_point now;
public:
    FPSMonitor(int ms);
    void setInterval(int ms);
    void count();
    float getFPS() const;
};

#endif // FPSMONITOR_H_INCLUDED
