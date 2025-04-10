#ifndef TAULER_H
#define TAULER_H

#include "posicio.h"

void inicialitzaTauler(char tauler[N_FILES][N_COLUMNES]);
void llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);

#endif
