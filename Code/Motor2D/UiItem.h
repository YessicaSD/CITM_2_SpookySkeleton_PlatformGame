#ifndef ITEM_UI_H
#define ITEM_UI_H
#include "SDL/include/SDL.h"


#include "p2Point.h"
#include "p2List.h"

enum UI_STATES
{
	IDLE,
	HOVER,
	CLICK,
	MAX_STATES,
};

class UiItem
{
private:
	iPoint localPos;
	iPoint worldPos;
	UiItem* parent = nullptr;
	p2List<UiItem*> childs;

public:
	bool showHitBox = false;
	bool enable = true;
	bool interactive = true;
	bool draggable = true;


	UI_STATES state = IDLE;
	SDL_Rect hitBox = {0,0,0,0};
	p2Point<int> pivot = {0,0};
	uint mouseButtonDown = 0;
	
	
	UiItem() {}
	UiItem(SDL_Rect hitBox, p2Point<int> pivot = {0,0}, UiItem * parent = NULL):pivot(pivot)
	{
		this->hitBox = hitBox;
		if (parent)
		{
			this->hitBox.x += parent->localPos.x;
			this->hitBox.y += parent->localPos.y;
		}
		
		
		
	
	}
	iPoint DefineScreenPos()
	{
		worldPos = localPos;
		for (UiItem*  thisParent = this->parent; thisParent; thisParent= thisParent->parent)
		{
			worldPos += thisParent->localPos;
		}
	
		return worldPos;
	}
	iPoint GetScreenRect()
	{
		return worldPos;
	}
	uint ReturnNumOfChilds() const 
	{
		return childs.Count();
	}

	void DrawChildrens();
	bool AddParent(UiItem* parent);

	virtual void Draw() {};
	virtual void OnClickDown(){}
	virtual void OnClickUp(){}


};

#endif // !ITEM_UI_H

