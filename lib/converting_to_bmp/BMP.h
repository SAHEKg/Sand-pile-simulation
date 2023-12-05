#pragma once

#include "../sandpile/sandpile.h"

#include <iostream>

struct Colour {
    float r;
    float g;
    float b;
    Colour();
    Colour(float r_construct, float g_construct, float b_construct);
};

struct Image {
    uint16_t width;
    uint16_t length;
    Colour* colours;

    Image(Sandpile* sandpile);
    Colour GetColour(uint16_t x, uint16_t y) const;
    void SetColour(uint16_t x, uint16_t y, const Colour& colour);
    void BuildImage(const std::string& path, const std::string name);
};
