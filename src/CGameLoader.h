#pragma once
#include "CField.h"
#include "CTower.h"
#include "CEnemy.h"
#include "CEmpty.h"
#include "CEntity.h"
#include "CGameStats.h"
#include "CAstar.h"
#include "CTokenTower.h"
#include "CTokenEnemy.h"

#include "CGameSaver.h"

#include <utility>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <set>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <list>
#include <regex>

/**
 * @class CGameLoader
 * @brief CGameLoader is used for loading maps, loading and erasing saves.
 * It is possible to load tower and enemies from saves. 
 * WARNING: Loading enemies may lead to undefined behaviour. (Only in map printing, can't break the game, game still works).
 */
class CGameLoader
{
public:
    /**
     * @param[out] map          2D representation of game plan
     * @param[out] stats        player stats
     * @param[out] entities     list of entities
     * @param[out] enemies      list of enemies
     * @param[out] enemyCnt     enemy count
     * @param[in] pathfinder    pathfinder added to CEnemy class
     * @param[out] mapSize
     * @param[out] start 
     * @param[out] end 
     * @param[in] towerTokens   tower tokens for cloning towers
     * @param[in] enemyTokens   enemy tokens for cloning enemies
     */
    CGameLoader( std::vector< std::vector < std::shared_ptr< CField > > > & map , std::shared_ptr< CGameStats > stats ,
                 std::list< std::shared_ptr< CEntity > > & entities , std::list< std::shared_ptr< CEnemy > > & enemies ,
                 int & enemyCnt , std::shared_ptr< CAstar > pathfinder , std::pair< int , int > & mapSize ,
                 std::pair< int , int > & start , std::pair< int , int > & end ,
                 const std::vector< std::unique_ptr< CTokenTower > > & towerTokens , 
                 const std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens );
    /**
     * @class SGNode
     * @brief SGNode (saved game node) is used as container for save game info.
     */
    class SGNode
    {
    public:
        /**
         * @param[in] mapPath   path to map file
         * @param[in] savePath  path to save file
         * @param[in] name      save name
         * @param[in] date      save creation date
         */
        SGNode( const std::string & mapPath , const std::string & savePath , 
                const std::string & name , const std::string & date )
            : m_mapPath( mapPath ) , m_savePath( savePath ) , m_name ( name ) , m_date( date )
        {
        }
        std::string m_mapPath, m_savePath, m_name, m_date;
    };
    /**
     * @brief returns content of map index file
     * @return list of pairs (map path, map name)
     */
    const std::list< std::pair< std::string , std::string > > & GetMapIndexFile() const;
    /**
     * @brief returns content of save index file
     * @return list of SGNode objects 
     */
    const std::list< std::unique_ptr< SGNode > > & GetSaveIndexFile() const;
    /**
     * @param[in] index index of map in m_mapIndex variable representing map index file
     * @brief loads maps defined by index from m_mapIndex
     * @return success - FILE_OKAY / else - FILE_ERROR
     */
    int LoadMapByIndex( size_t index );
    /**
     * @param[in] index index of save in m_saveIndex variable representing save index file
     * @brief loads save defined by index from m_savendex
     * @return success - FILE_OKAY / else - FILE_ERROR
     */
    int LoadSaveByIndex( size_t index );
    /**
     * @param[in] saveName name of save game
     * @brief save a game to file in format saveName.txt and to save index as saveName with current time
     * @return success - FILE_OKEY / else - FILE_ERROR
     */
    int Save( const std::string & saveName );
    /**
     * @param[in] index index of save in m_saveIndex variable representing save index file
     * @brief erases unwanted game
     */
    void Erase( size_t index );
private:
    /**
     * @param[in] saveName 
     * @brief erases save if exists
     */
    void EraseIfInSaveIndexer( const std::string & saveName );
    /**
     * @brief loads map index file to m_mapIndex variable
     * @return success - FILE_OKAY / else - FILE_ERROR
     */
    int LoadMapIndexFile();
    /**
     * @brief loads save index file to m_saveIndex variable
     * @return success - FILE_OKAY / else - FILE_ERROR
     */
    int LoadSaveIndexFile();
    /**
     * @param[in] coords 
     * @brief checks if is coordinate in range of map size
     * @return true / false
     */
    bool IsValidCoordRange( const std::pair< int , int > & coords ) const;
    /**
     * @param[in] content 
     * @brief checks if content is WALL, SPACE, START or END
     * @return true / false
     */
    bool IsValidMapEntity( char content ) const;
    /**
     * @param[in] is input stream (file here)
     * @brief reads map size
     * @return success - FILE_OKAY / else - FILE_ERROR
     */
    int ReadMapSize( std::ifstream & is );
    /**
     * @param[in] content 
     * @param[in] xPos 
     * @param[in] yPos 
     * @brief adds WALL, SPACE, START or END to map
     */
    void AddToMapByLoadMap( char content , int xPos , int yPos );
    /**
     * @param[in] coords 
     * @brief add to m_contained to prevent adding more stuff to one tile
     */
    void AddToContained( std::pair< int , int > coords );
    /**
     * @param[in] xPos 
     * @param[in] yPos 
     * @brief checks if given coord is a border
     * @return true / false
     */
    bool IsBorder( int xPos , int yPos ) const;
    /**
     * @param[in] xPos 
     * @param[in] yPos 
     * @brief checks if given position is last tile of map
     * @return true / false
     */
    bool IsLastField( int xPos , int yPos ) const;
    /**
     * @param[in] xPos 
     * @param[in] yPos 
     * @brief checks if given position is a corner
     * @return true / false 
     */
    bool IsCorner( int xPos , int yPos ) const;
    /**
     * @param[in] content 
     * @param[in] xPos 
     * @param[in] yPos 
     * @brief checks if given position and given entity is allowed to be placed to given coords
     * @return true / false
     */
    bool IsValidMapPlacement( char content , int xPos , int yPos ) const;
    /**
     * @brief checks if start and end are not right next to each other, may cause problems
     * @return distance 
     */
    double StartEndDistance() const;
    /**
     * @brief checks if start and end are placed in correct spots
     * @return true / false
     */
    bool StartEndValidPlacement() const;
    /**
     * @param[in] is input stream (file here)
     * @brief reads map layout
     * @return success - FILE_OKAY / else - FILE_ERROR
     */
    int ReadMapLayout( std::ifstream & is );
    /**
     * @param[in] xSize 
     * @param[in] ySize 
     * @brief checks if map file size and save file size is equal
     * @return yes - true / no - false
     */
    bool IsEqualMapSaveSize( int xSize , int ySize ) const;
    /**
     * @param[in] is input stream (file here)
     * @brief reads map size and call IsEqualMapSaveSize
     * @return true - FILE_OKAY / else - FILE_ERROR
     */
    int ReadControlMapSize( std::ifstream & is ) const;
    /**
     * @param[in] is input stream (file here)
     * @brief reads and saves player stats
     * @return FILE_OKAY / FILE_ERROR
     */
    int ReadPlayerStats( std::ifstream & is );
    /**
     * @param[in] coords 
     * @brief checks if given coords are in m_contained list
     * @return true / false
     */
    bool IsInContained( const std::pair< int , int > & coords ) const;
    /**
     * @param[in] coords        
     * @param[in] turretType    turretType is index of nth turrent in m_towerTokens vector
     * @brief adds turret to map and to m_entities list
     */
    void AddTurret( const std::pair< int , int > & coords , int turretType );
    /**
     * @param[in] coords 
     * @param[in] enemyType     enemyType is index of nth enemy in m_enemyTokens vector
     * @param[in] HP 
     * @brief adds enemy to map, to m_enemies and to m_entities list
     */
    void AddEnemy( const std::pair< int , int > & coords , int enemyType , int HP );
    /**
     * @param[in]           is input stream (file here)
     * @param[in] unitType  type of unit (TOWER / ENEMY)
     * @brief reads unit by type, specifically TOWER or ENEMY (then calls AddTurret / addEnemy)
     */
    void ReadUnitsByType( std::ifstream & is , int unitType );
    /**
     * @param[in] is is input stream (file here)
     * @brief reads unit type and then calls ReadUnitsByType with read type
     */
    void ReadUnits( std::ifstream & is );
    /**
     * @param[in] mapPath map path
     * @brief loads map by given map path
     * @return success - FILE_OKAY / FILE_ERROR 
     */
    int LoadMap( const std::string & mapPath );
    /**
     * @param[in] savePath save path
     * @brief loads map by given save path
     * @return success - FILE_OKAY / FILE_ERROR
     */
    int LoadSave( const std::string & savePath );
    /**
     * @brief resets all crucial values for working game loader
     */
    void ResetLoader();
    /** 2D representation of game plan */
    std::vector< std::vector < std::shared_ptr< CField > > > & m_map;
    /** player stats */
    std::shared_ptr< CGameStats > m_stats;
    /** list of entities - CTower / CEnemy */
    std::list< std::shared_ptr< CEntity > > & m_entities;
    /** list of enemies */
    std::list< std::shared_ptr< CEnemy > > & m_enemies;
    /** enemy count */
    int & m_enemyCnt;
    /** pathfinder used to check if map is valid and added to CEnemy */
    std::shared_ptr< CAstar > m_pathfinder;
    std::pair< int , int > & m_mapSize, & m_start, & m_end;
    /** tower tokens used for cloning towers */
    const std::vector< std::unique_ptr< CTokenTower > > & m_towerTokens;
    /** enemy tokens used for cloning enemies */
    const std::vector< std::unique_ptr< CTokenEnemy > > & m_enemyTokens;
    /** map path leading to loaded map */
    std::string m_loadedMapPath = "";
    /** string used for regex reading */
    mutable std::string m_readString;
    /** CGameSaver object for saving */
    std::unique_ptr< CGameSaver > m_saver;
    bool startRead = false, endRead = false;
    /** set of coordinates to check if tile is already occupied */
    std::set< std::pair< int , int > > m_contains;
    /** map index file in list */
    std::list< std::pair< std::string , std::string > > m_mapIndex;
    /** save index file in list */
    std::list< std::unique_ptr< SGNode > > m_saveIndex;
};
