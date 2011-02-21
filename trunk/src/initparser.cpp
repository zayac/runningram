#include "initparser.h"
#include <string.h>

string No_spaces (const string& str);

File_loader::File_loader()
{
}
//-----------------------------------------------------------------------------------------------
File_loader::~File_loader()
{
}
//-----------------------------------------------------------------------------------------------
File_loader::File_loader (char* fname)
	:_file (fname)
{
}
//-----------------------------------------------------------------------------------------------
bool File_loader::OpenFile (char* fname)
{
	_file.open (fname);
	return _file.is_open();
}
//-----------------------------------------------------------------------------------------------
bool File_loader::closeFile()
{
	if (_file.is_open()) return false;
	_file.close();
	return !_file.is_open();
}
//-----------------------------------------------------------------------------------------------
bool File_loader::readSector (Serializator* prop)
{
	return prop->unserialise (_file);
}
//-----------------------------------------------------------------------------------------------
bool File_loader::isGood()
{
	return _file.good();
}
//-----------------------------------------------------------------------------------------------
ifstream& File_loader::getData()
{
	return _file;
}
//-----------------------------------------------------------------------------------------------
//===============================================================================================
Serializator::Serializator (const string& name)
		:_name (No_spaces (name))
{
}
//-----------------------------------------------------------------------------------------------
Serializator::~Serializator()
{
}
//-----------------------------------------------------------------------------------------------
bool Serializator::isItMyName (const string& name, bool no_spaced)
{
	if (no_spaced) return _name == name;
	return _name == No_spaces (name);
}
//-----------------------------------------------------------------------------------------------
bool Serializator::unserialise (ifstream &file)
{
	return beforeRead (file) && readFrag (file) && afterRead (file);
}
//-----------------------------------------------------------------------------------------------
bool Serializator::afterRead (ifstream &file){return true;}
bool Serializator::beforeRead (ifstream &file){return true;}
//-----------------------------------------------------------------------------------------------
//===============================================================================================
Sectionp::Sectionp (string name, char break_name)
	:Serializator (name), _break_name (break_name)
{
}
Sectionp::~Sectionp()
{
}
//-----------------------------------------------------------------------------------------------
void Sectionp::addParam (Serializator* p)
{
	_props.push_back (p);
}
//-----------------------------------------------------------------------------------------------
bool Sectionp::readFrag (ifstream &file)
{
	string cur_name;
	char cn[1024] = "wrong walue";

	while (!file.eof())
	{
		No_spaces_begin (file);
		int pos = file.tellg();
		file.getline (cn, 1024, _break_name);
		cur_name = No_spaces (string (cn));
		
		bool is_it_my_param = false;
		for (vector <Serializator*>::iterator i = _props.begin(); i != _props.end(); ++i)
		{
			if ((*i)->isItMyName (cur_name))
			{
				No_spaces_begin (file);
				(*i)->unserialise (file);
				is_it_my_param = true;
				break;
			}
		}
		if (!is_it_my_param)
		{
			if (file.eof());//���� ����� eof ���!!!
			file.seekg (pos);
			
			return true;
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------------------------
void Sectionp::deleteProps()
{
	for (vector <Serializator*>::iterator i = _props.begin(); i != _props.end(); ++i)
	{
		delete *i;
	}
}
//-----------------------------------------------------------------------------------------------
//===============================================================================================
//-------------------------------------------------------------------
template <>
bool St_loader<int>::readFrag (ifstream &file)
{
	char istr[64] = "wrong value";
	file.getline (istr, 64);
	*_val = atoi (istr);
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<double>::readFrag (ifstream &file)
{
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = atof (istr);
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<float>::readFrag (ifstream &file)
{
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = atof (istr);
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<string>::readFrag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	Cut_end_spaces (istr);
	*_val = istr;
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<char>::readFrag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = istr[0];
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<unsigned char>::readFrag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = istr[0];
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<bool>::readFrag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	if (istr[0] == 'f' || istr[0] == '0')// "false" or "0"
		*_val = false;
	else
		*_val = true;
	return true;
}
//-----------------------------------------------------------------------------------------------
//===============================================================================================
//-----------------------------------------------------------------------------------------------
string No_spaces (const string& str)
{
	string ret;
	for (int i = 0; i < str.size(); ++i)
	{
		char c = str[i];
		if (c != ' ' && c != '\t' && c != '\n' && c != 13) ret += c; //13 - not know what it is, but required
	}
	return ret;
}
//-----------------------------------------------------------------------------------------------
void No_spaces_begin (ifstream& file)
{
	char c = 0;
	while ((c = file.peek()) == ' ' || c == '\t' || c == '\n' || c == '\r') file.get();
}
//-----------------------------------------------------------------------------------------------
void Cut_end_spaces (char* str)
{
	for (int i = strlen(str) - 1; i > 0; ++i)
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r') str[i] = 0;
		else return;
}
//-----------------------------------------------------------------------------------------------