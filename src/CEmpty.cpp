#include "CEmpty.h"

CEmpty::CEmpty( int xPos , int yPos )
    : CField( xPos , yPos , THROUGH_TRUE , BASE_ENTITY_SPACE )
{
}

CEmpty::CEmpty( int xPos , int yPos , bool isStart , bool isEnd )
    : CField( xPos , yPos , THROUGH_FALSE , BASE_ENTITY_SPACE , isStart , isEnd )
{
    if( ! isStart && ! isEnd )
        SetContent( BASE_ENTITY_WALL );
    else if( isStart )
    {
        SetContent( BASE_ENTITY_START );
        m_through = THROUGH_TRUE;
    }
    else if( isEnd )
    {
        SetContent( BASE_ENTITY_END );
        m_through = THROUGH_TRUE;
    }
}

void CEmpty::Print( std::ostream & os ) const
{
    if( m_content == BASE_ENTITY_SPACE )
        os << m_content;
    else if( m_content == BASE_ENTITY_START || m_content == BASE_ENTITY_END )
        os << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_BLINK << ";" << CONSOLE_FOREGROUND_WHITE << "m"
                                                        << m_content << CONSOLE_TEXT_POSTFIX;
    else
        os << CONSOLE_TEXT_PREFIX << CONSOLE_TEXT_LIGHT << ";" << CONSOLE_FOREGROUND_YELLOW << "m" 
                                                        << m_content << CONSOLE_TEXT_POSTFIX;
}
