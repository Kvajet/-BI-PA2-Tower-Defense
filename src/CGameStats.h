#pragma once
#include <iostream>

/**
 * @class CGameStats
 * @brief CGameStats is container for player's info about money, score and health points.
 */
class CGameStats
{
public:
    /**
     * @param HP        initial HP value, 0 if not entered
     * @param score     initial score value, 0 if not entered
     * @param money     initial money value, 0 if not entered
     */
    CGameStats( int HP = 0 , int score = 0 , int money = 0 );
    int GetHP() const;
    int GetScore() const;
    int GetMoney() const;
    /** @brief adds HP to current m_HP, subtraction is achieved by adding negative value */
    void AddHP( int HP );
    /** @brief adds score to current m_score, subtraction is achieved by adding negative value */
    void AddScore( int score );
    /** @brief adds money to current m_money, subtraction is achieved by adding negative value */
    void AddMoney( int money );
    void SetHP( int HP );
    void SetScore( int score );
    void SetMoney( int money );
private:
    int m_HP, m_score, m_money;
};
