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
const char* Handler_dispatcher = "dispatch-c-cl_object-function-cl_object";
#define Output_write_Handler "print-character-to-output"
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
cl_object eclCallCHandler (cl_object fnum, cl_object arg)
{
    return (cl_object)(*Interpreter::getHandler (fixint(fnum)))
			(uniValueByLObj(arg)).getVal();
}
//-----------------------------------------------------------


//+++++++++++++UniValue+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--------------------------------------------------------------------------------------------------
UniValue::UniValue (const UniValue& orig) :val (orig.val) {}
//--------------------------------------------------------------------------------------------------
UniValue uniValueByLObj(cl_object obj)
{
	return UniValue((SomeValueType*)obj);
}
//--------------------------------------------------------------------------------------------------
template <>
UniValue UniValue::by<bool> (bool orig)
{
	return UniValue((SomeValueType*)ecl_make_bool(orig));
}
//--------------------------------------------------------------------------------------------------
template <>
UniValue UniValue::by<char> (char orig)
{
	return UniValue((SomeValueType*)CODE_CHAR(orig));
}
//--------------------------------------------------------------------------------------------------
template <>
UniValue UniValue::by<int> (int orig)
{
	return UniValue((SomeValueType*)ecl_make_integer(orig));
}
//--------------------------------------------------------------------------------------------------
template <>
UniValue UniValue::by<float> (float orig)
{
	return UniValue((SomeValueType*)ecl_make_singlefloat(orig));
}
//--------------------------------------------------------------------------------------------------
template <>
UniValue UniValue::by<double> (double orig)
{
	return UniValue((SomeValueType*)ecl_make_doublefloat(orig));
}
//--------------------------------------------------------------------------------------------------
template <>
UniValue UniValue::by<const char*> (const char* orig)
{
	return UniValue((SomeValueType*)make_constant_base_string(orig));
}
//--------------------------------------------------------------------------------------------------
template <>
UniValue UniValue::by<string> (string /*const string&*/ orig)
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
template <>
void UniValue::get<void>()
{
	return ; //for universality
}
//--------------------------------------------------------------------------------------------------
template <>
bool UniValue::get<bool>()
{
	return ecl_to_bool((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
char UniValue::get<char>()
{
	return ecl_to_char((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
int UniValue::get<int>()
{
	return ecl_to_int((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
float UniValue::get<float>()
{
	return ecl_to_float((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
double UniValue::get<double>()
{
	return ecl_to_double((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
char* UniValue::get<char*>()
{
	return ecl_base_string_pointer_safe((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
string UniValue::get<string>()
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
template <>
bool UniValue::is<bool>()
{
	return (cl_object)val == Ct || (cl_object)val == Cnil;
}
//--------------------------------------------------------------------------------------------------
template <>
bool UniValue::is<char>()
{
	return CHARACTERP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
bool UniValue::is<int>()
{
	return FIXNUMP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
bool UniValue::is<float>()
{
	return ECL_SINGLE_FLOAT_P((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
bool UniValue::is<double>()
{
	return ECL_DOUBLE_FLOAT_P((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
template <>
bool UniValue::is<string>()
{
	return ECL_STRINGP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
bool UniValue::isList()
{
	return LISTP((cl_object)val);
}
//--------------------------------------------------------------------------------------------------
//=====================UniValue=====================================================================




//++++++++++++++++++++Interpreter+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
Handler* Interpreter::getHandler (int number)
{
	assert(0 <= number && number < instance->Handlers.size());
	return instance->Handlers[number];
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
	regDispatcher (Handler_dispatcher, 2, (void*)eclCallCHandler);

	regHandler (Output_write_Handler,
			new Arg_Method<UniValue, UniValue, Printer>
							(&printer, &Printer::charWriteHandler));
	regInfo (Output_flush_informer,
			new Arg_Method<UniValue, void, Printer>
							(&printer, &Printer::charFlushInformer));
}
//--------------------------------------------------------------------------------------------------
Interpreter::~Interpreter()
{
    for (vector<Functor*>::iterator i = funcs.begin(); i != funcs.end(); ++i)
		delete *i;
    for (vector<Informer*>::iterator i = infos.begin(); i != infos.end(); ++i)
		delete *i;
    for (vector<Handler*>::iterator i = Handlers.begin(); i != Handlers.end(); ++i)
		delete *i;
    cl_shutdown();
}
//--------------------------------------------------------------------------------------------------
UniValue Interpreter::Printer::charWriteHandler (UniValue c)
{
	if (preview) (*preview)(accumulator);
	if (c.get<char>() == '\n') charFlushInformer();
	else accumulator += c.get<char>();
	return c;
}
//--------------------------------------------------------------------------------------------------
UniValue Interpreter::Printer::charFlushInformer()
{
	if (flush && !accumulator.empty()) (*flush)();
	if (preview && !accumulator.empty()) (*preview)(string());
	accumulator.clear();
	return UniValue::by<bool>(false);
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
int Interpreter::regHandler (string name, Handler* fun)
{
    int fnum = Handlers.size();
    Handlers.push_back(fun);

	defun(name, Handler_dispatcher, fnum, true);
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
			   "("Output_write_Handler" ch))");
	unsafeEval("(defmethod gray:stream-force-output "
			   "((stream constol-output))"
			   "("Output_flush_informer"))");
	unsafeEval("(setq *standard-output* "
			   "  (make-instance 'console-output))");
//	unsafeEval("(setq *error-output* *standart-output*)");


}
//--------------------------------------------------------------------------------------------------
UniValue Interpreter::funcall (const string& name, const UniValue& arg)
{
	cl_object rezult = eclEvalForm(CONS(c_string_to_object(name.c_str()),
								   CONS((cl_object)arg.getVal(), Cnil)));
    if (eclIsError (rezult))
		return uniValueByLObj(Cnil);//evaluating error
    return uniValueByLObj(rezult);
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
    if (eclIsError (result))
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
string Interpreter::toString (const UniValue& val)
{
	UniValue rez = funcall("princ-to-string", val);
	return rez.get<string>();
}
//--------------------------------------------------------------------------------------------------
bool Interpreter::loadFile (const char* fname)
{
    return eclIsError (eclEvalForm(CONS(c_string_to_object("load"),
								   CONS(make_constant_base_string(fname), Cnil))));
}
//--------------------------------------------------------------------------------------------------
//====================Interpreter===================================================================