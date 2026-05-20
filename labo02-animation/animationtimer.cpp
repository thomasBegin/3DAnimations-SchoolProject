#include "animationtimer.h"
#include <cmath>

using namespace std::chrono;

AnimationTimer::AnimationTimer( int nbFrames, int fps ) : m_numberOfFrames(nbFrames) 
{ 
    m_milisecPerFrame = 1000.0 / static_cast<double>(fps); 
    m_startTime = high_resolution_clock::now(); 
}

void AnimationTimer::Start() 
{ 
    m_startTime = high_resolution_clock::now(); 
}

void AnimationTimer::GetAnimationTime( int & baseFrame, int & nextFrame, double & interval )
    {
        std::chrono::duration<double, std::milli> fp_ms = high_resolution_clock::now() - m_startTime;
        double time = fp_ms.count();
        double animFrame = time / m_milisecPerFrame;
        baseFrame = static_cast<int>(std::floor(animFrame)) % m_numberOfFrames;
        nextFrame = ( baseFrame + 1 ) % m_numberOfFrames;
        double intPart;
        interval = std::modf(animFrame,&intPart);
    }
