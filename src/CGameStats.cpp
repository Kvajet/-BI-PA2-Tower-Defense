#include "CGameStats.h"
#include <iostream>

CGameStats::CGameStats( int HP , int score , int money )
    : m_HP( HP ) , m_score( score ) , m_money( money )
{
}

int CGameStats::GetHP() const
{
    return m_HP;
}

int CGameStats::GetScore() const
{
    return m_score;
}

int CGameStats::GetMoney() const
{
    return m_money;
}

void CGameStats::AddHP( int HP )
{
    m_HP += HP;
}

void CGameStats::AddScore( int score )
{
    m_score += score;
}

void CGameStats::AddMoney( int money )
{
    m_money += money;
}

void CGameStats::SetHP( int HP )
{
    m_HP = HP;
}

void CGameStats::SetScore( int score )
{
    m_score = score;
}

void CGameStats::SetMoney( int money )
{
    m_money = money;
}
