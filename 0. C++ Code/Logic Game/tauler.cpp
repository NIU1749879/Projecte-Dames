#include "tauler.h"	
#include "GraphicManager.h"
#include "info_joc.hpp"



#include <iostream>
#include <fstream>



Tauler::Tauler()
{
    m_tauler.resize(N_FILES);   //Ajustar el nombre de files
    for (int i = 0; i < N_FILES; ++i)
        m_tauler[i].resize(N_COLUMNES, nullptr);    //Ajustar el nombre de columnes
}

Tauler::~Tauler()
{
    buidarTauler();
}


void llegeixTauler(const std::string& nomFitxer, char estatTauler[N_FILES][N_COLUMNES])
{
    // Inicialitzar tot a “_”
    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
            estatTauler[i][j] = '_';
    }

    // Obrir el fitxer
    std::ifstream fitxer(nomFitxer);
    bool ok = fitxer.is_open();
    if (!ok) {
        std::cerr << "Error obrint " << nomFitxer << "\n";
    }
    else {
        // Llegir parells <fitxa> <posició>
        char fitxa;
        std::string pos;
        while (fitxer >> fitxa >> pos) {
            int j = pos[0] - 'a';
            int i = N_FILES - (pos[1] - '0');
            if (i >= 0 && i < N_FILES && j >= 0 && j < N_COLUMNES) {
                estatTauler[i][j] = fitxa;
            }
        }
        fitxer.close();
    }
}



//DEixa totes les caselles a nullptr
void Tauler::buidarTauler()
{
    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            if (m_tauler[i][j] != nullptr)  //Si no esta a nullptr, la posa en nullptr.
            {
                delete m_tauler[i][j];
                m_tauler[i][j] = nullptr;
            }
        }
    }
}



void Tauler::inicialitza(const std::string& nomFitxer)
{
    buidarTauler();     //Buida com estava el tauler
    char estat[N_FILES][N_COLUMNES];
    llegeixTauler(nomFitxer, estat);    //Omple la matriu 

    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            char c = estat[i][j];
            TipusFitxa tipus = TIPUS_EMPTY;
            ColorFitxa color = COLOR_NEGRE;
            if (c == 'O') {     //depenent de la fitxa que sigui s'assigna un color i el tipus que es.
                tipus = TIPUS_NORMAL;
                color = COLOR_BLANC;
            }
            else if (c == 'X') {
                tipus = TIPUS_NORMAL;
                color = COLOR_NEGRE;
            }
            else if (c == 'D') {
                tipus = TIPUS_DAMA;
                color = COLOR_BLANC;
            }
            else if (c == 'R') {
                tipus = TIPUS_DAMA;
                color = COLOR_NEGRE;
            }
            if (tipus != TIPUS_EMPTY) {     //Si hi ha una fitxa s'afegeix al tauler.
                m_tauler[i][j] = new Fitxa(tipus, color, Posicio(i, j));
            }
        }
    }
}

void Tauler::actualitzaMovimentsValids()
{
    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            Fitxa* f = m_tauler[i][j];
            if (f != nullptr)
                f->actualitzarMovimentsValids(*this, Posicio(i, j));
        }
    }
}


void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const
{
    nPosicions = 0;
    int fila = origen.getFila();
    int columna = origen.getColumna();
    Fitxa* f = getFitxa(fila, columna);
    if (f != nullptr)
    {
        int total = f->getNMoviments();
        for (int i = 0; i < total; ++i)
        {
            Moviment m = f->getMoviment(i);
            int passos = m.getNPosicions();
            if (passos > 1)
            {
                Posicio dest = m.getPosicio(passos - 1);
                // Evitar duplicats  
                bool trobat = false;
                for (int k = 0; k < nPosicions; ++k)
                {
                    if (posicionsPossibles[k] == dest)
                        trobat = true;
                }
                if (!trobat)
                    posicionsPossibles[nPosicions++] = dest;
            }
        }
    }
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    bool moguda = false;
    // Mirar posicions de peces amb captures maximes
    Fitxa* fOrigen = getFitxa(origen.getFila(), origen.getColumna());
    if (fOrigen) {
        ColorFitxa colorJugador = fOrigen->getColor();

        int globalMaxCaptures = 0;
        std::vector<Posicio> posMaxCaptures;
        for (int i = 0; i < N_FILES; ++i) {
            for (int j = 0; j < N_COLUMNES; ++j) {
                Fitxa* f = getFitxa(i, j);
                if (f && f->getColor() == colorJugador) {
                    f->actualitzarMovimentsValids(*this, Posicio(i, j));
                    int localMax = 0;
                    int nm = f->getNMoviments();
                    for (int k = 0; k < nm; ++k)
                        localMax = std::max(localMax, f->getMoviment(k).getNMatades());
                    if (localMax > globalMaxCaptures) {
                        globalMaxCaptures = localMax;
                        posMaxCaptures.clear();
                        posMaxCaptures.push_back(Posicio(i, j));
                    }
                    else if (localMax == globalMaxCaptures) {
                        posMaxCaptures.push_back(Posicio(i, j));
                    }
                }
            }
        }

        fOrigen->actualitzarMovimentsValids(*this, origen);
        // recalcular moviments d'origen
        int total = fOrigen->getNMoviments();
        if (total > 0)
        {
            // trobar moviment triat
            int maxCapturesOrigen = 0;
            Moviment movTriat;
            bool trobat = false;
            for (int i = 0; i < total; ++i) {
                Moviment m = fOrigen->getMoviment(i);
                maxCapturesOrigen = std::max(maxCapturesOrigen, m.getNMatades());
                if (m.getPosicio(m.getNPosicions() - 1) == desti) {
                    movTriat = m;
                    trobat = true;
                }
            }

            if (trobat)
            {
                TipusFitxa tipus = fOrigen->getTipus();

                // Elimina la fitxa d'origen              
                delete m_tauler[origen.getFila()][origen.getColumna()];
                m_tauler[origen.getFila()][origen.getColumna()] = nullptr;
                // Elimina les fitxes capturades
                for (int j = 1; j < movTriat.getNPosicions(); j += 2)
                {
                    Posicio cap = movTriat.getPosicio(j);
                    delete m_tauler[cap.getFila()][cap.getColumna()];
                    m_tauler[cap.getFila()][cap.getColumna()] = nullptr;
                }

                //Canviar a dama
                if (tipus == TIPUS_NORMAL) {
                    if ((colorJugador == COLOR_BLANC && desti.getFila() == 0) ||
                        (colorJugador == COLOR_NEGRE && desti.getFila() == N_FILES - 1)) {
                        tipus = TIPUS_DAMA;
                    }
                }

                // Crea i posar la nova fitxa al desti
                Fitxa* nova = new Fitxa(tipus, colorJugador, desti);
                m_tauler[desti.getFila()][desti.getColumna()] = nova;

                //bufar si no es fa el millor moviment
                if (movTriat.getNMatades() < globalMaxCaptures) {
                    bool origenEsMax = false;
                    for (int idx = 0; idx < (int)posMaxCaptures.size(); ++idx) {
                        if (posMaxCaptures[idx] == origen)
                            origenEsMax = true;
                    }
                    if (origenEsMax) {
                        delete m_tauler[desti.getFila()][desti.getColumna()];
                        m_tauler[desti.getFila()][desti.getColumna()] = nullptr;
                    }
                    else {
                        Posicio p = posMaxCaptures.front();
                        delete m_tauler[p.getFila()][p.getColumna()];
                        m_tauler[p.getFila()][p.getColumna()] = nullptr;
                    }
                }
                moguda = true;
            }
        }
    }

    return moguda;
}

std::string Tauler::toString() const
{
    std::string resultat;
    for (int i = 0; i < N_FILES; ++i)
    {
        // Nombre de fila al principi  
        resultat += char('0' + (N_FILES - i));
        resultat += ": ";
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            Fitxa* f = m_tauler[i][j];
            char c = '_';  // casella buida 

            if (f != nullptr)
            {
                // Si es normal
                if (f->getTipus() == TIPUS_NORMAL)
                {
                    if (f->getColor() == COLOR_BLANC)
                        c = 'O';
                    else
                        c = 'X';
                }
                // Si es dama
                else
                {
                    if (f->getColor() == COLOR_BLANC)
                        c = 'D';
                    else
                        c = 'R';
                }
            }

            resultat += c;
            resultat += ' ';
        }
        resultat += '\n';
    }
    // Etiquetes de columnes
    resultat += "  a b c d e f g h\n";
    return resultat;
}



void Tauler::visualitza() const
{
    //Dibuixar tauler
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);

    //Dibuixar cada fitxa
    for (int i = 0; i < NUM_FILES_TAULER; ++i)
    {
        for (int j = 0; j < NUM_COLS_TAULER; ++j)
        {
            Fitxa* f = m_tauler[i][j];
            if (f != nullptr)
            {
                f->visualitza(POS_X_TAULER, POS_Y_TAULER);
            }
        }
    }

}