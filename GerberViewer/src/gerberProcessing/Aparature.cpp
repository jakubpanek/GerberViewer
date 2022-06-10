//
// Created by Jakub Pánek on 28.05.2021.
//

#include "Aperture.h"

Ap_Circle::Ap_Circle(unsigned int _orderIndex, double _diameter, double _hole_diameter)
{
    orderIndex = _orderIndex;
    diameter = _diameter;
    hole_diameter = _hole_diameter;
    generatePoints();
}

/// Generates points to be used as the shape, ran by constructor
void Ap_Circle::generatePoints() {
    //TODO: handle hole
    pathPoints = {};
    auto startangle = 0;
    auto endangle = M_PI * 2;
    for (int i = 0; i <= 20; i++)
    {
        const double a = endangle + ((float)i / (float)20) * (startangle - endangle);
        pathPoints.emplace_back(std::cos(a) * diameter/2, std::sin(a) * diameter/2);
    }
}

Ap_Rect::Ap_Rect(unsigned int _orderIndex, double _sizeX, double _sizeY, double _hole_diameter)
{
    orderIndex = _orderIndex;
    sizeX = _sizeX;
    sizeY = _sizeY;
    hole_diameter = _hole_diameter;

    generatePoints();
}

/// Generates points to be used as the shape, ran by constructor
void Ap_Rect::generatePoints() {
    //TODO: handle hole
    pathPoints = {};
    pathPoints.emplace_back(doubleVec2(sizeX/2,sizeY/2));
    pathPoints.emplace_back(doubleVec2(sizeX/2,-sizeY/2));
    pathPoints.emplace_back(doubleVec2(-sizeX/2,-sizeY/2));
    pathPoints.emplace_back(doubleVec2(-sizeX/2,sizeY/2));
}

Ap_Obround::Ap_Obround(unsigned int _orderIndex, double _sizeX, double _sizeY, double _hole_diameter)
{
    orderIndex = _orderIndex;
    sizeX = _sizeX;
    sizeY = _sizeY;
    hole_diameter = _hole_diameter;
    pathPoints = {};
}

Ap_Poly::Ap_Poly(unsigned int _orderIndex, double _diameter, double _rotation, double _hole_diameter)
{
    orderIndex = _orderIndex;
    diameter = _diameter;
    rotation = _rotation;
    hole_diameter = _hole_diameter;
    pathPoints = {};
}