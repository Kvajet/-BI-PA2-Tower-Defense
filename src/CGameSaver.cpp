#include "CGameSaver.h"

#include <fstream>
#include <string>
#include <list>
#include <memory>
#include <ctime>

CGameSaver::CGameSaver( const std::list< std::shared_ptr< CEntity > > & entities , const std::pair< int , int > & mapSize )
    : m_entities( entities ) , m_mapSize( mapSize )
{
}

int CGameSaver::Save( const std::string & saveName , const std::string & mapPath , int HP , int score , int money )
{
    std::ofstream os;
    os.open( SAVE_PREFIX + saveName + SAVE_POSTFIX );

    if( ! os.good() )
        return FILE_ERROR;

    SaveSizeAndStats( os , HP , score , money );
    SaveEntities( os );
    os.close();

    if( SaveToIndexer( saveName , mapPath ) == FILE_ERROR )
        return FILE_ERROR;

    return FILE_OKAY;
}

void CGameSaver::RefreshTime()
{
    std::time_t tmp = std::time( nullptr );
    m_actTime = std::localtime( &tmp );
}

void CGameSaver::SaveEntities( std::ofstream & os )
{
    for( const auto & entity : m_entities )
    {
        entity->Save( os );
        os << std::endl;
    }
}

void CGameSaver::SaveSizeAndStats( std::ofstream & os , int HP , int score , int money )
{
    os << m_mapSize.first << " " << m_mapSize.second << std::endl;
    os << HP << " " << score << " " << money << std::endl << std::endl;
}

int CGameSaver::SaveToIndexer( const std::string & saveName , const std::string & mapPath )
{
    std::ofstream indexer;
    indexer.open( SAVE_INDEXER , std::ofstream::out | std::ofstream::app );
    if( ! indexer.good() )
        return FILE_ERROR;
    
    RefreshTime();
    indexer << std::endl << mapPath << "|" << saveName << SAVE_POSTFIX << "|" << saveName
            << "|" << m_actTime->tm_mday << "." << m_actTime->tm_mon + 1 << "." << m_actTime->tm_year + 1900 << " "
            << m_actTime->tm_hour << ":" << m_actTime->tm_min << ":" << m_actTime->tm_sec << std::endl;
    indexer.close();
    return FILE_OKAY;
}
