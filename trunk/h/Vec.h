#ifndef VEC_H_INCLUDED

#undef SIMPLE_TYPE
#include "Vec_spec.h"

#define SIMPLE_TYPE
#define LIGHT_TYPE

#define NUMBER_DIMENSIONS 0
#define DYNAMIC_SIZE
#include "Vec_spec.h"
#undef DYNAMIC_SIZE
#undef NUMBER_DIMENSIONS

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

#undef LIGHT_TYPE
#undef SIMPLE_TYPE

#ifndef ABOUTNULL_DEFINED
const long double aboutnull = 1e-8;
const long double nearnull = 0.001;
#define ABOUTNULL_DEFINED
#endif

#define VEC_H_INCLUDED

#ifdef MATR_H_INCLUDED
#error "Include MatrSQ.h before Vec.h"
#endif //MATR_H_INCLUDED

typedef Vec<long, 2> Polong;
typedef Vec<int, 2> Point;//!!! Take attention !!!
typedef Vec<float, 2> Vector2f;

#endif//!VEC_H_INCLUDED