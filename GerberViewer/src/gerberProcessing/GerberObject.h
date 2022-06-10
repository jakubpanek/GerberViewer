//
// Created by Jakub Pánek on 03.03.2021.
//

#ifndef GERBERVIEWER_GERBEROBJECT_H
#define GERBERVIEWER_GERBEROBJECT_H

#include <iostream>
#include <cstdint>
#include <math.h>       /* sqrt */
#include <variant>
#include <vector>
#include <string>

#include <mapbox/earcut.hpp>
#include <../src/utils.h>

enum Polarity {Clear, Dark};
enum Mirroring {None, Xaxis, Yaxis, XYaxis};

class LinearSegment
{
public:
    doubleVec2 start;
    doubleVec2 end;

    LinearSegment() = default;
    LinearSegment(doubleVec2 _start, doubleVec2 _end){start = _start; end = _end;}
};

class CircularSegment
{
public:
    doubleVec2 start;
    doubleVec2 end;
    doubleVec2 offset;
    bool clockwise = false;

    std::vector<doubleVec2> pathPoints;

    CircularSegment() = default;
    CircularSegment(doubleVec2 _start, doubleVec2 _end, doubleVec2 _offset, bool _clockwise){
        start = _start;
        end = _end;
        offset = _offset;
        clockwise = _clockwise;
        GeneratePath();
    }

    void GeneratePath();
    float GetRadius();
};

class Line : public LinearSegment
{
public:
    Polarity polarity;
    uint32_t aperture;

    Line(doubleVec2 _start, doubleVec2 _end, Polarity _polarity, uint32_t _aperture) {start = _start; end = _end; polarity = _polarity; aperture = _aperture;}

    std::string ToGerber()
    {
        return "";
    }
};


class Arc : public CircularSegment
{
public:
    Polarity polarity;
    uint32_t aperture;

    Arc(doubleVec2 _start, doubleVec2 _end, doubleVec2 _offset, Polarity _polarity, uint32_t _aperture) { start = _start; end = _end; offset = _offset; polarity = _polarity; aperture = _aperture;}
};

class Contour
{
public:
    Polarity polarity;
    std::vector<std::variant<LinearSegment, CircularSegment>> objects;

    std::vector<doubleVec2> polygon;
    std::vector<uint32_t> indices;

    void ProcessObjects();
};

class Flash
{
public:
    doubleVec2 position;
    Polarity polarity;
    uint32_t aperture;
    double rotation;
    double scale;

    Flash(doubleVec2 _position,uint32_t _aperture, Polarity _polarity, double _rotation, double _scale){
        position = _position;
        aperture = _aperture;
        polarity = _polarity;
        rotation = _rotation;
        scale = _scale;
    }
};



#endif //GERBERVIEWER_GERBEROBJECT_H
