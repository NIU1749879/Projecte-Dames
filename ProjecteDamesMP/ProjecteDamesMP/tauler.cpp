#include "tauler.h"	



void inicialitzaTauler(char tauler[N_FILES][N_COLUMNES]) {
    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++) {
            tauler[i][j] = ' ';
        }
    }
}

void llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
{
    inicialitzaTauler(tauler);

    ifstream fitxer;
    fitxer.open(nomFitxer);
    if (fitxer.is_open())
    {
        char figura;
        Posicio posicio;

        fitxer >> figura;
        fitxer >> posicio;

        tauler[posicio.getFila()][posicio.getColumna()] = figura;

        while (!fitxer.eof()) {

            fitxer >> figura;
            fitxer >> posicio;

            tauler[posicio.getFila()][posicio.getColumna()] = figura;


        }


        fitxer.close();

    }


}

void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
{
    ofstream fitxer;
    fitxer.open(nomFitxer);

    if (fitxer.is_open())
    {
        string tipusFitxa;

        for (int i = 0; i < N_FILES; i++)
        {
            for (int j = 0; j < N_COLUMNES; j++)
            {
                if (tauler[i][j] != ' ')
                {
                    Posicio posicio(i, j);
                    posicio.toString();
                    fitxer << tauler[i][j] << " ";
                    fitxer << posicio << endl;
                }


            }
        }

        fitxer.close();
    }

}

void Tauler::inicialitza(const string& nomFitxer)
{



}



void Tauler::actualitzaMovimentsValids()
{



}


void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{



}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{



}



string Tauler::toString() const
{



}




