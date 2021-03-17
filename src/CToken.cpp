#include "CToken.h"
#include <string>

CToken::CToken( int id , const std::string & description , char content , int damage )
    : m_typeID( id ) , m_description( description ) , m_tokenContent( content ) , m_damage( damage )
{
}
