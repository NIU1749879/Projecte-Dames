#ifndef TAULER_H
#define TAULER_H

#include "posicio.h"
#include <string>
#include "fitxa.h"


void inicialitzaTauler(char tauler[N_FILES][N_COLUMNES]);
void llegeixTauler(const std::string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
void escriuTauler(const std::string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);

class Tauler {
public:
    Tauler();

    void inicialitza(const string& nomFitxer);
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    string toString() const;

    const Fitxa& getFitxa(int fila, int columna) const { return m_tauler[fila][columna]; }

private:
    Fitxa m_tauler[N_FILES][N_COLUMNES];

};



#endif