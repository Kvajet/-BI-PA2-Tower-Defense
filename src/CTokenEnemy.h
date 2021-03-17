#pragma once
#include "CToken.h"
#include "CEnemy.h"

/**
 * @class CTokenEnemy
 * @brief CTokenEnemy is template class for printing and cloning (generating) enemies.
 * CTokenEnemy overrrides ConsolePrintShort and ConsolePrintLong methods for polymorphic printing to console.
 * Main reason for this class is keeping only one object with lore, description, ... attributes to save memory.
 */
class CTokenEnemy : private CToken
{
public:
    /**
     * @param[in] id                enemy ID used for saving
     * @param[in] HP
     * @param[in] killValue         player reward for killing enemy
     * @param[in] lore              enemy lore (longer than description)
     * @param[in] description
     * @param[in] damage
     * @param[in] content           enemy look to be displayed
     * @param[in,out] pathfinder    pathifinder for getting path
     */
    CTokenEnemy( int id , int HP , int killValue , const std::string & lore , const std::string & description , int damage , char content , std::shared_ptr< CAstar > pathfinder );
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
     * @brief Method used for cloning enemy. It is used in CGameLoader (there is no HP modifier).
     * @return Shared pointer to CEnemy object.
     */
    std::shared_ptr< CEnemy > Clone( int xPos , int yPos ) const;
    /**
     * @param[in] xPos
     * @param[in] yPos
     * @param[in] HPmodifier    modifier HP by formula: HP + HPmodifier / 2
     * @brief Method used for cloning enemy. With HP modifier option.
     * @return Shared pointer to CEnemy object.
     */
    std::shared_ptr< CEnemy > Clone( int xPos , int yPos , int HPmodifier ) const;
private:
    int m_HP, m_killValue;
    std::string m_lore;
    /** pathfinder pointer for getting path */
    std::shared_ptr< CAstar > m_pathfinder;
};