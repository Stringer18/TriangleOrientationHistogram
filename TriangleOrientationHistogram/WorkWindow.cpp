#include "WorkWindow.h"


// ----------------------------------------------------------------------------
WorkWindow::WorkWindow()
{
    getRectFromIni( m_sdlWindowPlace, "workWindow" );
    getDataFromIni( &m_iFontSize, "font", "iSize", 12 );
    getDataFromIni( m_strFontFile, "font", "strFont",
            ".\\Resources\\OldStandardTTItalic.ttf" );
    getColorFromIni( m_sdlFontColor, "font" );
    m_fIsSuccess = initSdlComponents();
}


// ----------------------------------------------------------------------------
WorkWindow::~WorkWindow()
{
    if( m_psdlRenderer != nullptr ) { SDL_DestroyRenderer( m_psdlRenderer ); }
	if( m_psdlWorkWindow != nullptr ) { SDL_DestroyWindow( m_psdlWorkWindow ); }
    TTF_Quit();
    SDL_Quit();
}


// ----------------------------------------------------------------------------
bool WorkWindow::initSdlComponents()
{
    // SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        setToLog( std::string( "SDL_Init Error: ") + SDL_GetError() );
        return false;
    }

    // Window
    std::string strWindowName = "";
    getDataFromIni( strWindowName, "workWindow", "name", "-----" );
    m_psdlWorkWindow = SDL_CreateWindow( strWindowName.c_str(),
            m_sdlWindowPlace.x, m_sdlWindowPlace.y,
            m_sdlWindowPlace.w, m_sdlWindowPlace.h, SDL_WINDOW_SHOWN );
    if( m_psdlWorkWindow == nullptr )
    {
        setToLog( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError() );
        return false;
    }

    // Renderer
    m_psdlRenderer = SDL_CreateRenderer( m_psdlWorkWindow, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( m_psdlRenderer == nullptr )
    {
        setToLog( std::string( "SDL_CreateRenderer Error: " ) +
                SDL_GetError() );
        return false;
    }

    // Font
    if( TTF_Init() != 0 )
    {
        setToLog( std::string( "TTF_Init Error: " ) + SDL_GetError() );
        return false;
    }


    return true;
}


// ----------------------------------------------------------------------------
bool WorkWindow::strTextToSdlTexture( SDL_Texture*& psdlResultTexture,
        int &iWidthTexture, int &iHeightTexture, std::string strMessage )
{
    if( psdlResultTexture != nullptr )
    {
        setToLog( "Texture is not empty." );
        return false;
    }

    TTF_Font *pFont = TTF_OpenFont( m_strFontFile.c_str(), m_iFontSize );
    if( pFont == nullptr )
    {
        setToLog( std::string( "TTF_OpenFont Error: " ) + SDL_GetError() );
        return false;
    }

    SDL_Surface *sdlBuffSurface = TTF_RenderText_Blended( pFont,
            strMessage.c_str(), m_sdlFontColor );
    if( sdlBuffSurface == nullptr )
    {
        TTF_CloseFont( pFont );
        setToLog( std::string( "TTF_RenderText Error: " ) + SDL_GetError() );
        return false;
    }
    psdlResultTexture = SDL_CreateTextureFromSurface( m_psdlRenderer,
            sdlBuffSurface );
    if( psdlResultTexture == nullptr )
    {
        TTF_CloseFont( pFont );
        setToLog( std::string( "CreateTexture Error: " ) + SDL_GetError() );
        return false;
    }

    SDL_QueryTexture( psdlResultTexture, NULL, NULL, &iWidthTexture,
            &iHeightTexture);

    SDL_FreeSurface( sdlBuffSurface );
    TTF_CloseFont( pFont );
    return true;
}


// ----------------------------------------------------------------------------
bool WorkWindow::intToUint8( Uint8 *uint8Result, int iNumber )
{
    if( uint8Result == nullptr )
    {
        setToLog( "Error: Empty pointer Uint8 *uint8Result." );
        return false;
    }
    if( iNumber < 0 )
    {
        *uint8Result = 0;
        return false;
    }
    if( iNumber > 255 )
    {
        *uint8Result = 255;
        return false;
    }
    *uint8Result = (Uint8) iNumber;
    return true;
}


// ----------------------------------------------------------------------------
void WorkWindow::getRectFromIni( SDL_Rect &sdlRect, char *szGroup,
                SDL_Rect sdlDefaultRect /*= { 0, 0, 0, 0 }*/ )
{
    getDataFromIni( &(sdlRect.x), szGroup, "iPositionX", sdlDefaultRect.x );
    getDataFromIni( &(sdlRect.y), szGroup, "iPositionY", sdlDefaultRect.y );
    getDataFromIni( &(sdlRect.w), szGroup, "iWidth", sdlDefaultRect.w );
    getDataFromIni( &(sdlRect.h), szGroup, "iHeight", sdlDefaultRect.h );
}


// ----------------------------------------------------------------------------
void WorkWindow::getColorFromIni( SDL_Color &sdlColor, char *szGroup,
                SDL_Color sdlDefaultColor /*= { 255, 255, 255, 255 }*/ )
{
    int iBuff = 0;

    getDataFromIni( &iBuff, szGroup, "isdlColorR", sdlDefaultColor.r );
    if( !( intToUint8( &(sdlColor.r), iBuff ) ) )
    {
        setToLog( toString( "Invalid value for " ) + szGroup +
                " isdlColorR. Rounded." );
    }

    getDataFromIni( &iBuff, szGroup, "isdlColorG", sdlDefaultColor.g );
    if( !( intToUint8( &(sdlColor.g), iBuff ) ) )
    {
        setToLog( toString( "Invalid value for " ) + szGroup +
                " isdlColorG. Rounded." );
    }

    getDataFromIni( &iBuff, szGroup, "isdlColorB", sdlDefaultColor.b );
    if( !( intToUint8( &(sdlColor.b), iBuff ) ) )
    {
        setToLog( toString( "Invalid value for " ) + szGroup +
                " isdlColorB. Rounded." );
    }

    getDataFromIni( &iBuff, szGroup, "isdlColorA", sdlDefaultColor.a );
    if( !( intToUint8( &(sdlColor.a), iBuff ) ) )
    {
        setToLog( toString( "Invalid value for " ) + szGroup +
                " isdlColorA. Rounded." );
    }
}


// ----------------------------------------------------------------------------
void WorkWindow::displayHistogram( std::map<double, double> &mapHistogram )
{
    SDL_RenderClear( m_psdlRenderer );

    // ------------------------------------------------------------------------
    SDL_Rect sdlBuffRect;
    SDL_Color sdlBuffColor;
    int iBuff;

    // ------------------------------------------------------------------------
    // Background
    sdlBuffRect = { 0, 0, m_sdlWindowPlace.w, m_sdlWindowPlace.h };
    getColorFromIni( sdlBuffColor, "background" );
    SDL_SetRenderDrawColor( m_psdlRenderer, sdlBuffColor.r, sdlBuffColor.g,
            sdlBuffColor.b, sdlBuffColor.a );
    SDL_RenderFillRect( m_psdlRenderer, &sdlBuffRect );

    // ------------------------------------------------------------------------
    // Histogram cadre
    SDL_Rect sdlCadreRect;
    int iElelmentW;

    getDataFromIni( &sdlCadreRect.x, "histogramCadre", "iPositionX", 0 );
    getDataFromIni( &sdlCadreRect.y, "histogramCadre", "iPositionY", 0 );
    getDataFromIni( &iBuff, "histogramCadre", "iDownField", 0 );
    sdlCadreRect.h = m_sdlWindowPlace.h - sdlCadreRect.y - iBuff;

    getDataFromIni( &iBuff, "histogramCadre", "iRightMinimalField", 0 );
    iElelmentW = (int) floor( ( (double) ( m_sdlWindowPlace.w -
            sdlCadreRect.x - iBuff ) ) / mapHistogram.size() );
    if ( iElelmentW < 1 ) { iElelmentW = 1; }

    sdlCadreRect.w = mapHistogram.size() * iElelmentW;

    getColorFromIni( sdlBuffColor, "histogramCadre" );

    SDL_SetRenderDrawColor( m_psdlRenderer, sdlBuffColor.r, sdlBuffColor.g,
            sdlBuffColor.b, sdlBuffColor.a );
    SDL_RenderDrawRect( m_psdlRenderer, &sdlCadreRect );

    // ------------------------------------------------------------------------
    // Scales divisions and values
    double dblMaxValueElement = 0.0;

    for( std::map<double, double>::iterator it = mapHistogram.begin() ; it !=
            mapHistogram.end(); it++)
    {
        if( dblMaxValueElement < it->second )
        {
            dblMaxValueElement = it->second;
        }
    }
    dblMaxValueElement = ( dblMaxValueElement < 10.0 ?
            ceil( dblMaxValueElement ) :
            ( ceil( dblMaxValueElement / 10.0 ) * 10.0 ) );

    SDL_Texture* psdlBuffTexture = nullptr;
    std::vector<SDL_Texture*> vecPSdlTextTextures;
    int iScaleValueIndent;
    
    getDataFromIni( &iBuff, "histogramCadre", "iScaleDivisionLen", 0 );

    //On vertical scale
    getDataFromIni( &iScaleValueIndent, "histogramCadre", "iScaleValueIndentX",
            0 );
    for( int i = 0 ; i < 11 ; i++ )
    {
        int iScaleY = sdlCadreRect.y + (int) round( sdlCadreRect.h * i / 10.0);

        strTextToSdlTexture( psdlBuffTexture, sdlBuffRect.w, sdlBuffRect.h,
                toString( dblMaxValueElement * ( 10 - i ) / 10.0 ) );
        vecPSdlTextTextures.push_back( psdlBuffTexture );
        sdlBuffRect.x = sdlCadreRect.x - sdlBuffRect.w - iScaleValueIndent;
        sdlBuffRect.y = iScaleY - sdlBuffRect.h / 2;
        SDL_RenderCopy( m_psdlRenderer, psdlBuffTexture, NULL, &sdlBuffRect );
        psdlBuffTexture = nullptr;

        SDL_RenderDrawLine( m_psdlRenderer, sdlCadreRect.x, iScaleY,
                sdlCadreRect.x + iBuff, iScaleY );
        SDL_RenderDrawLine( m_psdlRenderer, sdlCadreRect.x + sdlCadreRect.w -
                iBuff, iScaleY, sdlCadreRect.x + sdlCadreRect.w - 1, iScaleY );
    }

    // On horizontal scale
    getDataFromIni( &iScaleValueIndent, "histogramCadre", "iScaleValueIndentY",
            0 );
    sdlBuffRect.y = sdlCadreRect.y + sdlCadreRect.h + iScaleValueIndent;
    for( int i = 0 ; i < 5 ; i++ )
    {
        int iScaleX = sdlCadreRect.x + (int) round( sdlCadreRect.w * i / 4.0 );

        strTextToSdlTexture( psdlBuffTexture, sdlBuffRect.w, sdlBuffRect.h,
                toString( mapHistogram.begin()->first + i * (
                mapHistogram.rbegin()->first - mapHistogram.begin()->first ) /
                4.0 ) );
        vecPSdlTextTextures.push_back( psdlBuffTexture );
        sdlBuffRect.x = iScaleX - sdlBuffRect.w / 2;
        SDL_RenderCopy( m_psdlRenderer, psdlBuffTexture, NULL, &sdlBuffRect );
        psdlBuffTexture = nullptr;

        SDL_RenderDrawLine( m_psdlRenderer, iScaleX, sdlCadreRect.y, iScaleX,
                sdlCadreRect.y + iBuff );
        SDL_RenderDrawLine( m_psdlRenderer, iScaleX,
                sdlCadreRect.y + sdlCadreRect.h - iBuff, iScaleX,
                sdlCadreRect.y + sdlCadreRect.h - 1 );
    }

    // ------------------------------------------------------------------------
    // Help text for exit
    sdlBuffRect.x = sdlCadreRect.x;
    sdlBuffRect.y = sdlBuffRect.y + sdlBuffRect.h + iScaleValueIndent;
    strTextToSdlTexture( psdlBuffTexture, sdlBuffRect.w, sdlBuffRect.h,
            "Press any keyboard key or mouse button to close the window." );
    vecPSdlTextTextures.push_back( psdlBuffTexture );
    SDL_RenderCopy( m_psdlRenderer, psdlBuffTexture, NULL, &sdlBuffRect );
    psdlBuffTexture = nullptr;

    // ------------------------------------------------------------------------
    // Elements of histogram
    SDL_Color sdlElementFillColor;
    SDL_Color sdlElementOutlineColor;

    getColorFromIni( sdlElementFillColor, "elementFill" );
    getColorFromIni( sdlElementOutlineColor, "elementOutline" );
    sdlBuffRect.w = iElelmentW;

    int i = 0;
    for( std::map<double, double>::iterator it = mapHistogram.begin() ; it !=
            mapHistogram.end() ; it++ )
    {
        sdlBuffRect.x = sdlCadreRect.x + i * iElelmentW;
        i++;
        if( it->second == 0.0 ) { continue; }
        sdlBuffRect.h = (int) round( sdlCadreRect.h * it->second /
                dblMaxValueElement );
        sdlBuffRect.y = sdlCadreRect.y + sdlCadreRect.h - sdlBuffRect.h;
        if( sdlBuffRect.h == 0 ) { continue; }

        SDL_SetRenderDrawColor( m_psdlRenderer,
                sdlElementFillColor.r, sdlElementFillColor.g,
                sdlElementFillColor.b, sdlElementFillColor.a );
        SDL_RenderFillRect( m_psdlRenderer, &sdlBuffRect );

        SDL_SetRenderDrawColor( m_psdlRenderer,
                sdlElementOutlineColor.r, sdlElementOutlineColor.g,
                sdlElementOutlineColor.b, sdlElementOutlineColor.a );
        SDL_RenderDrawRect( m_psdlRenderer, &sdlBuffRect );
    }

    // ------------------------------------------------------------------------
    // Display
    SDL_RenderPresent( m_psdlRenderer );

    // ------------------------------------------------------------------------
    // Wait push, for people can see
    SDL_Event sdlEvent;
    while( m_fIsSuccess )
    {
        SDL_WaitEvent( &sdlEvent );
        switch( sdlEvent.type )
        {
            case SDL_QUIT:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_KEYDOWN:
            {
                m_fIsSuccess = false;
                break;
            }
        }
    }

    // ------------------------------------------------------------------------
    // Clear text
    for( SDL_Texture* i : vecPSdlTextTextures ) { SDL_DestroyTexture( i ); }
    vecPSdlTextTextures.clear();
}