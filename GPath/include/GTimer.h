#ifndef GTIMER_H
#define GTIMER_H

#include <iostream>

#include <functional>
#include <thread>

////////////////////////////////////////////////////////
// Basic GTimer implementation
////////////////////////////////////////////////////////

class GTimer {
public:
    GTimer() {}
    virtual ~GTimer() noexcept {}

    void StartTimer(long long uiTimeUSec, std::function<void()> callback);
    void Stop();
private:
    std::thread _thTimer;

    bool        _isWorking = true;
};

#endif /* GTIMER_H */
