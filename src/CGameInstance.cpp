#include "CGameInstance.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <memory>
#include <list>
#include <regex>
#include <termios.h>

CGameInstance::CGameInstance()
    : m_stats( std::make_shared< CGameStats >() ) , m_renderer( std::make_unique< CMainMenu >( m_errorState , m_verticalPos ) ) 
{
    m_logic = std::make_unique< CGameLogic >( m_towerTokens , m_enemyTokens , m_entities , m_map , m_stats , m_gameState , 
                                              m_errorState , m_verticalPos , m_remainingEnemies , m_mapSize , m_selectPos );
    tcgetattr( STDIN_FILENO, &m_originalTerminal );
}

int CGameInstance::Play()
{
    if( ! AreValidTokens() )
        return UnplayableGame();

    SetNoEchoTerminal();
    m_renderer->Render();
    while( m_gameState != GAME_STATE_END_GAME )
    {
        m_logic->GetInput();
        if( m_logic->Process() )
            SwitchRenderer();
        m_renderer->Render();
    }
    RestoreTerminal();

    return 0;
}

int CGameInstance::UnplayableGame() const
{
    std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_BACKGROUND_RED << "m" << ERROR_LINE_NO_TOKENS << CONSOLE_TEXT_POSTFIX << std::endl;
    return FILE_ERROR;
}

bool CGameInstance::AreValidTokens() const
{
    return m_towerTokens.size() && m_enemyTokens.size();
}

void CGameInstance::SetNoEchoTerminal() const
{
    termios newTerminal = m_originalTerminal;
    newTerminal.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal);
}

void CGameInstance::RestoreTerminal() const
{
    tcsetattr(STDIN_FILENO, TCSANOW, &m_originalTerminal);
}

void CGameInstance::SwitchRenderer()
{
    if     ( m_gameState == MAIN_MENU_DEFAULT         ) m_renderer = std::make_unique< CMainMenu >( m_errorState , m_verticalPos );
    else if( m_gameState == MAIN_MENU_NEW_GAME        ) m_renderer = std::make_unique< CNewGame >( m_errorState , m_verticalPos , m_logic->GetMaps() );
    else if( m_gameState == MAIN_MENU_LOAD_GAME       ) m_renderer = std::make_unique< CLoadGame >( m_errorState , m_verticalPos , m_logic->GetSaves() );
    else if( m_gameState == MAIN_MENU_GAME_INFO       ) m_renderer = std::make_unique< CGameInfo >( m_errorState , m_verticalPos , m_towerTokens , m_enemyTokens );
    else if( m_gameState == GAME_STATE_GAME_PHASE
          || m_gameState == GAME_STATE_BUY_PHASE_ARRAY 
          || m_gameState == GAME_STATE_BUY_PHASE_MAP  
          || m_gameState == GAME_STATE_WIN
          || m_gameState == GAME_STATE_LOST           ) m_renderer = std::make_unique< CGameRenderer >( m_mapSize.first , m_mapSize.second , m_map , m_stats , m_selectPos , m_verticalPos , m_towerTokens , m_gameState , m_errorState , m_remainingEnemies );
}
