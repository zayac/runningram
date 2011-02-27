#pragma once
#ifndef INITPARSER_H_INCLUDED
#define INITPARSER_H_INCLUDED

#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "Functor.h"

using std::ifstream;
using std::vector;
using std::string;

class Serializator;

class Initializable
{
public:
	virtual Serializator* newParser() = 0;
};

class Serializator
{
protected:
	string _name;

protected:
	virtual bool beforeRead (ifstream &file);
	virtual bool readFrag (ifstream &file) = 0;
	virtual bool afterRead (ifstream &file);
public:
	Serializator (const string& name);
	virtual ~Serializator();

	bool isItMyName (const string& name, bool no_spaced = false);
	bool unserialise (ifstream &file);
//	virtual string Serialise() = 0;
};
//===========================================
class Sectionp :public Serializator
{
	vector <Serializator *> _props;
	char _break_name;

protected:
	virtual bool readFrag (ifstream &file);
public:
	Sectionp (string name, char break_name);
	virtual ~Sectionp();
	void addParam (Serializator* p);

	void deleteProps();
};
//===================================================================

class File_loader
{
	ifstream _file;

public:
	File_loader();
	File_loader (char* fname);
	~File_loader();
	
	bool OpenFile (char* fname);
	bool closeFile();

	bool readSector (Serializator* prop);

	bool isGood();
	ifstream& getData();
};
//===================================================================
template <typename T>
class St_loader :public Serializator
{
	T* _val;
protected:
	virtual bool readFrag (ifstream &file){return 0;}
public:
	St_loader (const string& name, T* val) :Serializator (name), _val (val){}
	virtual ~St_loader(){}
};
//===================================================================
template <typename T>
class St_processor :public St_loader<T>
{
	T _val;
	Arg_Functor <void, T&> *_proc;
protected:
	virtual bool afterRead (ifstream &file)
	{
		(*_proc)(_val);
		return true;
	}
public:
	St_processor (const string& name, Arg_Functor <void, T&> *proc) :St_loader<T> (name, &_val), _proc (proc){}
	virtual ~St_processor(){}
};
void No_spaces_begin (ifstream& file);
void Cut_end_spaces (char* str);
//-------------------------------------------------------------------
template <>
bool St_loader<int>::readFrag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<double>::readFrag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<float>::readFrag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<string>::readFrag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<char>::readFrag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<unsigned char>::readFrag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<bool>::readFrag (ifstream &file);
//-------------------------------------------------------------------
#ifdef TENSOR_H_INCLUDED
#ifndef ST_LOADER_FOR_VECTOR3D_DEFINED
#define ST_LOADER_FOR_VECTOR3D_DEFINED
//template <int coors>
template <>
bool St_loader<Vector3d/*Tensor <double, coors>*/ >::readFrag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.get();// ������� ������
	for (int i = 0; i < 3/*coors*/; ++i)
	{
		No_spaces_begin (file);

		for (int j = 0; j < 1024; ++j)
		{
			if (file.eof()) return false;
			char c = file.get();
			if (c == '\n') return false;
			if (c == ' ' || c == '\t') continue;
			if (c == ',' || c == ')')
			{
				istr[j] = 0;
				break;
			}
			istr[j] = c;
		}
		(*_val)[i] = atof (istr);
	}
	return true;
}
#endif
#endif
//-------------------------------------------------------------------
#endif //INITPARSER_H_INCLUDED
