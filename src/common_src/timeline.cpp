#include "timeline.hpp"

using namespace std::chrono;

Timeline::Timeline() {
    // c = Clock();
    tic = 1;
    startTime = duration_cast<std::chrono::milliseconds>(steady_clock::now().time_since_epoch()).count() / 1000.f;
    pausedStartTime = 0;
    pausedElapsedTime = 0;
    // elapsedTime = 0;
    paused = false;
    // wasPaused = false;
    // ellapsedTime = 0;
    anchor = NULL;
    lastCalculatedTime = 0;
    lastTime = 0;
    lastTic = 0;
}

Timeline::Timeline(float tic) {
    // c = Clock();
    this->tic = tic;
    startTime = duration_cast<std::chrono::milliseconds>(steady_clock::now().time_since_epoch()).count() / 1000.f;
    pausedStartTime = 0;
    pausedElapsedTime = 0;
    // elapsedTime = 0;
    paused = false;
    anchor = NULL;
    lastCalculatedTime = 0;
    lastTime = 0;
    lastTic = 0;
}

Timeline::Timeline(Timeline* anchor, float tic) {
    this->tic = tic;
    this->anchor = anchor;
    startTime = anchor->getTime();
    paused = false;
    // elapsedTime = 0;
    pausedStartTime = 0;
    pausedElapsedTime = 0;
    lastCalculatedTime = 0;
    lastTime = 0;
    lastTic = 0;
}

float Timeline::getTime() {
    // if (paused) {
    //     float returnVal = ((pausedStartTime - startTime) * tic  - pausedElapsedTime);
    //     return returnVal;
    // }

    // If it has an anchor, then it is not a global timeline, so defer to that. Otherwise, just get elapsed time since beginning.A
    if (anchor != NULL) { // yes, I know the "!= NULL" isn't necessary, this just makes it easier to read. 
        float currentTime = anchor->getTime();
        float returnVal = ((currentTime - startTime) * tic - pausedElapsedTime + lastCalculatedTime);
        startTime = currentTime;
        lastCalculatedTime = returnVal;
        return returnVal;
    } else {
        float currentTime = duration_cast<std::chrono::milliseconds>(steady_clock::now().time_since_epoch()).count() / 1000.f;
        float returnVal = ((currentTime - startTime) * tic - pausedElapsedTime + lastCalculatedTime);
        startTime = currentTime;
        lastCalculatedTime = returnVal;
        return returnVal;
    }
}

void Timeline::pause() {
    pausedStartTime = getTime();
    lastTic = tic;
    changeTic(0);
    paused = 1;
}

bool Timeline::isPaused() {
    return paused;
}

void Timeline::unpause() {
    paused = 0;
    pausedElapsedTime += getTime() - pausedStartTime;
    pausedStartTime = 0;
    changeTic(lastTic);
}

float Timeline::getTic() {
    if (paused) {
        return lastTic;
    } else {
        return tic;
    }
    // return tic;
}

void Timeline::changeTic(float newTic) {
    if (paused) {
        lastTic = newTic;
    } else {
        tic = newTic;
    }
    // elapsedTime = getTime() * (newTic - tic);
    // tic = newTic;
}

// void Timeline::reset() {
//     // if (anchor != NULL) {
//     //     startTime = anchor->getTime();
//     // } else {
//     //     c.restart();
//     // }

//     startTime = getTime();

//     // ellapsedTime = 0;
//     pausedElapsedTime = 0;
// }