/* 
 * File:   Collision_detection.cpp
 * Author: necto
 * 
 * Created on February 20, 2010, 6:16 PM
 */

#include "Collision_detection.h"




inline Vector2f Ortho (const Vector2f& dir) {return Vector2f (-dir.y, dir.x);}

void Shape::updateOrthos()
{
	for (int i = 0; i < NV - 1; ++i)
	{
		orthos[i] = Ortho (v[i+1] - v[i]);
	}
	orthos[NV - 1] = Ortho (v[0] - v[NV - 1]);
}

void Shape::getProjectionLimits (Vector2f axis, float* minproj, float* maxproj,
							Vector2f* minp, Vector2f* maxp)
{
	*minproj = *maxproj = v[0]^axis;
	*minp = *maxp = v[0].proj (axis);
	for (int i = 1; i < NV; ++i)
	{
		float curproj = v[i]^axis;

		if (*minproj > curproj) { *minproj = curproj; *minp = v[i].proj (axis);}
		if (*maxproj < curproj) { *maxproj = curproj; *maxp = v[i].proj (axis);}
	}
}



bool Has_intersection (Shape& s1, Shape& s2, float time_interval, Collision_vector* cv, Vector2f* center, Vector2f axis)
{
	Vector2f minp1, maxp1;
	float min1, max1, speed1 = s1.vel^axis;
	Vector2f minp2, maxp2;
	float min2, max2, speed2 = s2.vel^axis;

	s1.getProjectionLimits (axis, &min1, &max1, &minp1, &maxp1);
	s2.getProjectionLimits (axis, &min2, &max2, &minp2, &maxp2);

	if (min1 < max2 && min2 < max1)
	{
		if ((max2 - min1) < (max1 - min2))
		{
			cv->depth = (max2 - min1)*axis/axis.lenSq();
//			cv->papp = (maxp2 + minp1)/2;
//			*center = (minp1 + maxp2)/2;
		}
		else
		{
			cv->depth = (min2 - max1)*axis/axis.lenSq();
//			cv->papp = (maxp1 + minp2)/2;
//			*center = (minp2 + maxp1)/2;
		}

		Vector2f left = minp1;
		Vector2f right = maxp2;
		if (min2 > min1) left = minp2;
		if (max1 < max2) right = maxp1;
		*center = (left + right)/2;
		cv->time = 0;
		return true;
	}

	cv->depth = axis;
	if (min1 > max2 && speed2 - speed1 > aboutnull)//approach
	{
		cv->time = (max2 - min1)/(speed2 - speed1);

		minp1 += cv->time*speed1*axis/axis.lenSq ();
		maxp2 += cv->time*speed2*axis/axis.lenSq ();
//		cv->papp = (maxp2 + minp1)/2;
		Vector2f left = minp1;
		Vector2f right = maxp2;				//!! Attention! not tested
		*center = (left + right);

		return cv->time < time_interval;
	}
	if (min2 > max1 && speed1 - speed2 > aboutnull)//approach
	{
		cv->time = (min2 - max1)/(speed1 - speed2);
		minp2 += cv->time*speed2*axis/axis.lenSq ();
		maxp1 += cv->time*speed1*axis/axis.lenSq ();
//		cv->papp = (maxp1 + minp2)/2;
		*center = (minp2 + maxp1)/2;

		return cv->time > time_interval;
	}
	return false;
}

Collision_vector detectCollision (Shape &s1, Shape &s2, float time_interval)
{
	s1.updateOrthos ();
	s2.updateOrthos ();

	Collision_vector ret;
	Vector2f curordinate, ordinate, abscissa;//projections of center of intersection
	ret.time = time_interval*2;
	ret.depth = Vector2f (1/aboutnull, 1/aboutnull);
	Collision_vector cur;

#define CHECK_AXES(SHAPE)\
	for (int i = 0; i < NV; ++i)\
		if (Has_intersection (s1, s2, time_interval, &cur, &curordinate, SHAPE.orthos[i]))\
		{\
			if (cur.time < ret.time + aboutnull)\
				if (ret.time > aboutnull || ret.depth.lenSq () > cur.depth.lenSq ())\
					{\
						ret = cur;\
						ordinate = curordinate;\
					}\
		}\
		else return Collision_vector();

	CHECK_AXES(s1);
	CHECK_AXES(s2);

#undef CHECK_AXES

	Vector2f ort_dir = Ortho (ret.depth);
	Has_intersection (s1, s2, time_interval, &cur, &abscissa, ort_dir);
	ret.papp = ordinate + abscissa;
	return ret;
}