#include "TriangleOrientationHistogram.h"

int main(int argc, char *argv[])
{
    setToLog( "Starting." );

    // ------------------------------------------------------------------------
    std::string strInputFile = "";
    std::string strOutputFile = "";

    std::map<int, double> mapHistogram;    // <angle in degrees, sum area>
    for( int i = - ( _HISTOGRAM_SIZE_ - 1 ) / 2 ; i <=
            ( _HISTOGRAM_SIZE_ - 1 ) / 2 ; i++ ) { mapHistogram[i] = 0.0; }

    // ------------------------------------------------------------------------
    prepareFilenames( strInputFile, strOutputFile );
    DWORD ulError = readInputFile( mapHistogram, strInputFile );
    if( ulError == 0 )
    {
        ulError = writeOutputFile( mapHistogram, strOutputFile );
    }

    // ------------------------------------------------------------------------
    WorkWindow *workWindow = new WorkWindow();
    workWindow->displayHistogram( mapHistogram );

    delete workWindow;
    
    // ------------------------------------------------------------------------
    mapHistogram.clear();
    //getchar();  getchar();

    if( ulError == 0 ) { setToLog( "Exit." ); }
    else { setToLog( "Exit error " + toString(ulError) ); }

    return ulError;
}


// ----------------------------------------------------------------------------
void prepareFilenames(  std::string &strInputFile,
                        std::string &strOutputFile )
{
    char szBuff[_MAX_STR_BUFF_SIZE_];
    for( int i = 0 ; i < _MAX_STR_BUFF_SIZE_ ; i++ ) { szBuff[i] = '\0'; }

    // ------------------------------------------------------------------------

    printf( "Please enter a filename (without file extension) :\n" );
    scanf_s( "%s", &szBuff, _MAX_STR_BUFF_SIZE_ );
    getDataFromIni( strInputFile, "workFolder", "input", ".\\Input" );
    getDataFromIni( strOutputFile, "workFolder", "output", ".\\Output" );
    strInputFile = strInputFile + "\\" + szBuff + ".stl";
    strOutputFile = strOutputFile + "\\" + szBuff + ".txt";
}


// ----------------------------------------------------------------------------
DWORD readInputFile( std::map<int, double> &mapHistogram,
        std::string &strInputFile )
{
    FILE *fpInput;
    if( fopen_s( &fpInput, strInputFile.c_str(), "r") != 0 )
    {
        DWORD ulError = GetLastError();
        setToLog( "Error " + toString(ulError) + ". Cannot open input file " +
                strInputFile );
        return ulError;
    }
    else
    {
        setToLog( "Opened input file " + strInputFile );

        // --------------------------------------------------------------------
        char szBuff[_MAX_STR_BUFF_SIZE_];
        for( int i = 0 ; i < _MAX_STR_BUFF_SIZE_ ; i++ ) { szBuff[i] = '\0'; }
        
        double dblarrNormal[_SPACE_DIMENSION_];
        double dblarrPointA[_SPACE_DIMENSION_];
        double dblarrPointB[_SPACE_DIMENSION_];
        double dblarrPointC[_SPACE_DIMENSION_];

        // --------------------------------------------------------------------

        // Line "solid STL generated by MeshLab" - skip.
        fgets( szBuff, _MAX_STR_BUFF_SIZE_, fpInput );

        while( !( feof( fpInput ) ) )
        {
            fscanf_s( fpInput, "%s", szBuff, _MAX_STR_BUFF_SIZE_ );
            if( strcmp( szBuff, "facet" ) != 0 ) { break; }

            // Read normal.
            fscanf_s( fpInput, "%s %le %le %le ", szBuff, _MAX_STR_BUFF_SIZE_,
                    &dblarrNormal[0], &dblarrNormal[1], &dblarrNormal[2] );

            // Line "outer loop" - skip.
            fgets( szBuff, _MAX_STR_BUFF_SIZE_, fpInput );

            // Read points A, B and C.
            fscanf_s( fpInput, "%s %le %le %le ", szBuff, _MAX_STR_BUFF_SIZE_,
                    &dblarrPointA[0], &dblarrPointA[1], &dblarrPointA[2] );
            fscanf_s( fpInput, "%s %le %le %le ", szBuff, _MAX_STR_BUFF_SIZE_,
                    &dblarrPointB[0], &dblarrPointB[1], &dblarrPointB[2] );
            fscanf_s( fpInput, "%s %le %le %le ", szBuff, _MAX_STR_BUFF_SIZE_,
                    &dblarrPointC[0], &dblarrPointC[1], &dblarrPointC[2] );

            // Lines "endloop" and "endfacet" - skip.
            fgets( szBuff, _MAX_STR_BUFF_SIZE_, fpInput );
            fgets( szBuff, _MAX_STR_BUFF_SIZE_, fpInput );

            processingData( mapHistogram, dblarrNormal,  dblarrPointA,
                    dblarrPointB, dblarrPointC );
        }

        fclose( fpInput );
    }
    return 0;
}


// ----------------------------------------------------------------------------
void processingData( std::map<int, double> &mapHistogram,
        double dblarrNormal[], double dblarrPointA[], double dblarrPointB[],
        double dblarrPointC[] )
{
    // ------------------------------------------------------------------------
    // Find angle between normal and plane XY.
    double dblarrNull[_SPACE_DIMENSION_];
    double dblarrNormalP[_SPACE_DIMENSION_];
    for( int i = 0 ; i < _SPACE_DIMENSION_ - 1 ; i++ )
    {
        dblarrNull[i] = 0.0;
        dblarrNormalP[i] = dblarrNormal[i];
    }
    dblarrNull[_SPACE_DIMENSION_ - 1] = 0.0;
    dblarrNormalP[_SPACE_DIMENSION_ - 1] = 0.0;

    int iAngle = (int) round( asin( lenLine( dblarrNormal, dblarrNormalP ) /
            lenLine( dblarrNull, dblarrNormal ) ) * 180 / M_PI ) *
            ( dblarrNormal[_SPACE_DIMENSION_ - 1] < 0 ? -1 : 1 );

    // ------------------------------------------------------------------------
    // Find area of a triangle (Heron's formula) .
    double dblLenLineAB = lenLine( dblarrPointA, dblarrPointB );
    double dblLenLineAC = lenLine( dblarrPointA, dblarrPointC );
    double dblLenLineBC = lenLine( dblarrPointB, dblarrPointC );
    double dblHalfPerimeter = ( dblLenLineAB + dblLenLineAC + dblLenLineBC )/2;
    double dblArea = sqrt( ( dblHalfPerimeter - dblLenLineAB ) *
            ( dblHalfPerimeter - dblLenLineAC ) *
            ( dblHalfPerimeter - dblLenLineBC ) * dblHalfPerimeter );
    
    // ------------------------------------------------------------------------
    // Add result.
    mapHistogram[iAngle] += dblArea;
}


// ----------------------------------------------------------------------------
double lenLine( double dblarrPointA[], double dblarrPointB[] )
{
    double dblLenAB = 0.0;
    for( int i = 0 ; i < _SPACE_DIMENSION_ ; i++ )
    {
        dblLenAB += ( dblarrPointB[i] - dblarrPointA[i] ) *
                ( dblarrPointB[i] - dblarrPointA[i] );
    }
    return sqrt( dblLenAB );
}


// ----------------------------------------------------------------------------
DWORD writeOutputFile( std::map<int, double> &mapHistogram,
        std::string &strOutputFile )
{
    FILE *fpOutput;
    if( fopen_s( &fpOutput, strOutputFile.c_str() , "w") != 0 )
    {
        DWORD ulError = GetLastError();
        setToLog( "Error " + toString(ulError) + ". Cannot open output file " +
                strOutputFile );
        return ulError;
    }
    else
    {
        setToLog( "Opened output file " + strOutputFile );

        // --------------------------------------------------------------------
        fprintf( fpOutput, "Angle_in_degrees Sum_area\n" );
        for( std::map<int, double>::iterator it = mapHistogram.begin() ; it !=
                mapHistogram.end(); it++)
        {
            fprintf( fpOutput, "%i %le\n", it->first, it->second );
        }
        fclose( fpOutput );
        setToLog( "Histogram saved to output file.");
    }
    return 0;

}


// ----------------------------------------------------------------------------