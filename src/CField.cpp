#include "CField.h"

CField::CField( int xPos , int yPos , bool through , char content , bool isStart , bool isEnd )
    : m_xPos( xPos ) , m_yPos( yPos ) , m_through( through ) , m_isStart( isStart ) , m_isEnd( isEnd ) , m_content( content )
{
}

bool CField::IsStart() const
{
    return m_isStart;
}

bool CField::IsEnd() const
{
    return m_isEnd;
}

bool CField::IsThrough() const
{
    return m_through;
}

void CField::SetContent( char content )
{
    m_content = content;
}

std::ostream & operator << ( std::ostream & os , const CField & field )
{
    field.Print( os );
    return os;
}
