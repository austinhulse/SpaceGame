/* Simple sound class: load up a sound from a file, then 
 * tell it to play.
 */

#ifndef _SOUND_H
#define _SOUND_H

#ifdef __linux__
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#else
#include "SDL.h"
#include "SDL_mixer.h"
#endif

class GameObject;
#include "GameCamera.h"

class Sound {
public:
	Sound(char* filename, float maxVolume=1.0f);
	// Sounds are either attached to objects (e.g. roars from monsters) or positions
	// (e.g. the sound of a collision). So there are separate constructors for these 
	// two cases.
	Sound(char* filename, GameObject *object, float maxVolume=1.0f);
	Sound(char* filename, Vector3f position, float maxVolume=1.0f);
	~Sound();

	void Play();
	void Loop();
	// Update sound volume and left/right panning to account for new position.
	void Update(Vector3f playerPosition, Vector3f playerLook, Vector3f playerUp);
	void Update(GameCamera *camera);
	void Pause();

	static int InitializeSounds();
	static void UninitializeSounds();
private:
	// Private constructor to minimize code reuse
	void init(char* filename, GameObject *object, Vector3f position, float maxVolume);

	int channel;
	Mix_Chunk *chunk;
	float maxVolume;
	GameObject *object;
	Vector3f position;

	static map<char *, Mix_Chunk*> *filenamesToChunks;
};

class Music {
public:
	Music(char *filename, float maxVolume=1.0f);
	~Music();
	void Play();
	void Loop();
	void Pause();
private:
	int channel;
	float maxVolume;
	Mix_Music *music;
};

#endif
