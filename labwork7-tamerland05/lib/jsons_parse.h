#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>

static const std::string ConfigFileLittle = R"(C:\C++\labwork7-tamerland05\config_little.json)";
static const std::string ConfigFileBig = R"(C:\C++\labwork7-tamerland05\config_big.json)";


static std::map<int, std::string> WeatherCodes{
    {0, "Ясно"},
    {1, "Малооблачно"},
    {2, "Облачно"},
    {3, "Пасмурно"},
    {45,"Туман"},
    {48,"Изморозь"},
    {51,"Легкий моросящий дождь"},
    {53,"Легкий моросящий дождь"},
    {55,"Средний моросящий дождь"},
    {56,"Моросящий дождь со снегом"},
    {57,"Моросящий дождь со снегом"},
    {61,"Легкий дождь"},
    {63,"Дождь"},
    {65,"Сильный дождь"},
    {66,"Легкий дождь со снегом"},
    {67,"Дождь со снегом"},
    {71,"Небольшой снег"},
    {73,"Снег"},
    {75,"Сильный снег"},
    {77,"Снежные зерна"},
    {80,"Небольшой ливень"},
    {81,"Ливень"},
    {82,"Сильный ливень"},
    {85,"Снегопад"},
    {86,"Сильный снегопад"},
    {95,"Гроза"},
    {96,"Гроза с незначительным градом"},
    {99,"Гроза с градом"}
};

static std::map<std::string, std::string> ErrorMessages {
        {"cities", "Конфигурационный файл не содержит городов."},
        {"params", "Конфигурационный файл не содержит параметров по умолчанию."},
        {"days", "Параметры по умолчанию не содержат значения количества отображаемых дней."},
        {"frequency", "Параметры по умолчанию не содержат значения частоты обновления."},
        {"name", "Не найден город "},
        {"latitude", "Не найдена широта города "},
        {"longitude", "Не найдена долгота города "},
        {"timezone_abbreviation", "Не найден часовой пояс города "},
        {"elevation", "Не найдена высота над уровнем моря города "},
        {"hourly", "Не найдены почасовые характеристики погоды для города "},
        {"time", "Не найдено время для города "},
        {"temperature_2m", "Не найдена температура для города "},
        {"precipitation_probability", "Не найдена вероятность осадков для города "},
        {"precipitation", "Не найден процент осадков для города "},
        {"rain", "Не найдена вероятность дождя для города "},
        {"weather_code", "Не найден погодный код для города "},
        {"visibility", "Не найдена видимость для города "},
        {"wind_speed_10m", "Не найдена скорость ветра для города "},
        {"wind_direction_10m", "Не найдено направление ветра для города "},
        {"daily", "Не найдены ежедневные характеристики погоды для города "},
        {"temperature_2m_max", "Не найдены максимальные температуры для города "},
        {"temperature_2m_min", "Не найдены минимальные температуры для города "},
        {"precipitation_sum", "Не найдено количество атмосферных осадков температуры для города "},
        {"precipitation_probability_max", "Не найдено количество атмосферных осадков температуры для города "},
        {"wind_speed_10m_max", "Не найдены скорости ветра для города "},
        {"wind_gusts_10m_max", "Не найдены направления ветра для города "}
};

struct Config {
    std::vector<std::string> cities;
    int days;
    int frequency;
};

struct CityInfo {
    std::string name;

    double latitude;
    double longitude;
};

struct WeatherInfo {
    std::string name;

    std::string timezone;
    float elevation;
    std::string time;
    float temperature;
    int precipitation_probability;
    float precipitation;
    float rain;
    int weather_code;
    float visibility;
    float wind_speed;
    int wind_direction;

    std::vector<std::string> times;
    std::vector<int> weather_codes;
    std::vector<float> temperatures_max;
    std::vector<float> temperatures_min;
    std::vector<float> precipitations_sum;
    std::vector<int> precipitation_probabilities_max;
    std::vector<float> wind_speeds_max;
    std::vector<float> wind_gusts_max;
};

Config ReadConfig(const std::string& ConfigFile);

CityInfo ReadCoordinates(const std::string& city_name, const std::string& info);

WeatherInfo ReadWeather(const CityInfo& city, const std::string& info);

bool Checker(const nlohmann::json&, const std::string& key, const std::string& city_name="");