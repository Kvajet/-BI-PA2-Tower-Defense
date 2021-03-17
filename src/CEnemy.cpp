#include "CEnemy.h"
#include <map>
#include <vector>
#include <memory>


CEnemy::CEnemy( int xPos , int yPos , int killValue , int damage , char content , int HP , std::shared_ptr< CAstar > pathfinder , int id )
    : CEntity( xPos , yPos , THROUGH_TRUE , id ) , m_HP( HP ) , m_damage( damage ) , m_killValue( killValue ) , m_pathfinder( pathfinder )
{
    SetContent( content );
}

const std::map< int , std::vector< int > > & CEnemy::Action()
{
    if( m_path.empty() )
    {
        m_path = m_pathfinder->GetPath( m_xPos , m_yPos );
        m_path.pop_back();
    }

    m_action.clear();
    std::vector< int > actionVec;

    if( m_path.size() == 1 )
        ActionEnd( actionVec );     // if in path is 1 tile, enemy is at end
    else if( m_HP <= 0 )
        ActionDeath( actionVec );
    else
        ActionMove( actionVec );

    return m_action;
}

void CEnemy::RecieveDamage( int damage )
{
    m_HP -= damage;
}

void CEnemy::Print( std::ostream & os ) const
{
    os << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_RED << "m" << m_content << CONSOLE_TEXT_POSTFIX;
}

void CEnemy::Save( std::ofstream & os )
{
    os << "ENEMY" << std::endl
       << m_typeID << std::endl
       << m_HP << std::endl
       << m_xPos << " " << m_yPos << std::endl;
}

bool CEnemy::operator == ( const std::pair< int , int > & coords )
{
    return m_xPos == coords.first && m_yPos == coords.second;
}

void CEnemy::ActionPushPos( std::vector< int > & actionVec )
{
    actionVec.push_back( m_xPos );
    actionVec.push_back( m_yPos );
}

void CEnemy::ActionEnd( std::vector< int > & actionVec )
{
    ActionPushPos( actionVec );
    actionVec.push_back( m_damage );
    m_action.insert( { ACTION_ENEMY_END , actionVec } );
}

void CEnemy::ActionMove( std::vector< int > & actionVec )
{
    ActionPushPos( actionVec );
    actionVec.push_back( m_path.back().first );
    actionVec.push_back( m_path.back().second );

    m_action.insert( { ACTION_MOVE , actionVec } );

    Move( m_path.back().first , m_path.back().second );
    m_path.pop_back();
}

void CEnemy::ActionDeath( std::vector< int > & actionVec )
{
    ActionPushPos( actionVec );
    actionVec.push_back( m_killValue );
    m_action.insert( { ACTION_DEATH , actionVec } );
}

void CEnemy::Move( int xPos , int yPos )
{
    m_xPos = xPos;
    m_yPos = yPos;
}
