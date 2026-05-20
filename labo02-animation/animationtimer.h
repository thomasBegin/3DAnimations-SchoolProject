#include <chrono>

class AnimationTimer
{
public:
    
    AnimationTimer( int nbFrames, int fps );
    void Start();
    void GetAnimationTime( int & baseFrame, int & nextFrame, double & interval );
    
protected:
    
    int m_numberOfFrames;
    double m_milisecPerFrame;
    std::chrono::high_resolution_clock::time_point m_startTime;
};
