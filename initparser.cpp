#include "initparser.h"

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
bool File_loader::Open_file (char* fname)
{
	_file.open (fname);
	return _file.is_open();
}
//-----------------------------------------------------------------------------------------------
bool File_loader::Close_file()
{
	if (_file.is_open()) return false;
	_file.close();
	return !_file.is_open();
}
//-----------------------------------------------------------------------------------------------
bool File_loader::Read_sector (Serializator* prop)
{
	return prop->Unserialise (_file);
}
//-----------------------------------------------------------------------------------------------
bool File_loader::Is_good()
{
	return _file.good();
}
//-----------------------------------------------------------------------------------------------
ifstream& File_loader::Get_data()
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
bool Serializator::Is_it_my_name (const string& name, bool no_spaced)
{
	if (no_spaced) return _name == name;
	return _name == No_spaces (name);
}
//-----------------------------------------------------------------------------------------------
bool Serializator::Unserialise (ifstream &file)
{
	return Before_read (file) && Read_frag (file) && After_read (file);
}
//-----------------------------------------------------------------------------------------------
bool Serializator::After_read (ifstream &file){return true;}
bool Serializator::Before_read (ifstream &file){return true;}
//-----------------------------------------------------------------------------------------------
//===============================================================================================
Section::Section (string name, char break_name)
	:Serializator (name), _break_name (break_name)
{
}
Section::~Section(){}
//-----------------------------------------------------------------------------------------------
void Section::Add_param (Serializator* p)
{
	_props.push_back (p);
}
//-----------------------------------------------------------------------------------------------
bool Section::Read_frag (ifstream &file)
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
			if ((*i)->Is_it_my_name (cur_name))
			{
				No_spaces_begin (file);
				(*i)->Unserialise (file);
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
void Section::Delete_props()
{
	for (vector <Serializator*>::iterator i = _props.begin(); i != _props.end(); ++i)
	{
		delete *i;
	}
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
		if (c != ' ' && c != '\t' && c != '\n') ret += c;
	}
	return ret;
}
//-----------------------------------------------------------------------------------------------
void No_spaces_begin (ifstream& file)
{
	char c = 0;
	while ((c = file.peek()) == ' ' || c == '\t' || c == '\n') file.get();
}
//-----------------------------------------------------------------------------------------------