#include "CGameLogic.h"

#include <memory>
#include <vector>
#include <list>
#include <unistd.h>
#include <termios.h>
#include <chrono>
#include <thread>

CGameLogic::CGameLogic( std::vector< std::unique_ptr< CTokenTower > > & towerTokens , std::vector< std::unique_ptr< CTokenEnemy > > & enemyTokens ,
                std::list< std::shared_ptr< CEntity > > & entities, std::vector< std::vector< std::shared_ptr< CField > > > & map ,
                std::shared_ptr< CGameStats > stats , int & gameState , int & errorState , int & verticalPos , int & remainingEnemies ,
                std::pair< int , int > & mapSize , std::pair< int , int > & selectPos )
    : m_towerTokens( towerTokens ), m_enemyTokens( enemyTokens ), m_entities( entities ), m_map( map ), m_stats( stats ),
      m_gameState( gameState ), m_errorState( errorState ), m_verticalPos( verticalPos ), m_remainingEnemies( remainingEnemies ),
      m_mapSize( mapSize ), m_selectPos( selectPos )
{
    m_pathfinder = std::make_shared< CAstar >( m_map , m_mapSize );
    m_tokenLoader = std::make_unique< CTokenLoader >( m_towerTokens , m_enemyTokens , m_pathfinder );
    m_loader = std::make_unique< CGameLoader >( m_map , m_stats , m_entities , m_enemies , m_enemyCount , m_pathfinder ,
                                                m_mapSize , m_start , m_end , m_towerTokens , m_enemyTokens );
    tcgetattr( STDIN_FILENO , &m_originalConsole );
    m_newConsole = m_originalConsole;
    m_newConsole.c_lflag &= ~ICANON;
    m_echoConsole = m_originalConsole;
    m_newConsole.c_cflag &= ~ECHO;
}

void CGameLogic::GetInput() const
{
    if( m_gameState != GAME_STATE_GAME_PHASE && m_gameState != GAME_STATE_GET_NAME )
    {
        tcsetattr( STDIN_FILENO , TCSANOW , &m_newConsole );
        m_input = getchar();
        tcsetattr( STDIN_FILENO , TCSANOW , &m_originalConsole );
    }
}

bool CGameLogic::Process()
{
    if( m_errorState != ERROR_CODE_WRONG_SAVE_NAME )
        m_errorState = ERROR_CODE_NO_ERROR;
    if( m_gameState >= MAIN_MENU_DEFAULT )
        return MenuPhase();
    else if( m_gameState == GAME_STATE_GAME_PHASE )
        return GamePhase();
    else if( m_gameState == GAME_STATE_BUY_PHASE_ARRAY )
        return BuyPhase();
    else if( m_gameState == GAME_STATE_SAVE_GAME )
        return SaveNameList();
    else if( m_gameState == GAME_STATE_GET_NAME )
        return ReadSaveNameAndSave();
    else if( m_gameState == GAME_STATE_BUY_PHASE_MAP )
        return MapMovement();
    else if( m_gameState == GAME_STATE_WIN || m_gameState == GAME_STATE_LOST )
        return ProcessWinLose();
    return false;
}

const std::list< std::pair< std::string , std::string > > & CGameLogic::GetMaps() const
{
    return m_loader->GetMapIndexFile();
}

const std::list< std::unique_ptr< CGameLoader::SGNode > > & CGameLogic::GetSaves() const
{
    return m_loader->GetSaveIndexFile();
}

void CGameLogic::ResetSelectPos()
{
    m_selectPos.first = m_start.first;
    m_selectPos.second = m_start.second;
}

bool CGameLogic::IsThroughNotSE( int xPos , int yPos ) const
{
    return ! m_map[ yPos ][ xPos ]->IsEnd() && ! m_map[ yPos ][ xPos ]->IsStart() && m_map[ yPos ][ xPos ]->IsThrough();
}

void CGameLogic::EnemyMove( const std::vector< int > & coords )
{
    m_map[ coords[ 3 ] ][ coords[ 2 ] ] = m_map[ coords[ 1 ] ][ coords[ 0 ] ];
    m_map[ coords[ 1 ] ][ coords[ 0 ] ] = std::make_shared< CEmpty >( coords[ 0 ] , coords[ 1 ] );
}

void CGameLogic::EnemyDeath( const std::vector< int > & coords , std::shared_ptr< CEntity > & enemy )
{
    m_removeFromEntitiesList.push_back( enemy );
    m_stats->AddMoney( coords[ 2 ] );
    m_stats->AddScore( coords[ 2 ] );
    m_map[ coords[ 1 ] ][ coords[ 0 ] ] = std::make_shared< CEmpty >( coords[ 0 ] , coords[ 1 ] );
    m_remainingEnemies--;
}

void CGameLogic::RemoveEntities()
{
    for( auto & entity : m_removeFromEntitiesList )
    {
        auto entityPosIt = std::find( m_entities.begin() , m_entities.end() , entity );
        if( entityPosIt != m_entities.end() )
            m_entities.erase( entityPosIt );
        auto enemyPosIt = std::find( m_enemies.begin() , m_enemies.end() , entity );
        if( enemyPosIt != m_enemies.end() )
            m_enemies.erase( enemyPosIt );
        m_enemyCount--;
    }
    m_removeFromEntitiesList.clear();
}

bool CGameLogic::PrepMap()
{
    m_enemies.clear();
    m_entities.clear();
    m_enemiesQueue.clear();
    if( m_loader->LoadMapByIndex( m_verticalPos ) == FILE_OKAY )
    {
        m_remainingEnemies = NUM_OF_ENEMIES_TO_BEAT;
        m_roundModifier = m_verticalPos = m_enemyCount = 0;
        ResetSelectPos();
        m_gameState = GAME_STATE_BUY_PHASE_MAP;
        return true;
    }
    m_errorState = ERROR_CODE_FAILED_LOAD;
    return false;
}

bool CGameLogic::LoadAndPrepMap()
{
    m_enemies.clear();
    m_entities.clear();
    m_enemiesQueue.clear();
    if( m_loader->LoadSaveByIndex( m_verticalPos ) == FILE_OKAY )
    {
        m_remainingEnemies = NUM_OF_ENEMIES_TO_BEAT;
        m_roundModifier = m_verticalPos = 0;
        ResetSelectPos();
        if( static_cast< int >( m_enemies.size() ) > 0 )
            m_gameState = GAME_STATE_GAME_PHASE;
        else
        {
            m_gameState = GAME_STATE_BUY_PHASE_MAP;
            m_enemyCount = 0;
        }
        return true;
    }
    m_errorState = ERROR_CODE_FAILED_LOAD;
    return false;
}

std::pair< int , int > CGameLogic::FirstValidTile( const std::pair< int , int > & coords ) const
{
    std::pair< int , int > validPair;
    if( coords.first - 1 > 0 && IsThroughNotSE( coords.first - 1 , coords.second ) )
        validPair = std::make_pair( coords.first - 1 , coords.second );
    else if( coords.first + 1 < m_mapSize.first && IsThroughNotSE( coords.first + 1 , coords.second ) )
        validPair = std::make_pair( coords.first + 1 , coords.second );
    else if( coords.second - 1 > 0 && IsThroughNotSE( coords.first , coords.second - 1 ) )
        validPair = std::make_pair( coords.first , coords.second - 1 );
    else if( coords.second + 1 < m_mapSize.second && IsThroughNotSE( coords.first , coords.second + 1 ) )
        validPair = std::make_pair( coords.first , coords.second + 1 );
    else
        validPair = std::make_pair( -1 , -1 );
    return validPair;
}

void CGameLogic::BuyTower()
{
    m_map[ m_selectPos.second ][ m_selectPos.first ] = std::make_shared< CEmpty >( m_selectPos.first , m_selectPos.second , false , false );
    if( m_pathfinder->IsValidPath( m_pathfinder->GetPath( m_start.first , m_start.second ) ) )
    {
        m_stats->AddMoney( -1 * m_towerTokens[ m_verticalPos ]->GetCost() );
        std::shared_ptr< CTower > tmpTower = m_towerTokens[ m_verticalPos ]->Clone( m_selectPos.first , m_selectPos.second );
        m_entities.push_front( tmpTower );
        m_map[ m_selectPos.second ][ m_selectPos.first ] = tmpTower;
    }
    else
    {
        m_map[ m_selectPos.second ][ m_selectPos.first ] = std::make_shared< CEmpty >( m_selectPos.first , m_selectPos.second );
        m_errorState = ERROR_CODE_FAILED_PLACE_T;
    }
    m_verticalPos = 0;
}

void CGameLogic::DeployEnemy()
{
    std::pair< int , int > validTile = FirstValidTile( m_start );
    if( validTile.first >= 0 && validTile.second >= 0 )
    {
        m_map[ validTile.second ][ validTile.first ] = m_enemiesQueue.front();
        m_enemies.push_back( m_enemiesQueue.front() );
        m_entities.push_back( m_enemiesQueue.front() );
        m_enemiesQueue.pop_front();
    }
}

void CGameLogic::FillQueue()
{
    std::pair< int , int > validTile = FirstValidTile( m_start );
    int enemyIndex = 0;
    for( int i = 0 ; i < NUM_OF_ENEMIES_TO_DEPLOY + m_roundModifier ; i++ )
    {
        enemyIndex = rand() % static_cast< int >( m_enemyTokens.size() );
        m_enemiesQueue.push_back( m_enemyTokens[ enemyIndex ]->Clone( validTile.first , validTile.second , m_roundModifier ) );
        m_enemyCount++;
    }
}

bool CGameLogic::WinLoseCondition()
{
    m_enemies.clear();
    m_entities.clear();
    m_enemiesQueue.clear();
    m_enemyCount = m_remainingEnemies = 0;
    if( m_stats->GetHP() <= 0 )
        m_gameState = GAME_STATE_LOST;
    else
        m_gameState = GAME_STATE_WIN;
    return true;
}

bool CGameLogic::GamePhase()
{
    if( m_stats->GetHP() <= 0 || m_remainingEnemies <= 0 )
        return WinLoseCondition();

    if( m_enemies.empty() )
        FillQueue();

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    for( auto & it : m_entities )
    {
        const std::map< int , std::vector< int > > & action = it->Action();
        if( action.empty() )
            continue;
        for( const auto & actionIt : action )
        {
            if     ( actionIt.first == ACTION_SKIP )            break;
            else if( actionIt.first == ACTION_MOVE )            EnemyMove( actionIt.second );
            else if( actionIt.first == ACTION_DEATH )           EnemyDeath( actionIt.second , it );
            else if( actionIt.first == ACTION_ENEMY_END )       EnemyAtEnd( it , actionIt.second[ 0 ] , actionIt.second[ 1 ] , actionIt.second[ 2 ] );
            else if( actionIt.first == ACTION_ATTACK )          TowerAttack( actionIt.second );
            else if( actionIt.first == TOWER_SPECIAL_GENERATE ) m_stats->AddMoney( actionIt.second[ 0 ] );
        }
    }
    if( ! m_enemiesQueue.empty() )
        DeployEnemy();
    RemoveEntities();
    if( m_stats->GetHP() <= 0 || m_remainingEnemies <= 0 )
        return WinLoseCondition();
    if( m_enemyCount <= 0 )
    {
        m_gameState = GAME_STATE_BUY_PHASE_MAP;
        return true;
    }
    return false;
}

void CGameLogic::EnemyAtEnd( std::shared_ptr< CEntity > & entity , int xPos , int yPos , int damage )
{
    m_removeFromEntitiesList.push_back( entity );
    m_map[ yPos ][ xPos ] = std::make_shared< CEmpty >( xPos , yPos );
    m_stats->AddHP( -1 * damage );
}

void CGameLogic::TowerAttack( const std::vector< int > & coords )
{
    unsigned int len = coords.size();
    for( unsigned int i = 1 ; i < len ; i += 2 )
        for( auto & enemy : m_enemies )
            if( *enemy == std::make_pair( coords[ i ] , coords[ i + 1 ] ) )
                enemy->RecieveDamage( coords[ 0 ] );
}

bool CGameLogic::BuyPhase()
{
    if( m_input == CONTROL_KEY_BACK )
    {
        m_gameState = GAME_STATE_BUY_PHASE_MAP;
        m_verticalPos = 0;
        return true;
    }
    else if( m_input == CONTROL_KEY_UP && m_verticalPos > 0 )
            m_verticalPos -= 1;
    else if( m_input == CONTROL_KEY_DOWN && m_verticalPos + 1 < static_cast< int >( m_towerTokens.size() ))
            m_verticalPos += 1;
    else if( m_input == CONTROL_KEY_CONFIRM )
    {
        if( IsThroughNotSE( m_selectPos.first , m_selectPos.second ) && m_stats->GetMoney() >= m_towerTokens[ m_verticalPos ]->GetCost() )
        {
            BuyTower();
            m_gameState = GAME_STATE_BUY_PHASE_MAP;
        }
        else if( IsThroughNotSE( m_selectPos.first , m_selectPos.second ) && m_stats->GetMoney() < m_towerTokens[ m_verticalPos ]->GetCost() )
            m_errorState = ERROR_CODE_INS_FUNDS;
        else if( ! IsThroughNotSE( m_selectPos.first , m_selectPos.second ) )
            m_errorState = ERROR_CODE_FAILED_PLACE_T;
    }
    return false;
}

bool CGameLogic::ReadSaveNameAndSave()
{
    std::string name;
    std::regex save_name_pattern("^" REGEX_NAME "$", std::regex_constants::ECMAScript );
    std::smatch matches;

    tcsetattr( STDIN_FILENO , TCSANOW , &m_newConsole );
    getline( std::cin , name );
    if( std::regex_search( name , matches , save_name_pattern ) && matches[ 1 ] != "save_indexer" )
    {
        m_gameState = MAIN_MENU_DEFAULT;
        m_errorState = ERROR_CODE_NO_ERROR;
        m_loader->Save( matches[ 1 ] );
        tcsetattr( STDIN_FILENO , TCSANOW , &m_originalConsole );
        return true;
    }
    m_errorState = ERROR_CODE_WRONG_SAVE_NAME;
    tcsetattr( STDIN_FILENO , TCSANOW , &m_originalConsole );
    return false;
}

bool CGameLogic::SaveNameList()
{
    if( m_input == CONTROL_KEY_UP && m_verticalPos > 0 )
        m_verticalPos -= 1;
    else if( m_input == CONTROL_KEY_DOWN && m_verticalPos + 1 < 3 )
        m_verticalPos += 1;
    else if( m_input == CONTROL_KEY_CONFIRM )
    {
        if     ( m_verticalPos == 0 ) m_gameState = GAME_STATE_GET_NAME;
        else if( m_verticalPos == 1 ) m_gameState = MAIN_MENU_DEFAULT;
        else if( m_verticalPos == 2 ) m_gameState = GAME_STATE_BUY_PHASE_MAP;
        m_verticalPos = 0;
        return true;
    }
    return false;
}

bool CGameLogic::MenuPhase()
{
    if( m_input == CONTROL_KEY_BACK )
    {
        m_verticalPos = 0;
        if( m_gameState == MAIN_MENU_DEFAULT ) m_gameState = GAME_STATE_END_GAME;
        else                                   m_gameState = MAIN_MENU_DEFAULT;
        return true;
    }
    else if( m_input == CONTROL_KEY_UP && m_verticalPos > 0 )
        m_verticalPos -= 1;
    else if( m_input == CONTROL_KEY_DOWN )
    {
        if( ( m_gameState == MAIN_MENU_DEFAULT && m_verticalPos + 1 < MENU_SIZE ) ||
            ( m_gameState == MAIN_MENU_NEW_GAME && m_verticalPos + 1 < MapsSize() ) ||
            ( m_gameState == MAIN_MENU_LOAD_GAME && m_verticalPos + 1 < SavesSize() ) )
            m_verticalPos += 1;
    }
    else if( m_input == CONTROL_KEY_CONFIRM )
    {
        if( m_gameState == MAIN_MENU_DEFAULT )
        {
            if     ( m_verticalPos == 0 ) m_gameState = MAIN_MENU_NEW_GAME;
            else if( m_verticalPos == 1 ) m_gameState = MAIN_MENU_LOAD_GAME;
            else if( m_verticalPos == 2 ) m_gameState = MAIN_MENU_GAME_INFO;
            else if( m_verticalPos == 3 ) m_gameState = GAME_STATE_END_GAME;
            m_verticalPos = 0;
            return true;
        }
        else if( m_gameState == MAIN_MENU_NEW_GAME && MapsSize() > 0 ) return PrepMap();
        else if( m_gameState == MAIN_MENU_LOAD_GAME && SavesSize() > 0 ) return LoadAndPrepMap();
    }
    else if( m_input == CONTROL_KEY_ERASE && m_gameState == MAIN_MENU_LOAD_GAME && SavesSize() > 0 )
    {
        m_loader->Erase( m_verticalPos );
        if( m_verticalPos >= SavesSize() )
            m_verticalPos--;
    }
    return false;
}

bool CGameLogic::MapMovement()
{
    if( m_input == CONTROL_KEY_BACK )
    {
        m_gameState = GAME_STATE_SAVE_GAME;
        m_verticalPos = 0;
        return true;
    }
    else if( m_input == CONTROL_KEY_UP )
    {
        for( int tmp = m_selectPos.second - 1 ; tmp >= 0 ; tmp-- )
            if( IsThroughNotSE( m_selectPos.first , tmp ) )
            {
                m_selectPos.second = tmp;
                break;
            }
    }
    else if( m_input == CONTROL_KEY_DOWN )
    {
        for( int tmp = m_selectPos.second + 1 ; tmp < m_mapSize.second ; tmp++ )
            if( IsThroughNotSE( m_selectPos.first , tmp ) )
            {
                m_selectPos.second = tmp;
                break;
            }
    }
    else if( m_input == CONTROL_KEY_LEFT )
    {
        for( int tmp = m_selectPos.first - 1 ; tmp >= 0 ; tmp-- )
            if( IsThroughNotSE( tmp , m_selectPos.second ) )
            {
                m_selectPos.first = tmp;
                break;
            }
    }
    else if( m_input == CONTROL_KEY_RIGHT )
    {
        for( int tmp = m_selectPos.first + 1 ; tmp < m_mapSize.first ; tmp++ )
            if( IsThroughNotSE( tmp , m_selectPos.second ) )
            {
                m_selectPos.first = tmp;
                break;
            }
    }
    else if( m_input == CONTROL_KEY_CONFIRM )
    {
        m_gameState = GAME_STATE_BUY_PHASE_ARRAY;
        return true;
    }
    else if( m_input == CONTROL_KEY_WAVE )
    {
        m_roundModifier++;
        m_gameState = GAME_STATE_GAME_PHASE;
        return true;
    }
    else if( m_input == CONTROL_KEY_MONEY_CHEAT )
        m_stats->AddMoney( 1000 );
    if( ! IsThroughNotSE( m_selectPos.first , m_selectPos.second ) && FirstValidTile( m_selectPos ).first < 0 )
        ResetSelectPos();
    return false;
}

bool CGameLogic::ProcessWinLose()
{
    if( m_input == CONTROL_KEY_CONFIRM )
    {
        m_verticalPos = 0;
        m_gameState = MAIN_MENU_DEFAULT;
        return true;
    }
    return false;
}

int CGameLogic::MapsSize() const
{
    return static_cast< int >( m_loader->GetMapIndexFile().size() );
}

int CGameLogic::SavesSize() const
{
    return static_cast< int >( m_loader->GetSaveIndexFile().size() );
}
