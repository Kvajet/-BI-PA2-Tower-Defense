#include "CGameLoader.h"

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

CGameLoader::CGameLoader( std::vector< std::vector < std::shared_ptr< CField > > > & map , std::shared_ptr< CGameStats > stats ,
                std::list< std::shared_ptr< CEntity > > & entities , std::list< std::shared_ptr< CEnemy > > & enemies ,
                int & enemyCnt , std::shared_ptr< CAstar > pathfinder , std::pair< int , int > & mapSize ,
                std::pair< int , int > & start , std::pair< int , int > & end ,
                const std::vector< std::unique_ptr< CTokenTower > > & towerTokens ,
                const std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens )
    : m_map( map ) , m_stats( stats ) , m_entities( entities ) , m_enemies( enemies ) , m_enemyCnt( enemyCnt ) , 
      m_pathfinder( pathfinder ) , m_mapSize( mapSize ) , m_start( start ) , m_end( end ) ,
      m_towerTokens( towerTokens ) , m_enemyTokens( enemyTokens )
{
    m_saver = std::make_unique< CGameSaver >( m_entities , m_mapSize );
    LoadMapIndexFile();
    LoadSaveIndexFile();
}

const std::list< std::pair< std::string , std::string > > & CGameLoader::GetMapIndexFile() const
{
    return m_mapIndex;
}

const std::list< std::unique_ptr< CGameLoader::SGNode > > & CGameLoader::GetSaveIndexFile() const
{
    return m_saveIndex;
}

int CGameLoader::LoadMapByIndex( size_t index )
{
    if( index > m_mapIndex.size() )
        return FILE_ERROR;
    
    ResetLoader();
    auto it = m_mapIndex.begin();
    std::advance( it , index );
    return LoadMap( it->first );
}

int CGameLoader::LoadSaveByIndex( size_t index )
{
    ResetLoader();
    if( index >= m_saveIndex.size() )
        return FILE_ERROR;

    auto it = m_saveIndex.begin();
    std::advance( it , index );

    if( LoadMap( (*it)->m_mapPath ) == FILE_ERROR )
    {
        ResetLoader();
        return FILE_ERROR;
    }

    return LoadSave( (*it)->m_savePath );
}

int CGameLoader::Save( const std::string & saveName )
{
    EraseIfInSaveIndexer( saveName );
    int state = m_saver->Save( saveName , m_loadedMapPath , m_stats->GetHP() , m_stats->GetScore() , m_stats->GetMoney() );
    m_saveIndex.clear();
    LoadSaveIndexFile();
    return state;
}

void CGameLoader::Erase( size_t index )
{
    auto it = m_saveIndex.begin();
    std::advance( it , index );
    remove( ( SAVE_PREFIX + (*it)->m_savePath ).c_str() );
    std::ofstream os;
    os.open( SAVE_INDEXER );
    for( const auto & save : m_saveIndex )
        if( (*it) != save )
            os << save->m_mapPath << "|" << save->m_savePath << "|" << save->m_name << "|" << save->m_date << std::endl;
    os.close();
    m_saveIndex.clear();
    LoadSaveIndexFile();
}

void CGameLoader::EraseIfInSaveIndexer( const std::string & saveName )
{
    size_t index = 0;
    for( const auto & save : m_saveIndex )
    {
        if( save->m_name == saveName )
        {
            Erase( index );
            break;
        }
        index++;
    }
}

int CGameLoader::LoadMapIndexFile()
{
    std::ifstream is;
    std::string mapPath , mapName;
    std::regex map_index_pattern("^" REGEX_MAP_PATH "\\|" REGEX_NAME "$", std::regex_constants::ECMAScript );
    std::smatch matches;
    
    is.open( MAP_INDEXER );
    if( ! is.good() )
        return FILE_ERROR;
        
    while( is.peek() != EOF )
    {
        mapPath.clear();
        mapName.clear();
        getline( is , m_readString );

        if( std::regex_search( m_readString , matches , map_index_pattern ) )
        {
            mapPath = matches[ 1 ];
            mapName = matches[ 2 ];
            m_mapIndex.push_back( std::make_pair( mapPath , mapName ) );
        }
    }

    is.close();

    return FILE_OKAY;
}

int CGameLoader::LoadSaveIndexFile()
{
    std::ifstream is;
    std::string mapPath, savePath, saveName, date;
    std::regex save_index_pattern("^" REGEX_MAP_PATH "\\|" REGEX_SAVE_PATH "\\|" REGEX_NAME "\\|" REGEX_TIME "$", std::regex_constants::ECMAScript );
    std::smatch matches;
    is.open( SAVE_INDEXER );
    if( ! is.good() )
        return FILE_ERROR;

    while( is.peek() != EOF )
    {
        mapPath.clear();
        savePath.clear();
        saveName.clear();
        date.clear();
        getline( is , m_readString );

        if( std::regex_search( m_readString , matches , save_index_pattern ) )
        {
            mapPath = matches[ 1 ];
            savePath = matches[ 2 ];
            saveName = matches[ 3 ];
            date = matches[ 4 ];
            m_saveIndex.push_back( std::make_unique< CGameLoader::SGNode >( mapPath , savePath , saveName , date ) );
        }
    }
    is.close();
    return FILE_OKAY;
}

bool CGameLoader::IsValidCoordRange( const std::pair< int , int > & coords ) const
{
    return coords.first  >= 0 && coords.first < m_mapSize.first && coords.second >= 0 && coords.second < m_mapSize.second;
}

bool CGameLoader::IsValidMapEntity( char content ) const
{
    return ( content == BASE_ENTITY_SPACE || content == BASE_ENTITY_START || content == BASE_ENTITY_END || content == BASE_ENTITY_WALL );
}

int CGameLoader::ReadMapSize( std::ifstream & is )
{
    int xSize, ySize;
    std::stringstream ss;
    getline( is , m_readString );
    ss << m_readString;

    if( ! ( ss >> xSize ) || ! ( ss >> ySize ) || ! ss.eof() )
        return FILE_ERROR;

    m_mapSize.first = xSize;
    m_mapSize.second = ySize;

    return FILE_OKAY;
}

void CGameLoader::AddToMapByLoadMap( char content , int xPos , int yPos )
{
    if( content != BASE_ENTITY_SPACE )
        AddToContained( std::make_pair( xPos , yPos ) );
    if( content == BASE_ENTITY_START )
    {
        m_map[ yPos ].push_back( ( std::make_shared< CEmpty >( xPos , yPos , true , false ) ) );
        startRead = true;
        m_start = std::make_pair( xPos , yPos );
    }
    else if( content == BASE_ENTITY_END )
    {
        m_map[ yPos ].push_back( ( std::make_shared< CEmpty >( xPos , yPos , false , true ) ) );
        endRead = true;
        m_end = std::make_pair( xPos , yPos );
    }
    else if( content == BASE_ENTITY_WALL )
        m_map[ yPos ].push_back( ( std::make_shared< CEmpty >( xPos , yPos , false , false ) ) );
    else
        m_map[ yPos ].push_back( ( std::make_shared< CEmpty >( xPos , yPos ) ) );
}

void CGameLoader::AddToContained( std::pair< int , int > coords )
{
    m_contains.insert( coords );
}

bool CGameLoader::IsBorder( int xPos , int yPos ) const
{
    return ( xPos == 0 || xPos + 1 == m_mapSize.first || yPos == 0 || yPos + 1 == m_mapSize.second );
}

bool CGameLoader::IsLastField( int xPos , int yPos ) const
{
    return ( xPos + 1 == m_mapSize.first && yPos + 1 == m_mapSize.second );
}

bool CGameLoader::IsCorner( int xPos , int yPos ) const
{
    return (   ( xPos     == 0               && yPos     == 0 )
            || ( xPos     == 0               && yPos + 1 == m_mapSize.second )
            || ( xPos + 1 == m_mapSize.first && yPos     == 0 )
            || ( xPos + 1 == m_mapSize.first && yPos + 1 == m_mapSize.second ));
}

bool CGameLoader::IsValidMapPlacement( char content , int xPos , int yPos ) const
{
    // there is maximum of one START and one END, checks if they were already
    // loaded and current content char is not START or END as well
    if( ( content == BASE_ENTITY_START && startRead ) || ( content == BASE_ENTITY_END && endRead ) )
        return false;

    // checks if START or END is defined to be at corners
    // what leads to unplayable map design
    if( ( content == BASE_ENTITY_START || content == BASE_ENTITY_END ) && ( IsCorner( xPos , yPos ) || ! IsBorder( xPos , yPos ) ) )
        return false;

    // checks if START or END should be placed to game plan, not to border
    if( ( content == BASE_ENTITY_START || content == BASE_ENTITY_END ) && ! IsBorder( xPos , yPos ) )
        return false;

    // when placed object is empty space checks if is not at border of map
    // what leads to unplayable map design
    if( content == BASE_ENTITY_SPACE && IsBorder( xPos , yPos ) )
        return false;

    // checks if START and AND were read
    if( IsLastField( xPos , yPos ) && ( ! startRead || ! endRead ) )
        return false;

    return true;
}

double CGameLoader::StartEndDistance() const
{
    return std::sqrt( std::pow( m_end.first - m_start.first , 2 ) + std::pow( m_end.second - m_start.second , 2 ) );
}

bool CGameLoader::StartEndValidPlacement() const
{
    return ! ( StartEndDistance() < MIN_START_END_DISTANCE );
}

int CGameLoader::ReadMapLayout( std::ifstream & is )
{
    std::stringstream ss;
    char readChar;

    m_map.clear();
    for( int i = 0 ; i < m_mapSize.second ; i++ )
    {
        ss.clear();
        getline( is , m_readString );
        ss << m_readString;
        m_map.push_back( std::vector< std::shared_ptr< CField > >() );

        for( int k = 0 ; k < m_mapSize.first ; k++ )
        {
            ss >> std::noskipws >> readChar;
            if( ! IsValidMapEntity( readChar ) || ( k + 2 != m_mapSize.first && ss.eof() ) || ! IsValidMapPlacement( readChar , k , i ) )
            {
                ResetLoader();
                return FILE_ERROR;
            }
            AddToMapByLoadMap( readChar , k , i );
        }
        if( ss >> readChar )
        {
            ResetLoader();
            return FILE_ERROR;
        }
    }
    m_pathfinder->Prepare();
    if( is.peek() != EOF || ! StartEndValidPlacement() || ! m_pathfinder->IsValidPath( m_pathfinder->GetPath( m_start.first , m_start.second ) ) )
    {
        ResetLoader();
        return FILE_ERROR;
    }
    return FILE_OKAY;
}

bool CGameLoader::IsEqualMapSaveSize( int xSize , int ySize ) const
{
    return ( xSize == m_mapSize.first && ySize == m_mapSize.second );
}

int CGameLoader::ReadControlMapSize( std::ifstream & is ) const
{
    int xSize , ySize;
    std::regex size_pattern("^" REGEX_NUMBER " " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    std::smatch matches;

    getline( is , m_readString );
    if( std::regex_search( m_readString , matches , size_pattern ) && std::stoi( matches[ 1 ] ) > 0 && std::stoi( matches[ 2 ] ) > 0 )
    {
        xSize = std::stoi( matches[ 1 ] );
        ySize = std::stoi( matches[ 2 ] );
    }
    if( xSize <= 0 || ySize <= 0 || ! IsEqualMapSaveSize( xSize , ySize ) )
        return FILE_ERROR;
    return FILE_OKAY;
}

int CGameLoader::ReadPlayerStats( std::ifstream & is )
{
    std::regex stats_pattern("^" REGEX_NUMBER " " REGEX_NUMBER " " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    std::smatch matches;

    getline( is , m_readString );
    if( std::regex_search( m_readString , matches , stats_pattern ) && std::stoi( matches[ 1 ] ) > 0 )
    {
        m_stats->SetHP( std::stoi( matches[ 1 ] ) );
        m_stats->SetScore( std::stoi( matches[ 2 ] ) );
        m_stats->SetMoney( std::stoi( matches[ 3 ] ) );
        return FILE_OKAY;
    }
    return FILE_ERROR;
}

bool CGameLoader::IsInContained( const std::pair< int , int > & coords ) const
{
    return ( m_contains.find( coords ) != m_contains.end() );
}

void CGameLoader::AddTurret( const std::pair< int , int > & coords , int turretType )
{
    if( turretType > 0 && turretType <= static_cast< int >( m_towerTokens.size() ) )
    {
        AddToContained( coords );
        std::shared_ptr< CTower > tmpTower = m_towerTokens[ turretType - 1 ]->Clone( coords.first , coords.second );
        m_map[ coords.second ][ coords.first ] = tmpTower;
        m_entities.push_front( tmpTower );
    }
}

void CGameLoader::AddEnemy( const std::pair< int , int > & coords , int enemyType , int HP )
{
    if( enemyType > 0 && enemyType <= static_cast< int >( m_enemyTokens.size() ) )
    {
        AddToContained( coords );
        std::shared_ptr< CEnemy > tmpEnemy = m_enemyTokens[ enemyType - 1 ]->Clone( coords.first , coords.second , HP );
        m_map[ coords.second ][ coords.first ] = tmpEnemy;
        m_entities.push_back( tmpEnemy );
        m_enemies.push_back( tmpEnemy );
        m_enemyCnt++;
    }
}

void CGameLoader::ReadUnitsByType( std::ifstream & is , int unitType )
{
    int unitTypeNumber, HP = 0;
    std::pair< int , int > coords;
    std::regex single_number_pattern("^" REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    std::regex coords_pattern("^" REGEX_NUMBER " " REGEX_NUMBER "$", std::regex_constants::ECMAScript );
    std::smatch matches;

    while( isdigit( is.peek() ) )
    {
        getline( is , m_readString );
        if( std::regex_search( m_readString , matches , single_number_pattern ) )
            unitTypeNumber = std::stoi( matches[ 1 ] );
        else
            continue;
        if( unitType == ENEMY )
        {
            getline( is , m_readString );
            if( std::regex_search( m_readString , matches , single_number_pattern ) )
                HP = std::stoi( matches[ 1 ] );
        }        

        while( isdigit( is.peek() ) )
        {
            getline( is , m_readString );
            if( std::regex_search( m_readString , matches , coords_pattern ) )
            {
                coords.first = std::stoi( matches[ 1 ] );
                coords.second = std::stoi( matches[ 2 ] );
            }
            else
                continue;
            if( ! IsValidCoordRange( coords ) || IsInContained( coords ) )
                continue;
            if( unitType == TOWER )
                AddTurret( coords , unitTypeNumber );
            else
                AddEnemy( coords , unitTypeNumber , HP );
        }
        if( is.peek() == '\n' )
            getline( is , m_readString );
    }
}

void CGameLoader::ReadUnits( std::ifstream & is )
{
    std::regex enemy_tower_pattern("^" REGEX_ENEMY_TOWER "$", std::regex_constants::ECMAScript );
    std::smatch matches;
    do{
        getline( is , m_readString );
        if( std::regex_search( m_readString , matches , enemy_tower_pattern ) )
        {
            if( matches[ 1 ] == "TOWER" )
                ReadUnitsByType( is , TOWER );
            else
                ReadUnitsByType( is , ENEMY );
        }
    } while( is.peek() != EOF );
}

int CGameLoader::LoadMap( const std::string & mapPath )
{
    std::ifstream is;
    is.open( MAP_PREFIX + mapPath );

    if( ! is.good() || ReadMapSize( is ) == FILE_ERROR || ReadMapLayout( is ) == FILE_ERROR )
        return FILE_ERROR;
    is.close();
    m_loadedMapPath = mapPath;
    return FILE_OKAY;
}

int CGameLoader::LoadSave( const std::string & savePath )
{
    std::pair< int , int > coords;
    std::ifstream is;
    is.open( SAVE_PREFIX + savePath );

    if( ! is.good() || ReadControlMapSize( is ) == FILE_ERROR || ReadPlayerStats( is ) == FILE_ERROR )
        return FILE_ERROR;
    ReadUnits( is );
    m_pathfinder->Prepare();
    if( ! m_pathfinder->IsValidPath( m_pathfinder->GetPath( m_start.first , m_start.second ) ) )
        return FILE_ERROR;

    is.close();

    return FILE_OKAY;
}

void CGameLoader::ResetLoader()
{
    m_map.clear();
    m_contains.clear();
    m_mapSize.first = m_mapSize.second = 0;
    m_start.first = m_start.second = 0;
    m_end.first = m_end.second = 0;
    startRead = endRead = false;
    m_stats->SetHP( GAME_STATS_INITIAL_HP );
    m_stats->SetScore( GAME_STATS_INITIAL_SCORE );
    m_stats->SetMoney( GAME_STATS_INITIAL_MONEY );
}
