/* 
 * File:   Collision_detection.h
 * Author: necto
 *
 * Created on February 20, 2010, 6:16 PM
 */

#ifndef _COLLISION_DETECTION_H
#define	_COLLISION_DETECTION_H

#include "Vec.h"

struct Collision_vector
{
	Vector2f papp;
	Vector2f depth;
	float time;

	inline float Weight() {return depth.Lensq();}
	inline bool Vital() {return Weight() > aboutnull;}
};

const int NV = 4;//number of vertexes in shapes

struct Shape
{
	Vector2f v[NV];
	Vector2f orthos[NV];
	Vector2f vel;

	void Update_orthos();
	void Get_projection_limits (Vector2f axis, float* minproj, float* maxproj,
								Vector2f* minp, Vector2f* maxp);
};

Collision_vector Detect_collision (Shape &s1, Shape &s2, float time_interval);

#endif	/* _COLLISION_DETECTION_H */

