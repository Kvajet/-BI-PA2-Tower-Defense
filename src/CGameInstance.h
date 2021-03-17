#include "game_constants.cpp"
#include "CField.h"
#include "CGameLogic.h"

#include "CGameRenderer.h"
#include "CMainMenu.h"
#include "CNewGame.h"
#include "CLoadGame.h"
#include "CGameInfo.h"

#include "CGameStats.h"
#include "CTokenTower.h"
#include "CTokenEnemy.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <memory>
#include <list>
#include <termios.h>

/**
 * @class CGameInstance
 * @brief CGameInstance is TDEngine main container. CGameInstance is set up by calling Play method.
 * It is able to read save and maps (which are easy to create). Enemies and towers can be added
 * or removed in token/token.txt file, more in token.txt or in tokens_template.txt. For valid run
 * there has to be at least one enemy and at least one tower.
 */
class CGameInstance
{
public:
    CGameInstance();
    /**
     * @brief main method for everything, consists of inifity loop
     * @return game was playable - 0 / else - FILE_ERROR (defined in game_constants.cpp)
     */
    int Play();
private:
    /**
     * @brief game has no enemies or no enities, therefore it is considered as unplayable
     * @return FILE_ERROR (defined in game_constants.cpp)
     */
    int UnplayableGame() const;
    /**
     * @brief checks if at least one enemy and at least one tower token was loader
     * @return were loaded - true / else - false
     */
    bool AreValidTokens() const;
    /**
     * @brief disables echo in terminal
     */
    void SetNoEchoTerminal() const;
    /**
     * @brief enables echo in terminal
     */
    void RestoreTerminal() const;
    /**
     * @brief switches CRenderer based class depending on m_gameState
     */
    void SwitchRenderer();
    /** CGameLogic object, does all game logic for game */
    std::unique_ptr< CGameLogic > m_logic;
    /** CTokenTower object, vector of tokens for generating towers to the game */
    std::vector< std::unique_ptr< CTokenTower > > m_towerTokens;
    /** CTokenEnemy object, vector of tokens for generating enemies to the game */
    std::vector< std::unique_ptr< CTokenEnemy > > m_enemyTokens;
    /** list of entities currently loaded in game */
    std::list< std::shared_ptr< CEntity > > m_entities;
    /** 2D representation of game plan */
    std::vector< std::vector< std::shared_ptr< CField > > > m_map;
    /** CGameStats object, takes care of player's money, HP and score information */
    std::shared_ptr< CGameStats > m_stats;
    /** game state for game logic and renderer, (defined in game_constants.cpp) */
    int m_gameState = MAIN_MENU_DEFAULT;
    /** error state for game logic and renderer, (defined in game_constants.cpp) */
    int m_errorState = ERROR_CODE_NO_ERROR;
    /** index defining position in vertical base lists - menu, ... , important for renderer */
    int m_verticalPos = 0;
    /** number of remaining enemies to win the game */
    int m_remainingEnemies = 0;
    /** coordination of "cursor" in game plan */
    std::pair< int , int > m_selectPos;
    std::pair< int , int > m_mapSize;
    /** CGameRender instance, used for polymorphism
     *  CRenderer derived classes are inserted into m_renderer in dependence on m_gameState
     */
    std::unique_ptr< CRenderer > m_renderer;
    /** original terminal settings */
    mutable termios m_originalTerminal;
};
