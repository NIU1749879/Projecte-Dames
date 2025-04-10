#ifndef FITXA_H
#define FITXA_H

#include "posicio.h"
#include "moviment.h"

const int MAX_MOVIMENTS = 4;


typedef enum
{
    TIPUS_NORMAL,
    TIPUS_DAMA,
    TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
    COLOR_NEGRE,
    COLOR_BLANC,
} ColorFitxa;



class Fitxa {    
public:
    Fitxa() : m_tipus(TIPUS_EMPTY), m_color(COLOR_NEGRE), m_nMoviments(0) {}
    Fitxa(TipusFitxa tipus, ColorFitxa color, const Posicio& posicioFitxa) : m_tipus(tipus), m_color(color), m_posicioFitxa(posicioFitxa) {}

    TipusFitxa getTipus() const { return m_tipus; }
    ColorFitxa getColor() const { return m_color; }
    Moviment getMoviment(int posicioMoviment) { return m_movimentsValids[posicioMoviment]; }
    int getNMoviments() { return m_nMoviments; }
    Posicio getPosicio() { return m_posicioFitxa; }

    void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
    void setColor(ColorFitxa color) { m_color = color; }
    void setMoviment(Moviment moviment, int posicioMoviment) { m_movimentsValids[posicioMoviment] = moviment; }
    void setNMoviments(int nMoviments) { m_nMoviments = nMoviments; }
    void setPosicio(Posicio posicio) { m_posicioFitxa = posicio; }

    void actualitzarMovimentsValids(const Tauler& tauler, const Posicio& posicio);



private:
    TipusFitxa m_tipus;
    ColorFitxa m_color;
    Moviment m_movimentsValids[MAX_MOVIMENTS]; 
    int m_nMoviments;
    Posicio m_posicioFitxa;
};

#endif /
