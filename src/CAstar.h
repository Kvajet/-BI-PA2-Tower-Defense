#pragma once
#include "CField.h"
#include <signal.h>
#include <vector>
#include <memory>
#include <list>

/**
 * @class CAstar
 * @brief A star search algorithm used for finding shortest path to the end. Enemies are able able to move
 * left, right, up and down only, so it checks only 4 neighbours each time. Manhattan distance method is used for heuristic.
 * Uses private CNode class for representation of map.
 * For more info: https://en.wikipedia.org/wiki/A*_search_algorithm
 */
class CAstar
{
public:
    /**
     * @param[in] map       2D representation of game plan
     * @param[in] mapSize
     */
    CAstar( const std::vector< std::vector< std::shared_ptr< CField > > > & map , const std::pair< int , int > & mapSize );
    /**
     * @brief releases CNode map copy if needed
     */
    ~CAstar();
    /**
     * @brief returns shortest path from the start point to the end
     * @return list of coordinates
     */
    const std::list< std::pair< int , int > > & GetPath();
    /**
     * @param[in] startX    start xPos coordinate
     * @param[in] startY    start yPos coordinate
     * @brief returns shortest path from the start point to the end with custom start point
     * @return list of coordinates
     */
    const std::list< std::pair< int , int > > & GetPath( int startX , int startY );
    /**
     * @brief resets map copy for A* search when new map is loaded
     */
    void Prepare();
    /**
     * @param path list of map coordinates
     * @brief checks if given path is valid path to the end
     * @return valid - true / invalid - false
     */
    bool IsValidPath( const std::list< std::pair< int , int > > & path );
    /**
     * @brief creates path from start (m_startX, m_startY) to the end (m_endX, m_endY)
     * @return path to the end: exists - true / doesn't exist - false
     */
    bool Process();
private:
    /**
     * @class CNode
     * @brief container representing map tile
     */
    class CNode
    {
    public:
        /**
         * @param[in] x     xPos
         * @param[in] y     yPos
         * @param[in] pass  if something can go through this tile
         */
        CNode( int x , int y , bool pass );
        /**
         * @param[in] pass  if something can go through this tile
         * @brief resets node to default state
         */
        void ResetNode( bool pass );
        /** vector of neighbours to be controlled */
        std::vector< CNode * > m_neighbours;
        /** path cost from start */
        int m_gScore = 0;
        /** xPos */
        int m_x;
        /** yPos */
        int m_y;
        /** if something can go through this tile */
        bool m_pass;
        /** if node was already visited */
        bool m_visited = false;
        /** heurstic cost to the end */
        double m_hScore = 0;
        /** total cost for the path, G score + H score */
        double m_fScore = 0;
        /** parent node pointer */
        CNode * m_parent = nullptr;
    };
    /**
     * @param[in] x xPos
     * @param[in] y yPos
     */
    void SetStart( int x , int y );
    /**
     * @param[in] x xPos
     * @param[in] y yPos
     */
    void SetEnd( int x , int y );
    /**
     * @brief copies original map to CNode map copy
     */
    void CreateMapCopy();
    /**
     * @brief frees memory allocated by CreateMapCopy
     */
    void DestroyMapCopy();
    /**
     * @brief resets all map copy CNodes to initial state
     */
    void ResetMapNodes();
    /**
     * @param[in] some  from node
     * @param[in] end   to node
     * @brief gets herustic based on Manhattan distance method
     * @return heuristic value - double
     */
    double GetHeuristic( const CNode * some , const CNode * end ) const;
    /**
     * @param[in] first     first node to be compared
     * @param[in] second   second node to be compared
     * @brief static method for std::sort method to compare CNodes
     * @return same node - true / different - false
     */
    static bool Compare( const CNode * first , const CNode * second );
    /**
     * @param[in,out] current   CNode finding neighbour nodes
     * @brief finds neighbour CNodes and adds them to m_neighbours vector
     */
    void GetNeighbours( CNode * current );
    /**
     * @param[in] openList  list of CNodes to visit
     * @param[in] elem      CNode checked if is in open list
     * @return is in - true / is not - false
     */
    bool IsInOpenList( const std::list< CNode * > & openList , const CNode * elem ) const;
    /**
     * @brief constructs maps path from end CNode to start CNode by accessing parent pointer
     * @return able to construct path - true / else - false
     */
    bool ConstructPath();
    int m_startX, m_startY, m_endX, m_endY;
    /** list of coordinates from the start to the end */
    std::list< std::pair< int , int > > m_path;
    /** copy of original map consisting of CNodes */
    std::vector< std::vector< CNode * > > m_mapCopy;
    /** original map reference - used for reset of map copy nodes */
    const std::vector< std::vector< std::shared_ptr< CField > > > & m_origMap;
    const std::pair< int , int > & m_mapSize;
};
