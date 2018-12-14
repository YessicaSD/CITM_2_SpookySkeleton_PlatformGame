#ifndef __j1SCENE_H__
#define __j1SCENE_H__


#include "j1Module.h"
#include "j1ModuleEntity.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include <map>

class UiItem_Image;
class UiItem;
struct SDL_Texture;
enum class SceneState
{
	STARTMENU,
	GAME,
	PAUSE,
	SETTING,
	MAX_STATES
};

void FadeToScene();


class j1Scene : public j1Module
{
	
private:
	//Start Menu variables -------------------------
	
	p2List<UiItem*> thisMenuItems;
	std::map<const char*, void(*)()> mapOfFuntions =
	{ { "FadeToScene" , FadeToScene } };
	
	//Game variables --------------------------------
	p2DynArray<EntitiesInfo> entitiesArrayInfo;
	SDL_Texture * Background = nullptr;
	UiItem_Image* titleImage = nullptr;
	uint fx_death_aux;
	pugi::xml_document	sceneFile;
	pugi::xml_node sceneNode;
	pugi::xml_node saveNode;
	float horizontalScreenDivision;

public:
	//Start Menu variables -------------------------
	SceneState state = SceneState::GAME;
	UiItem* starMenupanel = nullptr;
	UiItem* settingPanel = nullptr;

	// Game variables ------------------
	uint num_thismaplvl = 1;
	bool loadingSaveFile = false;

	
public:
	bool loadedLeve = true;
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&) override;

	// Called before the first frame
	bool Start()override;

	void LoadEntities(const pugi::xml_node & entitiesNode);

	// Called before all Updates
	bool PreUpdate(float dt)override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool PostUpdate() override;

	void DebugControls();

	void CameraLogic(float dt);


	// Called before quitting
	bool CleanUp()override;

	bool Load(pugi::xml_node&) override;
	

	bool Save(pugi::xml_node&) const override;
	void AudioControl();
	bool LoadStartMenu();

private:
	SDL_Texture * debug_tex;
};

#endif // __j1SCENE_H__
