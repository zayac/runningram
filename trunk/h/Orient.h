/* 
 * File:   Orient.h
 * Author: necto
 *
 * Created on January 10, 2010, 4:18 PM
 */

#ifndef _ORIENT_H
#define	_ORIENT_H
#include "Vec.h"


const float PI = 3.1415926;
inline float inRange (float what) {while (what > PI) what -= 2*PI; while (what < -PI) what += 2*PI; return what;}

class Orient
{
	bool updated;
	float ang;
	Vector2f dir;

//	friend class Orient;

public:
	Orient ();
	Orient (float angle, bool update = false);
	Orient (Vector2f direction, bool normed = false);
	Orient (const Orient& orig);
	virtual ~Orient();

	void setAngle (float a);
	void update();

	float getAngle() const;
	Vector2f getDir() const;
	Vector2f getDir();

	Orient operator -() const;
	Orient operator - (const Orient& that) const;
	Orient operator + (const Orient& that) const;

	inline float operator - (float that) const {return ang - that;}
	inline float operator + (float that) const {return ang + that;}

	Orient& operator -= (const Orient& that);
	Orient& operator += (const Orient& that);

	inline bool operator < (float that) const {return ang < that;}
	inline bool operator < (const Orient& that) const {return ang < that.ang;}
	inline bool operator > (float that) const {return ang > that;}
	inline bool operator > (const Orient& that) const {return ang > that.ang;}

	Vector2f rotate (Vector2f what);

	bool ok() const;
};

inline float operator - (float one, const Orient& two) {return inRange (one - two.getAngle());}
inline float operator + (float one, const Orient& two) {return inRange (one + two.getAngle());}

#endif	/* _ORIENT_H */

