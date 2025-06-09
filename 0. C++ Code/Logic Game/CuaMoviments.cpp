#include "CuaMoviments.h"

CuaMoviments::CuaMoviments()
    : m_head(nullptr)
    , m_tail(nullptr)
{}

CuaMoviments::~CuaMoviments()
{
    clear();
}

void CuaMoviments::push(const Moviment& mov)
{
    Node* nou = new Node{ mov, nullptr };
    if (m_tail == nullptr)
    {
        m_head = nou;
        m_tail = nou;
    }
    else
    {
        m_tail->next = nou;
        m_tail = nou;
    }
}

bool CuaMoviments::pop(Moviment& movOut)
{
    bool resultat = false;
    if (m_head != nullptr)
    {
        Node* prim = m_head;
        movOut = prim->mov;
        m_head = prim->next;
        if (m_head == nullptr)
            m_tail = nullptr;
        delete prim;
        resultat = true;
    }
    return resultat;
}

bool CuaMoviments::empty() const
{
    return (m_head == nullptr);
}

void CuaMoviments::clear()
{
    while (m_head != nullptr)
    {
        Node* actual = m_head;
        m_head = m_head->next;
        delete actual;
    }
    m_tail = nullptr;
}