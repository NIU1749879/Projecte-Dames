#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "posicio.h"

const int MAX_POSICIONS_PER_MOVIMENT = 20;


class Moviment {
public:
	Moviment() : m_nPosicions(0) {}

	Posicio getPosicio( int numeroPosicio) { return m_posicions[ numeroPosicio]; }
	int getNPosicions() { return m_nPosicions; }
	
	void setPosicio(int numeroPosicio, Posicio posicio) { m_posicions[numeroPosicio] = posicio; }
	void setNPosicions(int nPosicions) { m_nPosicions = nPosicions; }

private:
	Posicio m_posicions[MAX_POSICIONS_PER_MOVIMENT];
	int m_nPosicions;

};

#endif 

