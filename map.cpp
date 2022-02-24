#include "map.hpp"

Map::District::House::House(const int curX, const int curY, const int curW, const int curH) : 
xy(curX, curY), w(curW), h(curH) { 
        houseRNG.seed(std::chrono::steady_clock::now().time_since_epoch().count());
        doorMarking();
}

bool Map::District::House::intersect(const House &r) const{
    return !(r.xy.first > (xy.first + w + 1) || xy.first > (r.xy.first + r.w + 1) 
        || r.xy.second > (xy.second + h + 1) || xy.second > (r.xy.second + r.h + 1));    
}

void Map::District::House::doorMarking() {

    int worldSide = 1 + houseRNG() % 4;

    switch (worldSide)
    {
    case 1:
        doorXY = std::make_pair(xy.first + houseRNG() % w, xy.second);
        break;

    case 2:
        doorXY = std::make_pair(xy.first, xy.second + houseRNG() % h);
        break;
        
    case 3:
        doorXY = std::make_pair(xy.first + houseRNG() % w, xy.second + h - 1);
        break;

    case 4:
        doorXY = std::make_pair(xy.first + w - 1, xy.second + houseRNG() % h);
        break;

    default:
        std::cout << "Error: wrong world side!" << std::endl;
        break;
    }
}

Map::District::District(const int curHeight, const int curWidth, const int curRoadSize, const int curHouseCount, 
const int curA, const int curB) : m_dHeight(curHeight + 2 * curRoadSize), m_dWidth(curWidth + 2 * curRoadSize), 
m_roadSize(curRoadSize), m_houseCount(curHouseCount), m_a(curA), m_b(std::max(curB, m_a)) {

    districtRNG.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    init();
    generate();
}

void Map::District::init() {
    m_district = std::vector<std::vector<char>>(m_dHeight, std::vector<char>(m_dWidth, '0'));
    for (size_t i = 0; i < m_dHeight; ++i) 
        for (size_t j = 0; j < m_dWidth; ++j) { 
            if (i < m_roadSize || j < m_roadSize || i >= m_dHeight - m_roadSize || j >= m_dWidth - m_roadSize)
                m_district[i][j] = 'H';
        }
}

void Map::District::generate() {
    for (size_t i = 0; i < m_houseCount; ++i) 
        for (size_t j = 0; j < 1000; ++j) {
            const int w = m_a + districtRNG() % (m_b - m_a + 1), h = m_a + districtRNG() % (m_b - m_a + 1);
            const House house{
                static_cast<const int>(3 + districtRNG() % (m_dWidth - w - 6)), // x
                static_cast<const int>(3 + districtRNG() % (m_dHeight - h - 6)), // y
                w, 
                h
            };
            
            bool intersection = false;
            for(size_t k = 0; k < m_houses.size(); ++k) {
                intersection = house.intersect(m_houses[k]);
                if (intersection) break;
            }
            
            if (!intersection) {
                m_houses.push_back(house);
                break;
            }
        }

    for (const House &house : m_houses) {
        for (size_t y = 0; y < house.h; ++y)
            for (size_t x = 0; x < house.w; ++x)
                if (x != 0 && x != (house.w - 1) && y != 0 && y != (house.h - 1))
                    m_district[house.xy.second + y][house.xy.first + x] = '2';
                else
                    m_district[house.xy.second + y][house.xy.first + x] = '1';
        m_district[house.doorXY.second][house.doorXY.first] = 'D';
    }

    buildRoads();
}

void Map::District::buildRoads() {
    std::vector<std::pair<int, int>> path;
    for (const House &house : m_houses) {        
        path = dfs(house);
        for (size_t i = 0; i < path.size() - 1; ++i) {
            m_district[path[i].second][path[i].first] = 'H';
        }
    }
}

std::vector<std::pair<int, int>> Map::District::dfs(const House &house) const {
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<int>> cameFrom(m_district.size(), std::vector<int>(m_district[0].size(), 0));
    for (size_t i = 0; i < m_district.size(); ++i)
        for (size_t j = 0; j < m_district[0].size(); ++j) {
            if (m_district[i][j] != '0') cameFrom[i][j] = -1;
            if (m_district[i][j] == 'H') cameFrom[i][j] = 8;
        }
    q.push(house.doorXY);

    std::pair<int, int> temp;
    while(!q.empty()) {
        temp = q.front();
        q.pop();

        if (m_district[temp.second][temp.first] == 'H') break;

        if (cameFrom[temp.second + 1][temp.first] == 0 || cameFrom[temp.second + 1][temp.first] == 8) {
            q.push({temp.first, temp.second + 1});
            cameFrom[temp.second + 1][temp.first] = 1;
        }
        if (cameFrom[temp.second][temp.first + 1] == 0 || cameFrom[temp.second][temp.first + 1] == 8) {
            q.push({temp.first + 1, temp.second});
            cameFrom[temp.second][temp.first + 1] = 2;
        }
        if (cameFrom[temp.second - 1][temp.first] == 0 || cameFrom[temp.second - 1][temp.first] == 8) {
            q.push({temp.first, temp.second - 1});
            cameFrom[temp.second - 1][temp.first] = 3;
        }
        if (cameFrom[temp.second][temp.first - 1] == 0 || cameFrom[temp.second][temp.first - 1] == 8) {
            q.push({temp.first - 1, temp.second});
            cameFrom[temp.second][temp.first - 1] = 4;
        }
    }

    std::vector<std::pair<int, int>> path;
    while(temp != house.doorXY) {
        switch (cameFrom[temp.second][temp.first])
        {
        case 1:
            temp = {temp.first, temp.second - 1};
            path.push_back(temp);
            break;
        case 2:
            temp = {temp.first - 1, temp.second};
            path.push_back(temp);
            break;
        case 3:
            temp = {temp.first, temp.second + 1};
            path.push_back(temp);
            break;
        case 4:
            temp = {temp.first + 1, temp.second};
            path.push_back(temp);
            break;
        default:
            std::cout << "Error: wrong side!" << std::endl;
            break;
        }
    }
    return path;
}

std::vector<std::vector<char>> Map::District::getDistrict() const {
    return m_district;
}

void Map::District::print(std::string name) const {
    std::ofstream fout(name);
    for(size_t i = 0; i < m_district.size(); ++i) {
        for(size_t j = 0; j < m_district[i].size(); ++j)
            fout << m_district[i][j];// << '\t';
        fout << "\n";
    }
    fout.close();
}

Map::Map(const int height, const int width, const int wideCount, const int heightCount, const double diffPercent,
const int roadSize, const double housePercent) : m_height(height), m_width(width), m_wideCount(wideCount), 
m_heightCount(heightCount), m_diffPercent(diffPercent), m_roadSize(roadSize), m_housePercent(housePercent) {
    
    mapRNG.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    m_h = splitting(m_height, m_heightCount);
    for (size_t i = 0; i < m_heightCount; ++i)
        m_w.push_back(splitting(m_width, m_wideCount));

    mapInit();
    printMap("Map.txt");
}

std::vector<int> Map::splitting(int commomLen, int count) {
    int avgLen = commomLen / count;
    const int delta = static_cast<const int>(avgLen * m_diffPercent);
    int minLen = avgLen - delta, maxLen = avgLen + delta;
    std::vector<int> lens(count);

    if (! (count % 2)) 
        for (size_t i = 0; i < count / 2; ++i) {
            lens[i] = minLen + mapRNG() % (maxLen - minLen + 1);
            lens[i + count / 2] = 2 * avgLen - lens[i];
        }
    else {
        for (size_t i = 0; i < count / 2; ++i) {
            lens[i] = minLen + mapRNG() % (maxLen - minLen + 1);
            lens[i + count / 2 + 1] = 2 * avgLen - lens[i];
        }
        lens[count / 2] = avgLen;        
    }

    for(size_t i = 0; i < commomLen - avgLen * count; ++i)
        lens[mapRNG() % count] += 1;

    shuffling(lens);
    return lens;
}

void Map::shuffling(std::vector<int> &arr) {
    std::vector<int> temp = arr;
    int j = 0;
    for(size_t i = 0; i < arr.size(); ++i){
        while(!temp[j])
          j = mapRNG() % arr.size();
        arr[i] = temp[j];
        temp[j] = 0;
    }
}

void Map::mapInit() {
    m_mapWidth = m_width + (m_wideCount + 1) * m_roadSize + 2; 
    m_mapHeight = m_height + (m_heightCount + 1) * m_roadSize + 2;
    m_map = std::vector<std::vector<char>>(m_mapHeight, std::vector<char>(m_mapWidth, '0'));

    for (size_t i = 0; i < m_mapWidth; ++i) { m_map[0][i] = '*'; m_map[m_mapHeight - 1][i] = '*'; }
    for (size_t i = 0; i < m_mapHeight; ++i) { m_map[i][0] = '*'; m_map[i][m_mapWidth - 1] = '*'; }

    std::vector<std::vector<char>> tempArr, tempArr2, tempArr3, tempArr4, tempArr5;
    for (size_t i = 0; i < m_w.size(); ++i)
        for (size_t j = 0; j < m_w[0].size(); ++j) {
            District temp(
                m_h[i], m_w[i][j], m_roadSize, 1 / m_housePercent, 3, 
                std::min(m_width / m_wideCount, m_height / m_heightCount)*m_housePercent
            );
            tempArr = temp.getDistrict();
            draw(tempArr);
        }
}

void Map::draw(std::vector<std::vector<char>> &arr) {
    for (size_t i = 0; i < arr.size(); ++i)
        for(size_t j = 0; j < arr[0].size(); ++j)
            m_map[curY + i][curX + j] = arr[i][j];
    
    curX = curX + arr[0].size() - m_roadSize;
    if (m_map[curY][curX + m_roadSize] == '*') {
        curX = 1;
        curY = curY + arr.size() - m_roadSize;
    }    
}

std::vector<std::vector<char>> Map::getMap() {
    return m_map;
}

void Map::printMap(std::string name) const {
    std::ofstream fout(name);
    for(size_t i = 0; i < m_map.size(); ++i) {
        for(size_t j = 0; j < m_map[i].size(); ++j)
            fout << m_map[i][j];
        fout << "\n";
    }
    fout << "\n";
    fout.close();
}