#ifndef STAGE_H
#define STAGE_H

#include "../utils.h"

class Button;

class Stage
{
public:

	Stage();
	virtual void render();
	virtual void update(double seconds_elapsed);

};

class Intro : public Stage
{
public:

	Intro();
	virtual void render();
	virtual void update(double seconds_elapsed);

	int n_intro;
	Button* next;

};

class Menu : public Stage
{
public:

	Menu();
	virtual void render();
	virtual void update(double seconds_elapsed);

	std::vector<Button*> buttons;//Conjunt de botos que s'inicialitza al principi
	int last_index;

	void moveButtons();
	void draw_bg();
	void draw_controls();
	void choose_difficulty();
};

class Loading : public Stage
{
public:

	Loading();
	virtual void render();
	virtual void update(double seconds_elapsed);

	void draw_bg();
};

class End : public Stage
{
public:

	End();
	virtual void render();
	virtual void update(double seconds_elapsed);

	Button* menu;

	void end1();
	void end2();
	void end3();
	void end4();

};
#endif 