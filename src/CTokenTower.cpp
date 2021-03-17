#include "CTokenTower.h"

CTokenTower::CTokenTower( int id , int range , int fireDelay , int cost , int attackType , const std::string & description , int damage , char content )
    : CToken( id , description , content , damage ) , m_range( range ) , m_fireDelay( fireDelay ) , m_cost( cost ) , m_attackType( attackType )
{
}

CTokenTower::CTokenTower( int id , int range , int fireDelay , int cost , int attackType , int specialType , const std::string & description , int damage , char content )
    : CToken( id , description , content , damage ) , m_range( range ) , m_fireDelay( fireDelay ) , m_cost( cost ) , m_attackType( attackType ) , m_specialType( specialType )
{
}

void CTokenTower::ConsolePrintShort() const
{
    std::cout << "Cost: " << m_cost <<  " Damage: " << m_damage << " FireDelay: " << m_fireDelay << " AttackType: ";
    std::cout << ( ( m_attackType == TOWER_ATTACK_CROSS ) ? TOWER_ATTACK_CROSS_DESC : TOWER_ATTACK_AOE_DESC );
    if( m_specialType == TOWER_SPECIAL_GENERATE )
        std::cout << " SpecialType: " << TOWER_SPECILA_GENERATE_DESC;
}

void CTokenTower::ConsolePrintLong() const
{
    std::cout << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" 
              << CONSOLE_FOREGROUND_BLUE << "m" << "Tower:\t" << m_tokenContent << std::endl
              << "Cost:\t" << m_cost << std::endl
              << "Damage:\t" << m_damage << std::endl
              << "FireDelay:\t" << m_fireDelay << std::endl
              << "AttackType:\t" << ( ( m_attackType == TOWER_ATTACK_CROSS ) ? TOWER_ATTACK_CROSS_DESC : TOWER_ATTACK_AOE_DESC ) << std::endl;
    if( m_specialType == TOWER_SPECIAL_GENERATE )
        std::cout << "SpecialType:\t" << TOWER_SPECILA_GENERATE_DESC << std::endl;
    std::cout << "Description:\t" << m_description << CONSOLE_TEXT_POSTFIX << std::endl;
}

std::shared_ptr< CTower > CTokenTower::Clone( int xPos , int yPos ) const
{
    return std::make_shared< CTower >( xPos , yPos , m_range , m_fireDelay , m_damage , m_tokenContent , m_attackType , m_specialType , m_typeID );
}

int CTokenTower::GetCost() const
{
    return m_cost;
}
