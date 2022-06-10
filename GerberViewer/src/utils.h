//
// Created by Jakub Pánek on 21.03.2021.
//

#ifndef GERBERVIEWER_UTILS_H
#define GERBERVIEWER_UTILS_H

#include <../lib/mapbox/earcut.hpp>

// 2D vector (used to store positions)
struct doubleVec2
{
    double  x, y;
    doubleVec2(){ x = y = 0.0f; }
    doubleVec2(float _x, float _y) { x = _x; y = _y; }

    explicit operator std::string() const
    {
        return ("(" + std::to_string(x) + " , " + std::to_string(y) + ")");
    }

    bool operator== (doubleVec2 &other) const
    {
        return (x == other.x && y == other.y);
    }
};

//allows to use custom point type as input for mapbox::earcut
namespace mapbox::util {
    template <>
    struct nth<0, doubleVec2> {
        inline static auto get(const doubleVec2 &t) {
            return t.x;
        };
    };
    template <>
    struct nth<1, doubleVec2> {
        inline static auto get(const doubleVec2 &t) {
            return t.y;
        };
    };
} // namespace mapbox::util


#endif //GERBERVIEWER_UTILS_H
