#include "CRenderer.h"
#include "game_constants.cpp"

#include <iostream>
#include <memory>
#include <cstring>

/**
 * @class CMainMenu
 * @brief CMainMenu is derived from CRenderer class and prints main menu to console.
 */
class CMainMenu : public CRenderer
{
public:
    /**
     * @param[in] errorState    error state defining printing error message
     * @param[in] verticalPos   position in menu for highlighting features
     */
    CMainMenu( const int & errorState , const int & verticalPos );
    /**
     * @brief CMainMenu Render overriders CRenderer Render method for polymorphic printing to console
     */
    void Render() const override;
private:
    /**
     * Prints simple logo and author's name to console
     */
    void PrintLogoAuthor() const;
    /** Represents main menu list, done this way for some possible menu extensions */
    const char m_menuList[ MENU_SIZE ][ MAX_MENU_ITEM_SIZE ] = { MAIN_MENU_NEW_GAME_CONTENT , MAIN_MENU_LOAD_GAME_CONTENT , MAIN_MENU_GAME_INFO_CONTENT , MAIN_MENU_EXIT_CONTENT };
};
