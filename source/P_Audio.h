#pragma once

#include <nds.h>

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

void Audio_Init();
void PlaySoundEX( int i , u8 volume, u8 panning);
void PlaySound(int i);
void PlayMusic(int id);
