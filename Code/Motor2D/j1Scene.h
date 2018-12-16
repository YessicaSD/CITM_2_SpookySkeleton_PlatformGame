#ifndef __j1SCENE_H__
#define __j1SCENE_H__


#include "j1Module.h"
#include "j1ModuleEntity.h"
#include "p2Point.h"
#include "p2DynArray.h"


class UiItem_Image;
class UiItem_Bar;
class UiItem_Label;
class UiItem;

struct SDL_Texture;
enum class SceneState
{
	STARTMENU,
	GAME,
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
public:
	UiItem * startMenupanel = nullptr;
	UiItem* settingPanel = nullptr;
	UiItem* PausePanel = nullptr;
	UiItem* GameUiPanel = nullptr;
	UiItem* CreditsPanel = nullptr;
	UiItem_Label * label_coin = nullptr;
	UiItem_Label* label_points = nullptr;
	UiItem* ImagePanel = nullptr;
	UiItem* heart[3] = { nullptr,nullptr,nullptr };
	
private:
	bool LoadStartMenu(pugi::xml_node& nodeScene);
	bool LoadSettings(pugi::xml_node& SettingNode);
	bool LoadPauseGameUi(pugi::xml_node& SettingNode);
	bool LoadGameUi(pugi::xml_node& SettingNode);
	bool LoadCredits(pugi::xml_node& SceneNode);

	void LoadUiElement(UiItem*parent, pugi::xml_node node);

private:
	SDL_Texture * debug_tex;
	
private:
	//Start Menu variables -------------------------
	p2DynArray<sfx> arraySfx;
	bool LoadedUi = false;

	//Game variables --------------------------------
	p2DynArray<EntitiesInfo> entitiesArrayInfo;
	SDL_Texture * Background = nullptr;

	UiItem_Image* titleImage = nullptr;
	UiItem_Bar* volume_bar = nullptr;
	UiItem_Bar* fx_bar = nullptr;

	
	UiItem_Bar* bar = nullptr;

	uint fx_death_aux = 0;
	pugi::xml_document	sceneFile;
	pugi::xml_node levelsNode;
	pugi::xml_node saveNode;
	pugi::xml_node sceneNode;
	float horizontalScreenDivision;
	float result_volume = 0.0f;
	float result_fx = 0.0f;
	const char* findSfxPath(const char*);
	
	void AudioControl();


public:
	bool exitGame = false;
	//Start Menu variables -------------------------
	SceneState state = SceneState::STARTMENU;

	// Game variables ------------------
	uint num_thismaplvl = 1;
	bool loadingSaveFile = false;
	uint points = 0;
	uint coin_points = 0;
	uint player_lives = 3;
	
public:
	bool loadedLevel = true;
	j1Scene();
	~j1Scene();
	bool Awake(pugi::xml_node&) override;
	bool Start()override;
	void LoadEntities(const pugi::xml_node & entitiesNode);
	bool PreUpdate(float dt)override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	void DebugControls();
	void CameraLogic(float dt);
	bool CleanUp()override;
	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) const override;

};

#endif // __j1SCENE_H__
