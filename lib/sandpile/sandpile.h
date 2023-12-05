#pragma once

#include <set>
#include <utility>
#include <iostream>

typedef std::pair<int, int> pairs;

struct Sandpile {
    uint16_t length;
    uint16_t width;
    uint64_t** pile;
    std::string input;
    std::string output;
    int max_iter;
    int freq;
    std::set<pairs> current_iter_disperses;
    std::set<pairs> next_iter_disperses;
    std::set<pairs> barriers;

    void Parse(int argc, char** argv);
    void Create();
    void Disperse();

    void AddBarrier(pairs top_left, pairs bottom_rigth);
    void RemoveBarrier(pairs top_left, pairs bottom_rigth);
    void ClearBarriers();

    void AddEllipse(pairs left_centre, pairs rigth_centre, int radius);
    int Distance(int x, int y, pairs centre);


    void AmogusFunc();
    bool AMOGUS = false;
};

