#include "UiItem.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"

void UiItem::AddToPos(const iPoint & value)
{
	iPoint v = value;
	if (v.x == -1 || v.x == 1)
		v.x = 0;

	if (v.y == -1 || v.y == 1)
		v.y = 0;
	iPoint auxWorld = worldPos;
	auxWorld += v;
	
	if (auxWorld.x >= this->parent->hitBox.x && auxWorld.x + this->hitBox.w <= this->parent->hitBox.x + this->parent->hitBox.w)
	{
		worldPos.x = auxWorld.x;
		localPos.x += v.x;
		hitBox.x = worldPos.x;
	}
	if (auxWorld.y >= this->parent->hitBox.y && auxWorld.y + this->hitBox.h <= this->parent->hitBox.y + this->parent->hitBox.h)
	{
		worldPos.y = auxWorld.y;
		localPos.y += v.y;
		hitBox.y = worldPos.y;
	}
	
	if (this->childs.Count() > 0)
	{
		for (p2List_item<UiItem*>* thisItem = this->childs.start;thisItem; thisItem= thisItem->next)
		{
			thisItem->data->AddToPos(value);
		}
	}

}

void UiItem::DrawChildrens(float dt)
{
	BROFILER_CATEGORY("Draw_UiItem.cpp", Profiler::Color::Aqua)
	if (this->childs.Count() > 0)
	{
		for (p2List_item<UiItem*>* thisItem = this->childs.start; thisItem; thisItem = thisItem->next)
		{
			if (thisItem->data->enable)
			{
				thisItem->data->Draw(dt);
				if (thisItem->data->showHitBox)
				{
					
					App->render->DrawQuad(thisItem->data->hitBox, 255, 255, 255, 255, false, false);
				}
			}

		}
		for (p2List_item<UiItem*>* thisItem = this->childs.start; thisItem; thisItem = thisItem->next)
		{
			if (thisItem->data->enable && thisItem->data->childs.Count() > 0)
			{
				thisItem->data->DrawChildrens(dt);
			}
		}
	}

}

UiItem::~UiItem()
{
	/*if (this->parent != nullptr)
	{
		hitBox = { 0,0,0,0 };
		UiItem* thisParent = this->parent;
		uint pos = thisParent->childs.find(this);
		thisParent->childs.del(thisParent->childs.At(pos));
		this->parent = nullptr;
	}*/
}

//bool UiItem::AddParent(UiItem * parent)
//{
//	assert(parent != nullptr);
//	if (this->parent == nullptr)
//	{
//		this->parent = parent;
//		parent->childs.add(this);
//	}
//	else
//	{
//		int pos = this->parent->childs.find(this);
//		assert(pos != -1);
//
//		p2List_item<UiItem*>* me = this->parent->childs.At(pos);
//		this->parent->childs.del(me);
//		this->parent = parent;
//		parent->childs.add(this);
//	}
//	
//	return false;
//}

UiItem::UiItem(const iPoint & pos, UiItem* const parent): parent(parent)
{
		hitBox.x = pos.x;
		hitBox.y = pos.y;
		localPos = pos;
		if (parent != nullptr)
		{
			this->parent = parent;
			parent->childs.add(this);
			this->UpdateScreenPos();
			this->hitBox.x = worldPos.x;
			this->hitBox.y = worldPos.y;
		}
}

UiItem::UiItem(SDL_Rect hitBox, UiItem *const parent, p2Point<int> pivot) :pivot(pivot),parent(parent)
{

	this->hitBox = hitBox;
	localPos = { hitBox.x, hitBox.y };
		if (parent != nullptr)
		{
			this->parent = parent;
			parent->childs.add(this);
			this->UpdateScreenPos();
			this->hitBox.x = worldPos.x;
			this->hitBox.y = worldPos.y;

		}
		
	

}

void UiItem:: returnChildList(p2List<UiItem*>& List)
{
	List.add(this);
	for (p2List_item<UiItem*>* iterator = List.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->enable)
		{
			for (p2List_item<UiItem*>* childIterator = iterator->data->childs.start; childIterator != nullptr; childIterator = childIterator->next) {
				List.add(childIterator->data);
			}
		}
	}
}
