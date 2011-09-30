/*
 * univalueext.h
 *
 *  Created on: Sep 30, 2011
 *      Author: necto
 */

#ifndef UNIVALUEEXT_H_
#define UNIVALUEEXT_H_

#include "Interpreter.h"
#include "Vec.h"
#include "Car.h"

template<>
UniValue UniValue::by<Vector2f> (Vector2f v);
template<>
UniValue UniValue::by<const Car*> (const Car* c);


#endif /* UNIVALUEEXT_H_ */
