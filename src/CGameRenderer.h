#include "CRenderer.h"
#include "CGameStats.h"
#include "CField.h"
#include "CTokenTower.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

/**
 * @class CGameRenderer
 * @brief CGameRenderer is derived from CRenderer class and map, stats, save game options and buy list.
 */
class CGameRenderer : public CRenderer
{
public:
    /**
     * @param[in] xSize             map width
     * @param[in] ySize             map height
     * @param[in] map               2D representation of map
     * @param[in] stats             player stats
     * @param[in] selectPos         position of cursos in game plan while buying
     * @param[in] verticalPos       vertical position in save game menu and buy list, used for hightlighting
     * @param[in] towerTokens       tower tokens for tower buy
     * @param[in] rendererState     game state (used for render only map / with save list / with buy list)
     * @param[in] errorState        error state to be print
     * @param[in] remainingEnemies  count of enemies to be killed for win
     */
    CGameRenderer( int xSize , int ySize , const std::vector< std::vector < std::shared_ptr< CField > > > & map ,
                   const std::shared_ptr< CGameStats > & stats , const std::pair< int , int > & selectPos , 
                   const int & verticalPos , const std::vector< std::unique_ptr< CTokenTower > > & towerTokens , const int & rendererState , const int & errorState , 
                   const int & remainingEnemies );
    /**
     * @brief Render overriders CRenderer Render method for polymorphic printing to console
     */
    void Render() const override;
private:
    /**
     * @brief prints map to console
     */
    void PrintMap() const;
    /**
     * @brief prints player stats to console
     */
    void PrintStats() const;
    /**
     * @brief print how many enemies player has to kill to win the game
     */
    void PrintObjective() const;
    /**
     * @brief prints buy list to console
     */
    void PrintBuyList() const;
    /**
     * @brief prints save list to console
     */
    void PrintSaveList() const;
    /**
     * @brief prints ask for name for save game file, error if occured
     */
    void PrintGetName() const;
    /**
     * @brief decides if print should be printed, if true => calls PrintError method from CRenderer
     */
    void PrintErrorList() const;
    /**
     * @brief end game quote
     */
    void PrintWinLost() const;
    int m_xSize, m_ySize;
    /** 2D representation of map */
    const std::vector< std::vector < std::shared_ptr< CField > > > & m_map;
    /** player stats */
    const std::shared_ptr< CGameStats > & m_stats;
    /** position of cursos in game plan while buying */
    const std::pair< int , int > & m_selectPos;
    /** tower tokens for tower buy */
    const std::vector< std::unique_ptr< CTokenTower > > & m_towerTokens;
    /** game state (used for render only map / with save list / with buy list) */
    const int & m_rendererState;
    /** number of enemies player have to kill to win the game */
    const int & m_remainingEnemies;
    /** Represents save game list */
    const char m_saveList[ GAME_STATE_SAVE_SIZE ][ GAME_STATE_SAVE_ITEM_M_LEN ] = { GAME_STATE_SAVE_GAME_YES , GAME_STATE_SAVE_GAME_NO , GAME_STATE_SAVE_GAME_CANCEL };
};
