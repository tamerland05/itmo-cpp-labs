#include "cpr/cpr.h"

#include "jsons_parse.h"

#include <vector>
#include <iostream>


static const std::string ApiNinjasBase = "https://api.api-ninjas.com/v1/city?name=";
static const std::string ApiKey = "8PlH0FiIPEasHaVoxsGhaQ==STaMtuexbBioonBF";

static const std::string WeatherParams = "https://api.open-meteo.com/v1/forecast?latitude={}&longitude={}&"
                                  "hourly=temperature_2m,precipitation_probability,precipitation,rain,"
                                  "weather_code,visibility,wind_speed_10m,wind_direction_10m"
                                  "&daily=weather_code,temperature_2m_max,temperature_2m_min,"
                                  "precipitation_sum,precipitation_probability_max,wind_speed_10m_max,"
                                  "wind_gusts_10m_max&timezone=Europe%2FMoscow&forecast_days={}";

CityInfo GetCoordinates(const std::string& city);
WeatherInfo GetWeather(const CityInfo& city, int days);
