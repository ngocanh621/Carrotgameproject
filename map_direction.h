#ifndef MAPDIRECTIONS_H
#define MAPDIRECTIONS_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

struct MapDirections {
    int index;
    std::vector<std::string> directions;
    std::vector<int> stops;

    MapDirections() {
        index = 0;
    }

    void setNext(std::string direction, int stop) {
        directions.push_back(direction);
        stops.push_back(stop);
    }

    bool next() {
        if ((index + 1) > directions.size()) {
            std::cout << "Unable to move to next index in MapDirections vector." << std::endl;
            return false;
        }
        index++;
        return true;
    }

    std::string getDir() {
        return directions[index];
    }

    int getStop() {
        return stops[index];
    }

    bool isEnd() {
        return (index + 1 > directions.size());
    }
};

#endif
