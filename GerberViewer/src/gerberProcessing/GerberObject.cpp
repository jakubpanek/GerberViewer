//
// Created by Jakub Pánek on 28.05.2021.
//

#include "GerberObject.h"

void CircularSegment::GeneratePath()
{
    pathPoints = {};
    auto endangle = std::atan2(start.y - (start.y + offset.y),start.x - (start.x + offset.x));
    auto startangle = std::atan2(end.y - (start.y + offset.y),end.x- (start.x + offset.x));
    if(start == end)
    {
        startangle = 0;
        endangle = M_PI * 2;
    }
    auto radius = GetRadius();
    for (int i = 0; i <= 20; i++)
    {
        const double a = endangle + ((float)i / (float)20) * (startangle - endangle);
        pathPoints.emplace_back((start.x + offset.x) + std::cos(a) * radius, (start.y + offset.y) + std::sin(a) * radius);
    }
}

float CircularSegment::GetRadius()
{
    double centerposX = start.x + offset.x;
    double centerposY = start.y + offset.y;

    return sqrt(pow(centerposX - start.x, 2) + pow(centerposY - start.y, 2) * 1.0);
}

void Contour::ProcessObjects()
{
    polygon = {};
    for (std::variant<LinearSegment, CircularSegment> object : objects)
    {
        if(std::holds_alternative<LinearSegment>(object))
        {
            LinearSegment segment = std::get<LinearSegment>(object);
            polygon.emplace_back(segment.end);
        }
        else if(std::holds_alternative<CircularSegment>(object))
        {
            CircularSegment segment = std::get<CircularSegment>(object);
            segment.GeneratePath();
            polygon.insert(polygon.end(), std::begin(segment.pathPoints), std::end(segment.pathPoints));
        }
    }
    indices = mapbox::earcut<uint32_t>(std::vector<std::vector<doubleVec2>> {polygon});

}