#ifndef FUNCTOR_H_INCLUDED
#define FUNCTOR_H_INCLUDED

#include <functional>

namespace fun
{

using std::binary_function;
using std::unary_function;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <typename Return_Type, typename Taken_Type1, typename Taken_Type2 = void>
	class Arg_Functor :public binary_function <Return_Type, Taken_Type1, Taken_Type2>
	{
	public:
		virtual Return_Type operator ()(Taken_Type1, Taken_Type1) = 0;
		virtual ~Arg_Functor(){};
	};
//---------------------------------
template <typename Return_Type, typename Taken_Type>
class Arg_Functor <Return_Type, Taken_Type, void> :public unary_function <Return_Type, Taken_Type>
	{
	public:
		virtual Return_Type operator ()(Taken_Type) = 0;
		virtual ~Arg_Functor(){};
	};
//---------------------------------
template <typename Return_Type>
	class Arg_Functor <Return_Type, void, void>
	{
	public:
		virtual Return_Type operator ()() = 0;
		virtual ~Arg_Functor(){};
	};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef Arg_Functor <void, void, void> Functor;
typedef Arg_Functor <void, double> Paraop;//PARAmetruc OPeration
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <typename Return_Type, typename Taken_Type>
class Arg_Function :public Arg_Functor <Return_Type, Taken_Type>
	{
	Return_Type (*func)(Taken_Type);
	public:
		Arg_Function (Return_Type (*Func)(Taken_Type)) :func (Func){};
		virtual Return_Type operator ()(Taken_Type arg) {return func (arg);};
	};
//-----------------------------------
template <typename Return_Type>
class Arg_Function <Return_Type, void> :public Arg_Functor <Return_Type, void>
	{
	 Return_Type (*func)();
	public:
		Arg_Function ( Return_Type (*Func)()) :func (Func){};
		virtual Return_Type operator ()() {return func ();};
	};
//-----------------------------------
template <>
class Arg_Function <void, void> :public Arg_Functor <void, void>
	{
	 void (*func)();
	public:
		Arg_Function ( void (*Func)()) :func (Func){};
		virtual void operator ()() {func ();};
	};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef Arg_Function <void, void> Function;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <typename Return_Type, typename Taken_Type, class T>
class Arg_Method :public Arg_Functor <Return_Type, Taken_Type>
	{
	T* object;
	Return_Type (T::*func)(Taken_Type);
	public:
		Arg_Method (T* Object, Return_Type (T::*Func)(Taken_Type)) :object (Object), func (Func){}
		virtual Return_Type operator ()(Taken_Type arg) {return (object->*func) (arg);}
	};
//-----------------------------------
template <typename Return_Type, class T>
class Arg_Method <Return_Type, void, T> :public Arg_Functor <Return_Type, void>
	{
	T* object;
	Return_Type (T::*func)();
	public:
		Arg_Method (T* Object, Return_Type (T::*Func)()) :object (Object), func (Func){}
		virtual Return_Type operator ()() {return (object->*func) ();}
	};
//-----------------------------------
template <typename Taken_Type, class T>
class Arg_Method <void, Taken_Type, T> :public Arg_Functor <void, Taken_Type>
	{
	T* object;
	void (T::*func)(Taken_Type);
	public:
		Arg_Method (T* Object, void (T::*Func)(Taken_Type)) :object (Object), func (Func){}
		virtual void operator ()(Taken_Type arg) {(object->*func) (arg);}
	};
//-----------------------------------
template <class T>
class Arg_Method <void, void, T> :public Arg_Functor <void, void>
	{
	T* object;
	void (T::*func)();
	public:
		Arg_Method (T* Object, void (T::*Func)()) :object (Object), func (Func){}
		virtual void operator ()() {(object->*func) ();}
	};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <typename Return_Type, typename Taken_Type, class T>
class Method_Arg :public Arg_Functor <Return_Type, T*>
	{
	Taken_Type& arg;
	Return_Type (T::*func)(Taken_Type);
	public:
		Method_Arg (Taken_Type& Arg, Return_Type (T::*Func)(Taken_Type)) :arg (Arg), func (Func){}
		virtual Return_Type operator ()(T* that) {return (that->*func)(arg);}
	};
//-----------------------------------
template <typename Return_Type, class T>
class Method_Arg <Return_Type, void, T> :public Arg_Functor <Return_Type, T*>
	{
	Return_Type (T::*func)();
	public:
		Method_Arg (Return_Type (T::*Func)()) :func (Func){}
		virtual Return_Type operator ()(T* that) {return (that->*func) ();}
	};
//-----------------------------------
template <class T>
class Method_Arg <void, void, T> :public Arg_Functor <void, T*>
	{
	void (T::*func)();
	public:
		Method_Arg (void (T::*Func)()) :func (Func){}
		virtual void operator ()(T* that) {(that->*func) ();}
	};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
};
using namespace fun;
#endif