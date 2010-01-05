/* 
 * File:   Init_client.cpp
 * Author: necto
 * 
 * Created on January 5, 2010, 5:53 PM
 */

#include "Init_client.h"

Init_client::Init_client (string name)
    :Sectionp (name, '=')
{
    Add_param (new St_loader<int> ("X size", &win_x));
    Add_param (new St_loader<int> ("Y size", &win_y));
    Add_param (new St_loader<string> ("app name", &win_name));
}
//------------------------------------------------------------------------------
Init_client::~Init_client()
{
    Delete_props();
}
//------------------------------------------------------------------------------

