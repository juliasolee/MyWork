#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "includes.h"
#include "extra/bass.h"

#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <algorithm>

class Audiomanager
{

public:
	

	std::map<std::string, HSAMPLE> mapOfAudios;
	
	static Audiomanager* instance;//Singleton

	Audiomanager();

	HSAMPLE loadSample(const char* filename, bool loop);

	void PlaySound(const char* filename, bool loop);
	
};

#endif