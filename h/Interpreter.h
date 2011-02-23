/* 
 * File:   Interpreter.h
 * Author: necto
 *
 * Created on February 3, 2011, 11:38 PM
 */

#ifndef INTERPRETER_H
#define	INTERPRETER_H

#include <string>
#include <vector>
#include "Functor.h"

using std::string;
using std::vector;

class SomeValueType;
class Interpreter;

class UniValue
{
	SomeValueType* val;
public:
	SomeValueType* getVal() const {return val;}

public:
	explicit UniValue (SomeValueType* val_):val(val_) {};
	UniValue (const UniValue& orig);

	template <typename T>
	static inline UniValue by (T orig);

	void append (UniValue& what);
	UniValue head();		//Makes sense only in list case
	UniValue tail();
	template <typename T>
	T get();

	bool isNull();
	bool isList();

	template <typename T>
	bool is(); 
};

typedef Arg_Functor<UniValue, UniValue, void> Hundler;
typedef Arg_Functor<UniValue, void, void> Informer;

class Interpreter
{
    static Interpreter* instance;

private:
    Interpreter (int argc, char *argv[]);

    vector<Functor*> funcs;
	vector<Informer*> infos;
	vector<Hundler*> hundlers;

	struct Printer
	{
		string accumulator;
		Arg_Functor<void, const string&>* preview;
		Functor* flush;

		UniValue charWriteHundler (UniValue c);
		UniValue charFlushInformer();

		Printer():preview(0), flush(0) {}
		~Printer()
		{
			if (preview) delete preview;
			if (flush) delete flush;
		}
	} printer;


	void regDispatcher (const char* name, int num_args, void* fun);
	void defun (const string& name, const char* dispatcher,
				int number, bool has_arg);

public:
    static Interpreter* create (int argc, char *argv[]);
    static Interpreter* getInstance();
    static void destroy();
    static Functor* getFun (int number);
    static Informer* getInfo (int number);
    static Hundler* getHundler (int number);

    virtual ~Interpreter();

    int regFun (string name, Functor* fun);
    int regInfo (string name, Informer* fun);
    int regHundler (string name, Hundler* fun);

	void regOutput (Arg_Functor<void, const string&> *preview, Functor* flush);

	UniValue funcall (const string& name, const UniValue& arg);
    UniValue eval (const string& code);
    UniValue evalNprint (const string& code);
    UniValue unsafeEval (char* code);//It can break the whole program
	string toString (const UniValue& val);
	bool loadFile (char* fname);

};

template <typename T>
class InfoTrans :public Informer
{
	Arg_Functor<T, void, void>* fun;
	public:
		InfoTrans (Arg_Functor<T, void, void>* _fun)
		:fun(_fun) {}
		virtual UniValue operator ()()
		{
			if (!fun) return UniValue::by ((*fun)());
			return UniValue::by (false);
		}
		virtual ~InfoTrans()
		{ if (!fun) delete fun;};
};

template <typename RET, typename TAK>
class HundTrans :public Hundler
{
	Arg_Functor<RET, TAK, void>* fun;
	public:
		HundTrans (Arg_Functor<RET, TAK, void>* _fun)
		:fun(_fun) {}
		virtual UniValue operator ()(UniValue val)
		{
			if (!fun) return UniValue::by ((*fun)(val.get<TAK>()));
			return UniValue::by (false);
		}
		virtual ~HundTrans()
		{ if (!fun) delete fun;};
};

class InformerS :public Informer
{
	string exec;
public:
	InformerS (const string& exe) :exec (exe) {}
	virtual UniValue operator ()()
	{ return Interpreter::getInstance()->eval(exec); }
};

class HundlerS :public Hundler
{
	string exec;
public:
	HundlerS (const string& exe) :exec (exe) {}
	virtual UniValue operator ()(UniValue arg)
	{ return Interpreter::getInstance()->funcall(exec, arg); }
};

template <typename T>
class InfoTranS: public Arg_Functor<T, void, void>
{
	string exec;
public:
	InfoTranS (const string& exe) :exec (exe) {}
	virtual T operator ()()
	{ return Interpreter::getInstance()->eval(exec).get<T>(); }
};

template <typename RET, typename TAK>
class HundlTranS: public Arg_Functor<RET, TAK, void>
{
	string exec;
public:
	HundlTranS (const string& exe) :exec (exe) {}
	virtual RET operator ()(TAK val)
	{ UniValue ret = Interpreter::getInstance()->
				funcall (exec, UniValue::by<TAK>(val));//TODO: optimize this.
	return ret.get<RET>();}
};

#endif	/* INTERPRETER_H */
