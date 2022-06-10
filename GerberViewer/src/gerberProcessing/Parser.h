//
// Created by Jakub Pánek on 02.03.2021.
//
#include <fstream>
#include "GerberFile.h"
#ifndef GERBERVIEWER_PARSER_H
#define GERBERVIEWER_PARSER_H

enum InterpolationMode {linear, clockwise, counterclockwise};

///Returns double from gerber encoded coordinate given the starting position.
double GetDoublePosition(const std::string& line, const std::size_t start, std::size_t& end, const unsigned char intigers,const unsigned char decimal);

///Returns double starting at given starting position, returns ending position
double GetDoubleFromPos(const std::string& line, std::size_t start, std::size_t& end);

///Returns double starting at given starting position
double GetDoubleFromPos(const std::string& line, std::size_t start);

///Returns int starting at given starting position, returns ending position
int32_t GetIntFromPos(const std::string& line, std::size_t start, std::size_t& end);

///Returns int starting at given starting position
int32_t GetIntFromPos(const std::string& line, std::size_t start);

struct Parser
{
    ///Creates GerberFile from the given file
    GerberFile static fromFile(std::string path);
};



#endif //GERBERVIEWER_PARSER_H
