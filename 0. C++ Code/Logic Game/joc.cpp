//
//  CurrentGame.cpp
//  LearnChess
//
//  Created by Enric Vergara on 21/2/22.
//

#include "joc.hpp"
#include <iostream>
#include <fstream>
#include "GraphicManager.h"


bool Joc::calculaPosicioClick(int mousePosX, int mousePosY, Posicio& posicioOut) const
{
    int relX = mousePosX - (POS_X_TAULER + CASELLA_INICIAL_X);
    int relY = mousePosY - (POS_Y_TAULER + CASELLA_INICIAL_Y);
    int col = relX / AMPLADA_CASELLA;
    int fil = relY / ALCADA_CASELLA;
    bool dins = (fil >= 0 && fil < NUM_FILES_TAULER && col >= 0 && col < NUM_COLS_TAULER);
    if (dins)
    {
        posicioOut.setFila(fil);
        posicioOut.setColumna(col);
    }
    return dins;
}

void Joc::inicialitza(ModeJoc mode, const std::string& nomFitxerTauler, const std::string& nomFitxerMoviments)
{
    m_mode = mode;
    m_nomFitxerMoviments = nomFitxerMoviments;

    // Netejar cua de moviments i estat final
    m_cuaMoviments.clear();
    m_finalPartida = false;

    // El blanc comença
    m_jugadorActual = COLOR_BLANC;

    m_tauler.inicialitza(nomFitxerTauler);

    // Si és mode REPLAY, carregar moviments del fitxer
    if (m_mode == MODE_JOC_REPLAY)
    {
        std::ifstream fitxer(m_nomFitxerMoviments);
        if (!fitxer.is_open()) {
            std::cerr << "Error: no s'ha pogut obrir " << m_nomFitxerMoviments << "\n";
        }
        if (fitxer.is_open())
        {
            std::string orig, dest;
            while (fitxer >> orig >> dest)
            {
                Moviment mov;
                mov.setPosicio(Posicio(orig));
                mov.setPosicio(Posicio(dest));
                mov.setNMatades(0);
                m_cuaMoviments.push(mov);
            }
            fitxer.close();
        }
    }

    m_tauler.actualitzaMovimentsValids();
}

bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus)
{
    // Dibuixar tauler i peces
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
    m_tauler.visualitza();

    // Dibuixar posicions valides
    for (size_t k = 0; k < m_posicionsValidas.size(); ++k)
    {
        Posicio p = m_posicionsValidas[k];
        float x = POS_X_TAULER + CASELLA_INICIAL_X + p.getColumna() * AMPLADA_CASELLA;
        float y = POS_Y_TAULER + CASELLA_INICIAL_Y + p.getFila() * ALCADA_CASELLA;
        GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, x, y);
    }

    // mostrar mode de joc
    std::string modeText = "Mode Joc: ";
    if (m_mode == MODE_JOC_NORMAL)
        modeText += "Normal";
    else
        modeText += "Replay";
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, 630, 1.0f, modeText);

    // mostrar text del torn actual
    std::string torn = "Jugador actual: ";
    if (m_jugadorActual == COLOR_BLANC)
        torn += "Blanc";
    else
        torn += "Negre";
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, 665, 1.0f, torn);

    m_tauler.actualitzaMovimentsValids();

    // Detectar ratoli
    bool clicRatoli = (mouseStatus && !m_mousePremsatAnterior);

    bool acabada = false;

    //Si la partida ha acabat, mostrar guanyador
    if (m_finalPartida)
    {
        std::string msg = "Guanyador: ";
        if (m_guanyadorPartida == COLOR_BLANC)
            msg += "Blanc";
        else
            msg += "Negre";
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, 700, 1.0f, msg);
        acabada = true;
    }

    if (!acabada)
    {
        if (m_mode == MODE_JOC_NORMAL)
        {
            bool clicRatoli = (mouseStatus && !m_mousePremsatAnterior);
            if (clicRatoli)
            {
                Posicio clic;
                if (calculaPosicioClick(mousePosX, mousePosY, clic))
                {
                    if (m_posicionsValidas.empty())
                    {
                        Fitxa* f = m_tauler.getFitxa(clic.getFila(), clic.getColumna());
                        if (f != nullptr && f->getColor() == m_jugadorActual)
                        {
                            m_posicioSeleccionada = clic;
                            Posicio possibles[NUM_FILES_TAULER * NUM_COLS_TAULER];
                            int n = 0;
                            m_tauler.getPosicionsPossibles(m_posicioSeleccionada, n, possibles);
                            m_posicionsValidas.clear();
                            for (int i = 0; i < n; ++i)
                                m_posicionsValidas.push_back(possibles[i]);
                        }
                    }
                    else
                    {
                        bool valid = false;
                        for (size_t i = 0; i < m_posicionsValidas.size(); ++i)
                        {
                            if (m_posicionsValidas[i] == clic)
                                valid = true;
                        }
                        if (valid)
                        {
                            bool fet = m_tauler.mouFitxa(m_posicioSeleccionada, clic);
                            if (fet)
                            {
                                // Afegir a la cua de moviments
                                Moviment mov;
                                mov.setPosicio(m_posicioSeleccionada);
                                mov.setPosicio(clic);
                                mov.setNMatades(0);
                                m_cuaMoviments.push(mov);
                                // Canviar torn
                                if (m_jugadorActual == COLOR_BLANC)
                                    m_jugadorActual = COLOR_NEGRE;
                                else
                                    m_jugadorActual = COLOR_BLANC;
                            }
                            //Detectar final de partida
                            int nFitxesBlanques = 0;
                            int nFitxesNegres = 0;
                            for (int f = 0; f < N_FILES; f++)
                            {
                                for (int c = 0; c < N_COLUMNES; c++)
                                {
                                    Fitxa* fitxaActual = m_tauler.getFitxa(f, c);
                                    if (fitxaActual != nullptr)
                                    {
                                        if (fitxaActual->getColor() == COLOR_BLANC)
                                            nFitxesBlanques = nFitxesBlanques + 1;
                                        else
                                            nFitxesNegres = nFitxesNegres + 1;
                                    }
                                }
                            }
                            if (nFitxesBlanques == 0 || nFitxesNegres == 0)
                            {
                                m_finalPartida = true;
                                if (nFitxesBlanques == 0)
                                    m_guanyadorPartida = COLOR_NEGRE;
                                else
                                    m_guanyadorPartida = COLOR_BLANC;
                            }
                        }
                        m_posicionsValidas.clear();
                    }
                }
            }
        }
        else if (m_mode == MODE_JOC_REPLAY)
        {
            // Si no queden moviments però la partida no ha acabat, mostrar avis
            if (m_cuaMoviments.empty() && !m_finalPartida)
            {
                GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, 700, 1.0f, "No hi ha mes moviments per reproduir");
            }
            else
            {                
                if (clicRatoli && !m_cuaMoviments.empty())
                {
                    // Reproduir el seguent moviment de la cua
                    Moviment mov;
                    if (m_cuaMoviments.pop(mov))
                    {
                        Posicio o = mov.getPosicio(0);
                        Posicio d = mov.getPosicio(1);
                        m_tauler.mouFitxa(o, d);
                    }
                }
            }
        }
    }
    m_mousePremsatAnterior = mouseStatus;
    return acabada;
}






void Joc::finalitza()
{
    if (m_mode == MODE_JOC_NORMAL)
    {
        std::ofstream fitxer(m_nomFitxerMoviments);
        if (fitxer.is_open())
        {
            // Guardar cada moviment: origen i desti
            Moviment mov;
            while (m_cuaMoviments.pop(mov))
            {
                Posicio o = mov.getPosicio(0);
                Posicio d = mov.getPosicio(1);
                fitxer << o.toString() << " " << d.toString() << "\n";
            }
            fitxer.close();
        }
    }
}