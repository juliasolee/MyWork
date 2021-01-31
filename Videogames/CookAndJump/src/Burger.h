#pragma once

#ifndef _BURGER
#define _BURGER

#include <vector>
#include <iostream>
#include "peces/pecaEmpty.h"
#include "grid/grid.h"


class Burger
{
public:

	Burger();
	~Burger();

	//std::vector<int> clientOrder; //Vector que conté peça id
	//std::vector<int> playerOrder;

	void createOrder(int level, std::vector<int> &clientOrder, std::vector<pecaEmpty> &aliments);
	void lastSelected(std::vector<pecaEmpty> &aliments, int &ultimPecaIndex);
	bool checkOrder(std::vector<int> clientOrder, std::vector<int> playerOrder);
	void drawOrder(std::vector<pecaEmpty> aliments, grid myGrid, std::vector<int> clientOrder);
	void drawPlayerOrder(std::vector<pecaEmpty> aliments, grid myGrid, std::vector<int> playerOrder);

};

#endif