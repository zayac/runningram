/* 
 * File:   Interpreter.cpp
 * Author: necto
 * 
 * Created on February 3, 2011, 11:38 PM
 */

#include <assert.h>
#include "ecl/ecl.h"
#include "Interpreter.h"

Interpreter* Interpreter::instance = 0;
static cl_object eclError;

const char* Functor_dispatcher = "dispatch-c-void-function-void";
const char* Informer_dispatcher = "dispatch-c-cl_object-function-void";
const char* Hundler_dispatcher = "dispatch-c-cl_object-function-cl_object";
#define Output_write_hundler "print-character-to-output"
#define Output_flush_informer "flush-printed-string-to-output"

UniValue uniValueByLObj(cl_object obj);


cl_object eclEvalForm (cl_object form)
{
    cl_object value;
    CL_CATCH_ALL_BEGIN(ecl_process_env())
	value = si_safe_eval (3, form, Cnil, eclError);
    CL_CATCH_ALL_END;
    return value;
}
//-----------------------------------------------------------
cl_object eclSafeStringToObj (string str)
{
    cl_object cstr = make_constant_base_string(str.c_str());
    cl_object rfs = c_string_to_object("read-from-string");
    cl_object exec = CONS(rfs, CONS(cstr, Cnil));
    return eclEvalForm (exec);
}
//-----------------------------------------------------------
inline bool eclIsError (cl_object obj) {return obj == eclError;}
//-----------------------------------------------------------
cl_object eclCallCFun (cl_object fnum)
{
    (*Interpreter::getFun (fixint(fnum)))();
    return Cnil;
}
//-----------------------------------------------------------
cl_object eclCallCInfo (cl_object fnum)
{
    return (cl_object)(*Interpreter::getInfo (fixint(fnum)))().getVal();
}
//-----------------------------------------------------------
cl_object eclCallCHundler (cl_object fnum, cl_object arg)
{
    return (cl_object)(*Interpreter::getHundler (fixint(fnum)))
			(uniValueByLObj(arg)).getVal();
}
//-----------------------------------------------------------



Interpreter* Interpreter::create (int argc, char *argv[])
{
    if (instance) return 0;//You can create only one instance
    instance = new Interpreter (argc, argv);
    return instance;
}
//--------------------------------------------------------------------------------------------------
Interpreter* Interpreter::getInstance()
{
    return instance;
}
//--------------------------------------------------------------------------------------------------
Functor* Interpreter::getFun (int number)
{
	assert(0 <= number && number < instance->funcs.size());
    return instance->funcs[number];
}
//--------------------------------------------------------------------------------------------------
Informer* Interpreter::getInfo (int number)
{
	assert(0 <= number && number < instance->infos.size());
    return instance->infos[number];
}
//--------------------------------------------------------------------------------------------------
Hundler* Interpreter::getHundler (int number)
{
	assert(0 <= number && number < instance->hundlers.size());
	return instance->hundlers[number];
}
//--------------------------------------------------------------------------------------------------
void Interpreter::destroy()
{
    if (instance) delete instance;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Interpreter::Interpreter (int argc, char *argv[])
{
    cl_boot(argc, argv);
    eclError = cl_gensym(0);

	regDispatcher (Functor_dispatcher, 1, (void*)eclCallCFun);
	regDispatcher (Informer_dispatcher, 1, (void*)eclCallCInfo);
	regDispatcher (Hundler_dispatcher, 2, (void*)eclCallCHundler);

	regHundler (Output_write_hundler,
			new Arg_Method<UniValue, UniValue, Printer>
							(&printer, &Printer::charWriteHundler));
	regInfo (Output_flush_informer,
			new Arg_Method<UniValue, void, Printer>
							(&printer, &Printer::charFlushInformer));
}
//--------------------------------------------------------------------------------------------------
Interpreter::~Interpreter()
{
    for (vector<Functor*>::iterator i = funcs.begin(); i != funcs.end(); ++i)
	delete *i;
    cl_shutdown();
}
//--------------------------------------------------------------------------------------------------
#include <iostream>
UniValue Interpreter::Printer::charWriteHundler (UniValue c)
{
	std::cout <<"c:" <<c.getChar() <<std::endl;

	if (preview) (*preview)(accumulator);
	if (c.getChar() == '\n') charFlushInformer();
	else accumulator += c.getChar();
	return c;
}
//--------------------------------------------------------------------------------------------------
UniValue Interpreter::Printer::charFlushInformer()
{
	if (flush && !accumulator.empty()) (*flush)();
	if (preview && !accumulator.empty()) (*preview)(string());
	accumulator.clear();
	return UniValue::byBool(false);
}
//--------------------------------------------------------------------------------------------------
void Interpreter::defun (const string& name, const char* dispatcher,
						 int number, bool has_arg)
{
	cl_object arg = has_arg? CONS(c_string_to_object("arg"), Cnil):Cnil;
    eclEvalForm(CONS(c_string_to_object("defun"),
				CONS(c_string_to_object(name.c_str()),
				CONS(arg,
				CONS(CONS(c_string_to_object(dispatcher),
				     CONS (ecl_make_integer(number), arg)), Cnil)))));
}
//--------------------------------------------------------------------------------------------------
void Interpreter::regDispatcher (const char* name, int num_args, void* fun)
{
    cl_object sym = c_string_to_object(name);
    ecl_def_c_function(sym, (cl_objectfn_fixed)fun, num_args);
}
//--------------------------------------------------------------------------------------------------
int Interpreter::regFun (string name, Functor* fun)
{
    int fnum = funcs.size();
    funcs.push_back(fun);

	defun(name, Functor_dispatcher, fnum, false);
    return fnum;
}
//--------------------------------------------------------------------------------------------------
int Interpreter::regInfo (string name, Informer* fun)
{
    int fnum = infos.size();
    infos.push_back(fun);

	defun(name, Informer_dispatcher, fnum, false);
    return fnum;
}
//--------------------------------------------------------------------------------------------------
int Interpreter::regHundler (string name, Hundler* fun)
{
    int fnum = hundlers.size();
    hundlers.push_back(fun);

	defun(name, Hundler_dispatcher, fnum, true);
    return fnum;
}
//--------------------------------------------------------------------------------------------------
void Interpreter::regOutput (Arg_Functor<void, const string&> *preview, Functor* flush)
{
	printer.preview = preview;
	printer.flush = flush;

	unsafeEval("(defclass console-output "
			   "(gray:fundamental-character-output-stream)"
			   "(last-symbol))");
	unsafeEval("(defmethod gray:stream-write-char "
			   "((stream console-output) ch) "
			   "("Output_write_hundler" ch))");
	unsafeEval("(defmethod gray:stream-force-output "
			   "((stream constol-output))"
			   "("Output_flush_informer"))");
	unsafeEval("(setq *standard-output* "
			   "  (make-instance 'console-output))");
//	unsafeEval("(setq *error-output* *standart-output*)");


}
//--------------------------------------------------------------------------------------------------
UniValue Interpreter::eval (const string& code)
{
    cl_object form = eclSafeStringToObj (code);
    if (eclIsError(form))
    {
		//reading error
		return uniValueByLObj(Cnil);
    }
	cl_object result = eclEvalForm (form);
    if (eclIsError (result));
		return uniValueByLObj(Cnil);//evaluating error
	return uniValueByLObj(result);
}
//--------------------------------------------------------------------------------------------------
UniValue Interpreter::evalNprint (const string& code)
{
	UniValue rez = eval(code);
	if (rez.isNull())
		unsafeEval ("(print nil)");
	else
		eclEvalForm(CONS(c_string_to_object("load"),
					CONS((cl_object)rez.getVal(), Cnil)));
	return rez;
}
//--------------------------------------------------------------------------------------------------
UniValue Interpreter::unsafeEval (char* code)
{
	cl_object rezult = eclEvalForm (c_string_to_object(code));
    if (eclIsError (rezult))
		return uniValueByLObj(Cnil);//evaluating error
    return uniValueByLObj(rezult);
}
//--------------------------------------------------------------------------------------------------
bool Interpreter::loadFile (char* fname)
{
    return eclIsError (eclEvalForm(CONS(c_string_to_object("load"),
								   CONS(make_constant_base_string(fname), Cnil))));
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
UniValue::UniValue (const UniValue& orig) :val (orig.val) {}
//--------------------------------------------------------------------------------------------------
UniValue uniValueByLObj(cl_object obj)
{
	return UniValue((SomeValueType*)obj);
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::byBool (bool orig)
{
	return UniValue((SomeValueType*)ecl_make_bool(orig));
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::byChar (char orig)
{
	return UniValue((SomeValueType*)CODE_CHAR(orig));
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::byInt (int orig)
{
	return UniValue((SomeValueType*)ecl_make_integer(orig));
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::byFloat (float orig)
{
	return UniValue((SomeValueType*)ecl_make_singlefloat(orig));
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::byDouble (double orig)
{
	return UniValue((SomeValueType*)ecl_make_doublefloat(orig));
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::byStr (const char* orig)
{
	return UniValue((SomeValueType*)make_constant_base_string(orig));
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::byString (const string& orig)
{
	return UniValue((SomeValueType*)make_constant_base_string(orig.c_str()));
}
//--------------------------------------------------------------------------------------------------
void UniValue::append (UniValue& what)
{
	val = (SomeValueType*)CONS((cl_object)what.val, (cl_object)val);
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::head()
{
	return UniValue((SomeValueType*)cl_car((cl_object)val));
}
//--------------------------------------------------------------------------------------------------
UniValue UniValue::tail()
{
	return UniValue((SomeValueType*)cl_cdr((cl_object)val));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool UniValue::getBool()
{
	return ecl_to_bool((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
char UniValue::getChar()
{
	return ecl_to_char((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
int UniValue::getInt()
{
	return ecl_to_int((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
float UniValue::getFloat()
{
	return ecl_to_float((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
double UniValue::getDouble()
{
	return ecl_to_double((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
char* UniValue::getStr()
{
	return ecl_base_string_pointer_safe((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
string UniValue::getString()
{
	return ecl_base_string_pointer_safe((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool UniValue::isNull()
{
	return (cl_object)val == Cnil;
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isBool()
{
	return (cl_object)val == Ct || (cl_object)val == Cnil;
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isChar()
{
	return CHARACTERP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isInt()
{
	return FIXNUMP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isFloat()
{
	return ECL_SINGLE_FLOAT_P((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isDouble()
{
	return ECL_DOUBLE_FLOAT_P((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isString()
{
	return ECL_STRINGP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isList()
{
	return LISTP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------