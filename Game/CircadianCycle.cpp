#include "CircadianCycle.h"
#include "Log.h"
#include <sstream>

namespace Sprocket {

CircadianCycle::CircadianCycle()
    : d_seconds(86400)
{   
}

void CircadianCycle::SetTime(int hours, int mins, int secs)
{
    d_seconds = hours * 3600 + mins * 60 + secs;
    Modulo();
}

int CircadianCycle::GetSeconds() const
{
    return d_seconds; 
}

void CircadianCycle::SetSeconds(int seconds)
{
    d_seconds = seconds;
    Modulo();
}

float CircadianCycle::GetAngle() const
{ 
    return 360.0f * (float)d_seconds / (float)SECONDS_IN_DAY;
}

void CircadianCycle::SetAngle(float angle)
{
    d_seconds = (int)((angle / 360.0f) * SECONDS_IN_DAY);
    Modulo();
}

bool CircadianCycle::IsDay() const
{
    return SIX_AM <= d_seconds && d_seconds < SIX_PM;
}

Maths::vec3 CircadianCycle::GetSunDir() const
{
    float angle = GetAngle();
    Maths::vec3 dir = {Maths::Sind(angle), Maths::Cosd(angle), 0.15f};
    Maths::Normalise(dir);
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
    int hours = d_seconds / 3600;
    int mins = (d_seconds - 3600 * hours) / 60;
    int seconds = d_seconds - 60 * mins - 3600 * hours;

    std::string suffix = "";
    if (twelveHour) {
        suffix = hours >= 12 ? "PM" : "AM";
        if (hours > 12) { hours -= 12; }
    }
    
    std::stringstream sstream;
    sstream << hours << ":" << mins << ":" << seconds << " " << suffix;
    return sstream.str();
}

}