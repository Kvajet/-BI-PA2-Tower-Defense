#pragma once
#include "CField.h"

#include <fstream>
#include <map>
#include <vector>

/**
 * @class CEntity
 * @brief CEntity is virtual class used for inheritance and polymorphism.
 * Has virtual method Action and Save.
 */
class CEntity : public CField
{
public:
    /**
     * @param[in] xPos      entity x position
     * @param[in] yPos      entity y position
     * @param[in] m_trough  defines if entities able to move through
     * @param[in] id        entity ID, only used for saving
     */
    CEntity( int xPos , int yPos , bool m_trough , int id );
    /**
     * @brief assigns action to entity, method to be overriden by derived class, polymorphism usage
     * @return map of actions and content
     */
    virtual const std::map< int , std::vector< int > > & Action() = 0;
    /**
     * @param[out] os 
     * @brief saves entity, method to be overriden by derived class, polymorphism usage
     */
    virtual void Save( std::ofstream & os ) = 0;
protected:
    /** entity ID, only used for saving */
    int m_typeID;
    /** map of actions - key defines action (defined in game_constants.cpp) and vector contains data for action */
    mutable std::map< int , std::vector< int > > m_action;
private:
};
