#pragma once

#include "CField.h"

/**
 * @class CEmpty
 * @brief CEmpty class is derived from CField and is used as representation in 2D map of game plan.
 * CEmpty acts like a empty space, therefore it consists only of one method for printing (which is overriden and server
 * for polymorphism).
 */
class CEmpty : public CField
{
public:
    /**
     * @param xPos 
     * @param yPos 
     */
    CEmpty( int xPos , int yPos );
    /**
     * @param xPos 
     * @param yPos 
     * @param isStart 
     * @param isEnd 
     */
    CEmpty( int xPos , int yPos , bool isStart , bool isEnd );
    /**
     * @param[out] os output stream (std::cout here)
     * @brief overrides CField method Print. Print CEmpty content to console.
     */
    void Print( std::ostream & os ) const override;
private:
};
