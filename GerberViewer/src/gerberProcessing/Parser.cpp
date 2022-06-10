//
// Created by Jakub Pánek on 28.05.2021.
//

#include "Parser.h"

///Returns double from gerber encoded coordinate given the starting position.
double GetDoublePosition(const std::string& line, const std::size_t start, std::size_t& end, const unsigned char intigers,const unsigned char decimal)
{
    // Determine the end of the number
    end = line.find_first_not_of("-1234567890", start);
    // Isolate the number from rest of the line
    std::string varstring = line.substr(start, end-start);
    // Check if the number is negative and if so, erase the character -
    bool negative = false;
    if(varstring[0] == '-')
    {
        negative = true;
        varstring.erase(0,1);
    }

    // Add zeros based on the file settings
    while(varstring.size() < intigers + decimal) varstring.insert(0,"0");

    // Isolate intiger and decimal part
    double intigerPart = std::stod(varstring.substr(0, intigers));
    double decimalPart = std::stod(varstring.substr(intigers, decimal + intigers )) / pow(10, decimal);

    // Create result
    double result = (intigerPart + decimalPart) * (1 - 2*negative);
    std::cout << "Getting pos from " << varstring << " and returning " << result << std::endl;

    return  result;
}

///Returns double starting at given starting position, returns ending position
double GetDoubleFromPos(const std::string& line, std::size_t start, std::size_t& end)
{
    end = line.find_first_not_of("-1234567890.", start);
    return std::stod(line.substr(start, end-start));
}

///Returns double starting at given starting position
double GetDoubleFromPos(const std::string& line, std::size_t start)
{
    std::size_t end = line.find_first_not_of("-1234567890.", start);
    return std::stod(line.substr(start, end-start));
}

///Returns int starting at given starting position, returns ending position
int32_t GetIntFromPos(const std::string& line, std::size_t start, std::size_t& end) {
    end = line.find_first_not_of("-1234567890", start);
    return std::stoi(line.substr(start, end - start));
}

///Returns int starting at given starting position
int32_t GetIntFromPos(const std::string& line, std::size_t start)
{
    std::size_t end = line.find_first_not_of("-1234567890", start);
    return std::stoi(line.substr(start, end-start));
}

///Creates GerberFile from the given file
GerberFile Parser::fromFile(std::string path) {
    std::string line;
    std::ifstream f (path);

    if (!f.is_open())
        perror("error while opening file");

    //create new file object
    GerberFile file = GerberFile();

    //create parsing control variables
    bool receivedG75 = false;               //A G75 must be called before creating the first arc to preserve compatibility with older versions
    bool firstUnits = false;
    bool firstPosition = false;

    //Position and selected interpolation mode
    doubleVec2 currentPos{};
    InterpolationMode currentInterpolation;    //linear/circular mode

    //Aperture information state
    uint32_t currentAperture;
    Polarity currentPolarity = Dark;
    Mirroring currentMirroring = None;
    double currentRotation = 0;
    double currentScaling = 1;

    //Region and contour information
    bool regionMode = false;
    Contour currentContour;

    //start parsing line by line
    while(getline(f, line)) {
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        std::cout << "Parsing line " << line << std::endl;


        if(line.empty())
        {
            std::cout << "Line empty, ignoring." << std::endl;
        }
        else
        {
            file.totalLines += 1;
            if (line.rfind("G01", 0) == 0)         //Set mode to linear
            {
                currentInterpolation = linear;
                line.erase(0,3);
            }
            else if (line.rfind("G02", 0) == 0)         //set mode to clockwise circular
            {
                currentInterpolation = clockwise;
                line.erase(0,3);
            }
            else if (line.rfind("G03", 0) == 0)         //set mode to counterclockwise circular
            {
                currentInterpolation = counterclockwise;
                line.erase(0,3);
            }

            if(line.empty())
            {
                std::cout << "Line empty, ignoring." << std::endl;
            }
            else if (line.rfind("G04", 0) == 0)         //comment
            {
                file.comments.push_back(line.substr(4));
            }
            else if (line.rfind("G36", 0) == 0)         //start region statement
            {
                regionMode = true;
                currentContour = Contour();
                currentContour.polarity = currentPolarity;
            }
            else if (line.rfind("G37", 0) == 0)         //ends region statement
            {
                regionMode = false;
                currentContour.ProcessObjects();
                file.objects.emplace_back(currentContour);
            }
            else if (line.rfind("G75", 0) == 0)         //has to be called before first circular
            {
                receivedG75 = true;
            }
            else if (line[0] == 'D')                            //sets the current aparature
            {
                currentAperture = GetIntFromPos(line, 1);
                std::cout << "Aperture set to " << currentAperture << std::endl;
            }
            else if (line.rfind("%AD", 0) == 0)         //defines template-based aparature, assigns a D code to it
            {
                int32_t newApID;
                std::size_t currentIndex = 6;
                if(line[3] != 'D') perror("Parsing aparature failed");
                try {
                    newApID = GetIntFromPos(line,4,currentIndex);
                }
                catch(std::invalid_argument& e ){
                    std::cout << "Parsing aparature ID failed : " << e.what() << std::endl;
                }
                if((newApID < 10) | file.aperatures.find(newApID) != file.aperatures.end())
                {
                    perror("Parsing aparature failed, ID incorrect");
                }

                std::unique_ptr<Aperture> newAp;
                //determine type
                if(line[currentIndex] == 'C')                   //aparature is circle
                {
                    auto diam = GetDoubleFromPos(line, currentIndex + 2, currentIndex);
                    auto hole_diam = 0;
                    if(line[currentIndex] == 'X')
                    {
                        hole_diam = GetDoubleFromPos(line, currentIndex + 1, currentIndex);
                    }
                    Ap_Circle newCircle = Ap_Circle(newApID,diam,hole_diam);
                    auto ptr = std::make_shared<Ap_Circle>(newCircle);
                    file.aperatures.insert(std::pair<uint32_t,std::shared_ptr<Ap_Circle>>(newApID, ptr));
                }
                else if(line[currentIndex] == 'R')               //aparature is rectangle
                {
                    auto sizeX = GetDoubleFromPos(line, currentIndex + 2, currentIndex);
                    auto sizeY = GetDoubleFromPos(line, currentIndex + 1, currentIndex);
                    auto hole_diam = 0;
                    if(line[currentIndex] == 'X')
                    {
                        hole_diam = GetDoubleFromPos(line, currentIndex + 1, currentIndex);
                    }
                    Ap_Rect newRect = Ap_Rect(newApID,sizeX,sizeY,hole_diam);
                    auto ptr = std::make_shared<Ap_Rect>(newRect);
                    file.aperatures.insert(std::pair<uint32_t,std::shared_ptr<Ap_Rect>>(newApID, ptr));
                }
                else if(line[currentIndex] == 'O')                  //aparature is obround
                {
                    auto sizeX = GetDoubleFromPos(line, currentIndex + 2, currentIndex);
                    auto sizeY = GetDoubleFromPos(line, currentIndex + 1, currentIndex);
                    auto hole_diam = 0;
                    if(line[currentIndex] == 'X')
                    {
                        hole_diam = GetDoubleFromPos(line, currentIndex + 1, currentIndex);
                    }
                    Ap_Obround newObr = Ap_Obround(newApID,sizeX,sizeY,hole_diam);
                    auto ptr = std::make_shared<Ap_Obround>(newObr);
                    file.aperatures.insert(std::pair<uint32_t,std::shared_ptr<Ap_Obround>>(newApID, ptr));
                }
                else if(line[currentIndex] == 'P')                      //aparature is polygon
                {
                    auto diameter = GetDoubleFromPos(line, currentIndex + 2, currentIndex);
                    auto vertices = GetIntFromPos(line, currentIndex + 1, currentIndex);
                    auto rotation = 0;
                    auto hole_diam = 0;
                    if(line[currentIndex] == 'X')
                    {
                        rotation = GetDoubleFromPos(line, currentIndex + 1, currentIndex);
                    }
                    if(line[currentIndex] == 'X')
                    {
                        hole_diam = GetDoubleFromPos(line, currentIndex + 1, currentIndex);
                    }
                    Ap_Poly newPoly = Ap_Poly(newApID, diameter, rotation, hole_diam);
                    auto ptr = std::make_shared<Ap_Poly>(newPoly);
                    file.aperatures.insert(std::pair<uint32_t,std::shared_ptr<Ap_Poly>>(newApID, ptr));
                }
                else
                {
                    std::cout << "Parsing aparature failed: " << line << std::endl;
                }
            }
            else if (line.rfind("%MO", 0) == 0)         //sets the units
            {
                if(line[3] == 'M')                              //metric
                {
                    file.metricUnit = true;
                    firstUnits = true;
                }
                else if(line[3] == 'I')                         //imperial
                {
                    file.metricUnit = false;
                    firstUnits = true;
                }
                else
                {
                    perror("Parsing unit failed");
                }
            }
            else if (line.rfind("%FSLA", 0) == 0)       //sets the coordinate format - intiger and decimal places
            {
                file.intigerX = line[6] - '0';
                file.decimalX = line[7] - '0';
                file.intigerY = line[9] - '0';
                file.decimalY = line[10] - '0';
            }
            else if (line.rfind("%L", 0) == 0)          //sets the object transformation parmeters
            {
                switch (line[2]) {
                    case 'P':                                       // set polarity
                        if(line[3] == 'C') currentPolarity = Clear;
                        else if(line[3] == 'D') currentPolarity = Dark;
                        else perror("error while parsing polarity");
                        break;
                    case 'M':                                           // set mirroring
                        if(line[3] == 'X' && line[4] == 'Y') currentMirroring = XYaxis;
                        else if(line[3] == 'X') currentMirroring = Xaxis;
                        else if(line[3] == 'Y') currentMirroring = Yaxis;
                        else if(line[3] == 'N') currentMirroring = None;
                        else perror("error while parsing mirroring");
                        break;
                    case 'R':                                   // set rotation
                        try {
                            currentRotation = GetDoubleFromPos(line, 3);
                        }
                        catch(std::invalid_argument& e ){
                            std::cout << "Parsing rotation failed : " << e.what() << std::endl;
                        }
                    case 'S':                           //set scaling
                        try {
                            currentScaling = GetDoubleFromPos(line, 3);
                        }
                        catch(std::invalid_argument& e ){
                            std::cout << "Parsing scale failed : " << e.what() << std::endl;
                        }
                    default:
                        perror("error while parsing Aperture information state");
                }
            }
            else if (line.find("D01*", line.size() -4) == line.size()-4)        //interpolate operation
            {
                //Check if position is initialized
                if(!firstPosition)
                {
                    perror("error: inital position is not set");
                    break;
                }
                //Update position
                std::size_t currentIndex = 0;
                doubleVec2 tobePos = currentPos;
                if(line[currentIndex] == 'X')
                {
                    tobePos.x = GetDoublePosition(line, currentIndex+1, currentIndex, file.intigerX, file.decimalX);
                }
                if(line[currentIndex] == 'Y')
                {
                    tobePos.y = GetDoublePosition(line, currentIndex+1, currentIndex, file.intigerY, file.decimalY);
                }

                //Create shape based on current interpolation mode
                if(currentInterpolation == linear)
                {

                    std::cout <<"Creating line " << std::string(currentPos) << " " << std::string(tobePos) << std::endl;

                    if(regionMode)
                    {
                        LinearSegment newSegment = LinearSegment(currentPos, tobePos);
                        currentContour.objects.emplace_back(newSegment);
                    }
                    else
                    {
                        Line newLine = Line(currentPos, tobePos, currentPolarity, currentAperture);
                        file.objects.emplace_back(newLine);
                    }
                }
                else if(currentInterpolation == counterclockwise || currentInterpolation == clockwise)
                {
                    doubleVec2 offset{};
                    if(line[currentIndex] == 'I')
                    {
                        offset.x = GetDoublePosition(line, currentIndex + 1, currentIndex, file.intigerY, file.decimalY);
                    }
                    if(line[currentIndex] == 'J')
                    {
                        offset.y = GetDoublePosition(line, currentIndex + 1, currentIndex, file.intigerY, file.decimalY);
                    }
                    std::cout << "Creating arc " << std::string(currentPos) << " to " << std::string(tobePos) << " with offset " << std::string(offset) << std::endl;
                    if(regionMode)
                    {
                        CircularSegment newSegment = CircularSegment(currentPos, tobePos, offset, (currentInterpolation == clockwise));
                        currentContour.objects.emplace_back(newSegment);
                    }
                    else
                    {
                        Arc newArc = Arc(currentPos, tobePos, offset, currentPolarity, currentAperture);
                        file.objects.emplace_back(newArc);
                    }
                }
                currentPos = tobePos;
            }
            else if(line.find("D02*", line.size() -4) == line.size()-4)         //Move operation
            {
                std::size_t currentIndex = 0;
                firstPosition = true;

                if(line[currentIndex] == 'X')
                {
                    currentPos.x = GetDoublePosition(line, currentIndex+1, currentIndex, file.intigerX, file.decimalX);
                }
                if(line[currentIndex] == 'Y')
                {
                    currentPos.y = GetDoublePosition(line, currentIndex+1, currentIndex, file.intigerY, file.decimalY);
                }

                if(regionMode)
                {
                    currentContour.ProcessObjects();
                    file.objects.emplace_back(currentContour);
                    currentContour = Contour();
                    currentContour.polarity = currentPolarity;
                }
                std::cout <<"Moving pos " << std::string(currentPos) << std::endl;
            }
            else if(line.find("D03*", line.size() -4) == line.size()-4)         //Flash operation
            {
                // Update position
                std::size_t currentIndex = 0;
                firstPosition = true;
                if(line[currentIndex] == 'X')
                {
                    currentPos.x = GetDoublePosition(line, currentIndex+1, currentIndex, file.intigerX, file.decimalX);
                }
                if(line[currentIndex] == 'Y')
                {
                    currentPos.y = GetDoublePosition(line, currentIndex+1, currentIndex, file.intigerY, file.decimalY);
                }
                //Create flash
                Flash newFlash = Flash(currentPos, currentAperture, currentPolarity, currentRotation, currentScaling);
                file.objects.emplace_back(newFlash);
            }
            else
            {
                if(line.length() > 1)       // if contains more than one character, add to unrecognized lines
                {
                    file.unrecognizedLines+=1;
                }
                std::cout << "Could not recognize line: " << line << std::endl;
            }
        }

    }
    if (f.bad())
        perror("error while reading file");

    const size_t slashpos = path.find_last_of("\\/");
    if (std::string::npos != slashpos)
    {
        path.erase(0, slashpos + 1);
    }
    file.filename = path;

    return file;
}