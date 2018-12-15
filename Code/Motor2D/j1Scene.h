#ifndef __j1SCENE_H__
#define __j1SCENE_H__


#include "j1Module.h"
#include "j1ModuleEntity.h"
#include "p2Point.h"
#include "p2DynArray.h"


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

struct sfx
{
	p2SString name = "";
	p2SString path= "";
	sfx(){}
	sfx(const char* name, const char* path) : name(name), path(path)
	{	}
};

class j1Scene : public j1Module
{
	
private:
	//Start Menu variables -------------------------
	const char* backgroundPath;
	const char* mainMusicStartMenu;
	p2List<UiItem*> thisMenuItems;
	p2DynArray<sfx> arraySfx;
	void LoadUiElement(UiItem*parent, pugi::xml_node node);
	
	//Game variables --------------------------------
	p2DynArray<EntitiesInfo> entitiesArrayInfo;
	SDL_Texture * Background = nullptr;
	UiItem_Image* titleImage = nullptr;
	uint fx_death_aux = 0;
	pugi::xml_document	sceneFile;
	pugi::xml_node sceneNode;
	pugi::xml_node saveNode;
	float horizontalScreenDivision;
	const char* findSfxPath(const char*);

public:
	bool exitGame = false;
	//Start Menu variables -------------------------
	SceneState state = SceneState::STARTMENU;

	UiItem* startMenupanel = nullptr;
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
	bool LoadStartMenu(pugi::xml_node& nodeScene);

private:
	SDL_Texture * debug_tex;
};

#endif // __j1SCENE_H__
