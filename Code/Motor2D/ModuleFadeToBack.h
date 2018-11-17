#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"
#include "j1Timer.h"
class ModuleFadeToBlack : public j1Module
{
public:
	ModuleFadeToBlack()
	{
		name.create("fade");
	}
	~ModuleFadeToBlack();

	bool Start() override;
	bool Update(float dt) override;
	bool FadeToBlack(uint lvlnum, float time=2.0f);
	bool Draw(float dt) override;
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
	j1Timer timer;
	
};

#endif //__MODULEFADETOBLACK_H__
