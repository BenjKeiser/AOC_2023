#include "elf.h"
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <queue>


enum dir_t {
    WEST = 0,
    SOUTH = 1,
    EAST = 2,
    NORTH = 3,
    START = 4
};

// structure for information of each cell
struct cell {
    int x;
    int y;
    uint64_t distance;
    dir_t dir;
    int straight;

    cell(int x, int y, uint64_t distance, dir_t dir, int straight) : x(x), y(y), distance(distance), dir(dir), straight(straight)
    {}

    bool operator>(const cell& other) const { return distance > other.distance; }
};

//disregarding limits
std::vector<cell> get_neighbours(cell k, int row, int col, int min_steps, int max_steps)
{
    std::vector<cell> neighbours;

    // direction arrays for simplification of getting neighbour
    int dx[] = { -1, 0, 1, 0 };
    int dy[] = { 0, 1, 0, -1 };
    int straight = k.straight;

    // looping through all neighbours
    for (int i = 0; i < 4; i++) 
    {
        int delta_x = dx[i];
        int delta_y = dy[i];
        int x = k.x + delta_x;
        int y = k.y + delta_y;

        if(y >= 0 && y < row && x >= 0 && x < col)
        {

            //we don't move the way back we came
            if((abs(k.dir - i) == 2) && k.dir != START)
            {
                continue;
            }
            
            //too straight?
            straight = (dir_t)i == k.dir ? k.straight + 1 : 1;
            if(straight > max_steps)
            {
                continue;
            }

            if(k.dir != START && (dir_t)i != k.dir && k.straight < min_steps)
            {
                continue;
            }

            neighbours.push_back(cell(x, y, 0, (dir_t)i, straight));
        }
    }

    return neighbours;
}

uint64_t Elves::dijkstra(int min_steps, int max_steps)
{
    uint64_t heat_loss = UINT64_MAX;
    int row = heat_map.size();
    int col = heat_map[0].size();

    //set
    std::priority_queue<cell, std::vector<cell>, std::greater<cell>> queue;
    std::set<std::tuple<int, int, dir_t, int>> visited;

    //Starting point
    queue.push(cell(0, 0, 0, START, 0));

    //dijkstra loop
    while(!queue.empty())
    {
        cell current = queue.top();
        queue.pop();

        //check if this was already visited
        if(visited.count({current.x, current.y, current.dir, current.straight}))
        {
            continue;
        }

        if(current.distance >= heat_loss)
        {
            continue;
        }

        if(current.x == col - 1 && current.y == row - 1 && current.straight >= min_steps)
        {
            //we reached the end
            heat_loss = current.distance;
        }

        visited.insert({current.x, current.y, current.dir, current.straight});

        //get the valid nodes and put them on the queue
        std::vector<cell> neighbours = get_neighbours(current, row, col, min_steps, max_steps);
        for(auto & n : neighbours)
        {
            //add the node
            n.distance = current.distance + heat_map[n.y][n.x];
            queue.push(n);
        }
    }

    return heat_loss;
}


uint64_t Elves::get_heat_loss(int min_steps, int max_steps)
{
    uint64_t heat_loss = 0;

    heat_loss = dijkstra(min_steps, max_steps);

    return heat_loss;
}

Elves::Elves(char * file_name)
{
    std::ifstream file(file_name);
    std::string line;

    std::vector<int> heat;

    int nb = 0;
    // parse the file
    if (file.is_open()) 
    {
        while (std::getline(file, line)) 
        {
            heat.clear();
            for(int i=0; i < line.length(); i++)
            {
                heat.push_back(line[i]-0x30);
            }
            heat_map.push_back(heat);
        }

        for(int y = 0; y < heat_map.size(); y++)
        {
            for(int x = 0; x < heat_map[0].size(); x++)
            {
                std::cout << heat_map[y][x];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        file.close();
    }
}

