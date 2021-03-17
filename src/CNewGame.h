#pragma once
#include "CRenderer.h"
#include "game_constants.cpp"

#include <iostream>
#include <list>
#include <string>
#include <cstring>

/**
 * @class CNewGame
 * @brief CNewGame is derived from CRenderer class and prints new game content to console.
 */
class CNewGame : public CRenderer
{
public:
    /**
     * @param[in] verticalPos   position in menu for highlighting features
     * @param[in] errorState    error state defining printing error message
     * @param[in] mapList       list of maps to display
     */
    CNewGame( const int & verticalPos , const int & errorState , const std::list< std::pair< std::string , std::string > > & mapList );
    /**
     * @brief CNewGame Render overriders CRenderer Render method for polymorphic printing to console
     */
    void Render() const override;
private:
    /** list of maps to display */
    const std::list< std::pair< std::string , std::string > > & m_mapList;
};
