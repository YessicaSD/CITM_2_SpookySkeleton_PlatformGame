#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"
#include "j1Timer.h"
class j1FadeToBlack : public j1Module
{

public:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	};
	j1FadeToBlack()
	{
		name.create("fade");
	}
	~j1FadeToBlack();

	bool Start() override;
	bool Update(float dt) override;
	bool FadeToBlack(uint lvlnum, float time=2.0f);
	bool PostUpdate() override;

	fade_step GetStateFade()
	{
		return current_step;
	}
private:
	fade_step current_step = fade_step::none;
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen = {0,0,0,0};
	float normalized=0;
	j1Timer timer;
	
};

#endif //__MODULEFADETOBLACK_H__
