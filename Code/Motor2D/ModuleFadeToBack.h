#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class ModuleFadeToBlack : public j1Module
{
public:
	~ModuleFadeToBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(uint lvlnum, float time=2.0f);
	bool Draw();
private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	float normalized;
	
	
};

#endif //__MODULEFADETOBLACK_H__
