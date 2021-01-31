#include "audiomanager.h"

Audiomanager* Audiomanager::instance = NULL;

Audiomanager::Audiomanager()
{
	instance = this;

	if (BASS_Init(1, 44100, 0, 0, NULL) == false)
	{
		//error abriendo la tarjeta de sonido...
		std::cout << "\nError obrint la targeta de so\n" << std::endl;
	}


}

HSAMPLE Audiomanager::loadSample(const char* filename, bool loop) {
	
	HSAMPLE hSample;

	if(!mapOfAudios.empty()) {
		
		auto it = mapOfAudios.find(filename);

		if (it != mapOfAudios.end()) {

			return it->second;

		}
	}

	if (loop == true) {
		hSample = BASS_SampleLoad(false, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
	}
	else {
		hSample = BASS_SampleLoad(false, filename, 0, 0, 3, 0);
	}
	if (hSample == 0)
	{
		//file not found
		std::cout << "\nArxiu no trobat\n" << std::endl;
	}

	mapOfAudios.insert(std::pair<std::string, HSAMPLE>(filename, hSample));

	return hSample;

}

void Audiomanager::PlaySound(const char* filename, bool loop) {
	//El handler para un sample
	HSAMPLE hSample;

	//El handler para un canal
	HCHANNEL hSampleChannel;

	hSample = loadSample(filename, loop);

	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample, false);

	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
}

