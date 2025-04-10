#include "posicio.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


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

ofstream& operator<<(ofstream& fitxer, const Posicio& posicio)
{
    string posAux = posicio.toString();
    fitxer << posAux;
    return fitxer;
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