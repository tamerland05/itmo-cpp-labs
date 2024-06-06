#include "jsons_parse.h"

bool Checker(const nlohmann::json& json, const std::string& key, const std::string& city_name) {
    if (json.find(key) == json.end()) {
        std::cerr << ErrorMessages[key] << city_name << '\n';
        system("pause");

        exit(EXIT_FAILURE);
    } else {
        return true;
    }
}

Config ReadConfig(const std::string& ConfigFile) {
    std::fstream file(ConfigFile);
    nlohmann::json config;
    Config cities;

    try {
        config = nlohmann::json::parse(file);
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Конфигурационный файл не является валидным." << '\n'
        << e.what() << '\n';
        system("pause");

        exit(EXIT_FAILURE);
    }

    if (Checker(config, "cities")) {
        auto cities_array = config["cities"];

        if (cities_array.is_array()
            && cities_array.front().is_string()) {
            for (const auto& city : cities_array) {
                cities.cities.emplace_back(city);
            }
        } else {
            std::cerr << "Города в конфигурационном файле должны быть представлены списком cтрок." << '\n';
            system("pause");

            exit(EXIT_FAILURE);
        }
    }

    if (Checker(config, "params")) {
        auto params = config["params"];

        if (Checker(params, "days")) {
            if (params["days"].is_number_integer()) {
                cities.days = params["days"];
            } else {
                std::cerr << "Значение количества отображаемых дней должно быть типа int." << '\n';
                system("pause");

                exit(EXIT_FAILURE);
            }
        }

        if (Checker(params, "frequency")) {
            if (params["frequency"].is_number_integer()) {
                cities.frequency = params["frequency"];
            } else {
                std::cerr << "Значение частоты обновления должно быть типа int.";
                system("pause");

                exit(EXIT_FAILURE);
            }
        }
    }

    return cities;
}

CityInfo ReadCoordinates(const std::string& city_name, const std::string& info) {
    nlohmann::json result;
    CityInfo city{city_name};

    try {
        result = nlohmann::json::parse(info);
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Результат запроса координат не валиден для города " + city.name << '\n'
        << e.what() << '\n';
        system("pause");

        exit(EXIT_FAILURE);
    }

    if (Checker(result, "name", city.name)
    && Checker(result, "latitude", city.name)
    && Checker(result, "longitude", city.name)) {
        city.name = result["name"];
        city.latitude = result["latitude"];
        city.longitude = result["longitude"];
    }

    return city;
}

WeatherInfo ReadWeather(const CityInfo& city, const std::string& info) {
    nlohmann::json result;
    WeatherInfo place{city.name};

    try {
        result = nlohmann::json::parse(info);
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Результат запроса погоды не валиден для города " + place.name << '\n'
        << e.what() << '\n';
        system("pause");

        exit(EXIT_FAILURE);
    }

    if (Checker(result, "timezone_abbreviation", place.name)) {
        place.timezone = result["timezone_abbreviation"];
    }

    if (Checker(result, "elevation", place.name)) {
        place.elevation = result["elevation"];
    }

    if (Checker(result, "hourly", place.name)) {
        auto hourly = result["hourly"];

        if (Checker(hourly, "time", place.name)) {
            place.time = hourly["time"][0];
        }

        if (Checker(hourly, "temperature_2m", place.name)) {
            place.temperature = hourly["temperature_2m"][0];
        }

        if (Checker(hourly, "precipitation_probability", place.name)) {
            place.precipitation_probability = hourly["precipitation_probability"][0];
        }

        if (Checker(hourly, "precipitation", place.name)) {
            place.precipitation = hourly["precipitation"][0];
        }

        if (Checker(hourly, "rain", place.name)) {
            place.precipitation = hourly["rain"][0];
        }

        if (Checker(hourly, "weather_code", place.name)) {
            place.weather_code = hourly["weather_code"][0];
        }

        if (Checker(hourly, "visibility", place.name)) {
            place.visibility = hourly["visibility"][0];
        }

        if (Checker(hourly, "wind_speed_10m", place.name)) {
            place.wind_speed = hourly["wind_speed_10m"][0];
        }

        if (Checker(hourly, "wind_direction_10m", place.name)) {
            place.wind_direction = hourly["wind_direction_10m"][0];
        }
    }

    if (Checker(result, "daily", place.name)) {
        auto daily = result["daily"];

        if (Checker(daily, "time", place.name)) {
            for (const auto& i : daily["time"]) {
                place.times.emplace_back(i);
            }
        }

        if (Checker(daily, "weather_code", place.name)) {
            for (const auto& i : daily["weather_code"]) {
                place.weather_codes.emplace_back(i);
            }
        }

        if (Checker(daily, "temperature_2m_max", place.name)) {
            for (const auto& i : daily["temperature_2m_max"]) {
                place.temperatures_max.emplace_back(i);
            }
        }

        if (Checker(daily, "temperature_2m_min", place.name)) {
            for (const auto& i : daily["temperature_2m_min"]) {
                place.temperatures_min.emplace_back(i);
            }
        }

        if (Checker(daily, "precipitation_sum", place.name)) {
            for (const auto& i : daily["precipitation_sum"]) {
                place.precipitations_sum.emplace_back(i);
            }
        }

        if (Checker(daily, "precipitation_probability_max", place.name)) {
            for (const auto& i : daily["precipitation_probability_max"]) {
                place.precipitation_probabilities_max.emplace_back(i);
            }
        }

        if (Checker(daily, "wind_speed_10m_max", place.name)) {
            for (const auto& i : daily["wind_speed_10m_max"]) {
                place.wind_speeds_max.emplace_back(i);
            }
        }

        if (Checker(daily, "wind_gusts_10m_max", place.name)) {
            for (const auto& i : daily["wind_gusts_10m_max"]) {
                place.wind_gusts_max.emplace_back(i);
            }
        }
    }

    return place;
}
