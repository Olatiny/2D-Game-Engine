#pragma once

#include "includes.hpp"

class Timeline {
    private:
        float tic;
        float startTime;
        float pausedStartTime;
        float pausedElapsedTime;
        bool paused;
        float lastTime;
        float lastCalculatedTime;
        float lastTic;
        Timeline* anchor;
    public:
        /**
         * @brief Construct a new Timeline object (defaults to global timeline with tic of 1)
         * 
         */
        Timeline();

        /**
         * @brief Construct a new Global Timeline, has no anchor.
         * 
         * @param tic tic size
         */
        Timeline(float tic);

        /**
         * @brief Construct a new local Timeline object, has an anchor
         * 
         * @param anchor anchor timeline
         * @param tic tic size
         */
        Timeline(Timeline* anchor, float tic);

        float getTime();

        void pause();

        bool isPaused();

        void unpause();
        
        float getTic();

        void changeTic(float newTic);

        // float calcNewTime();

        // void reset();
};