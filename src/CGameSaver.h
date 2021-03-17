#pragma once
#include "CEntity.h"

#include <fstream>
#include <string>
#include <list>
#include <memory>
#include <ctime>

/**
 * @class CGameSaver
 * @brief CGameSaver saves entities to specified file and appends save index file
 * CGameSaver is ready to save tower (available) and enemies. Currently saving of enemies is not used.
 * (Because there is no way to save game during game phase).
 */
class CGameSaver
{
public:
    /**
     * @param[in] m_entities    list of entities to be saved
     * @param[in] mapSize       size of gameplan
     */
    CGameSaver( const std::list< std::shared_ptr< CEntity > > & m_entities , const std::pair< int , int > & mapSize );
    /**
     * @param[in] saveName 
     * @param[in] mapPath 
     * @param[in] HP 
     * @param[in] score 
     * @param[in] money 
     * @brief saves entities to save file with HP, score and money
     * @return success - FILE_OKAY / else - FILE_ERROR (defined in game_constants.cpp)
     */
    int Save( const std::string & saveName , const std::string & mapPath , int HP , int score , int money );
private:
    /**
     * @brief refreshes time (m_actTime) for save reasons
     */
    void RefreshTime();
    /**
     * @param[out] os   outputstream (file here)
     * @brief saves entities
     */
    void SaveEntities( std::ofstream & os );
    /**
     * @param[out] os   outputstream (file here)
     * @param[in] HP    player health
     * @param[in] score player score
     * @param[in] money player money
     * @brief saves player's stats
     */
    void SaveSizeAndStats( std::ofstream & os , int HP , int score , int money );
    /**
     * @param[in] saveName  name of the save file
     * @param[in] mapPath   map on which was save game played
     * @brief appends save index file
     * @return success - FILE_OKAY / else - FILE_ERROR (defined in game_constants.cpp)
     */
    int SaveToIndexer( const std::string & saveName , const std::string & mapPath );
    /** list of entities to save */
    const std::list< std::shared_ptr< CEntity > > & m_entities;
    const std::pair< int , int > & m_mapSize;
    /** time structure from ctime standart library */
    std::tm * m_actTime;
};
