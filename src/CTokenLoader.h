#pragma once
#include "CAstar.h"
#include "CTokenTower.h"
#include "CTokenEnemy.h"

#include <fstream>
#include <vector>
#include <regex>
#include <memory>
#include <string>

/**
 * @class CTokenLoader
 * @brief CTokenLoader is used for token loading to the game. CTokenLoader uses regex to read tokens.
 */
class CTokenLoader
{
public:
    /**
     * @param[out] towerTokens tower token list to be load
     * @param[out] enemyTokens enemy token list to be load
     * @param[out] pathfinder  pathfidner inserted into enemy tokens
     */
    CTokenLoader( std::vector< std::unique_ptr< CTokenTower > > & towerTokens , 
                  std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens , 
                  std::shared_ptr< CAstar > pathfinder );
private:
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadRange( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadFireDelay( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadCost( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadAttack( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadSpecial( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadDesc( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadDamage( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadContent( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @brief calls other methods to read tower, if any fails, whole read of tower fails
     */
    void ReadTowerToken( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadHP( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadKillValue( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    bool ReadLore( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @return if matched - true / else - false 
     */
    void ReadEnemyToken( std::ifstream & is );
    /**
     * @param[out] is input stream (file)
     * @brief calls other methods to read enemy, if any fails, whole read of enemy fails
     */
    void ReadTokenType( std::ifstream & is );
    /**
     * @brief called in constructor, reads tokens
     */
    void ReadTokens();
    /** tower token list */
    std::vector< std::unique_ptr< CTokenTower > > & m_towerTokens;
    /** enemy token list */
    std::vector< std::unique_ptr< CTokenEnemy > > & m_enemyTokens;
    /** pathfinder inserted into enemy tokens */
    std::shared_ptr< CAstar > m_pathfinder;
    /** regex matches class */
    std::smatch m_matches;
    /** temp string which reads every line a file */
    std::string m_readString = "";
    /** token type - TOWER / ENEMY (defined in game_constants.cpp) */
    int m_tokenType = 0;
    int m_range = 0, m_fireDelay = 0, m_cost = 0, m_attack = 0, m_special = 0, 
        m_damage = 0, m_HP = 0, m_killValue = 0, m_towerID = 1, m_enemyID = 1;
    std::string m_desc = "", m_lore = "";
    char m_content = '0';
};
