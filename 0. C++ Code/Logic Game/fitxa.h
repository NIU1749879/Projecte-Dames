#ifndef FITXA_H
#define FITXA_H

#include "posicio.h"
#include "moviment.h"
#include <vector>

class Tauler;

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
    Fitxa() : m_tipus(TIPUS_EMPTY), m_color(COLOR_NEGRE) {}
    Fitxa(TipusFitxa tipus, ColorFitxa color, const Posicio& posicioFitxa) : m_tipus(tipus), m_color(color), m_posicioFitxa(posicioFitxa) {}

    TipusFitxa getTipus() const { return m_tipus; }
    ColorFitxa getColor() const { return m_color; }
    Moviment getMoviment(int posicioMoviment) const { return m_movimentsValids[posicioMoviment]; }
    int getNMoviments() const { return (int)m_movimentsValids.size(); }
    Posicio getPosicio() const { return m_posicioFitxa; }

    void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
    void setColor(ColorFitxa color) { m_color = color; }
    void setMoviment(const Moviment& moviment) { m_movimentsValids.push_back(moviment); }
    void setPosicio(const Posicio& posicio) { m_posicioFitxa = posicio; }

    void actualitzarMovimentsValids(const Tauler& tauler, const Posicio& posicio);

    void movimentsNormal(const Tauler& tauler, const Posicio& posicio);
    void movimentsNormalMatar(const Tauler& tauler, const Posicio& posicio);
    void movimentsDama(const Tauler& tauler, const Posicio& posicio);
    void movimentsDamaMatar(const Tauler& tauler, const Posicio& posicio);

    void afegirMovNormalDama(const Tauler& tauler, const Posicio& posicio, int fila, int columna);
    void intentarCaptura(const Tauler& tauler, Moviment movimentActual, std::vector<std::vector<bool>>& jaCapturada, int filaOrigen, int columnaOrigen, int direccioFila, int direccioColumna, bool esDama);
    void encadenarCaptures(const Tauler& tauler, Moviment movimentActual, std::vector<std::vector<bool>>& jaCapturada, int filaActual, int columnaActual);

    void convertirEnDama() { m_tipus = TIPUS_DAMA; }

    void visualitza(float baseX, float baseY) const;

private:
    TipusFitxa m_tipus;
    ColorFitxa m_color;
    std::vector<Moviment> m_movimentsValids;
    Posicio m_posicioFitxa;
};

#endif
