#pragma once
#include "Hitable.h"
class HitableList : public Hitable
{
public:
	Hitable **list;
	int listSize;
	HitableList() {}
	HitableList(Hitable **l, int n) { list = l; listSize = n; }
	virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hitRecord)const;
};
bool HitableList::hit(const Ray& ray, float t_min, float t_max, HitRecord& hitRecord)const
{
	HitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = t_max;
	for (int i = 0; i < listSize; i++)
	{
		if (list[i]->hit(ray, t_min, closestSoFar, tempRec))
		{
			hitAnything = true;
			closestSoFar = tempRec.t;
			hitRecord = tempRec;
		}
	}
	return hitAnything;
}