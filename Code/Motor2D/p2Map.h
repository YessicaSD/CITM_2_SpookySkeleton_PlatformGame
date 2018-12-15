#ifndef _P2MAP_H
#define _P2MAP_H
#include "p2DynArray.h"
#include "p2SString.h"

template <class TYPE>
class p2Map
{
private:
	uint numElements = 0;
	p2DynArray<p2SString> clave;
	p2DynArray<TYPE> varibles;
public:
	int Find(p2SString string)
	{
		int ret = -1;
		for (uint num=0;num<numElements; ++num)
		{
			if (clave[num] == string)
			{
				return num;
			}
		}

		return ret;
	}
	TYPE At(uint num)
	{
		assert(num < numElements);
		return varibles[num];
	}
	void pushBack(p2SString clave, TYPE variable)
	{
		++numElements;
		this->clave.PushBack(clave);
		this->varibles.PushBack(variable);
	}
	uint GetNumElemnts()
	{
		return numElements;
	}
};
#endif // !_P2MAP_H

