#pragma once
#include <glm/glm.hpp>

#include <string>

namespace spkt {

class CircadianCycle
{
    static constexpr float SIX_HOURS = 21600;
    static constexpr float SIX_AM = SIX_HOURS;
    static constexpr float NOON = 2 * SIX_HOURS;
    static constexpr float SIX_PM = 3 * SIX_HOURS;
    static constexpr float SECONDS_IN_DAY = 4 * SIX_HOURS;

    double d_seconds;
    bool   d_running;
    float  d_speed;

    void Modulo();

    std::string ToString(bool twelveHour = false) const;
        // Accessed via ToString24Hour and ToString12Hour

public:
    CircadianCycle();

    void on_update(double dt);
    void Start();
    void Stop();
    bool IsRunning() const { return d_running; }

    void SetSpeed(float speed);
    float GetSpeed() const;

    void SetTime(int hours, int mins, int secs);
    void AddSeconds(double seconds);

    double GetSeconds() const;
    void SetSeconds(double secs);
        // Access the value of the circadian cycle by treating it as
        // the number of seconds past midnight.

    double GetAngle() const;
    void SetAngle(double angle);
        // Access the value of the circadian cycle by treating it as
        // an angle. 0.0f is midnight and 180.0f is noon.

    bool IsDay() const;
        // Dayime is defined as being between 6am and 6pm.

    glm::vec3 GetSunDir() const;
        // Returns a direction vector to act at the direction of sunlight.
        // Calculated using the circadian cycle treated as an angle. The
        // direction at noon is straight down.

    std::string ToString24Hour() const;
    std::string ToString12Hour() const;
        // Returns a string representation of the current time.
};

}