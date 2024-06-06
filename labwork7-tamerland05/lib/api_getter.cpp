#include "api_getter.h"

CityInfo GetCoordinates(const std::string& city_name) {
    cpr::Url url{ApiNinjasBase + city_name};
    cpr::Header header{{"X-Api-Key", ApiKey}};

    auto response = cpr::Get(url, header);
    std::string text = response.text;
    if (response.error) {
        std::cerr << "Не найдены координаты города " + city_name << '\n';
        exit(EXIT_FAILURE);
    }

    std::string info{};
    for (int i = 1; i < text.size() - 1; ++i) {
        info += text[i];
    }

    return ReadCoordinates(city_name, info);
}

WeatherInfo GetWeather(const CityInfo& city, int days) {
    std::string request =
            std::vformat(WeatherParams, std::make_format_args(city.latitude, city.longitude, days));
    cpr::Url url{request};

    auto response = cpr::Get(url);

    return ReadWeather(city, response.text);
}




