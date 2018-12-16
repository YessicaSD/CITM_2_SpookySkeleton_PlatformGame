#include "ModuleFadeToBack.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"

#include "Brofiler/Brofiler.h"
j1FadeToBlack::~j1FadeToBlack()
{

}

// Load assets
bool j1FadeToBlack::Start()
{
	
	screen = { 0, 0, App->render->camera.w , App->render->camera.h };
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool  j1FadeToBlack::Update(float dt)
{
	this->dt = dt;
	BROFILER_CATEGORY("Update_Fade.cpp", Profiler::Color::Coral)
	if (current_step == fade_step::none)
		return true;

	Uint32 now = timer.Read();
	normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			
			
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		LOG("%f", normalized);
		normalized = 1.0f - normalized*dt;
		if (now >= total_time)
		{
			App->pause = false;
			App->map->active = true;
			App->map->CleanUp();
			App->map->Start();
			App->entity->DestroyAllEntities();
			normalized = 0;
			current_step = fade_step::none;
			App->scene->Start();

		}
			
	} break;
	}


	return true;
}
bool j1FadeToBlack::PostUpdate()
{
	// Finally render the black square with alpha on the screen
	App->render->DrawQuad(screen, 0, 0, 0, (Uint8)(normalized * 255.0f),true,false);
	return true;
}
bool j1FadeToBlack:: FadeToBlack(uint lvlnum, float time) {
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		this->timer.Start();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		App->scene->num_thismaplvl = lvlnum;
		App->map->activateScene(lvlnum);
		
		ret = true;
	}

	return ret;
}

