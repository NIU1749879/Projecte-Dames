#include "fitxa.h"
#include "tauler.h"
#include "info_joc.hpp"
#include "GraphicManager.h"


// Actualitza la llista de moviments vàlids per a aquesta fitxa
void Fitxa::actualitzarMovimentsValids(const Tauler& tauler, const Posicio& posicio)
{
    m_movimentsValids.clear();  // Netejem moviments antics

    // Si es normal
    if (m_tipus == TIPUS_NORMAL)
    {
        movimentsNormal(tauler, posicio);
        movimentsNormalMatar(tauler, posicio);
    }
    // Si es dama
    if (m_tipus == TIPUS_DAMA)
    {
        movimentsDama(tauler, posicio);
        movimentsDamaMatar(tauler, posicio);
    }
}

// Genera moviments diagonals senzills per a fitxa normal
void Fitxa::movimentsNormal(const Tauler& tauler, const Posicio& posicio)
{
    int fila = posicio.getFila();
    int columna = posicio.getColumna();
    // Direccio de moviment segons color: negre cap avall, blanca cap amunt
    int direccio;
    if (m_color == COLOR_NEGRE)
        direccio = +1;
    else
        direccio = -1;

    // Comprovem diagonal a la dreta
    int f2 = fila + direccio;
    int c2 = columna + 1;
    if (f2 >= 0 && f2 < N_FILES && c2 >= 0 && c2 < N_COLUMNES)
    {
        Fitxa* dest = tauler.getFitxa(f2, c2);
        if (dest == nullptr)  // si esta buida
        {
            Moviment mov;
            mov.setPosicio(posicio);              // posicio origen
            mov.setPosicio(Posicio(f2, c2));      // posicio destí
            mov.setNMatades(0);                   // sense captures
            m_movimentsValids.push_back(mov);     // afegir moviment
        }
    }

    // Comprovem diagonal a l’esquerra
    f2 = fila + direccio;
    c2 = columna - 1;
    if (f2 >= 0 && f2 < N_FILES && c2 >= 0 && c2 < N_COLUMNES)
    {
        Fitxa* dest = tauler.getFitxa(f2, c2);
        if (dest == nullptr)
        {
            Moviment mov;
            mov.setPosicio(posicio);
            mov.setPosicio(Posicio(f2, c2));
            mov.setNMatades(0);
            m_movimentsValids.push_back(mov);
        }
    }
}

// Genera moviments de dama
void Fitxa::movimentsDama(const Tauler& tauler, const Posicio& posicio)
{
    int dirs[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    for (int d = 0; d < 4; ++d)
    {
        int df = dirs[d][0];
        int dc = dirs[d][1];
        int distancia = 1;
        int f2 = posicio.getFila() + df * distancia;
        int c2 = posicio.getColumna() + dc * distancia;

        // Mentre estigui dins del tauler i buida
        while (f2 >= 0 && f2 < N_FILES && c2 >= 0 && c2 < N_COLUMNES && tauler.getFitxa(f2, c2) == nullptr)
        {
            Moviment mov;
            mov.setPosicio(posicio);  // origen
            // Afegir totes les posicions intermedies fins a distancia
            for (int k = 1; k <= distancia; ++k)
            {
                Posicio nova(posicio.getFila() + df * k, posicio.getColumna() + dc * k);
                mov.setPosicio(nova);
            }
            mov.setNMatades(0);
            m_movimentsValids.push_back(mov);

            // Incrementar distancia en aquesta direccio
            ++distancia;
            f2 = posicio.getFila() + df * distancia;
            c2 = posicio.getColumna() + dc * distancia;
        }
    }
}

// Inicia calcul de captures encadenades per a fitxa normal
void Fitxa::movimentsNormalMatar(const Tauler& tauler, const Posicio& posicio)
{
    // Matriu de control de captures ja realitzades
    std::vector<std::vector<bool>> jaCapturada(N_FILES, std::vector<bool>(N_COLUMNES, false));
    Moviment mov;
    mov.setPosicio(posicio);  // posar origen
    mov.setNMatades(0);
    // Crida recursiva per encadenar captures
    encadenarCaptures(tauler, mov, jaCapturada, posicio.getFila(), posicio.getColumna());
}

// Inicia calcul de captures encadenades per a dama
void Fitxa::movimentsDamaMatar(const Tauler& tauler, const Posicio& posicio)
{
    std::vector<std::vector<bool>> jaCapturada(N_FILES, std::vector<bool>(N_COLUMNES, false));
    Moviment mov;
    mov.setPosicio(posicio);
    mov.setNMatades(0);
    encadenarCaptures(tauler, mov, jaCapturada, posicio.getFila(), posicio.getColumna());
}


void Fitxa::intentarCaptura(const Tauler& tauler, Moviment movActual, std::vector<std::vector<bool>>& jaCapturada, int fila, int columna, int df, int dc, bool esDama)
{
    if (!esDama)
    {
        //Comprovar que la direccio de captura es cap endavant
        int direccio = -1;
        if (m_color == COLOR_NEGRE)
            direccio = 1;

        bool direccioOK = false;
        if (df == direccio)
            direccioOK = true;

        if (direccioOK)
        {
            // Posicio mig i desti per a fitxa normal
            int fm = fila + df;
            int cm = columna + dc;
            int fd = fila + 2 * df;
            int cd = columna + 2 * dc;
            bool dins = (fm >= 0 && fm < N_FILES && cm >= 0 && cm < N_COLUMNES && fd >= 0 && fd < N_FILES && cd >= 0 && cd < N_COLUMNES);
            if (dins)
            {
                Fitxa* mig = tauler.getFitxa(fm, cm);
                Fitxa* dest = tauler.getFitxa(fd, cd);
                // Comprovar fitxa contraria, desti buit i no repetida
                if (mig != nullptr && mig->getColor() != m_color && dest == nullptr && !jaCapturada[fm][cm])
                {
                    Moviment nou = movActual;
                    nou.setNMatades(movActual.getNMatades() + 1);
                    nou.setPosicio(Posicio(fm, cm));  // fitxa capturada
                    nou.setPosicio(Posicio(fd, cd));  // nova posicio
                    jaCapturada[fm][cm] = true;
                    encadenarCaptures(tauler, nou, jaCapturada, fd, cd);
                    // Si es mes llarga, afegir moviment
                    if (nou.getNPosicions() > movActual.getNPosicions())
                        m_movimentsValids.push_back(nou);
                    jaCapturada[fm][cm] = false;
                }
            }
        }
    }
    else
    {
        // Captura per dama: saltar buits fins trobar possible peça rival
        int f2 = fila + df;
        int c2 = columna + dc;
        while (f2 >= 0 && f2 < N_FILES && c2 >= 0 && c2 < N_COLUMNES && tauler.getFitxa(f2, c2) == nullptr)
        {
            f2 += df;
            c2 += dc;
        }
        bool dins = (f2 >= 0 && f2 < N_FILES && c2 >= 0 && c2 < N_COLUMNES);
        if (dins)
        {
            Fitxa* mig = tauler.getFitxa(f2, c2);
            int fd = f2 + df;
            int cd = c2 + dc;
            bool dins2 = (fd >= 0 && fd < N_FILES && cd >= 0 && cd < N_COLUMNES);
            // Comprovar peça rival i destí buit
            if (mig != nullptr && mig->getColor() != m_color && dins2 && tauler.getFitxa(fd, cd) == nullptr && !jaCapturada[f2][c2])
            {
                Moviment nou = movActual;
                nou.setNMatades(movActual.getNMatades() + 1);
                nou.setPosicio(Posicio(f2, c2));
                nou.setPosicio(Posicio(fd, cd));
                jaCapturada[f2][c2] = true;
                encadenarCaptures(tauler, nou, jaCapturada, fd, cd);
                if (nou.getNPosicions() > movActual.getNPosicions())
                    m_movimentsValids.push_back(nou);
                jaCapturada[f2][c2] = false;
            }
        }
    }
}

// Recorrer totes les quatre diagonals per encadenar captures
void Fitxa::encadenarCaptures(const Tauler& tauler,
    Moviment movActual,
    std::vector<std::vector<bool>>& jaCapturada,
    int fila, int columna)
{
    bool dama = (m_tipus == TIPUS_DAMA);
    // Provar totes les direccions
    intentarCaptura(tauler, movActual, jaCapturada, fila, columna, +1, +1, dama);
    intentarCaptura(tauler, movActual, jaCapturada, fila, columna, +1, -1, dama);
    intentarCaptura(tauler, movActual, jaCapturada, fila, columna, -1, +1, dama);
    intentarCaptura(tauler, movActual, jaCapturada, fila, columna, -1, -1, dama);

    // Si hi ha hagut alguna captura, afegir la seqüència sencera
    if (movActual.getNPosicions() > 1)
        m_movimentsValids.push_back(movActual);
}




void Fitxa::visualitza(float baseX, float baseY) const
{
    int fila = m_posicioFitxa.getFila();
    int col = m_posicioFitxa.getColumna();
    float x = baseX + CASELLA_INICIAL_X + col * AMPLADA_CASELLA;
    float y = baseY + CASELLA_INICIAL_Y + fila * ALCADA_CASELLA;

    //Selecciona la imatge que toca segons tipus i color
    IMAGE_NAME img;
    if (m_tipus == TIPUS_NORMAL)
    {
        if (m_color == COLOR_BLANC)
            img = GRAFIC_FITXA_BLANCA;
        else
            img = GRAFIC_FITXA_NEGRA;
    }
    else
    {
        if (m_color == COLOR_BLANC)
            img = GRAFIC_DAMA_BLANCA;
        else
            img = GRAFIC_DAMA_NEGRA;
    }

    GraphicManager::getInstance()->drawSprite(img, x, y);
}
