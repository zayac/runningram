/* 
 * File:   Key_storage.h
 * Author: necto
 *
 * Created on February 26, 2011, 6:19 PM
 */

#ifndef KEY_STORAGE_H_INCLUDED
#define	KEY_STORAGE_H_INCLUDED

#include "Control.h"

struct Key_storage :public Control
{
	Key_id up;
	Key_id down;
	Key_id left;
	Key_id right;

	GUEventman* evman;

	class Initializer;
	class ParserHolder;

	inline void reset() {up = down = left = right = KI_UNKNOWN;}

	void setControl (Car*);
	Key_storage* createCopy() const;

	static void preInit (GUEventman* evman);
};


#endif	/* KEY_STORAGE_H */

