#ifndef POSICIO_H
#define POSICIO_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

const int N_FILES = 8;
const int N_COLUMNES = 8;

class Posicio
{
public:
    Posicio() : m_fila(-1), m_columna(-1) {}
    Posicio(int fila, int columna) : m_fila(fila), m_columna(columna) {}
    Posicio(const string& pos);

    int getFila() const { return m_fila; }
    int getColumna() const { return m_columna; }

    void setFila(int fila) { m_fila = fila ;  }
    void setColumna(int columna) { m_columna = columna; }

    string toString() const;
    void fromString(const string& pos);

    bool operator==(const Posicio& pos) const;

private:
    int m_fila, m_columna;
};

ifstream& operator>>(ifstream& fitxer, Posicio& posicio);
ostream& operator<<(ostream& out, const Posicio& pos);


#endif 