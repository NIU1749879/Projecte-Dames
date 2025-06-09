#ifndef TAULER_H
#define TAULER_H

#include "posicio.h"
#include <string>
#include "fitxa.h"
#include <vector>

void llegeixTauler(const std::string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
void escriuTauler(const std::string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);

class Tauler {
public:
    Tauler();
    ~Tauler();

    void inicialitza(const std::string& nomFitxer);
    void buidarTauler();
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    std::string toString() const;
    Fitxa* getFitxa(int fila, int columna) const { return m_tauler[fila][columna]; }

    void visualitza() const;

private:
    std::vector<std::vector<Fitxa*>> m_tauler;
};



#endif