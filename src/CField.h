#pragma once
#include "game_constants.cpp"
#include <iostream>

/**
 * @class CField
 * @brief CField is virtual class used to inheritance and polymophic printing.
 * It is also used in 2D map representing game play. Base pointers are CField typed but in those are stored
 * derived classes, so there is usage of polymorphism as well.
 */
class CField
{
public:
    /**
     * @param[in] xPos
     * @param[in] yPos 
     * @param[in] through   defines if units are allowed to go through this
     * @param[in] content   units look to be print
     * @param[in] isStart
     * @param[in] isEnd
     */
    CField( int xPos , int yPos , bool through , char content = 0 , bool isStart = false , bool isEnd = false );
    /**
     * @param[out] os   output stream (std::cout here)
     * @brief virtual method to be overridden, used to print styled content to console
     */
    virtual void Print( std::ostream & os ) const = 0;
    /**
     * @return yes - true / no - false
     */
    bool IsStart() const;
    /**
     * @return yes - true / no - false 
     */
    bool IsEnd() const;
    /**
     * @return yes - true / no - false
     */
    bool IsThrough() const;
    /**
     * @param[in] content units look to be print
     * @brief used for setting content in state of generating unit by CTokenEnemy/CTokenTower Clone method
     */
    void SetContent( char content );
    /**
     * @param[out] os   output stream (std::cout here)
     * @param[in] field CField or derived class to be printed
     * @brief prints CField or derived class to console
     * @return return output stream reference (std::cout here)
     */
    friend std::ostream & operator << ( std::ostream & os , const CField & field );
protected:
    int m_xPos, m_yPos;
    /** defines if units are allowed to go through this */
    bool m_through;
    bool m_isStart, m_isEnd;
    /** units look to be print */
    char m_content;
};
