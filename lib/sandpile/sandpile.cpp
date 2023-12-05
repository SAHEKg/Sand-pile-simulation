#include "sandpile.h"
#include "../converting_to_bmp/BMP.h"

#include <fstream>
#include <cmath>

void Sandpile::Parse(int argc, char** argv) {
    if (argc < 13) {
        std::cout << "Not enough arguments" << std::endl;
        return;
    } else if (argc == 14 && static_cast<std::string>(argv[argc - 1]) != "AMOGUS" || argc > 14) {
        std::cout << "Too many arguments" << std::endl;
        return;
    }

    for (int i = 1; i < argc - 1; i++) {
        std::string elem = argv[i];
        std::string next_elem = argv[i + 1];

        if (elem == "-l" || elem == "--length") {
            length = std::stoi(next_elem);
        } else if (elem == "-w" || elem == "--width") {
            width = std::stoi(next_elem);
        } else if (elem == "-i" || elem == "--input") {
            input = next_elem;
        } else if (elem == "-o" || elem == "--output") {
            output = next_elem;
        } else if (elem == "-m" || elem == "--max-iter") {
            max_iter = std::stoi(next_elem);
        } else if (elem == "-f" || elem == "--freq") {
            freq = std::stoi(next_elem);
        } else if (next_elem == "AMOGUS") {
            AMOGUS = true;
        }
    }
}

void Sandpile::Create() {
    pile = new uint64_t*[width];
    for (int i = 0; i < width; i++) {
        pile[i] = new uint64_t[length];
        for (int j = 0; j < length; j++) {
            pile[i][j] = 0;
        }
    }

    std::fstream tsv_file(input);
    int x;
    int y;
    uint64_t grains;
    if (!tsv_file.is_open()) {
        std::cout << "Couldn't open input file" << std::endl;
        return;
    }
    while (!tsv_file.eof()) {
        tsv_file >> x >> y >> grains;
        pile[y][x] = grains;
        pairs coordinates = std::make_pair(x, y);
        next_iter_disperses.insert(coordinates);
    }

    if (AMOGUS) {
        AmogusFunc();
    }
}

void Sandpile::Disperse() {
    if (freq == 0) {
        freq = max_iter;
    }
    int iteration = 1;
    while (iteration <= max_iter && !(next_iter_disperses.empty())) {
        current_iter_disperses = next_iter_disperses;
        next_iter_disperses.clear();
        for (auto iter : current_iter_disperses) {
            if (pile[iter.second][iter.first] >= 4) {
                pile[iter.second][iter.first] -= 4;
                if (pile[iter.second][iter.first] >= 4) {
                    next_iter_disperses.insert(iter);
                } else {
                    next_iter_disperses.erase(iter);
                }
            }
            if (iter.first != 0) {
                if (barriers.find(std::make_pair(iter.first - 1, iter.second)) == barriers.end()) {
                    pile[iter.second][iter.first - 1] += 1;
                    if (pile[iter.second][iter.first - 1] >= 4) {
                        next_iter_disperses.insert(std::make_pair(iter.first - 1, iter.second));
                    }
                }
            }
            if (iter.second != 0) {
                if (barriers.find(std::make_pair(iter.first, iter.second - 1)) == barriers.end()) {
                    pile[iter.second - 1][iter.first] += 1;
                    if (pile[iter.second - 1][iter.first] >= 4) {
                        next_iter_disperses.insert(std::make_pair(iter.first, iter.second - 1));
                    }
                }
            }
            if (iter.first != length - 1) {
                if (barriers.find(std::make_pair(iter.first + 1, iter.second)) == barriers.end()) {
                    pile[iter.second][iter.first + 1] += 1;
                    if (pile[iter.second][iter.first + 1] >= 4) {
                        next_iter_disperses.insert(std::make_pair(iter.first + 1, iter.second));
                    }
                }
            }
            if (iter.second != width - 1) {
                if (barriers.find(std::make_pair(iter.first, iter.second + 1)) == barriers.end()) {
                    pile[iter.second + 1][iter.first] += 1;
                    if (pile[iter.second + 1][iter.first] >= 4) {
                        next_iter_disperses.insert(std::make_pair(iter.first, iter.second + 1));
                    }
                }
            }
        }
        if (iteration % freq == 0) {
            Image(this).BuildImage(output, std::to_string(iteration));
        }
        iteration++;
    }
    if (iteration < max_iter && iteration % freq != 0) {
        Image(this).BuildImage(output, std::to_string(iteration));
    }
}

void Sandpile::AddBarrier(pairs top_left, pairs bottom_rigth) {
    for (int y = top_left.second; y <= bottom_rigth.second; y++) {
        for (int x = top_left.first; x <= bottom_rigth.first; x++) {
            barriers.insert(std::make_pair(x, y));
        }
    }
}

void Sandpile::RemoveBarrier(pairs top_left, pairs bottom_rigth) {
    for (int y = top_left.second; y <= bottom_rigth.second; y++) {
        for (int x = top_left.first; x <= bottom_rigth.first; x++) {
            barriers.erase(std::make_pair(x, y));
        }
    }
}

void Sandpile::ClearBarriers() {
    barriers.clear();
}



void Sandpile::AmogusFunc() {
    uint16_t top_y = width / 8;
    uint16_t top_y2 = top_y + width / 16;
    uint16_t bottom_y = width - top_y;
    uint16_t left_x = length / 4;
    uint16_t rigth_x = length - left_x;
    uint16_t leg_left = length / 2 - length / 16;
    uint16_t leg_rigth = length / 2 + length / 16;
    uint16_t eye_bottom = width / 2 - width / 16;
    uint16_t eye_top = eye_bottom - width / 8;
    uint16_t eye_rigth = rigth_x + length / 16;
    uint16_t backpack_bottom = bottom_y - width / 8 - width / 16;
    uint16_t backpack_top = eye_bottom - width / 16;

    AddBarrier(std::make_pair(left_x, top_y2), std::make_pair(left_x, bottom_y));
    AddBarrier(std::make_pair(left_x, bottom_y), std::make_pair(leg_left, bottom_y));
    AddBarrier(std::make_pair(leg_left, backpack_bottom), std::make_pair(leg_left, bottom_y));
    AddBarrier(std::make_pair(leg_left, backpack_bottom), std::make_pair(leg_rigth, backpack_bottom));
    AddBarrier(std::make_pair(leg_rigth, backpack_bottom), std::make_pair(leg_rigth, bottom_y));
    AddBarrier(std::make_pair(leg_rigth, bottom_y), std::make_pair(rigth_x, bottom_y));
    AddBarrier(std::make_pair(rigth_x, eye_bottom), std::make_pair(rigth_x, bottom_y));
    AddBarrier(std::make_pair(rigth_x, top_y2), std::make_pair(rigth_x, eye_top));

    uint16_t iter_x = left_x;
    uint16_t iter_y = top_y2;
    while (iter_y >= top_y) {
        AddBarrier(std::make_pair(iter_x, iter_y), std::make_pair(iter_x, iter_y));
        iter_x++;
        iter_y--;
    }
    uint16_t top_left = iter_x;
    iter_x = rigth_x;
    iter_y = top_y2;
    while (iter_y >= top_y) {
        AddBarrier(std::make_pair(iter_x, iter_y), std::make_pair(iter_x, iter_y));
        iter_x--;
        iter_y--;
    }
    AddBarrier(std::make_pair(top_left, top_y), std::make_pair(iter_x, top_y));


    AddBarrier(std::make_pair(width / 8, backpack_top), std::make_pair(left_x, backpack_top));
    AddBarrier(std::make_pair(width / 8, backpack_top), std::make_pair(width / 8, backpack_bottom));
    AddBarrier(std::make_pair(width / 8, backpack_bottom), std::make_pair(left_x, backpack_bottom));

    AddBarrier(std::make_pair(width / 2, eye_top), std::make_pair(width / 2, eye_bottom));
    AddBarrier(std::make_pair(width / 2, eye_top), std::make_pair(eye_rigth, eye_top));
    AddBarrier(std::make_pair(width / 2, eye_bottom), std::make_pair(eye_rigth, eye_bottom));
    AddBarrier(std::make_pair(eye_rigth, eye_top), std::make_pair(eye_rigth, eye_bottom));
}

int Sandpile::Distance(int x, int y, pairs centre) {
    return static_cast<int>(sqrt(pow(abs(x - centre.first), 2) + pow(abs(y - centre.second), 2)));
}

void Sandpile::AddEllipse(pairs left_centre, pairs rigth_centre, int radius) {
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < length; x++) {
            if (Distance(x, y, left_centre) + Distance(x, y, rigth_centre) == radius ^ Distance(x, y, left_centre) + Distance(x, y, rigth_centre) == radius - 1) {
                AddBarrier(std::make_pair(x, y), std::make_pair(x, y));
            }
        }
    }
}
