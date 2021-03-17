#include "CAstar.h"
#include <vector>
#include <memory>
#include <list>

CAstar::CAstar( const std::vector< std::vector< std::shared_ptr< CField > > > & map , const std::pair< int , int > & mapSize )
    : m_origMap( map ), m_mapSize( mapSize )
{
}

CAstar::~CAstar()
{
    if( m_mapCopy.size() && m_mapCopy[ 0 ].size() )
        DestroyMapCopy();
}

const std::list< std::pair< int , int > > & CAstar::GetPath()
{
    Process();
    return m_path;
}

const std::list< std::pair< int , int > > & CAstar::GetPath( int startX , int startY )
{
    m_startX = startX;
    m_startY = startY;

    Process();
    return m_path;
}

void CAstar::Prepare()
{
    if( m_mapCopy.size() && m_mapSize.second != static_cast< int >( m_mapCopy.size() ) 
     && m_mapSize.first != static_cast< int >( m_mapCopy[0].size() ) )
        DestroyMapCopy();
    CreateMapCopy();
}

bool CAstar::IsValidPath( const std::list< std::pair< int , int > > & path )
{
    if( path.empty() )
        return false;
    for( const auto & it : path )
        if( ! m_origMap[ it.second ][ it.first ]->IsThrough() )
            return false;

    return true;
}

bool CAstar::Process()
{
    m_path.clear();
    ResetMapNodes();
    CNode * start = m_mapCopy[ m_startY ][ m_startX ];
    CNode * end = m_mapCopy[ m_endY ][ m_endX ];
    std::list< CNode * > openList;
    openList.push_back( start );
    CNode * current = nullptr;

    while( ! openList.empty() )
    {
        openList.sort( CAstar::Compare );
        current = openList.front();
        openList.pop_front();
        current->m_visited = true;

        if( current == end )
            break;

        if( current->m_neighbours.empty() )
            GetNeighbours( current );

        for( auto & it : current->m_neighbours )
        {
            if( it->m_visited || it->m_pass == false )
                continue;

            if( ! IsInOpenList( openList , it ) )
            {
                it->m_parent = current;
                it->m_gScore = current->m_gScore + 1;
                it->m_hScore = GetHeuristic( it , end );
                it->m_fScore = it->m_gScore + it->m_hScore;
                openList.push_back( it );
            }
            else
            {
                if( it->m_hScore + ( current->m_gScore + 1 ) < it->m_fScore )
                {
                    it->m_parent = current;
                    it->m_gScore = current->m_gScore + 1;
                    it->m_fScore = it->m_gScore + it->m_hScore;
                }
            }
            
        }
    }
    return ConstructPath();
}

CAstar::CNode::CNode( int x , int y , bool pass )
    : m_x( x ) , m_y( y ) , m_pass( pass )
{
}

void CAstar::CNode::ResetNode( bool pass )
{
     m_fScore = m_hScore = m_gScore = 0;
     m_parent = nullptr;
     m_neighbours.clear();
     m_visited = false;
     m_pass = pass;
}

void CAstar::SetStart( int x , int y )
{
    m_startX = x;
    m_startY = y;
}

void CAstar::SetEnd( int x , int y )
{
    m_endX = x;
    m_endY = y;
}

void CAstar::CreateMapCopy()
{
    for( int i = 0 ; i < m_mapSize.second ; i++ )
    {
        m_mapCopy.push_back( std::vector< CNode * >() );
        for( int k = 0 ; k < m_mapSize.first ; k++ )
        {
            m_mapCopy[ i ].push_back( new CNode( k , i , m_origMap[ i ][ k ]->IsThrough() ) );
            if( m_origMap[ i ][ k ]->IsStart() )
                SetStart( k , i );
            else if( m_origMap[ i ][ k ]->IsEnd() )
                SetEnd( k , i );
        }
    }
}

void CAstar::DestroyMapCopy()
{
    for( int i = 0 ; i < static_cast< int >( m_mapCopy.size() ) ; i++ )
    {
        for( int k = 0 ; k < static_cast< int >( m_mapCopy[ i ].size() ) ; k++ )
            delete m_mapCopy[ i ][ k ];
        m_mapCopy[ i ].clear();
    }
    m_mapCopy.clear();
}

void CAstar::ResetMapNodes()
{
    for( int i = 0 ; i < m_mapSize.second ; i++ )
        for( int k = 0 ; k < m_mapSize.first ; k++ )
            m_mapCopy[ i ][ k ]->ResetNode( m_origMap[ i ][ k ]->IsThrough() );
}

double CAstar::GetHeuristic( const CNode * some , const CNode * end ) const
{
    return abs( end->m_x - some->m_x ) + abs( end->m_y - some->m_y );
}

bool CAstar::Compare( const CNode * first , const CNode * second )
{
    return first->m_fScore < second->m_fScore;
}

void CAstar::GetNeighbours( CNode * current )
{
    if( current->m_y + 1 < m_mapSize.second ) current->m_neighbours.push_back( m_mapCopy[ current->m_y + 1 ][ current->m_x ]);
    if( current->m_y > 0 )                    current->m_neighbours.push_back( m_mapCopy[ current->m_y - 1 ][ current->m_x ]);
    if( current->m_x + 1 < m_mapSize.first )  current->m_neighbours.push_back( m_mapCopy[ current->m_y ][ current->m_x + 1 ]);
    if( current->m_x > 0 )                    current->m_neighbours.push_back( m_mapCopy[ current->m_y ][ current->m_x - 1 ]);
}

bool CAstar::IsInOpenList( const std::list< CNode * > & openList , const CNode * elem ) const
{
    for( const auto & it : openList )
        if( it == elem )
            return true;

    return false;
}

bool CAstar::ConstructPath()
{
    CNode * tmp = m_mapCopy[ m_endY ][ m_endX ];
    while( tmp != nullptr )
    {
        m_path.push_back( std::make_pair( tmp->m_x , tmp->m_y ) );
        tmp = tmp->m_parent;
    }

    if( m_path.back().first == m_startX && m_path.back().second == m_startY )
        return true;
    
    m_path.clear();
    return false;
}
