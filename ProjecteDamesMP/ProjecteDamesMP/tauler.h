#ifndef TAULER_H
#define TAULER_H

#include "posicio.h"

void inicialitzaTauler(char tauler[N_FILES][N_COLUMNES]);
void llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);


#include <string>
#include "fitxa.h"
#include "posicio.h"

class Tauler {
public:
    Tauler();

    void inicialitza(const string& nomFitxer);
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    string toString() const;

private:
    Fitxa m_tauler[N_FILES][N_COLUMNES];

};



#endif