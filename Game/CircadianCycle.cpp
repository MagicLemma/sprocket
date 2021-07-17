#include "CircadianCycle.h"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <format>

namespace spkt {

CircadianCycle::CircadianCycle()
    : d_seconds(0.0f)
    , d_running(true)
    , d_speed(1.0f)
{   
}

void CircadianCycle::on_update(double dt)
{
    if (d_running) {
        d_seconds += d_speed * dt;
        Modulo();
    }
}

void CircadianCycle::Start()
{
    d_running = true;
}

void CircadianCycle::Stop()
{
    d_running = false;
}

void CircadianCycle::SetSpeed(float speed)
{
    d_speed = speed;
}

float CircadianCycle::GetSpeed() const
{
    return d_speed;
}

void CircadianCycle::SetTime(int hours, int mins, int secs)
{
    d_seconds = hours * 3600.0 + mins * 60.0 + secs;
    Modulo();
}

void CircadianCycle::AddSeconds(double seconds)
{
    d_seconds += seconds;
    Modulo();
}

double CircadianCycle::GetSeconds() const
{
    return d_seconds; 
}

void CircadianCycle::SetSeconds(double seconds)
{
    d_seconds = seconds;
    Modulo();
}

double CircadianCycle::GetAngle() const
{ 
    return glm::two_pi<double>() * d_seconds / SECONDS_IN_DAY;
}

void CircadianCycle::SetAngle(double angle)
{
    d_seconds = (angle / glm::two_pi<double>()) * SECONDS_IN_DAY;
    Modulo();
}

bool CircadianCycle::IsDay() const
{
    return SIX_AM <= d_seconds && d_seconds < SIX_PM;
}

glm::vec3 CircadianCycle::GetSunDir() const
{
    float angle = GetAngle();
    glm::vec3 dir = {glm::sin(angle), glm::cos(angle), 0.1f * glm::sin(angle)};
    dir = glm::normalize(dir);
    return dir;
}

std::string CircadianCycle::ToString24Hour() const
{
    return ToString();
}

std::string CircadianCycle::ToString12Hour() const
{
    return ToString(true);
}

void CircadianCycle::Modulo()
{
    while (d_seconds > SECONDS_IN_DAY) {
        d_seconds -= SECONDS_IN_DAY;
    }
    while (d_seconds < 0.0f) {
        d_seconds += SECONDS_IN_DAY;
    }
}

std::string CircadianCycle::ToString(bool twelveHour) const
{
    int hours = 0;
    int mins = 0;
    double seconds = d_seconds;
    while (seconds > 3600.0f) { ++hours; seconds -= 3600.0f; }
    while (seconds > 60.0f) { ++mins; seconds -= 60.0f; }

    std::string suffix = "";
    if (twelveHour) {
        suffix = hours >= 12 ? "PM" : "AM";
        if (hours > 12) { hours -= 12; }
    }
    
    return std::format("{}:{}:{:.2f}:{}", hours, mins, seconds, suffix);
}

}