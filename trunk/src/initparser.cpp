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
Sectionp::Sectionp (string name, char break_name)
	:Serializator (name), _break_name (break_name)
{
}
Sectionp::~Sectionp()
{
}
//-----------------------------------------------------------------------------------------------
void Sectionp::Add_param (Serializator* p)
{
	_props.push_back (p);
}
//-----------------------------------------------------------------------------------------------
bool Sectionp::Read_frag (ifstream &file)
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
void Sectionp::Delete_props()
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
bool St_loader<int>::Read_frag (ifstream &file)
{
	char istr[64] = "wrong value";
	file.getline (istr, 64);
	*_val = atoi (istr);
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<double>::Read_frag (ifstream &file)
{
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = atof (istr);
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<float>::Read_frag (ifstream &file)
{
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = atof (istr);
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<string>::Read_frag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = istr;
	return true;
}
//-------------------------------------------------------------------
template <>
bool St_loader<char>::Read_frag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	*_val = istr[0];
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
	while ((c = file.peek()) == ' ' || c == '\t' || c == '\n') file.get();
}
//-----------------------------------------------------------------------------------------------