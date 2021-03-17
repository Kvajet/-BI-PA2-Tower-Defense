#include "CRenderer.h"

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

CRenderer::CRenderer( const int & verticalPos , const int & errorState )
    : m_verticalPos( verticalPos ) , m_errorState( errorState )
{
}

void CRenderer::RefreshWinSize() const
{
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &m_window );
}

void CRenderer::PrintMargin( int contentSize ) const
{
    RefreshWinSize();
    int len = ( m_window.ws_col - contentSize ) / 2;
    for( int i = 0 ; i < len ; i++ )
        std::cout << " ";
}

void CRenderer::PrintError( const char * error ) const
{
    std::cout << std::endl;
    PrintMargin( strlen( error ) );
    std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_RED << "m"
              << error
              << CONSOLE_TEXT_POSTFIX << std::endl;
}
