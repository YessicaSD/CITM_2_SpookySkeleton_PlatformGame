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
	protected:
		iPoint localPos = {0,0};
		iPoint worldPos = {0,0};
		UiItem* parent = NULL;
		p2List<UiItem*> childs;

	public:
		bool showHitBox = false;
		bool enable = true;
		bool interactive = true;
		bool draggable = false;

		UI_STATES state = IDLE;
		SDL_Rect hitBox = {0,0,0,0};
		p2Point<int> pivot = {0,0};
		uint mouseButtonDown = 0;
	
		UiItem(const iPoint& pos, UiItem* const parent);
		UiItem(SDL_Rect hitBox, UiItem *const parent, p2Point<int> pivot = { 0,0 }); 
	
		iPoint UpdateScreenPos()
		{
			worldPos = localPos;
			for (UiItem*  thisParent = this->parent; thisParent; thisParent= thisParent->parent)
			{
				worldPos += thisParent->localPos;
			}
	
			return worldPos;
		}
		iPoint GetScreenPos()
		{
			return worldPos;
		}
		uint ReturnNumOfChilds() const 
		{
			return childs.Count();
		}
		void AddToPos(const iPoint& value);
		void DrawChildrens();
	

		virtual void Draw() {};
		//virtual float GetBarValue(){}
		virtual void OnClickDown(){}
		virtual void OnClickUp(){}
		bool HaveParent()
		{
			return parent;
		}
		void returnChildList(p2List<UiItem*> & List);
		~UiItem();


};

#endif // !ITEM_UI_H

