/*
 * author: Necto(Arseniy Zaostrovnykh)
 *
 * See at Vec.h for example of usage.
 * Note, that you can define macro INLINE_OR_NOT as inline to
 * make all operators inline
 *
 * Define LIGHT_TYPE before including this file to use a
 * light types with default constructors such as long, or float.
 *
 * Define SIMPLE_TYPE before including this file to use
 * types which has non trivial constructor, or which can't be
 * copyed by simple memcpy operation
 *
 * Define NUMBER_DIMENSIONS = 1..4 to specify a concrete dimensionality
 * and improve calculations speed.
 *
 * Set DYNAMIC_SIZE to define the
 * dynamic sized vector (it's size will be choozen in runtime)
 *
 * Define CONCRETE_TYPE to make library specify current options
 * (mentioned above) to concrete type.
 * Note, if you instantaneously define CONCRETE_TYPE & NUMBER_DIMENSIONS
 * or DYNAMIC_SIZE you must do only it in single source file, because
 * it lead to full template specification.
 */

#include <assert.h>
#include <math.h>
#include <string.h> //for memcpy function

#undef Typical_arg
#ifdef LIGHT_TYPE			/*if type occupyes a small amount of memory,*/
	#define Typical_arg Type  /*it's cheaper to take it by value,*/
#else
	#define Typical_arg const Type& /*but reference*/
#endif

#ifdef DYNAMIC_SIZE
	#if defined(NUMBER_DIMENSIONS) && NUMBER_DIMENSIONS != 0
	#warning "Note, that here NUMBER_DIMENSIONS will be set to 0"
			 "If you wanted another specialization, undefine "
			 "DYNAMIC_SIZE before including this file"
	#endif
	#undef NUMBER_DIMENSIONS
	#define NUMBER_DIMENSIONS 0
#endif

//===============================================================================================
//====================================class=Vec==================================================
//===============================================================================================
#ifndef NUMBER_DIMENSIONS
	#ifdef CONCRETE_TYPE
		#define Type CONCRETE_TYPE
		template <int dim>
		class Vec<Type, dim>
	#else
		template <typename Type, int dim>
		class Vec
	#endif
#else
	#define dim NUMBER_DIMENSIONS
	#ifdef CONCRETE_TYPE
		#define Type CONCRETE_TYPE
		template<>
	#else
		template <typename Type>
	#endif
	class Vec<Type, dim>
#endif
//It is the long head for ###class Vec<Type, dim>###
{
#ifdef DYNAMIC_SIZE
private:
	Type *coor;//any size array
	int size;
#else //!DYNAMIC_SIZE
#define size dim /* for common usage of size value */

public:
	#ifdef SIMPLE_TYPE
		union	//only if the type is simple it hasn't untrivial constructor
		{		//and thus may be used in unions
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
	#else //!SIMPLE_TYPE
		Type coor[dim];
	#endif //SIMPLE_TYPE
#endif //DYNAMIC_SIZE
public:
#ifdef DYNAMIC_SIZE
	void ground() {for (int i = 0; i < size; ++i) coor[i] = Type();}

	inline Vec (int size_):size (size_), coor (new Type[size_]) {ground();}
	inline ~Vec() {delete [] coor;} //free all occupied space
#elif NUMBER_DIMENSIONS == 1
	inline Vec (Typical_arg x_ = 0) {coor[0] = x_;}
#elif NUMBER_DIMENSIONS == 2
	inline Vec (Typical_arg x_ = 0, Typical_arg y_ = 0) {coor[0] = x_; coor[1] = y_;}
#elif NUMBER_DIMENSIONS == 3
	inline Vec (Typical_arg x_ = 0, Typical_arg y_ = 0, Typical_arg z_ = 0)
				{coor[0] = x_; coor[1] = y_; coor[2] = z_;}
#elif NUMBER_DIMENSIONS == 4
	inline Vec (Typical_arg x_ = 0, Type y_ = 0, Typical_arg z_ = 0, Typical_arg w_ = 0)
				{coor[0] = x_; coor[1] = y_; coor[2] = z_; coor[3] = w_;}
#else
	inline Vec() :coor(){}
#endif

#ifdef DYNAMIC_SIZE
	inline Vec (int size_, const Type *coors_);
#else //!DYNAMIC_SIZE
	inline Vec (const Type coors_[dim]);
#endif //DYNAMIC_SIZE

	inline Vec (const Vec& that);

	inline Type& at (int i)			{assert(0 <= i && i < size); return coor[i];}
	inline Type  at (int i) const	{assert(0 <= i && i < size); return coor[i];}
	inline Type& operator[] (int i)			{return at(i);}
	inline Type operator[] (int i) const	{return at(i);}
	inline const Type* data() const {return coor;}
	inline Type* data() {return coor;}

	Type lenSq() const;
	inline Type len() const {return sqrt (lenSq());}

	inline int N() const {return size;}

	template <typename Target>
		Vec<Target, dim> to() const;

	template <int dim2>
		Vec<Type, dim2> getSubVec (int start); /* get a part of vector
		 	 starting from given position, and with given size */
	template <int dim2>
		void setSubVec (int start, Vec<Type, dim2> nval); /* set a part of
			vector starting from given position, and with given size */
#ifdef DYNAMIC_SIZE
	Vec<Type, 0> getSubVec (int dim2, int start);
	void setSubVec (int dim2, int start, Vec<Type, 0> nval);
#endif //DYNAMIC_SIZE

	Vec operator + (const Vec& that) const;
	Vec operator - (const Vec& that) const;
	Vec operator - () const;
	Vec operator ~ () const;
	Vec operator & (const Vec& that) const;	/* componentwise multiplication. Take it in brackets always!! */
	Vec operator | (const Vec& that) const;	/* componentwise division. Take it in brackets always!! */
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
	inline void copyCoor (const Type* coor);

	bool operator == (const Vec& that) const;
	bool operator != (const Vec& that) const;

	Vec proj (const Vec& direction) const;
};
//===============================================================================================
//===============================================================================================

#ifndef INLINE_OR_NOT
#define INLINEORNOT
#else
#define INLINEORNOT INTLINE_OR_NOT
#endif

#undef FUN_SPEC_HEADLINE
#ifndef NUMBER_DIMENSIONS
	#ifdef CONCRETE_TYPE
		#define FUN_SPEC_HEADLINE template <int dim> INLINEORNOT
	#else //CONCRETE_TYPE
		#define FUN_SPEC_HEADLINE template <typename Type, int dim> INLINEORNOT
	#endif
#else // NUMBER_DIMENSIONS != 0
	#ifdef CONCRETE_TYPE
		#define FUN_SPEC_HEADLINE INLINEORNOT
	#else//CONCRETE_TYPE
		#define FUN_SPEC_HEADLINE template <typename Type> INLINEORNOT
	#endif
#endif

#undef REPEAT

#if NUMBER_DIMENSIONS == 1
#define REPEAT(oper) oper(coor[0], that[0], ret[0])
#elif NUMBER_DIMENSIONS == 2
#define REPEAT(oper) oper(coor[0], that[0], ret[0])\
					 oper(coor[1], that[1], ret[1])
#elif NUMBER_DIMENSIONS == 3
#define REPEAT(oper) oper(coor[0], that[0], ret[0])\
					 oper(coor[1], that[1], ret[1])\
					 oper(coor[2], that[2], ret[2])
#elif NUMBER_DIMENSIONS == 4
#define REPEAT(oper) oper(coor[0], that[0], ret[0])\
					 oper(coor[1], that[1], ret[1])\
					 oper(coor[2], that[2], ret[2])\
					 oper(coor[3], that[3], ret[3])
#else //and olso if NUMBER_DIMENSIONS == 0 (DYNAMIC_SIZE case)
#define REPEAT(oper) for (int i = 0; i < size; ++i) oper(coor[i], that[i], ret[i])
#endif

#undef DEFAULT_CONSTRUCTOR_ARGS
#ifdef DYNAMIC_SIZE
#define DEFAULT_CONSTRUCTOR Vec<Type, dim>(size)
#define DEFAULT_CONSTRUCTORT(VTYPE) Vec<VTYPE, dim>(size)
#else //!DYNAMIC_SIZE
#define DEFAULT_CONSTRUCTOR Vec<Type, dim>()
#define DEFAULT_CONSTRUCTORT(VTYPE) Vec<VTYPE, dim>()
#endif //DYNAMIC_SIZE

//-----------------------------------------------------------------------------------------------
//=== constructor Vec(const Type* coor_array) ===
FUN_SPEC_HEADLINE
#ifdef DYNAMIC_SIZE
	inline Vec<Type, dim>::Vec (int size_, const Type *that)
		:size(size_), coor(new Type[size_])
	{
		ground();
#else //!DYNAMIC_SIZE
	inline Vec<Type, dim>::Vec (const Type that[dim])
	{
#endif //DYNAMIC_SIZE
	copyCoor(that);
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
inline Vec<Type, dim>::Vec (const Vec& that)
{
#ifdef DYNAMIC_SIZE
	size = that.size;
	coor = new Type[that.size];
#endif //DYNAMIC_SIZE
	copyCoor(that.coor);
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
inline void Vec<Type, dim>::copyCoor (const Type* that)
{
#ifdef SIMPLE_TYPE
	memcpy (coor, that, size*sizeof(Type));
#else
#define COPY_COOR(my_coor, that_coor, unused) my_coor = that_coor;
	REPEAT(COPY_COOR)
#undef COPY_COOR
#endif
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Type Vec<Type, dim>::lenSq() const
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
Vec<Target, dim> Vec<Type, dim>::to() const
{
	Vec<Target, dim> ret = DEFAULT_CONSTRUCTORT(Target);
#define CONVERT(my_coor, that_coor, ret_coor) ret_coor = Target (my_coor);
	REPEAT(CONVERT)
#undef CONVERT
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
template <int dim2>
Vec<Type, dim2> Vec<Type, dim>::getSubVec (int start)
{
	assert(start + dim2 <= size);
	return Vec<Type, dim2> (coor + start);
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
template <int dim2>
void Vec<Type, dim>::setSubVec (int start, Vec<Type, dim2> nval)
{
	/* it makes sense only with dimensions greater than 3 */
	assert(start + dim2 <= size);
	for (int i = 0; i < dim2; ++i)
		coor[i+start] = nval[i];
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator + (const Vec<Type, dim>& that) const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define PLUS(my_coor, that_coor, ret_coor) ret_coor = my_coor + that_coor;
	REPEAT(PLUS)
#undef PLUS
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator - (const Vec<Type, dim>& that) const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define MINUS(my_coor, that_coor, ret_coor) ret_coor = my_coor - that_coor;
	REPEAT(MINUS)
#undef MINUS
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator -() const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define UNARY_MINUS(my_coor, that_coor, ret_coor) ret_coor = -my_coor;
	REPEAT(UNARY_MINUS)
#undef UNARY_MINUS
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator ~() const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define TILDE(my_coor, that_coor, ret_coor) ret_coor = ~my_coor;
	REPEAT(TILDE)
#undef TILDE
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE	/* componentwise multiplication. Take it in brackets always!! */
Vec<Type, dim> Vec<Type, dim>::operator & (const Vec<Type, dim>& that) const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define COMPONENTWISE_MUL(my_coor, that_coor, ret_coor) ret_coor = my_coor*that_coor;
	REPEAT(COMPONENTWISE_MUL)
#undef COMPONENTWISE_MUL
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE 	/* componentwise division. Take it in brackets always!! */
Vec<Type, dim> Vec<Type, dim>::operator | (const Vec<Type, dim>& that) const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define COMPONENTWISE_DIV(my_coor, that_coor, ret_coor) ret_coor = my_coor/that_coor;
	REPEAT(COMPONENTWISE_DIV)
#undef COMPONENTWISE_DIV
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator * (Typical_arg that) const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define MULTIPLICATION(my_coor, that_coor, ret_coor) ret_coor = my_coor*that;
	REPEAT(MULTIPLICATION)
#undef MULTIPLICATION
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Vec<Type, dim> Vec<Type, dim>::operator / (Typical_arg that) const
{
	Vec<Type, dim> ret = DEFAULT_CONSTRUCTOR;
#define DIVISION(my_coor, that_coor, ret_coor) ret_coor = my_coor/that;
	REPEAT(DIVISION)
#undef DIVISION
	return ret;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
Type Vec<Type, dim>::operator ^ (const Vec<Type, dim>& that) const
{
	Type ret = Type();
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
	return Vec<Type, dim> (coor[1]*that[2] - coor[2]*that[1],
						   coor[2]*that[0] - coor[0]*that[2],
						   coor[0]*that[1] - coor[1]*that[0]);
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
#define NOTEQUAL(my_coor, that_coor, ret_coor)	if (my_coor != that_coor) return true;
	REPEAT(NOTEQUAL)
#undef NOTEQUAL
	return false;
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
inline Vec<Type, dim> Vec<Type, dim>::proj (const Vec<Type, dim>& direction) const
{
	if (direction.lenSq() == 0) return *this;
	return (operator^(direction))*direction/direction.lenSq();
}
//-----------------------------------------------------------------------------------------------
FUN_SPEC_HEADLINE
inline Vec<Type, dim> operator * (Typical_arg a, const Vec<Type, dim>& b) {return b*a;}
//-----------------------------------------------------------------------------------------------

#undef REPEAT
#undef FUN_SPEC_HEADLINE

#undef Type
#undef dim
#undef INLINEORNOT
#undef DEFAULT_CONSTRUCTOR
#undef DEFAULT_CONSTRUCTORT
#undef Typical_arg
#undef size
#ifdef DYNAMIC_SIZE
	#undef NUMBER_DIMENSIONS /* it was defined in the beginning of this file */
#endif
