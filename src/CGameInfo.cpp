#include "CGameInfo.h"

CGameInfo::CGameInfo( const int & errorState , const int & verticalPos , 
                      const std::vector< std::unique_ptr< CTokenTower > > & towerTokens , 
                      const std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens )
    : CRenderer( verticalPos , errorState ), m_towerTokens( towerTokens ), m_enemyTokens( enemyTokens )
{

}

void CGameInfo::Render() const
{
    system("clear"); // beware of this line, this was done to clean terminal properly without using
                     // external library like ncurses
                     // so it's probably not so wrong but I had to mention it
    for( const auto & token : m_towerTokens )
    {
        token->ConsolePrintLong();
        std::cout << std::endl;
    }

    for( const auto & token : m_enemyTokens )
    {
        token->ConsolePrintLong();
        std::cout << std::endl;
    }
}
