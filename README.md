Projecte Dames Metodologia de la programació:
Aquest projecte tracta sobre fer una implementació completa del joc de les dames en C++.
Hi ha dos modes de joc, el normal i el replay. El normal és per jugar i el replay per veure una repetició d’una partida (Per canviar el mode s’ha d’anar al fitxer main.cpp i a la fila 48, on posa ModeJoc mode = MODE_JOC_REPLAY;, canviar el mode per MODE_JOC_NORMAL) :
- Normal: A sota indica el torn del jugador, per jugar has de clicar a una fitxa del color que toca i la pots moure a les diferents posicions en verd que es marcaran en el taulell, si no realitzes el moviment que mata el major nombre de fitxes, s’eliminarà la fitxa que podia fer el moviment. Guanya el jugador que hagi eliminat totes les fitxes del contrari.
- Replay: Pots reproduir els moviments que es posin en moviments.txt, cada clic que facis serà un moviment de fitxa que es farà. El moviment.txt pot guardar els moviments de la partida jugada anteriorment si se surt amb l'esc.
