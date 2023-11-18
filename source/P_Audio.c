#include "P_Audio.h"

void Audio_Init()
{
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	//mmLoad(ID);
	//Load effects
	mmLoadEffect(SFX_LASER);
	mmLoadEffect(SFX_SWISH);
}

void PlaySoundEX( int i , u8 volume, u8 panning)
{
	//Declare a sound effect
	mm_sound_effect effect;
	//Set the id of the sound effect with the input parameter
	effect.id = i;
	//Set the rate to the default one (1024)
	effect.rate = 1024;
	//Set the volume to the maximum (range 0...255)
	effect.volume = volume;
	//Set the panning depending on the effect (0-left....255-right)
	effect.panning = panning;

	//Play the effect using the sound structure
	mmEffectEx(&effect);
}

void PlaySound(int id){
	mmEffect(id);
}


void PlayMusic( int id )
{
	//Start playing music in a loop
	mmStart(id,MM_PLAY_LOOP);
	//Set module volume to 512 (range 0...1024) using the function mmSetModuleVolume(...)
	mmSetModuleVolume(512);
	//...
}
