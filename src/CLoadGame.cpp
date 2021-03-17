#include "CLoadGame.h"

#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <memory>

CLoadGame::CLoadGame( const int & errorState , const int & verticalPos , 
                      const std::list< std::unique_ptr< CGameLoader::SGNode > > & saveList )
    : CRenderer( verticalPos , errorState ) , m_saveList( saveList )
{
}

void CLoadGame::Render() const
{
    system("clear"); // beware of this line, this was done to clean terminal properly without using
                     // external library like ncurses
                     // so it's probably not so wrong but I had to mention it
                     
    PrintMargin( strlen( MAIN_MENU_LOAD_GAME_CONTENT ) );
    std::cout << MAIN_MENU_LOAD_GAME_CONTENT << std::endl;

    int pos = 0;
    for( const auto & it : m_saveList )
    {
        PrintMargin( 3 + it->m_name.length() + 3 + it->m_date.length() );
        if( pos == m_verticalPos )
            std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_BLUE << "m";
        
        std::cout << pos + 1 << ". " << it->m_name << " | " << it->m_date;

        if( pos == m_verticalPos )
            std::cout << CONSOLE_TEXT_POSTFIX;
        std::cout << std::endl;
        pos++;
    }

    if( m_errorState == ERROR_CODE_FAILED_LOAD )
        PrintError( ERROR_LINE_FAILED_LOAD );
}
