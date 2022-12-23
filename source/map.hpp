#pragma once

#include "include.hpp"

class Map {
private:
    std::mt19937 mapRNG;
    const int m_width, m_height, m_wideCount, m_heightCount, m_roadSize;
    const double m_diffPercent, m_housePercent;
    int m_mapWidth, m_mapHeight;
    int curX = 1, curY = 1;
    std::vector<std::vector<char>> m_map;
    std::vector<int> m_h;
    std::vector<std::vector<int>> m_w;

    class District {
    private:
        std::mt19937 districtRNG;
        const int m_dWidth, m_dHeight, m_roadSize, m_houseCount, m_a, m_b;
        std::vector<std::vector<char>> m_district;
        struct House {
            std::mt19937 houseRNG;
            std::pair<const int, const int> xy;
            std::pair<int, int> doorXY;
            const int w, h;
            House(const int, const int, const int, const int);
            bool intersect(const House &) const;
            void doorMarking();
        };
        std::vector<House> m_houses;
    public:
        District(const int, const int, const int, const int, const int, const int);
        void init();
        void generate();
        void buildRoads();
        std::vector<std::pair<int, int>> dfs(const House &) const;
        std::vector<std::vector<char>> getDistrict() const;
        void print(std::string) const;
    };
public:
    Map(const int, const int, const int, const int, const double, const int, const double);
    std::vector<int> splitting(int, int);
    void shuffling(std::vector<int>&);
    void mapInit();
    void draw(std::vector<std::vector<char>> &);
    std::vector<std::vector<char>> getMap();
    void printMap(std::string) const;
};