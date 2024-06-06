#include "api_getter.h"

#include <vector>

#include "conio.h"
#include "windows.h"
#include "ctime"

enum Keycaps {
    esc = 27,
    n = 110,
    p = 112,
    plus = 43,
    minus = 45
};

const std::string WeatherNowBase = "Данные приведены для временной зоны {}, "
                                   "высоты над уровнем моря {} m, "
                                   "даты/времени {}"
                                   "Погода в городе {}:\n"
                                   "Температура {} °C\n"
                                   "Вероятность осадков {}%, количество осадков {} mm\n"
                                   "{}, видимость {} m\n"
                                   "Скорость ветра {} km/h, направление ветра {}° относительно севера\n\n";

const std::string WeatherDayBase = "{}\n"
                                   "{}, температура {}({}) °C\n"
                                   "Осадки {} mm, вероятность осадков {}%\n"
                                   "Скорость ветра {} km/h\n\n";

void StartApp();
std::string GetTime();

void PrintWeather(const WeatherInfo& city, const int& days, long long& current_time);
