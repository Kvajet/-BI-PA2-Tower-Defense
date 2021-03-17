#pragma once
#include "CRenderer.h"
#include "CGameLoader.h"
#include "game_constants.cpp"

#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <memory>

/**
 * @class CLoadGame
 * @brief CLoadGame is derived from CRenderer class. It prints load game content to console.
 */
class CLoadGame : public CRenderer
{
public:
    /**
     * @param[in] errorState    error state defining printing error message
     * @param[in] verticalPos   position of "cursor" in menu for highlighting features
     * @param[in] saveList      list of saves to display
     */
    CLoadGame( const int & errorState , const int & verticalPos , 
               const std::list< std::unique_ptr< CGameLoader::SGNode > > & saveList );
    /**
     * @brief CLoadGame Render overriders CRenderer Render method for polymorphic printing to console
     */
    void Render() const override;
private:
    /** list of saves to display */
    const std::list< std::unique_ptr< CGameLoader::SGNode > > & m_saveList;
};
