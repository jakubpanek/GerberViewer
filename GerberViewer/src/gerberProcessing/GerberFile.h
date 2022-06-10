//
// Created by Jakub Pánek on 02.03.2021.
//
#include <vector>
#include <map>
#include <variant>
#include "Aperture.h"
#include "GerberObject.h"

#ifndef GERBERVIEWER_LAYER_H
#define GERBERVIEWER_LAYER_H

///Stores information found in Gerber files
class GerberFile {
public:
    std::string filename;                                           //stores the original filename
    std::map<uint32_t, std::shared_ptr<Aperture>> aperatures;       //map of aparatures
    std::vector<std::variant<Line, Arc, Contour, Flash>> objects;   //all objects of the file

    std::vector<std::string> comments;                              //stores comments
    std::vector<std::string> errors;                                //stores errors encountered while parsing

    //Format specification data
    unsigned char intigerX;
    unsigned char decimalX;
    unsigned char intigerY;
    unsigned char decimalY;
    bool metricUnit;

    //Parsing data
    int totalLines = 0;
    int unrecognizedLines = 0;
};


#endif //GERBERVIEWER_LAYER_H
