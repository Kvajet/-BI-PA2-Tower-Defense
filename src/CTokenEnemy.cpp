#include "CTokenEnemy.h"

CTokenEnemy::CTokenEnemy( int id , int HP , int killValue , const std::string & lore , const std::string & description , int damage , char content , std::shared_ptr< CAstar > pathfinder )
    : CToken( id , description , content , damage ) , m_HP( HP ) , m_killValue( killValue ) , m_lore( lore ) , m_pathfinder( pathfinder )
{
}

void CTokenEnemy::ConsolePrintShort() const
{
}

void CTokenEnemy::ConsolePrintLong() const
{
    std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_RED << "m"
              << "Enemy:\t" << m_tokenContent << std::endl
              << "Health:\t" << m_HP << std::endl
              << "Damage:\t" << m_damage << std::endl
              << "Reward:\t" << m_killValue << std::endl
              << "Description:\t" << m_description << std::endl
              << "Lore: \t" << m_lore << CONSOLE_TEXT_POSTFIX << std::endl;
}

std::shared_ptr< CEnemy > CTokenEnemy::Clone( int xPos , int yPos ) const
{
    return std::make_shared< CEnemy >( xPos , yPos , m_killValue , m_damage , m_tokenContent , m_HP , m_pathfinder , m_typeID );
}

std::shared_ptr< CEnemy > CTokenEnemy::Clone( int xPos , int yPos , int HPmodifier ) const
{
    return std::make_shared< CEnemy >( xPos , yPos , m_killValue , m_damage , m_tokenContent , m_HP + HPmodifier / 2 , m_pathfinder , m_typeID );
}
