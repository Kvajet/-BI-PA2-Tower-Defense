#include "CMainMenu.h"
#include <iostream>
#include <memory>
#include <cstring>

CMainMenu::CMainMenu( const int & errorState , const int & verticalPos )
    : CRenderer( verticalPos , errorState )
{
}

void CMainMenu::Render() const
{
    system("clear");
    PrintLogoAuthor();

    int pos = 0;
    for( const auto & it : m_menuList )
    {
        PrintMargin( MAX_MENU_ITEM_SIZE );
        if( pos == m_verticalPos )
            std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_BLUE << "m";
        
        std::cout << it;
        
        if( pos == m_verticalPos )
            std::cout << CONSOLE_TEXT_POSTFIX;
        std::cout << std::endl;
        pos++;
    }
}

void CMainMenu::PrintLogoAuthor() const
{
    PrintMargin( strlen( MAIN_MENU_LOGO_AUTHOR ) );
    std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_YELLOW << "m"
              << MAIN_MENU_LOGO_AUTHOR << CONSOLE_TEXT_POSTFIX << std::endl << std::endl;
}
