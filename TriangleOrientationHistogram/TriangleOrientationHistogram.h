#pragma once

#include <map>
#include <cstring>

#define  _USE_MATH_DEFINES
#include <math.h>

#include "HelpFunctions.hpp"
#include "WorkWindow.h"


#define     _HISTOGRAM_SIZE_	201
#define     _SPACE_DIMENSION_   3


// ----------------------------------------------------------------------------
// Asks for the filename and generate string paths for further work.
void prepareFilenames(  std::string &strInputFile,
                        std::string &strOutputFile );


// ----------------------------------------------------------------------------
// Read data from the input file (block by block) and calls the function for
// process the received data.
DWORD readInputFile( std::map<int, double> &mapHistogram,
        std::string &strInputFile );


// ----------------------------------------------------------------------------
// Input the block of data, calculate it and add the result to the histogram.
void processingData( std::map<int, double> &mapHistogram,
        double dblarrNormal[], double dblarrPointA[], double dblarrPointB[],
        double dblarrPointC[] );


// ----------------------------------------------------------------------------
// Calculate length between two points.
double lenLine( double dblarrPointA[], double dblarrPointB[] );


// ----------------------------------------------------------------------------
// Write data to the output file in format "Angle_in_degrees Sum_area".
// The first line contains the column headings.
DWORD writeOutputFile( std::map<int, double> &mapHistogram,
        std::string &strOutputFile );


// ----------------------------------------------------------------------------