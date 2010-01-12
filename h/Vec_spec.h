#include <assert.h>
#include <string.h> //for memcpy function

#undef Typical_arg
#ifdef SIMPLE_TYPE			//if type is a simple, it's cheaper to take it by value,
#define Typical_arg Type
#else
#define Typical_arg const Type& //but reference
#endif

//===============================================================================================
//====================================class=Vec==================================================
//===============================================================================================
#if !defined(NUMBER_DIMENSIONS) || NUMBER_DIMENSIONS == 0
template <typename Type, int dim>
class Vec
#else
template <typename Type>
#define dim NUMBER_DIMENSIONS
class Vec<Type, dim>
#endif
//It is the long head for ###class Vec<Type, dim>###
{
public:
	union
	{
		Type coor[dim];
#if NUMBER_DIMENSIONS == 1
		Type x;
#elif NUMBER_DIMENSIONS == 2
		struct {Type x; Type y;};
#elif NUMBER_DIMENSIONS == 3
		struct {Type x; Type y; Type z;};
#elif NUMBER_DIMENSIONS == 4
		struct {Type x; Type y; Type z; Type w;};
#endif
	};
public:
#if NUMBER_DIMENSIONS == 1
	inline Vec (Type x_ = 0) {x = x_;}//=0 or not (may be more fast if !(=0), because default constructor already defined)
#elif NUMBER_DIMENSIONS == 2
	inline Vec (Type x_ = 0, Type y_ = 0) {x = x_; y = y_;}
#elif NUMBER_DIMENSIONS == 3
	inline Vec (Type x_ = 0, Type y_ = 0, Type z_ = 0) {x = x_; y = y_; z = z_;}
#elif NUMBER_DIMENSIONS == 4
	inline Vec (Type x_ = 0, Type y_ = 0, Type z_ = 0, Type w_ = 0) {x = x_; y = y_; z = z_; w = w_;}
#else
	inline Vec() :coor(){}
#endif

	inline Vec (Type coors_[dim]) {memcpy (coor, coors_, dim*sizeof(Type));}
	inline Vec (const Vec& that) {memcpy (coor, that.coor, dim*sizeof(Type));}

	inline Type& operator[] (int i)		{assert(0 <= i || i < dim); return coor[i];}
	inline Type operator[] (int i) const	{assert(0 <= i || i < dim); return coor[i];}
	inline const Type* Data() const {return coor;}
	inline Type* Data() {return coor;}

	Type Lensq() const;
	inline Type Len() const {return sqrt (Lensq());}

	template <typename Target>
		Vec<Target, dim> To() const;

	Vec operator + (const Vec& that) const;
	Vec operator - (const Vec& that) const;
	Vec operator - () const;
	Vec operator * (Typical_arg that) const;
	Vec operator / (Typical_arg that) const;
	Type operator ^ (const Vec& that) const;
#if NUMBER_DIMENSIONS == 3
	Vec operator * (const Vec& that) const;
#endif

	Vec& operator += (const Vec& that);
	Vec& operator -= (const Vec& that);
	Vec& operator *= (Typical_arg that);
	Vec& operator /= (Typical_arg that);
	Vec& operator = (const Vec& that);

	bool operator == (const Vec& that) const;
	bool operator != (const Vec& that) const;
};
//===============================================================================================
//===============================================================================================
#undef FUN_SPEC_HEADLINE
#if !defined(NUMBER_DIMENSIONS) || NUMBER_DIMENSIONS == 0
#define FUN_SPEC_HEADLINE template <typename Type, int dim>
#else
#define FUN_SPEC_HEADLINE template <typename Type>
#endif

#undef REPEAT

#if NUMBER_DIMENSIONS == 1
#define REPEAT(oper) oper(x, that.x, ret.x)
#elif NUMBER_DIMENSIONS == 2
#define REPEAT(oper) oper(x, that.x, ret.x)\
					 oper(y, that.y, ret.y)
#elif NUMBER_DIMENSIONS == 3
#define REPEAT(oper) oper(x, that.x, ret.x)\
					 oper(y, that.y, ret.y)\
					 oper(z, that.z, ret.z)
#elif NUMBER_DIMENSIONS == 4
#define REPEAT(oper) oper(x, that.x, ret.x)\
					 oper(y, that.y, ret.y)\
					 oper(z, that.z, ret.z)\
					 oper(w, that.w, ret.w)
#else
#define REPEAT(oper) for (int i = 0; i < dim; ++i) oper(coor[i], that.coor[i], ret.coor[i])
#endif

//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Type Vec<Type, dim>::Lensq() const
{
	Type ret = 0;
#define PLUSSQ(my_coor, that_coor, ret_coor) ret += my_coor*my_coor;
	REPEAT(PLUSSQ)
#undef PLUSSQ
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
template <typename Target>
Vec<Target, dim> Vec<Type, dim>::To() const
{
	Vec<Target, dim> ret;
#define CONVERT(my_coor, that_coor, ret_coor) ret_coor = Target (my_coor);
	REPEAT(CONVERT)
#undef CONVERT
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator + (const Vec<Type, dim>& that) const
{
	Vec<Type, dim> ret;
#define PLUS(my_coor, that_coor, ret_coor) ret_coor = my_coor + that_coor;
	REPEAT(PLUS)
#undef PLUS
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator - (const Vec<Type, dim>& that) const
{
	Vec<Type, dim> ret;
#define MINUS(my_coor, that_coor, ret_coor) ret_coor = my_coor - that_coor;
	REPEAT(MINUS)
#undef MINUS
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator -() const
{
	Vec<Type, dim> ret;
#define UNARY_MINUS(my_coor, that_coor, ret_coor) ret_coor = -my_coor;
	REPEAT(UNARY_MINUS)
#undef UNARY_MINUS
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator * (Typical_arg that) const
{
	Vec<Type, dim> ret;
#define MULTIPLICATION(my_coor, that_coor, ret_coor) ret_coor = my_coor*that;
	REPEAT(MULTIPLICATION)
#undef MULTIPLICATION
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator / (Typical_arg that) const
{
	Vec<Type, dim> ret;
#define DIVISION(my_coor, that_coor, ret_coor) ret_coor = my_coor/that;
	REPEAT(DIVISION)
#undef DIVISION
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Type Vec<Type, dim>::operator ^ (const Vec<Type, dim>& that) const
{
	Type ret;
#define PLUS_MUL(my_coor, that_coor, ret_coor) ret += my_coor*that_coor;
	REPEAT(PLUS_MUL)
#undef PLUS_MUL
	return ret;
}
//-----------------------------------------------------------------------------------------------
#if NUMBER_DIMENSIONS == 3
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator * (const Vec<Type, dim>& that) const
{
	return Vec<Type, dim> (y*that.z - z*that.y, z*that.x - x*that.z, x*that.y - y*that.x);
}
#endif
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim>& Vec<Type, dim>::operator += (const Vec<Type, dim>& that)
{
#define PLUS(my_coor, that_coor, ret_coor) my_coor += that_coor;
	REPEAT(PLUS)
#undef PLUS
	return *this;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim>& Vec<Type, dim>::operator -= (const Vec<Type, dim>& that)
{
#define MINUS(my_coor, that_coor, ret_coor) my_coor -= that_coor;
	REPEAT(MINUS)
#undef MINUS
	return *this;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim>& Vec<Type, dim>::operator *= (Typical_arg that)
{
#define MULTIPLICATION(my_coor, that_coor, ret_coor) my_coor *= that;
	REPEAT(MULTIPLICATION)
#undef MULTIPLICATION
	return *this;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim>& Vec<Type, dim>::operator /= (Typical_arg that)
{
#define DIVISION(my_coor, that_coor, ret_coor) my_coor /= that;
	REPEAT(DIVISION)
#undef DIVISION
	return *this;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim>& Vec<Type, dim>::operator = (const Vec<Type, dim>& that)
{
#define EQUAL(my_coor, that_coor, ret_coor)	my_coor = that_coor;
	REPEAT(EQUAL)
#undef EQUAL
	return *this;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
bool Vec<Type, dim>::operator == (const Vec<Type, dim>& that) const
{
#define EQUAL(my_coor, that_coor, ret_coor)	if (my_coor != that_coor) return false;
	REPEAT(EQUAL)
#undef EQUAL
	return true;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
bool Vec<Type, dim>::operator != (const Vec<Type, dim>& that) const
{
#define EQUAL(my_coor, that_coor, ret_coor)	if (my_coor != that_coor) return true;
	REPEAT(EQUAL)
#undef EQUAL
	return false;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
#if !defined(NUMBER_DIMENSIONS) || NUMBER_DIMENSIONS == 0
inline Vec<Type, dim> operator * (Typical_arg a, const Vec<Type, dim>& b) {return b*a;}
#else
inline Vec<Type, dim> operator * (Typical_arg a, const Vec<Type, dim>& b) {return b*a;}
#endif
//-----------------------------------------------------------------------------------------------

#undef REPEAT
#undef FUN_SPEC_HEADLINE

#ifdef Type
#undef Type
#endif

#ifdef dim
#undef dim
#endif

#ifdef Typical_arg
#undef Typical_arg
#endif