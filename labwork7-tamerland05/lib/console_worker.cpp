#include "console_worker.h"

std::string GetTime() {
    auto sec = time(nullptr);

    return std::asctime(localtime(&sec));
}

void PrintWeather(const WeatherInfo& city, const int& days, long long& current_time) {
    std::cout <<
    std::vformat(WeatherNowBase,
                 std::make_format_args(city.timezone, city.elevation, GetTime(),
                                       city.name, city.temperature,
                                       city.precipitation_probability,
                                       city.precipitation,
                                       WeatherCodes[city.weather_code],
                                       city.visibility,
                                       city.wind_speed, city.wind_direction));

    for (int i = 1; i < days; ++i) {
        std::cout <<
        std::vformat(WeatherDayBase,
                     std::make_format_args(city.times[i],
                                           WeatherCodes[city.weather_codes[i]],
                                           city.temperatures_max[i], city.temperatures_min[i],
                                           city.precipitations_sum[i],
                                           city.precipitation_probabilities_max[i],
                                           city.wind_speeds_max[i]));
    }

    std::cout << std::string(110, '-') << '\n';
    current_time = time(nullptr);
}

void StartApp() {
    Config config = ReadConfig(ConfigFileLittle);

    std::vector<CityInfo> cities;

    for (auto& city : config.cities) {
        cities.emplace_back(GetCoordinates(city));
    }

    int city_pointer = 0;
    long long current_time;

    WeatherInfo weather = GetWeather(cities[city_pointer], config.days);
    PrintWeather(weather, config.days, current_time);

    while (true) {
        if (kbhit()) {
            int keycap = getch();

            if (keycap == Keycaps::esc) {
                std::cout << "Спасибо за использование\n";
                return;
            } else if (keycap == Keycaps::n) {
                if (city_pointer == cities.size() - 1) {
                    city_pointer = 0;
                } else {
                    ++city_pointer;
                }

                weather = GetWeather(cities[city_pointer], config.days);
                PrintWeather(weather, config.days, current_time);
            } else if (keycap == Keycaps::p) {
                if (city_pointer == 0) {
                    city_pointer = static_cast<int>(cities.size() - 1);
                } else {
                    --city_pointer;
                }

                weather = GetWeather(cities[city_pointer], config.days);
                PrintWeather(weather, config.days, current_time);
            } else if (keycap == Keycaps::plus) {
                ++config.days;

                weather = GetWeather(cities[city_pointer], config.days);
                PrintWeather(weather, config.days, current_time);
            } else if (keycap == Keycaps::minus) {
                if (config.days == 1) {
                    config.days = 2;
                }
                --config.days;

                weather = GetWeather(cities[city_pointer], config.days);
                PrintWeather(weather, config.days, current_time);
            }
        } else if (time(nullptr) - current_time > config.frequency) {
            weather = GetWeather(cities[city_pointer], config.days);
            PrintWeather(weather, config.days, current_time);
        }
    }
}
