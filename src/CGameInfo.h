#pragma once
#include "CRenderer.h"
#include "CTokenTower.h"
#include "CTokenEnemy.h"

#include <list>
#include <memory>

/**
 * @class CGameInfo
 * @brief CGameInfo is derived from CRenderer class and prints units info to console.
 */
class CGameInfo : public CRenderer
{
public:
    /**
     * @param[in] errorState    error state defining printing error message
     * @param[in] verticalPos   position in menu for highlighting features
     * @param[in] towerTokens   list of tower tokens
     * @param[in] enemyTokens   list of enemy tokens
     */
    CGameInfo( const int & errorState , const int & verticalPos , const std::vector< std::unique_ptr< CTokenTower > > & towerTokens , 
               const std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens );
    /**
     * @brief CGameInfo Render overriders CRenderer Render method for polymorphic printing to console
     */
    void Render() const override;
public:
    /** list of tower tokens */
    const std::vector< std::unique_ptr< CTokenTower > > & m_towerTokens;
    /** list of enemy tokens */
    const std::vector< std::unique_ptr< CTokenEnemy > > & m_enemyTokens;
};
