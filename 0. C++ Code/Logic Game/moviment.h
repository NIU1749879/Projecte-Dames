#pragma once
#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "posicio.h"
#include <vector>



class Moviment {
public:
	Moviment() : m_nMatades(0) {}

	Posicio getPosicio(int numeroPosicio) const { return m_posicions[numeroPosicio]; }

	int getNPosicions() const { return (int)m_posicions.size(); }
	int getNMatades() const { return m_nMatades; }


	void setPosicio(const Posicio& posicio) { m_posicions.push_back(posicio); }
	void setNMatades(int nMatades) { m_nMatades = nMatades; }


private:
	std::vector<Posicio> m_posicions;
	int m_nMatades;

};

#endif 

