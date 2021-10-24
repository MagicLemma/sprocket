#pragma once
#include <glm/glm.hpp>

#include <string>

class day_night_cycle
{
    static constexpr float SIX_HOURS      = 21600;
    static constexpr float SIX_AM         = SIX_HOURS;
    static constexpr float NOON           = 2 * SIX_HOURS;
    static constexpr float SIX_PM         = 3 * SIX_HOURS;
    static constexpr float SECONDS_IN_DAY = 4 * SIX_HOURS;

    float d_seconds;
    bool  d_running;
    float d_speed;

    void modulo();

public:
    day_night_cycle();

    void on_update(double dt);
    void start();
    void stop();
    bool is_running() const { return d_running; }

    void set_speed(float speed);
    float get_speed() const;

    void set_time(int hours, int mins, int secs);
    void add_seconds(float seconds);

    float get_seconds() const;
    void set_seconds(float secs);
        // Access the value of the day-night cycle by treating it as
        // the number of seconds past midnight.

    float get_angle() const;
    void set_angle(float angle);
        // Access the value of the day-night cycle by treating it as
        // an angle. 0.0f is midnight and 180.0f is noon.

    bool is_day() const;
        // Dayime is defined as being between 6am and 6pm.

    glm::vec3 sun_direction() const;
        // Returns a direction vector to act at the direction of sunlight.
        // Calculated using the day-night cycle treated as an angle. The
        // direction at noon is straight down.

    std::string to_string() const;
};