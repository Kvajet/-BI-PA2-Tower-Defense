#pragma once
#include "game_constants.cpp"
#include "CTokenTower.h"
#include "CTokenEnemy.h"
#include "CGameStats.h"
#include "CTokenLoader.h"
#include "CGameLoader.h"

#include <memory>
#include <vector>
#include <list>
#include <unistd.h>
#include <termios.h>
#include <chrono>
#include <thread>

/**
 * @class CGameLogic
 * @brief CGameLogic is brain of the whole game. Takes care about entities responses from their Action methods.
 * Spawns enemies, buys towers, takes care of moving in game plan or menu.
 */
class CGameLogic
{
public:
    /**
     * @param[out] towerTokens       list of tower tokens
     * @param[out] enemyTokens       list of enemy tokens
     * @param[out] entities          list of entities
     * @param[out] map               2D representation of map
     * @param[out] stats             player stats
     * @param[out] gameState         game state for next actions (defined in game_constants.cpp)
     * @param[out] errorState        error state to be print (defined in game_constants.cpp)
     * @param[out] verticalPos       position of "cursor" in vertical lists
     * @param[out] remainingEnemies  enemy count to win the game
     * @param[out] mapSize      
     * @param[out] selectPos         poisiton of "cursos" in game plan
     */
    CGameLogic( std::vector< std::unique_ptr< CTokenTower > > & towerTokens , std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens ,
                std::list< std::shared_ptr< CEntity > > & entities, std::vector< std::vector< std::shared_ptr< CField > > > & map ,
                std::shared_ptr< CGameStats > stats , int & gameState , int & errorState, int & verticalPos , int & remainingEnemies , 
                std::pair< int , int > & mapSize , std::pair< int , int > & selectPos );
    /**
     * @brief reads input from console (in not during wave of enemies), MAIN thing called by play in CGameInstace
     */
    void GetInput() const;
    /**
     * @brief calls other logic functions depending on game state (defined in game_constants.cpp), MAIN thing called by play in CGameInstace
     * @return if game state has change - true / else - false
     */
    bool Process();
    /**
     * @brief returns map list to be print by CNewGame renderer
     * @return list of pairs (map path, map name)
     */
    const std::list< std::pair< std::string , std::string > > & GetMaps() const;
    /**
     * @brief returns save list to be print by CLoadGame renderer
     * @return list of SGNode class data
     */
    const std::list< std::unique_ptr< CGameLoader::SGNode > > & GetSaves() const;
private:
    /**
     * @brief resets position of m_selectPos
     */
    void ResetSelectPos();
    /**
     * @param[in] xPos 
     * @param[in] yPos 
     * @brief checks if given coordinates are free to step there and are not end or start
     * @return true / false
     */
    bool IsThroughNotSE( int xPos , int yPos ) const;
    /**
     * @param[in] coords 
     * @brief processes enemy movement
     */
    void EnemyMove( const std::vector< int > & coords );
    /**
     * @param[in] coords 
     * @param[in] enemy 
     * @brief processes enemy death and deletes it from m_entitites and m_enemies
     */
    void EnemyDeath( const std::vector< int > & coords , std::shared_ptr< CEntity > & enemy );
    /**
     * @brief removes etities from m_entity list base on m_removeFromEntitiesList
     */
    void RemoveEntities();
    /**
     * @brief loads map from m_loader and prepares everything important for game
     * @return success - true / else - false
     */
    bool PrepMap();
    /**
     * @brief loads map and save from m_loader and prepares everything important for game
     * @return success - true / else - false
     */
    bool LoadAndPrepMap();
    /**
     * @param[in] coords 
     * @brief gets a passable tile from given coordinates (may choose west, east, north, south)
     * @return valid tile's coords, if not found returns <-1,-1>
     */
    std::pair< int , int > FirstValidTile( const std::pair< int , int > & coords ) const;
    /**
     * @brief tries to buy tower, if not possible sets error state
     */
    void BuyTower();
    /**
     * @brief each game tick releases one enemy to game plan
     */
    void DeployEnemy();
    /**
     * @brief fills queue released by DeployEnemy with enemies affected by round modifier
     */
    void FillQueue();
    /**
     * @brief chenges state depending on win or lose of player
     * @return true
     */
    bool WinLoseCondition();
    /**
     * @brief processes game when GAME_STATE_GAME_PHASE (defined in game_constants.cpp) is active in m_gameState
     * @return m_gameState changed - true / else - false
     */
    bool GamePhase();
    /**
     * @param[in] entity 
     * @param[in] xPos 
     * @param[in] yPos 
     * @param[in] damage 
     * @brief deals damage to player and remove enemy from m_entities and m_enemies lists
     */
    void EnemyAtEnd( std::shared_ptr< CEntity > & entity , int xPos , int yPos , int damage );
    /**
     * @param[in] coords 
     * @brief processes tower attacks against enemies
     */
    void TowerAttack( const std::vector< int > & coords );
    /**
     * @brief processes game when GAME_STATE_BUY_PHASE_ARRAY (defined in game_constants.cpp) is active in m_gameState
     * @return m_gameState changed - true / else - false
     */
    bool BuyPhase();
    /**
     * @brief reads save name from unser input and saves game
     * @return if name was correct therefore game state changed - true / else - false
     */
    bool ReadSaveNameAndSave();
    /**
     * @brief processes game when GAME_STATE_SAVE_GAME (defined in game_constants.cpp) is active in m_gameState
     * @return player chose quit game or save game - true / canceled - false
     */
    bool SaveNameList();
    /**
     * @brief processes gmae when one MAIN_MENU (defined in game_constants.cpp) states is set
     * @return game state changed - true / else - false
     */
    bool MenuPhase();
    /**
     * @brief processes game when GAME_STATE_BUY_PHASE_MAP (defined in game_constants.cpp) is active in m_gameState
     * @return game state changed - true / else - false
     */
    bool MapMovement();
    /**
     * @brief processe game when player won or lost
     * @return game state changed - true / else - false
     */
    bool ProcessWinLose();
    /**
     * @brief gets size of map index file
     * @return size of map index file
     */
    int MapsSize() const;
    /**
     * @brief gets size of save index file
     * @return size of map index file
     */
    int SavesSize() const;
    /** tower tokens list */
    std::vector< std::unique_ptr< CTokenTower > > & m_towerTokens;
    /** enemy tokens list */
    std::vector< std::unique_ptr< CTokenEnemy > > & m_enemyTokens;
    /** list of in game intities */
    std::list< std::shared_ptr< CEntity > > & m_entities;
    /** 2D representation of map */
    std::vector< std::vector< std::shared_ptr< CField > > > & m_map;
    /** player stats */
    std::shared_ptr< CGameStats > m_stats;
    int & m_gameState, & m_errorState, & m_verticalPos, & m_remainingEnemies;
    std::pair< int , int > & m_mapSize , & m_selectPos, m_start , m_end;
    mutable char m_input;
    int m_enemyCount = 0;
    /** count of played round, works as modifier for enemy count and enemy HP */
    int m_roundModifier = 0;
    std::shared_ptr< CAstar > m_pathfinder;
    /** CTokenLoader object that loads token in its constructor */
    std::unique_ptr< CTokenLoader > m_tokenLoader;
    /** loads map, loads saves, saves saves */
    std::unique_ptr< CGameLoader > m_loader;
    /** list for deleting enemies from m_entities and m_enemies lists */
    std::list< std::shared_ptr< CEntity > > m_removeFromEntitiesList;
    /** list of enemies in game */
    std::list< std::shared_ptr< CEnemy > > m_enemies;
    /** queue of enemies to be placed to game */
    std::list< std::shared_ptr< CEnemy > > m_enemiesQueue;
    /** original terminal settings */
    termios m_originalConsole;
    /** raw terminal settings */
    termios m_newConsole;
    /** terminal with echo */
    termios m_echoConsole;
};
