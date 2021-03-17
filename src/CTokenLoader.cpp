#include "CTokenLoader.h"

#include <fstream>
#include <vector>
#include <memory>
#include <regex>

CTokenLoader::CTokenLoader( std::vector< std::unique_ptr< CTokenTower > > & towerTokens ,
                            std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens , 
                            std::shared_ptr< CAstar > pathfinder )
    : m_towerTokens( towerTokens ) , m_enemyTokens( enemyTokens ) , m_pathfinder( pathfinder )
{
    ReadTokens();
}

bool CTokenLoader::ReadRange( std::ifstream & is )
{
    std::regex pattern("^RANGE: " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_range = std::stoi( m_matches[ 1 ] ); return true; }
    return false;
}

bool CTokenLoader::ReadFireDelay( std::ifstream & is )
{
    std::regex pattern("^FIRE_DELAY: " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_fireDelay = std::stoi( m_matches[ 1 ] ); return true; }
    return false;
}

bool CTokenLoader::ReadCost( std::ifstream & is )
{
    std::regex pattern("^COST: " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_cost = std::stoi( m_matches[ 1 ] ); return true; }
    return false;
}

bool CTokenLoader::ReadAttack( std::ifstream & is )
{
    std::regex pattern("^" REGEX_ATTACK_TYPE "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_attack = m_matches[ 1 ] == "CROSS" ? TOWER_ATTACK_CROSS : TOWER_ATTACK_AOE; return true; }
    return false;
}

bool CTokenLoader::ReadSpecial( std::ifstream & is )
{
    std::regex pattern("^" REGEX_SPECIAL_TYPE "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_special = m_matches[ 1 ] == "GENERATE" ? TOWER_SPECIAL_GENERATE : TOWER_SPECIAL_NONE; return true; }
    return false;
}

bool CTokenLoader::ReadDesc( std::ifstream & is )
{
    std::regex pattern("^DESCRIPTION: " REGEX_LORE "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_desc = m_matches[ 1 ]; return true; }
    return false;
}

bool CTokenLoader::ReadDamage( std::ifstream & is )
{
    std::regex pattern("^DAMAGE: " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_damage = std::stoi( m_matches[ 1 ] ); return true; }
    return false;
}

bool CTokenLoader::ReadContent( std::ifstream & is )
{
    std::regex pattern("^CONTENT: " REGEX_CONTENT "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_content = m_matches[ 1 ].str()[0]; return true; }
    return false;
}

void CTokenLoader::ReadTowerToken( std::ifstream & is )
{
    if(    ReadRange( is ) && ReadFireDelay( is ) && ReadCost( is ) && ReadAttack( is )
        && ReadSpecial( is ) && ReadDesc( is ) && ReadDamage( is ) && ReadContent( is ) )
        m_towerTokens.push_back( std::make_unique< CTokenTower >( m_towerID++, m_range, m_fireDelay, m_cost, m_attack, m_special, m_desc, m_damage, m_content ) );
}

bool CTokenLoader::ReadHP( std::ifstream & is )
{
    std::regex pattern("^HP: " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_HP = std::stoi( m_matches[ 1 ] ); return true; }
    return false;
}

bool CTokenLoader::ReadKillValue( std::ifstream & is )
{
    std::regex pattern("^KILL_VALUE: " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_killValue = std::stoi( m_matches[ 1 ] ); return true; }
    return false;
}

bool CTokenLoader::ReadLore( std::ifstream & is )
{
    std::regex pattern("^LORE: " REGEX_LORE "$", std::regex_constants::ECMAScript );
    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , pattern ) ){ m_lore = m_matches[ 1 ]; return true; }
    return false;
}

void CTokenLoader::ReadEnemyToken( std::ifstream & is )
{
    if(    ReadHP( is ) && ReadKillValue( is ) && ReadLore( is ) && ReadDesc( is )
        && ReadDamage( is ) && ReadContent( is ) )
        m_enemyTokens.push_back( std::make_unique< CTokenEnemy >( m_enemyID++, m_HP, m_killValue, m_lore, m_desc, m_damage, m_content , m_pathfinder ) );
}

void CTokenLoader::ReadTokenType( std::ifstream & is )
{
    std::regex token_type_pattern("^" REGEX_ENEMY_TOWER "$", std::regex_constants::ECMAScript );

    getline( is , m_readString );
    if( std::regex_search( m_readString , m_matches , token_type_pattern ) )
    {
        if( m_matches[ 1 ] == "TOWER" )
            m_tokenType = TOWER;
        else if( m_matches[ 1 ] == "ENEMY" )
            m_tokenType = ENEMY;
        else
            m_tokenType = 0;
    }
}

void CTokenLoader::ReadTokens()
{
    std::ifstream is;
    is.open( TOKENS );
    if( ! is.good() )
        return;

    while( is.peek() != EOF )
    {
        ReadTokenType( is );
        if( m_tokenType == TOWER )
            ReadTowerToken( is );
        else if( m_tokenType == ENEMY )
            ReadEnemyToken( is );
        m_tokenType = 0;
    }
    is.close();
}
