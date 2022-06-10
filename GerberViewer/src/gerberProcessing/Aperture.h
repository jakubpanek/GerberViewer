//
// Created by Jakub Pánek on 02.03.2021.
//

#ifndef GERBERVIEWER_APERTURE_H
#define GERBERVIEWER_APERTURE_H

#include <../src/utils.h>
#include <cmath>
#include <cstdio>

///Base class for aparature
class Aperture{
public:
    std::vector<doubleVec2> pathPoints;             //vector of points to make up the aparature
    unsigned int orderIndex;                        //number of the aparature

    virtual ~Aperture() = default;;
    virtual explicit operator std::string() const { return ("unspecified Aparature");}
    virtual double getDiameter(){return 0;}         //returns diameter, for other aperatures than circle, returns 0
};

///Circle aparature. template_call = 'C' ',' diameter 'X' hole_diameter
class Ap_Circle : public Aperture {
    double diameter;                //diameter of the circle
    double hole_diameter;           //diameter of hole in the middle

    /// Generates points to be used as the shape, ran by constructor
    void generatePoints();

public:
    /// Generates points to be used as the shape, ran by constructor
    Ap_Circle(unsigned int _orderIndex, double _diameter, double _hole_diameter = 0);

    explicit operator std::string() const override
    {
        return ("CIRCLE: size: " + std::to_string(diameter) + (hole_diameter > 0 ? (" with hole: " + std::to_string(hole_diameter)) : ""));
    }

    double getDiameter() override {
        return diameter;
    }

};

///Rectangle aparature. template_call = 'R' ',' x_size 'X' y_size 'X' hole_diameter
class Ap_Rect : public Aperture{
    double sizeX;                   //width of the rectangle
    double sizeY;                   //height of the rectangle
    double hole_diameter;           //diameter of hole in the middle

    /// Generates points to be used as the shape, ran by constructor
    void generatePoints();
public:
    Ap_Rect(unsigned int _orderIndex, double _sizeX, double _sizeY, double _hole_diameter = 0);

    explicit operator std::string() const override
    {
        return ("RECT: sizeX: " + std::to_string(sizeX) + " sizeY: " + std::to_string(sizeY) + (hole_diameter>0 ? (" with hole: " + std::to_string(hole_diameter)):""));
    };

    double getDiameter() override {
        return 0;
    }
};

///Obround aparature. template_call = 'O' ',' x_size 'X' y_size 'X' hole_diameter
class Ap_Obround : public Aperture{
    double sizeX;                   //width of the obround
    double sizeY;                   //height of the obround
    double hole_diameter;           //diameter of hole in the middle
public:
    Ap_Obround(unsigned int _orderIndex, double _sizeX, double _sizeY, double _hole_diameter = 0);

    explicit operator std::string() const override
    {
        return ("OBRND: sizeX: " + std::to_string(sizeX) + " sizeY: " + std::to_string(sizeY) + (hole_diameter>0 ? (" with hole: " + std::to_string(hole_diameter)):""));
    };
};

///Polygon aparature. template_call = 'P' ',' outer_diameter 'X' vertices 'X' rotation 'X' hole_diameter
class Ap_Poly : public Aperture{
    double diameter;                //outer diameter
    unsigned char vertices;         //number of vertices in polygon
    double rotation = 0;            //rotation in degrees clockwise
    double hole_diameter;           //diameter of hole in the middle
public:
    Ap_Poly(unsigned int _orderIndex, double _diameter, double _rotation, double _hole_diameter = 0);

    explicit operator std::string() const override
    {
        return ("POLY: diam: " + std::to_string(diameter) + " vers: " + std::to_string(vertices) + (rotation>0 ? (" rot: " + std::to_string(rotation)):"") + (hole_diameter>0 ? (" with hole: " + std::to_string(hole_diameter)):""));
    };
};

class Ap_Macro : Aperture{
    std::string macro_name;
};
#endif //GERBERVIEWER_APERTURE_H