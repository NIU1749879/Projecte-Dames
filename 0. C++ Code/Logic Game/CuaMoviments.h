#ifndef CUA_MOVIMENTS_H
#define CUA_MOVIMENTS_H

#include "moviment.h"

class CuaMoviments {
public:
    CuaMoviments();
    ~CuaMoviments();

    // Empeny un moviment al final de la cua
    void push(const Moviment& mov);

    // Treu el primer moviment; retorna true si hi havia un element, false si era buida
    bool pop(Moviment& movOut);

    // Retorna true si esta buida
    bool empty() const;

    // Esborra tots els nodes
    void clear();

private:
    struct Node {
        Moviment mov;
        Node* next;
    };
    Node* m_head;
    Node* m_tail;
};

#endif