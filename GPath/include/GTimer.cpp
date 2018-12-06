#include "GTimer.h"

void GTimer::StartTimer(long long uiTimeUSec,
                        std::function<void()> callback) {

    _isWorking = true;
    auto tmStart = std::chrono::high_resolution_clock::now();

    std::cout << "\tStart Timer... " << uiTimeUSec << " us" << std::endl;
    _thTimer = std::thread{ [this, uiTimeUSec, tmStart, callback] {
        long long llElapsed;

        using namespace std::chrono;
        do {
            llElapsed = duration_cast<microseconds>(high_resolution_clock::now() - tmStart).count();
        } while (uiTimeUSec > llElapsed && _isWorking);

        std::cout << "\tEnd Timer..." << std::endl;

        if (_isWorking) callback();
    } };
}

void GTimer::Stop() {
    std::cout << "\tStop Timer..." << std::endl;

    _isWorking = false;
    _thTimer.join();
}