#include "posicio.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

Posicio::Posicio(const string& pos)
{
    fromString(pos);
}


bool Posicio::operator==(const Posicio& pos) const
{
    bool iguals = false;
    if (m_fila == pos.getFila() && m_columna == pos.getColumna())
        iguals = true;
    return iguals;
}

string Posicio::toString() const
{
    string posicio = "a1";
    posicio[0] = 'a' + m_columna;
    posicio[1] = '1' + (N_FILES - 1) - m_fila;
    return posicio;
}

void Posicio::fromString(const string& pos)
{
    m_fila = (N_FILES - 1) - (pos[1] - '1');
    m_columna = pos[0] - 'a';
}

ifstream& operator>>(ifstream& fitxer, Posicio& posicio)
{
    string posAux;
    fitxer >> posAux;
    posicio.fromString(posAux);
    return fitxer;
}

ostream& operator<<(ostream& out, const Posicio& pos)
{
    string posAux = pos.toString();
    out << posAux;
    return out;
}

void llegeixFitxa(const string& nomFitxer, char& tipusFitxa, Posicio& posicio)
{
    ifstream fitxer(nomFitxer);
    fitxer >> tipusFitxa;
    fitxer >> posicio;

    fitxer.close();
}

void escriuFitxa(const string& nomFitxer, char tipusFitxa, const Posicio& posicio)
{
    ofstream fitxer(nomFitxer);
    fitxer << tipusFitxa << " ";
    fitxer << posicio;
    fitxer.close();
}