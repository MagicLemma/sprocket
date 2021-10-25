#include "day_night_cycle.h"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include <format>

day_night_cycle::day_night_cycle()
    : d_seconds(0.0f)
    , d_running(true)
    , d_speed(1.0f)
{   
}

void day_night_cycle::on_update(double dt)
{
    if (d_running) {
        d_seconds += d_speed * dt;
        modulo();
    }
}

void day_night_cycle::start()
{
    d_running = true;
}

void day_night_cycle::stop()
{
    d_running = false;
}

void day_night_cycle::set_speed(float speed)
{
    d_speed = speed;
}

float day_night_cycle::get_speed() const
{
    return d_speed;
}

void day_night_cycle::set_time(int hours, int mins, int secs)
{
    d_seconds = hours * 3600.0 + mins * 60.0 + secs;
    modulo();
}

void day_night_cycle::add_seconds(float seconds)
{
    d_seconds += seconds;
    modulo();
}

float day_night_cycle::get_seconds() const
{
    return d_seconds; 
}

void day_night_cycle::set_seconds(float seconds)
{
    d_seconds = seconds;
    modulo();
}

float day_night_cycle::get_angle() const
{ 
    return glm::two_pi<float>() * d_seconds / SECONDS_IN_DAY;
}

void day_night_cycle::set_angle(float angle)
{
    d_seconds = (angle / glm::two_pi<float>()) * SECONDS_IN_DAY;
    modulo();
}

bool day_night_cycle::is_day() const
{
    return SIX_AM <= d_seconds && d_seconds < SIX_PM;
}

glm::vec3 day_night_cycle::sun_direction() const
{
    float angle = get_angle();
    glm::vec3 dir = {glm::sin(angle), glm::cos(angle), 0.1f * glm::sin(angle)};
    dir = glm::normalize(dir);
    return dir;
}

void day_night_cycle::modulo()
{
    while (d_seconds > SECONDS_IN_DAY) {
        d_seconds -= SECONDS_IN_DAY;
    }
    while (d_seconds < 0.0f) {
        d_seconds += SECONDS_IN_DAY;
    }
}

std::string day_night_cycle::to_string() const
{
    int hours = 0;
    int mins = 0;
    float seconds = d_seconds;
    while (seconds > 3600.0f) { ++hours; seconds -= 3600.0f; }
    while (seconds > 60.0f) { ++mins; seconds -= 60.0f; }

    std::string suffix = "";    
    return std::format("{}:{}:{:.2f}:{}", hours, mins, seconds, suffix);
}