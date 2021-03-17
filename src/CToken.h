#pragma once
#include "CTower.h"
#include "CEnemy.h"
#include <string>
#include <memory>

/**
 * @class CToken
 * @brief CToken is virtual class and it is used for inheritance and polymorphic printing.
 * CToken is meant as "generator" and template for entities. Clone method is defined and implemented in each
 * derived CToken class separately.
 */
class CToken
{
public:
    /**
     * @param[in] id            type of entity used for saving
     * @param[in] description   description of entity
     * @param[in] content       entity look to be displayed
     * @param[in] damage        entity damage
     */
    CToken( int id , const std::string & description , char content , int damage );
    /**
     * @brief Prints short info about unit to console (means that not all attributes are displayed, may be different in styling as well)
     */
    virtual void ConsolePrintShort() const = 0;
    /**
     * @brief Prints long info about unit to console
     */
    virtual void ConsolePrintLong() const = 0;
protected:
    /** type of entity used for saving */
    int m_typeID;
    std::string m_description;
    /** entity look */
    char m_tokenContent;
    int m_damage;
};
