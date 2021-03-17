#include "CGameRenderer.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

CGameRenderer::CGameRenderer( int xSize , int ySize , const std::vector< std::vector < std::shared_ptr< CField > > > & map , 
                              const std::shared_ptr< CGameStats > & stats , const std::pair< int , int > & selectPos ,
                              const int & verticalPos , const std::vector< std::unique_ptr< CTokenTower > > & towerTokens , 
                              const int & rendererState , const int & errorState , const int & remainingEnemies )
    : CRenderer( verticalPos , errorState ) , m_xSize( xSize ) , m_ySize( ySize ) , m_map( map ) , 
      m_stats( stats ) , m_selectPos( selectPos ) , m_towerTokens( towerTokens ) , m_rendererState( rendererState ) , m_remainingEnemies( remainingEnemies )
{
}

void CGameRenderer::Render() const
{
    system("clear"); // beware of this line, this was done to clean terminal properly without using
                     // external library like ncurses
                     // so it's probably not so wrong but I had to mention it

    PrintMap();
    PrintStats();
    PrintObjective();

    if( m_rendererState == GAME_STATE_BUY_PHASE_ARRAY )
        PrintBuyList();
    else if( m_rendererState == GAME_STATE_WIN || m_rendererState == GAME_STATE_LOST )
        PrintWinLost();
    else if( m_rendererState == GAME_STATE_SAVE_GAME )
        PrintSaveList();
    PrintErrorList();
    if( m_rendererState == GAME_STATE_GET_NAME )
        PrintGetName();
}

void CGameRenderer::PrintStats() const
{
    std::cout << std::endl;
    std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_FOREGROUND_CYAN << "m";
    PrintMargin( m_xSize / 2 );
    std::cout << "+ HP: " << m_stats->GetHP() << std::endl;
    PrintMargin( m_xSize / 2 );
    std::cout << "| Score: " << m_stats->GetScore() << std::endl;
    PrintMargin( m_xSize / 2 );
    std::cout << "+ Money: " << m_stats->GetMoney() << std::endl;
    std::cout << CONSOLE_TEXT_POSTFIX;
}

void CGameRenderer::PrintObjective() const
{
    std::cout << std::endl;
    PrintMargin( strlen( GAME_LINE_OBJECTIVE ) );
    std::cout << GAME_LINE_OBJECTIVE << m_remainingEnemies << std::endl << std::endl;
}

void CGameRenderer::PrintMap() const
{
    for( int i = 0 ; i < m_ySize ; i++ )
    {
        PrintMargin( m_xSize );
        for( int k = 0 ; k < m_xSize ; k++ )
            if( ( m_rendererState == GAME_STATE_BUY_PHASE_MAP || m_rendererState == GAME_STATE_BUY_PHASE_ARRAY ) && i == m_selectPos.second && k == m_selectPos.first )
                std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_BACKGROUND_CYAN << "m" << " " << CONSOLE_TEXT_POSTFIX;
            else
                std::cout << *m_map[ i ][ k ];
        std::cout << std::endl;
    }
}

void CGameRenderer::PrintBuyList() const
{
    std::cout << std::endl;
    int indexer = 0;
    for( const auto & it : m_towerTokens )
    {
        PrintMargin( 86 );
        std::cout << CONSOLE_TEXT_PREFIX
                  << ( ( indexer++ == m_verticalPos ) ? CONSOLE_FOREGROUND_BLUE : CONSOLE_FOREGROUND_WHITE ) << "m";
        it->ConsolePrintShort();
        std::cout << CONSOLE_TEXT_POSTFIX << std::endl;
    }
}

void CGameRenderer::PrintSaveList() const
{
    std::cout << std::endl;
    PrintMargin( m_xSize );
    std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << "m" << GAME_STATE_SAVE_GAME_ASK << CONSOLE_TEXT_POSTFIX << std::endl;
    int indexer = 0;
    for( const auto & item : m_saveList )
    {
        PrintMargin( m_xSize );
        std::cout << CONSOLE_TEXT_PREFIX << ( ( indexer++ == m_verticalPos ) ? CONSOLE_FOREGROUND_YELLOW : CONSOLE_FOREGROUND_WHITE ) 
                  << "m" << ">> " << item << CONSOLE_TEXT_POSTFIX << std::endl;
    }    
}

void CGameRenderer::PrintGetName() const
{
    PrintMargin( m_xSize );
    std::cout << GAME_STATE_SAVE_ENTER_NAME << std::endl;
    PrintMargin( m_xSize );
}

void CGameRenderer::PrintErrorList() const
{
    if( m_errorState != ERROR_CODE_NO_ERROR )
    {
        if( m_errorState == ERROR_CODE_FAILED_PLACE_T )
            PrintError( ERROR_LINE_FAILED_PLACE_T );
        else if( m_errorState == ERROR_CODE_WRONG_SAVE_NAME )
            PrintError( ERROR_LINE_WRONG_SAVE_NAME );
        else if( m_errorState == ERROR_CODE_INS_FUNDS )
            PrintError( ERROR_LINE_INS_FUNDS );
    }
}

void CGameRenderer::PrintWinLost() const
{
    if( m_rendererState == GAME_STATE_WIN )
    {
        PrintMargin( strlen( GAME_LINE_WIN ) );
        std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_FOREGROUND_YELLOW << "m" << GAME_LINE_WIN << CONSOLE_TEXT_POSTFIX << std::endl;
    }
    else
    {
        PrintMargin( strlen( GAME_LINE_LOST ) );
        std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_FOREGROUND_RED << "m" << GAME_LINE_LOST << CONSOLE_TEXT_POSTFIX << std::endl;
    }
    PrintMargin( strlen( "Press "" to exit." ) + 1 );
    std::cout << "Press " << CONTROL_KEY_CONFIRM << " to exit." << std::endl;
}
