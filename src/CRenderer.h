#pragma once
#include "game_constants.cpp"

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

/**
 * @class CRenderer
 * @brief CRenderer is virtual class and it is used for inheritance and polymorphic printing to console.
 */
class CRenderer
{
public:
    /**
     * @param[in] verticalPos   position in menu for highlighting features
     * @param[in] errorState    error state defining printing error message
     */
    CRenderer( const int & verticalPos , const int & errorState );
    /**
     * @brief Virtual method for printing to console. Ready for overriding and polymorphism.
     */
    virtual void Render() const = 0;
protected:
    /**
     * @brief Gets size of current instance window.
     */
    void RefreshWinSize() const;
    /**
     * @brief Prints margin in front of content for basic styling.
     * @param[in] contentSize   size of content to be print for margin + contentSize + margin = screen width
     */
    void PrintMargin( int contentSize ) const;
    /**
     * @param[in] error error message content
     */
    void PrintError( const char * error ) const;
    /** position of "cursor" in menu for highlighting features */
    const int & m_verticalPos;
    /** error state defining printing error message */
    const int & m_errorState;
    /** windows size container */
    mutable struct winsize m_window;
};
