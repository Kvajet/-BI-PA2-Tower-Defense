#include "CEntity.h"
#include <fstream>
#include <map>
#include <vector>
#include <memory>

CEntity::CEntity( int xPos , int yPos , bool trough , int id )
    : CField( xPos , yPos , trough ) , m_typeID( id )
{
}
