/* 
 * File:   Client.h
 * Author: necto
 *
 * Created on March 29, 2010, 2:00 AM
 */

#ifndef _CLIENT_H
#define	_CLIENT_H
#include <string>
#include <list>
#include "Transmitted.h"
#include "Socket.h"

using std::string;
using std::list;

class Client :protected Socket, public list <Transmitted*>
{
	char buffer[Buffer_size];
//	Socket my;
public:
	Client();
	virtual ~Client();

	void Connect (string adress, int port);
	int Receive (char* data, int max_size);
	void Confirm (int code);

	void Receive_next();

};

#endif	/* _CLIENT_H */

