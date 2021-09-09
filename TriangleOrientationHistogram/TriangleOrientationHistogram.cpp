#include "TriangleOrientationHistogram.h"

int main(int argc, char *argv[])
{
    setToLog( "Starting." );

    // ------------------------------------------------------------------------
    bool fReadStlFile;
    bool fWriteTxtFile;
    bool fShowHistogram;

    printf( "\nIt is a program for calculating and displaying a histogram " );
    printf( "of triangle orientation.\n" );

    dialogWhatToDo( fReadStlFile, fWriteTxtFile, fShowHistogram );

    // ------------------------------------------------------------------------
    std::string strStlFile = "";
    std::string strTxtFile = "";
    std::map<double, double> mapHistogram;    // <angle in degrees, sum area>
    DWORD ulError = 0;

    prepareFilenames( strStlFile, strTxtFile );

    // ------------------------------------------------------------------------
    if( fReadStlFile )
    {
        for( int i = 0 ; i < _HISTOGRAM_SIZE_ ; i++ )
        {
            mapHistogram[ -90.0 + i * 180.0 / ( _HISTOGRAM_SIZE_ - 1 ) ] = 0.0;
        }
        ulError = readStlFile( mapHistogram, strStlFile );
    }
    else
    {
        ulError = readTxtFile( mapHistogram, strTxtFile );
    }

    // ------------------------------------------------------------------------
    if( ulError == 0 )
    {
        if( fWriteTxtFile )
        {
            ulError = writeOutputFile( mapHistogram, strTxtFile );
        }
        
        if( fShowHistogram )
        {
            WorkWindow *workWindow = new WorkWindow();
            workWindow->displayHistogram( mapHistogram );
            delete workWindow;
        }
    }

    // ------------------------------------------------------------------------
    mapHistogram.clear();

    if( ulError == 0 )
    {
        setToLog( "Exit." );
    }
    else
    {
        setToLog( "Exit error " + toString(ulError) );
        getchar(); getchar();
    }

    return ulError;
}


// ----------------------------------------------------------------------------
void dialogWhatToDo( bool &fReadStlFile, bool &fWriteTxtFile,
        bool &fShowHistogram )
{
    printf( "Do you want to calculate a new histogram?\n" );
    printf( "\tY\tYes, new calculate (data from stl-file).\n" );
    printf( "\tN\tNo, open old result (data from txt-file).\n" );
    fReadStlFile = boolQuestion( "Choose your option, please" );

    if( fReadStlFile )
    {
        fWriteTxtFile = boolQuestion( toString( "Do you want to write" ) +
                " the result to the file?" );
        if( fWriteTxtFile )
        {
            fShowHistogram = boolQuestion( toString( "Do you want to" ) +
                " show histogram?" );;
        }
        else
        {
            fShowHistogram = true;
        }
    }
    else
    {
        fWriteTxtFile = false;
        fShowHistogram = true;
    }

    setToLog( toString( "Selected answer - " ) +
            ( fReadStlFile ? "open stl-file, " : "open txt-file, " ) +
            ( fWriteTxtFile ? "" : "don\'t " ) + "write result to txt-file, " +
            ( fShowHistogram ? "" : "don\'t " ) + "show histogram." );
}


// ----------------------------------------------------------------------------
bool boolQuestion( std::string strQuestion )
{
    char chAnswer[_MAX_STR_BUFF_SIZE_];
    printf( "%s [Y/N] ? ", strQuestion.c_str() );
    do
    {
        scanf_s( " %s", &chAnswer, _MAX_STR_BUFF_SIZE_ );
        switch( chAnswer[0] )
        {
            case 'y':
            case 'Y':
            {
                return true; // break;
            }
            case 'n':
            case 'N':
            {
                return false; // break;
            }
            default:
            {
                printf( "Unknown answer. Choose option Y or N, please: " );
                break;
            }
        }
    }
    while( true );
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
DWORD readStlFile( std::map<double, double> &mapHistogram,
        std::string &strInputFile )
{
    FILE *fpInput;
    if( fopen_s( &fpInput, strInputFile.c_str(), "r") != 0 )
    {
        DWORD ulError = GetLastError();
        setToLog( "Error " + toString(ulError) + ". Cannot open file " +
                strInputFile );
        return ulError;
    }
    else
    {
        setToLog( "Opened file " + strInputFile );

        // --------------------------------------------------------------------
        char szBuff[_MAX_STR_BUFF_SIZE_];
        
        double dblarrNormal[_SPACE_DIMENSION_];
        double dblarrPointA[_SPACE_DIMENSION_];
        double dblarrPointB[_SPACE_DIMENSION_];
        double dblarrPointC[_SPACE_DIMENSION_];

        // --------------------------------------------------------------------
        setToLog( "Reading and calculate data from file " + strInputFile );

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
DWORD readTxtFile( std::map<double, double> &mapHistogram,
        std::string &strInputFile )
{
    FILE *fpInput;
    if( fopen_s( &fpInput, strInputFile.c_str(), "r") != 0 )
    {
        DWORD ulError = GetLastError();
        setToLog( "Error " + toString(ulError) + ". Cannot open file " +
                strInputFile );
        return ulError;
    }
    else
    {
        setToLog( "Opened file " + strInputFile );

        char szBuff[_MAX_STR_BUFF_SIZE_];
        double dblAngle;
        double dblArea;
        
        setToLog( "Reading data from file " + strInputFile );

        // First line with headers - skip.
        fgets( szBuff, _MAX_STR_BUFF_SIZE_, fpInput );

        while( !( feof( fpInput ) ) )
        {
            fscanf_s( fpInput, "%le %le", &dblAngle, &dblArea );
            mapHistogram[dblAngle] = dblArea;
        }

        fclose( fpInput );
    }
    return 0;
}


// ----------------------------------------------------------------------------
void processingData( std::map<double, double> &mapHistogram,
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

    double dblAngle = ( asin( lenLine( dblarrNormal, dblarrNormalP ) /
            lenLine( dblarrNull, dblarrNormal ) ) * 180 / M_PI ) *
            ( dblarrNormal[_SPACE_DIMENSION_ - 1] < 0 ? -1 : 1 );
    double dblCoercionAngle = mapHistogram.begin()->first;
    for( std::map<double, double>::iterator it = ++mapHistogram.begin() ;
            it != mapHistogram.end(); it++)
    {
        if( abs( it->first - dblAngle ) < abs ( dblCoercionAngle - dblAngle ) )
        {
            dblCoercionAngle = it->first;
        }
        else
        {
            if( ( abs( it->first - dblAngle ) ==
                    abs ( dblCoercionAngle - dblAngle ) ) &&
                    ( abs( it->first ) < abs( dblCoercionAngle ) ) )
            {
                dblCoercionAngle = it->first;
            }
            break;
        }
    }

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
    mapHistogram[dblCoercionAngle] += dblArea;
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
DWORD writeOutputFile( std::map<double, double> &mapHistogram,
        std::string &strOutputFile )
{
    FILE *fpOutput;
    if( fopen_s( &fpOutput, strOutputFile.c_str() , "w") != 0 )
    {
        DWORD ulError = GetLastError();
        setToLog( "Error " + toString(ulError) + ". Cannot open file " +
                strOutputFile );
        return ulError;
    }
    else
    {
        setToLog( "Opened file " + strOutputFile );

        // --------------------------------------------------------------------
        fprintf( fpOutput, "Angle_in_degrees Sum_area\n" );
        for( std::map<double, double>::iterator it = mapHistogram.begin() ;
                it != mapHistogram.end(); it++)
        {
            fprintf( fpOutput, "%le %le\n", it->first, it->second );
        }
        fclose( fpOutput );
        setToLog( "Histogram saved to output file.");
    }
    return 0;

}


// ----------------------------------------------------------------------------