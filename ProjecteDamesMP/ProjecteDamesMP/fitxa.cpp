#include "fitxa.h"
#include "tauler.h"

void Fitxa::actualitzarMovimentsValids(const Tauler& tauler, const Posicio& posicio)
{
	m_nMoviments = 0;

	if (m_tipus == TIPUS_NORMAL)
	{
		movimentsNormal(tauler, posicio);
		movimentsNormalMatar(tauler, posicio);
	}

	if (m_tipus == TIPUS_DAMA)
	{
		movimentsDama(tauler, posicio);
		movimentsDamaMatar(tauler, posicio);
	}
}

void Fitxa::movimentsNormal(const Tauler& tauler, const Posicio& posicio)
{	
	int fila = posicio.getFila();
	int columna = posicio.getColumna();

	int f = -1;
	if (m_color == COLOR_NEGRE)
		f = 1;

	if ((fila + f) < N_FILES &&  (fila + f) >= 0 && (columna + 1) < N_COLUMNES)
	{
		if (tauler.getFitxa(fila + f, columna + 1).getTipus() == TIPUS_EMPTY && m_nMoviments < MAX_MOVIMENTS) {
		
			Moviment moviment;
			moviment.setNPosicions(2);
			moviment.setPosicio(0, posicio);
			moviment.setPosicio(1, Posicio(fila + f, columna + 1));
			m_movimentsValids[m_nMoviments] = moviment;
			m_nMoviments++;
		}
	}

	if ((fila + f) < N_FILES && (fila + f) >= 0 && (columna - 1) >= 0)
	{

		if (tauler.getFitxa(fila + f, columna - 1).getTipus() == TIPUS_EMPTY && m_nMoviments < MAX_MOVIMENTS) {
			Moviment moviment;
			moviment.setNPosicions(2);
			moviment.setPosicio(0, posicio);
			moviment.setPosicio(1, Posicio(fila + f, columna - 1));
			m_movimentsValids[m_nMoviments] = moviment;
			m_nMoviments++;
		}
	}
}

void Fitxa::movimentsDama(const Tauler& tauler, const Posicio& posicio)
{
	for (int i = 1; i < N_FILES; i++)
	{
		afegirMovNormalDama(tauler, posicio, i, i);
		afegirMovNormalDama(tauler, posicio, i, -i);
		afegirMovNormalDama(tauler, posicio, -i, i);
		afegirMovNormalDama(tauler, posicio, -i, -i);
	}
}

void Fitxa::afegirMovNormalDama(const Tauler& tauler, const Posicio& posicio, int fila, int columna)
{
	int filaPrincipal = posicio.getFila();
	int columnaPrincipal = posicio.getColumna();
	int filaDesti = filaPrincipal + fila;
	int columnaDesti = columnaPrincipal + columna;
	

	if (filaDesti >= 0 && filaDesti < N_FILES && columnaDesti >= 0 && columnaDesti < N_COLUMNES)
	{
		int signeFila = 1;
		if (fila < 0) 
			signeFila = -1;
		int signeColumna = 1;
		if (columna < 0) 
			signeColumna = -1;

		int nPassos;
		if ((fila * signeFila) > (columna * signeColumna)) {
			nPassos = (fila * signeFila);
		}
		else 
		{
			nPassos = (columna * signeColumna);
		}

		if (filaDesti < 0 || filaDesti >= N_FILES || columnaDesti < 0 || columnaDesti >= N_COLUMNES) {

		}
		else 
		{
			bool camiLliure = true;

			for (int i = 1; i <= nPassos; i++)
			{
				int filaTemporal = filaPrincipal + i * signeFila;
				int columnaTemporal = columnaPrincipal + i * signeColumna;
				if (tauler.getFitxa(filaTemporal, columnaTemporal).getTipus() != TIPUS_EMPTY)
				{
					camiLliure = false;
				}
			}

			if (camiLliure == true && tauler.getFitxa(filaDesti, columnaDesti).getTipus() == TIPUS_EMPTY && m_nMoviments < MAX_MOVIMENTS)
			{
				Moviment moviment;
				moviment.setNPosicions(nPassos + 1);
				moviment.setPosicio(0, posicio);

				for (int i = 1; i < nPassos; i++)
				{
					int novaFila = filaPrincipal + (i * signeFila);
					int novaColumna = columnaPrincipal + (i * signeColumna);
					moviment.setPosicio(i, Posicio(novaFila, novaColumna));
				}
				moviment.setPosicio(nPassos, Posicio(filaDesti, columnaDesti));

				m_movimentsValids[m_nMoviments] = moviment;
				m_nMoviments++;
			}
		}
	}
}

void Fitxa::movimentsNormalMatar(const Tauler& tauler, const Posicio& posicio)
{
	bool jaCapturada[N_FILES][N_COLUMNES];

	for (int i = 0; i < N_FILES; i++)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			jaCapturada[i][j] = false;
		}
	}

	Moviment moviment;
	moviment.setNPosicions(1);
	moviment.setPosicio(0, posicio);

	encadenarCaptures(tauler, moviment, jaCapturada, posicio.getFila(), posicio.getColumna());

}

void Fitxa::movimentsDamaMatar(const Tauler& tauler, const Posicio& posicio)
{
	bool jaCapturada[N_FILES][N_COLUMNES];

	for (int i = 0; i < N_FILES; i++)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			jaCapturada[i][j] = false;
		}
	}

	Moviment moviment;
	moviment.setNPosicions(1);
	moviment.setPosicio(0, posicio);

	encadenarCaptures(tauler, moviment, jaCapturada, posicio.getFila(), posicio.getColumna());

}


void Fitxa::intentarCaptura(const Tauler& tauler, Moviment movimentActual, bool jaCapturada[N_FILES][N_COLUMNES], int filaOrigen, int columnaOrigen, int direccioFila, int direccioColumna, bool esDama)
{
	if( esDama == false)
	{
		int filaMig = filaOrigen + (direccioFila);
		int columnaMig = columnaOrigen + (direccioColumna);
		int filaDesti = filaOrigen + (2 * direccioFila);
		int columnaDesti = columnaOrigen + (2 * direccioColumna);

		if (filaMig >= 0 && filaMig < N_FILES && columnaMig >= 0 && columnaMig < N_COLUMNES && filaDesti >= 0 && filaDesti < N_FILES && columnaDesti >= 0 && columnaDesti < N_COLUMNES)
		{
			Fitxa fitxaMig = tauler.getFitxa(filaMig, columnaMig);
			Fitxa fitxaDesti = tauler.getFitxa(filaDesti, columnaDesti);

			if (fitxaMig.getTipus() != TIPUS_EMPTY && fitxaMig.getColor() != m_color && fitxaDesti.getTipus() == TIPUS_EMPTY && jaCapturada[filaMig][columnaMig] == false)
			{
				Moviment moviment = movimentActual;
				int nPosicions = moviment.getNPosicions();
				moviment.setPosicio(nPosicions, Posicio(filaMig, columnaMig));
				moviment.setPosicio(nPosicions + 1, Posicio(filaDesti, columnaDesti));
				moviment.setNPosicions(nPosicions + 2);

				jaCapturada[filaMig][columnaMig] = true;
				encadenarCaptures(tauler, moviment, jaCapturada, filaDesti, columnaDesti);

				if (moviment.getNPosicions() > movimentActual.getNPosicions())
				{
					m_movimentsValids[m_nMoviments] = moviment;
					m_nMoviments++;
				}
				jaCapturada[filaMig][columnaMig] = false;
			}
		}
	}
	else
	{
		int filaCapturada = filaOrigen + direccioFila;
		int columnaCapturada = columnaOrigen + direccioColumna;

		while (filaCapturada >= 0 && filaCapturada < N_FILES && columnaCapturada >= 0 && columnaCapturada < N_COLUMNES && tauler.getFitxa(filaCapturada, columnaCapturada).getTipus() == TIPUS_EMPTY)
		{
			filaCapturada = filaCapturada + direccioFila; 
			columnaCapturada = columnaCapturada + direccioColumna;
		}

		if (filaCapturada >= 0 && filaCapturada < N_FILES && columnaCapturada >= 0 && columnaCapturada < N_COLUMNES)
		{
			Fitxa fitxaMig = tauler.getFitxa(filaCapturada, columnaCapturada);

			if (fitxaMig.getTipus() != TIPUS_EMPTY && fitxaMig.getColor() != m_color)
			{
				int filaFinal = filaCapturada + direccioFila;
				int columnaFinal = columnaCapturada + direccioColumna;

				if (filaFinal >= 0 && filaFinal < N_FILES && columnaFinal >= 0 && columnaFinal < N_COLUMNES && tauler.getFitxa(filaFinal, columnaFinal).getTipus() == TIPUS_EMPTY && jaCapturada[filaCapturada][columnaCapturada] == false)
				{
					Moviment moviment = movimentActual;
					int nombreposicions = moviment.getNPosicions();
					moviment.setPosicio(nombreposicions, Posicio(filaCapturada, columnaCapturada));
					moviment.setPosicio(nombreposicions + 1, Posicio(filaFinal, columnaFinal));
					moviment.setNPosicions(nombreposicions + 2);

					jaCapturada[filaCapturada][columnaCapturada] = true;
					encadenarCaptures(tauler, moviment, jaCapturada, filaFinal, columnaFinal);

					if (moviment.getNPosicions() > movimentActual.getNPosicions())
					{
						m_movimentsValids[m_nMoviments++] = moviment;
					}
					jaCapturada[filaCapturada][columnaCapturada] = false;
				}
			}
		}
	}
}

void Fitxa::encadenarCaptures(const Tauler& tauler, Moviment movimentActual, bool jaCapturada[N_FILES][N_COLUMNES], int filaActual, int columnaActual)
{
	bool esDama = false;

	if (m_tipus == TIPUS_DAMA)
		esDama = true;

	intentarCaptura(tauler, movimentActual, jaCapturada, filaActual, columnaActual, +1, +1, esDama);
	intentarCaptura(tauler, movimentActual, jaCapturada, filaActual, columnaActual, +1, -1, esDama);
	intentarCaptura(tauler, movimentActual, jaCapturada, filaActual, columnaActual, -1, +1, esDama);
	intentarCaptura(tauler, movimentActual, jaCapturada, filaActual, columnaActual, -1, -1, esDama);

	if (movimentActual.getNPosicions() > 1 && m_nMoviments < MAX_MOVIMENTS) {
		m_movimentsValids[m_nMoviments++] = movimentActual;
	}
}

