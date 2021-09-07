#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

#include "HelpFunctions.hpp"


// ----------------------------------------------------------------------------
class WorkWindow
{
    // ------------------------------------------------------------------------
    public:
        WorkWindow();
        ~WorkWindow();


        // --------------------------------------------------------------------
        // Turns message into texture (sdlResultTexture). It return true if 
        // everything is successful.
        // If you give a pointer to a non-nullptr texture,
        // then nothing will happen and the function will return false. 
        bool strTextToSdlTexture( SDL_Texture*& psdlResultTexture,
                int &iWidthTexture, int &iHeightTexture,
                std::string strMessage );


        // --------------------------------------------------------------------
        // Function for checking and correct conversion of int to uint8.
        bool intToUint8( Uint8 *uint8Result, int iNumber );


        // --------------------------------------------------------------------
        // Download group of data from ini-file,
        // which is responsible for the position and size.
        void getRectFromIni( SDL_Rect &sdlRect, char *szGroup,
                SDL_Rect sdlDefaultRect = { 0, 0, 0, 0 } );


        // --------------------------------------------------------------------
        // Download group of data from ini-file,
        // which is responsible for the color.
        void getColorFromIni( SDL_Color &sdlColor, char *szGroup,
                SDL_Color sdlDefaultColor = { 255, 255, 255, 255 } );


        // --------------------------------------------------------------------
        // Creates the contents of the window and draws a histogram in it.
        void displayHistogram( std::map<int, double> &mapHistogram );


        // --------------------------------------------------------------------
        // This flag is in case an error occurs while creating an object.
        // If it false, we correctly break all and destroy objects, which were
        // created earlier.
        bool m_fIsSuccess;


    // ------------------------------------------------------------------------
    protected:
        SDL_Window* m_psdlWorkWindow;
        SDL_Renderer* m_psdlRenderer;

        SDL_Rect m_sdlWindowPlace;

        int m_iFontSize;
        SDL_Color m_sdlFontColor;
        std::string m_strFontFile;

    // ------------------------------------------------------------------------
    private:
        // Initializing SDL (in general), image, window, render, font.
        bool initSdlComponents();
};