#include "CTower.h"
#include <map>
#include <vector>

CTower::CTower( int xPos , int yPos , int range , int fireDelay , int damage , char content , int attackType , int specialType , int id )
  : CEntity( xPos , yPos , THROUGH_FALSE , id ) , m_range( range ) , m_fireDelay( fireDelay ) , m_damage( damage ) , m_attackType( attackType ) , m_specialAction( specialType )
{
  SetContent( content );
  m_delayTimer = m_fireDelay;
}

const std::map< int , std::vector< int > > & CTower::Action()
{
  if( ! m_attackSet )
  {
    AssignAttack();
    AssignSpecials();
    m_attackSet = true;
  }
  m_delayTimer--;
  if( m_delayTimer < 0 )
  {
    RemoveSkipTag();
    m_delayTimer = m_fireDelay;
  }
  else
    AddSkipTag();

  return m_action;
}

void CTower::Save( std::ofstream & os )
{
    os << "TOWER" << std::endl
       << m_typeID << std::endl
       << m_xPos << " " << m_yPos << std::endl;
}

void CTower::Print( std::ostream & os ) const
{
    os << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_BLUE << "m" << m_content << CONSOLE_TEXT_POSTFIX;
}

void CTower::RemoveSkipTag()
{
  m_action.erase( ACTION_SKIP );    
}

void CTower::AddSkipTag()
{
  m_action.insert( { ACTION_SKIP , std::vector< int >() } );
}

void CTower::AssignAttack()
{
    std::vector< int > actionVec;
    actionVec.push_back( m_damage );
    if( m_attackType == TOWER_ATTACK_AOE )
      AOEAttack( actionVec );
    else if( m_attackType == TOWER_ATTACK_CROSS )
      CrossAttack( actionVec );
    m_action.insert( { ACTION_ATTACK , actionVec } );
}

void CTower::CrossAttack( std::vector< int > & actionVec )
{
  int middle = m_range + 1;
  int totalRange = m_range * 2 + 1;
  // cross attack pattern, x - damaged tile, A - tower
  // ..x..
  // ..x..
  // xxAxx
  // ..x..
  // ..x..
  for( int i = 0 ; i < totalRange ; i++ )
  {
      if( middle == 2 )
        continue;
      actionVec.push_back( m_xPos );
      actionVec.push_back( -2 + i + m_yPos );
  }
  for( int i = 0 ; i < totalRange ; i++ )
  {
      if( middle == 2 )
        continue;
      actionVec.push_back( -2 + i + m_xPos );
      actionVec.push_back( m_yPos );
  }
}

void CTower::AOEAttack( std::vector< int > & actionVec )
{
  int middle = m_range + 1;
  int totalRange = m_range * 2 + 1;
  // cross attack pattern, x - damaged tile, A - tower
  // xxxxx
  // xxxxx
  // xxAxx
  // xxxxx
  // xxxxx
  for( int i = 0; i < totalRange ; i++ )
    for( int k = 0 ; k < totalRange ; k++ )
    {
      if( middle == 2 && middle == 2 )
        continue;
      actionVec.push_back( -2 + k + m_xPos );
      actionVec.push_back( -2 + i + m_yPos );
    }
}

void CTower::AssignSpecials()
{
  std::vector< int > actionVec;
  if( m_specialAction == TOWER_SPECIAL_GENERATE )
  {
    GenerateSpecial( actionVec );
    m_action.insert( { TOWER_SPECIAL_GENERATE , actionVec } );
  }
}

void CTower::GenerateSpecial( std::vector< int > & actionVec )
{
  actionVec.push_back( SPECIAL_GENERATE_AMOUNT );
}
