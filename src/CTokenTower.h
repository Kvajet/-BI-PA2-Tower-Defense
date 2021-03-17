#pragma once
#include "CToken.h"
#include "CTower.h"

/**
 * @class CTokenTower
 * @brief CTokenTower is template class for printing and cloning (generating) towers.
 * CTokenTower overrrides ConsolePrintShort and ConsolePrintLong methods for polymorphic printing to console.
 * Main reason for this class is keeping only one object with lore, description, ... attributes to save memory.
 */
class CTokenTower : private CToken
{
public:
    /**
     * @param[in] id                tower ID for saving
     * @param[in] range             range of attack
     * @param[in] fireDelay         timer for attacks
     * @param[in] cost              tower cost
     * @param[in] attackType        attack type ID (defined in game_cosntants.cpp)
     * @param[in] description
     * @param[in] damage
     * @param[in] content           tower look to be displayed 
     */
    CTokenTower( int id , int range , int fireDelay , int cost , int attackType , const std::string & description , int damage , char content );
    /**
     * @param[in] id                tower ID for saving 
     * @param[in] range             range of attack
     * @param[in] fireDelay         timer for attacks
     * @param[in] cost              tower cost
     * @param[in] attackType        attack type ID (defined in game_constants.cpp)
     * @param[in] specialType       special type ID (defined in game_constants.cpp)
     * @param[in] description 
     * @param[in] damage 
     * @param[in] content           tower look to be displayed
     */
    CTokenTower( int id , int range , int fireDelay , int cost , int attackType , int specialType , const std::string & description , int damage , char content );
    /**
     * @brief Prints short info about unit to console (means that not all attributes are displayed, may be different in styling as well)
     * Overrides CToken method for polymorphic printing.
     */
    void ConsolePrintShort() const override;
    /**
     * @brief Prints long info about unit to console. Overrides CToken method for polymorphic printing.
     */
    void ConsolePrintLong() const override;
    /**
     * @param[in] xPos 
     * @param[in] yPos 
     * @brief Method used for cloning tower.
     * @return Shared pointer to CTower object.
     */
    std::shared_ptr< CTower > Clone( int xPos , int yPos ) const;
    /**
     * @brief gets tower cost
     * @return cost
     */
    int GetCost() const;
private:
    int m_range, m_fireDelay, m_cost;
    /** attack type ID (defined in game_constants.cpp) */
    int m_attackType;
    /** special type ID (defined in game_constants.cpp) */
    int m_specialType = TOWER_SPECIAL_NONE;
};
