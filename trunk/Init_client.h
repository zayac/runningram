/* 
 * File:   Init_client.h
 * Author: necto
 *
 * Created on January 5, 2010, 5:53 PM
 */

#ifndef _INIT_CLIENT_H
#define	_INIT_CLIENT_H

#include "initparser.h"

class Init_client :public Sectionp
{
public:
    int win_x;
    int win_y;

    string win_name;

public:
    Init_client (string name);
    virtual ~Init_client();
private:

};

#endif	/* _INIT_CLIENT_H */

