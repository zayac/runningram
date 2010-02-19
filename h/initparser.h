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
	virtual Serializator* Get_parser() = 0;
};

class Serializator
{
protected:
	string _name;

protected:
	virtual bool Before_read (ifstream &file);
	virtual bool Read_frag (ifstream &file) = 0;
	virtual bool After_read (ifstream &file);
public:
	Serializator (const string& name);
	virtual ~Serializator();

	bool Is_it_my_name (const string& name, bool no_spaced = false);
	bool Unserialise (ifstream &file);
//	virtual string Serialise() = 0;
};
//===========================================
class Sectionp :public Serializator
{
	vector <Serializator *> _props;
	char _break_name;

protected:
	virtual bool Read_frag (ifstream &file);
public:
	Sectionp (string name, char break_name);
	virtual ~Sectionp();
	void Add_param (Serializator* p);

	void Delete_props();
};
//===================================================================

class File_loader
{
	ifstream _file;

public:
	File_loader();
	File_loader (char* fname);
	~File_loader();
	
	bool Open_file (char* fname);
	bool Close_file();

	bool Read_sector (Serializator* prop);

	bool Is_good();
	ifstream& Get_data();
};
//===================================================================
template <typename T>
class St_loader :public Serializator
{
	T* _val;
protected:
	virtual bool Read_frag (ifstream &file){return 0;}
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
	virtual bool After_read (ifstream &file)
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
bool St_loader<int>::Read_frag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<double>::Read_frag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<float>::Read_frag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<string>::Read_frag (ifstream &file);
//-------------------------------------------------------------------
template <>
bool St_loader<char>::Read_frag (ifstream &file);
#ifdef TENSOR_H_INCLUDED
#ifndef ST_LOADER_FOR_VECTOR3D_DEFINED
#define ST_LOADER_FOR_VECTOR3D_DEFINED
//template <int coors>
template <>
bool St_loader<Vector3d/*Tensor <double, coors>*/ >::Read_frag (ifstream &file)
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
