#ifndef JOC_H
#define JOC_H

#include <stdio.h>
#include "info_joc.hpp"
#include "CuaMoviments.h"
#include "tauler.h"
#include "moviment.h"
#include <vector>
#include <string>



class Joc
{

public:
    Joc() : m_mode(MODE_JOC_NONE), m_nomFitxerMoviments(), m_jugadorActual(COLOR_BLANC), m_guanyadorPartida(COLOR_BLANC), m_finalPartida(false), m_mousePremsatAnterior(false) {}
    ~Joc() {};

    void inicialitza(ModeJoc mode, const std::string& nomFitxerTauler, const std::string& nomFitxerMoviments);

    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);

    void finalitza();


private:
    ModeJoc m_mode;
    Tauler m_tauler;
    CuaMoviments m_cuaMoviments;
    std::string m_nomFitxerMoviments;
    ColorFitxa m_jugadorActual;
    Posicio m_posicioSeleccionada;
    std::vector<Posicio> m_posicionsValidas;
    ColorFitxa m_guanyadorPartida;
    bool m_finalPartida;
    bool calculaPosicioClick(int mousePosX, int mousePosY, Posicio& posicioOut) const;
    bool m_mousePremsatAnterior;
};

#endif 
