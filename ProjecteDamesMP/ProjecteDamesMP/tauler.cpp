#include "tauler.h"	

Tauler::Tauler()
{
    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            m_tauler[i][j] = Fitxa();
        }
    }
}

void Tauler::inicialitza(const string& nomFitxer)
{
    for (int i = 0; i < N_FILES; i++) {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            m_tauler[i][j] = Fitxa();
        }
    }

    char tauler[N_FILES][N_COLUMNES];
    llegeixTauler(nomFitxer, tauler);

    char figura;

    Fitxa fitxa;

    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            figura = tauler[i][j];
            switch (figura)
            {
            case 'O':
                fitxa.setTipus(TIPUS_NORMAL);
                fitxa.setColor(COLOR_BLANC);
                break;
            case 'X':
                fitxa.setTipus(TIPUS_NORMAL);
                fitxa.setColor(COLOR_NEGRE);
                break;
            case 'D':
                fitxa.setTipus(TIPUS_DAMA);
                fitxa.setColor(COLOR_BLANC);
                break;
            case 'R':
                fitxa.setTipus(TIPUS_DAMA);
                fitxa.setColor(COLOR_NEGRE);
                break;
            default:
                fitxa.setTipus(TIPUS_EMPTY);
                break;
            }
            Posicio posicio(i, j);
            fitxa.setPosicio(posicio);
            m_tauler[i][j] = fitxa;
        }
    }
}



void Tauler::actualitzaMovimentsValids()
{
    for (int i = 0; i < N_FILES; i++) {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            Posicio posicio(i, j);
            m_tauler[i][j].actualitzarMovimentsValids(*this, posicio);
        }
    }
}


void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
    nPosicions = 0;
    int fila = origen.getFila();
    int columna = origen.getColumna();
    int total = m_tauler[fila][columna].getNMoviments();

    for (int i = 0; i < total; i++) {
        Moviment m = m_tauler[fila][columna].getMoviment(i);
        int nPassos = m.getNPosicions();
        if (nPassos > 1) {
            Posicio dest = m.getPosicio(nPassos - 1);
            bool trobat = false;
            for (int k = 0; k < nPosicions; ++k) {
                if (posicionsPossibles[k] == dest) { trobat = true; break; }
            }
            if (trobat == false) {
                posicionsPossibles[nPosicions++] = dest;
            }
        }
    }
}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    int filaInicial = origen.getFila();
    int columnaInicial = origen.getColumna();

    Fitxa fitxa = m_tauler[filaInicial][columnaInicial];
    m_tauler[filaInicial][columnaInicial] = Fitxa();

    bool capturaPossible = false;

    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            Fitxa f = m_tauler[i][j];
            if (f.getTipus() != TIPUS_EMPTY) {
                f.actualitzarMovimentsValids(*this, Posicio(i, j));
                int n = f.getNMoviments();
                for (int k = 0; k < n; ++k) {
                    if (f.getMoviment(k).getNPosicions() > 2) {
                        capturaPossible = true;
                    }
                }
            }
        }
    }

    fitxa.actualitzarMovimentsValids(*this, origen);
    int nMoviments = fitxa.getNMoviments();
    int maxPassos = 0;

    for (int i = 0; i < nMoviments; ++i) {
        int passos = fitxa.getMoviment(i).getNPosicions();
        if (passos > maxPassos) {
            maxPassos = passos;
        }
    }

    bool moguda = false;
    bool trobat = false;
    bool trobatMax = false;
    Moviment moviment;
    for (int i = 0; i < nMoviments; ++i) {
        Moviment m = fitxa.getMoviment(i);
        int passos = m.getNPosicions();
        Posicio final = m.getPosicio(passos - 1);
        if (final == desti) 
        {
            trobat = true;
            if(passos == maxPassos)
            {
                trobatMax = true;
                moviment = m;
                i = nMoviments;
            }
        }
    }
    if (trobat == false || capturaPossible == true && maxPassos == 2 || trobatMax == false)
    {
        m_tauler[filaInicial][columnaInicial] = fitxa;
        moguda = false;
    }
    else
    {
        int passosSel = moviment.getNPosicions();

        for (int j = 1; j < passosSel; j += 2) {
            Posicio cap = moviment.getPosicio(j);
            m_tauler[cap.getFila()][cap.getColumna()] = Fitxa();
        }

        fitxa.setPosicio(desti);

        if (fitxa.getTipus() == TIPUS_NORMAL) {
            if ((fitxa.getColor() == COLOR_BLANC && desti.getFila() == N_FILES - 1) || (fitxa.getColor() == COLOR_NEGRE && desti.getFila() == 0))
            {
                fitxa.convertirEnDama();
            }
        }

        m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
        moguda = true;
    }
          
        
     
    

    return moguda;
}





string Tauler::toString() const
{
    string resultat;

    for (int i = 0; i < N_FILES; i++) 
    {
        resultat = resultat + char('0' + (N_FILES - i));
        resultat = resultat + ':';
        resultat = resultat + ' ';

        for (int j = 0; j < N_COLUMNES; j++) 
        {
            Fitxa fitxa = m_tauler[i][j];
            char c = '_';

            if (fitxa.getTipus() != TIPUS_EMPTY) 
            {
                if (fitxa.getTipus() == TIPUS_NORMAL) 
                {
                    if (fitxa.getColor() == COLOR_BLANC)
                    {
                        c = 'O';
                    }
                    else 
                    {
                        c = 'X';
                    }
                }
                else 
                {
                    if (fitxa.getColor() == COLOR_BLANC)
                    {
                        c = 'D';
                    }
                    else
                    {
                        c = 'R';
                    }
                }
            }

            resultat = resultat + c;
            resultat = resultat +  ' ';
        }
        resultat = resultat + '\n';
    }

    resultat = resultat + "  a b c d e f g h\n";

    return resultat;

}




