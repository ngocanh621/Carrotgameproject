#ifndef MAPDIRECTIONS_H
#define MAPDIRECTIONS_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

// dinh nghia huong di map
struct MapDirections {
    int index;
    std::vector<std::string> directions;
    std::vector<int> stops;

    MapDirections() {
        index = 0;
    }
    //them huong di
    void setNext(std::string direction, int stop) {
        directions.push_back(direction);
        stops.push_back(stop);
    }
    // chuyen sang huong khac
    bool next() {
        if ((index + 1) > directions.size()) {
            std::cout << "Unable to move to next index in MapDirections vector." << std::endl;
            return false;
        }
        index++;
        return true;
    }
    //lay huong hien tai
    std::string getDir() {
        return directions[index];
    }
    //lay diem dung hien tai
    int getStop() {
        return stops[index];
    }
    // check ket thuc duong di
    bool isEnd() {
        return (index + 1 > directions.size());
    }
};

#endif
