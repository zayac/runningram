#pragma once

#define SIMPLE_TYPE

#define NUMBER_DIMENSIONS 0
#include "Vec_spec.h"
#undef  NUMBER_DIMENSIONS

#define NUMBER_DIMENSIONS 1
#include "Vec_spec.h"
#undef NUMBER_DIMENSIONS

#define NUMBER_DIMENSIONS 2
#include "Vec_spec.h"
#undef NUMBER_DIMENSIONS

#define NUMBER_DIMENSIONS 3
#include "Vec_spec.h"
#undef NUMBER_DIMENSIONS

#define NUMBER_DIMENSIONS 4
#include "Vec_spec.h"
#undef NUMBER_DIMENSIONS

#undef SIMPLE_TYPE

typedef Vec<long, 2> Polong;
typedef Vec<int, 2> Point;//!!! Take attention !!!
typedef Vec<float, 2> Vector2f;