#ifndef QUADLIFE_H
#define QUADLIFE_H
#include "autocell.h"
#include <QVector>
#include <QDebug>

class QuadLife : public AutoCell
{
/*
Dans cet automate, voici ce à quoi correspondent les états :
	- 2 : noir
	- 3 : rouge
	- 4 : vert
	- 5 : bleu
	- 6 : jaune
*/

public:
	QuadLife(int height = 100, int width=100, int cellWidth=10, int cellHeight=10,int nbMinVoisins=2,int nbMaxVoisins=3);
	void changeCellState(QPoint point);
	void nextState();
	void generateRandomly();

private:
	int nbMinVoisins;
	int nbMaxVoisins;
	int getNextValue(int x, int y, Etat etat);
	bool willBorn(int x, int y, Etat etat);

};

#endif // QUADLIFE_H
