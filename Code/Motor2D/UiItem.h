#ifndef ITEM_UI_H
#define ITEM_UI_H
#include "SDL/include/SDL.h"


#include "p2Point.h"
#include "p2List.h"
#include "p2SString.h"

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
		
		iPoint worldPos = {0,0};
		UiItem* parent = NULL;
		p2List<UiItem*> childs;

	public:
		iPoint localPos = { 0,0 };
		bool showHitBox = false;
		bool enable = true;
		bool interactive = true;
		bool draggable = false;
		bool clipping = false;

		p2SString name;
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

			if (this->childs.Count() > 0)
			{
				for (p2List_item<UiItem*>* iter = childs.start; iter; iter = iter->next)
				{
					iter->data->UpdateScreenPos();
				}
				
			}
			hitBox.x = worldPos.x;
			hitBox.y = worldPos.y;
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
		void DrawChildrens(float dt);
	

		virtual void Draw(const float& dt) {};
		//virtual float GetBarValue(){}
		virtual void OnClickDown(){}
		virtual void OnClickUp(){}
		bool HaveParent()
		{
			return parent;
		}
		void returnChildList(p2List<UiItem*> & List);
		~UiItem();
		UiItem* FindChildByName(const p2SString& name);
		

};

#endif // !ITEM_UI_H

