// Start time: 4:40pm
// End time: 5:43pm

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::tuple<size_t, size_t, size_t>> CateogryMap;

int FindValue(CateogryMap &map, size_t &value) {
    for (auto &t : map) {
        size_t dest_start = std::get<0>(t);
        size_t src_start = std::get<1>(t);
        size_t range = std::get<2>(t);
        if (value >= src_start && value < src_start + range) {
            return dest_start + value - src_start;
        }
    }
    return value;
}

int main() {
    CateogryMap seed_to_soil;
    CateogryMap soil_to_fertilizer;
    CateogryMap fertilizer_to_water;
    CateogryMap water_to_light;
    CateogryMap light_to_temperature;
    CateogryMap temperature_to_humidity;
    CateogryMap humidity_to_location;

    CateogryMap *reading_map = nullptr;
    std::vector<size_t> seeds;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        if (line.empty())
            continue;
        if (line.find("seeds:") != std::string::npos) {
            std::string s = line.substr(line.find(":") + 2, line.length());
            std::stringstream ss(s);
            while (ss >> line) {
                seeds.push_back(std::stoll(line));
            }
        } else if (line.find("map:") != std::string::npos) {
            std::string map_name = line.substr(0, line.find("map:") - 1);
            if (map_name == "seed-to-soil") {
                reading_map = &seed_to_soil;
            } else if (map_name == "soil-to-fertilizer") {
                reading_map = &soil_to_fertilizer;
            } else if (map_name == "fertilizer-to-water") {
                reading_map = &fertilizer_to_water;
            } else if (map_name == "water-to-light") {
                reading_map = &water_to_light;
            } else if (map_name == "light-to-temperature") {
                reading_map = &light_to_temperature;
            } else if (map_name == "temperature-to-humidity") {
                reading_map = &temperature_to_humidity;
            } else if (map_name == "humidity-to-location") {
                reading_map = &humidity_to_location;
            }
        } else if (reading_map != nullptr) {
            std::stringstream ss(line);
            size_t dest_start, src_start, range;
            ss >> dest_start >> src_start >> range;
            std::tuple<size_t, size_t, size_t> t(dest_start, src_start, range);
            reading_map->push_back(t);
        }
    }

    for (size_t &seed : seeds) {
        size_t soil = FindValue(seed_to_soil, seed);
        size_t fertilizer = FindValue(soil_to_fertilizer, soil);
        size_t water = FindValue(fertilizer_to_water, fertilizer);
        size_t light = FindValue(water_to_light, water);
        size_t temperature = FindValue(light_to_temperature, light);
        size_t humidity = FindValue(temperature_to_humidity, temperature);
        size_t location = FindValue(humidity_to_location, humidity);
        std::cout << location << std::endl;
    }
}