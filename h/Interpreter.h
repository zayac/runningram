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
	static UniValue byBool (bool orig);
	static UniValue byChar (char orig);
	static UniValue byInt (int orig);
	static UniValue byFloat (float orig);
	static UniValue byDouble (double orig);
	static UniValue byStr (const char* orig);
	static UniValue byString (const string& orig);

	void append (UniValue& what);
	UniValue head();		//Makes sense only in list case
	UniValue tail();

	bool getBool();
	char getChar();
	int getInt();
	float getFloat();
	double getDouble();
	char* getStr();
	string getString();

	bool isNull();

	bool isBool();
	bool isChar();
	bool isInt();
	bool isFloat();
	bool isDouble();
	bool isString();
	bool isList();
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

    UniValue eval (const string& code);
    UniValue evalNprint (const string& code);
    UniValue unsafeEval (char* code);//It can break the whole program
	bool loadFile (char* fname);

};

#endif	/* INTERPRETER_H */

