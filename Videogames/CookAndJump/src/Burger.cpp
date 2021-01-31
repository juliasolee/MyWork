#include "Burger.h"

//#include  <stdio>
//#include  <stdlib.h>

#include "ofApp.h"

Burger::Burger() {

}

Burger::~Burger() {

}

void Burger::createOrder(int level, std::vector<int> &clientOrder, std::vector<pecaEmpty> &aliments) {

	//clientOrder.push_back(10);

	clientOrder.push_back(0);
	aliments[0].isSelected = false;
	aliments[0].isPositioned = false;

	for (int i = 1; i < level + 2; i++) {

		int random_order = rand() % 6 + 2;
		//int random_order = 4;
		clientOrder.push_back(random_order);
		aliments[random_order].isSelected = false;
		aliments[random_order].isPositioned = false;
	}
	clientOrder.push_back(1);
	aliments[1].isSelected = false;
	aliments[1].isPositioned = false;
}

void Burger::lastSelected(std::vector<pecaEmpty> &aliments, int &ultimPecaIndex) {

	for (int i = 0; i < aliments.size(); i++) {

		if (aliments.at(i).estatPeca == TOCADA) {

			aliments[i].isSelected = true;
			ultimPecaIndex = i;

		}
	}
}

bool Burger::checkOrder(std::vector<int> clientOrder, std::vector<int> playerOrder) {

	if (clientOrder.size() != playerOrder.size()) {
		return false;
	}
	for (int i = 0; i < clientOrder.size(); i++) {
		if (clientOrder.at(i) != playerOrder.at(i)) {
			return false;
		}
	}
	return true;
}

void Burger::drawOrder(std::vector<pecaEmpty> aliments, grid myGrid, std::vector<int> clientOrder) {

	for (int i = 0; i < clientOrder.size(); i++) {

		int index = clientOrder.at(i);
		ofVec2f grid_pos = myGrid.returnPosicioOfPeca(13);
		ofImage icon = aliments.at(index).icon;
		icon.resize(150, 30);
		ofVec2f aux_pos;
		if (index == 1) {
			aux_pos = ofVec2f(grid_pos.x - 110, grid_pos.y - 0 - (17 * i));
		}
		else if (index == 4 || index == 5 || index == 7) {
			aux_pos = ofVec2f(grid_pos.x - 110, grid_pos.y + 5 - (15 * i));
		}
		else {
			aux_pos = ofVec2f(grid_pos.x - 110, grid_pos.y + 5 - (17 * i));
		}
		icon.draw(aux_pos);
	}

}

void Burger::drawPlayerOrder(std::vector<pecaEmpty> aliments, grid myGrid, std::vector<int> playerOrder) {

	for (int i = 0; i < playerOrder.size(); i++) {

		int index = playerOrder.at(i);
		ofVec2f grid_pos = myGrid.returnPosicioOfPeca(38);
		ofImage icon = aliments.at(index).icon;
		icon.resize(150, 30);
		ofVec2f aux_pos;
		if (index == 1) {
			aux_pos = ofVec2f(grid_pos.x - 65, grid_pos.y - 0 - (17 * i));
		}
		else if (index == 4 || index == 5 || index == 7) {
			aux_pos = ofVec2f(grid_pos.x - 65, grid_pos.y + 5 - (15 * i));
		}
		else {
			aux_pos = ofVec2f(grid_pos.x - 65, grid_pos.y + 5 - (17 * i));
		}
		icon.draw(aux_pos);
	}

}
