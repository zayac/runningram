/* 
 * File:   Client.h
 * Author: necto
 *
 * Created on March 29, 2010, 2:00 AM
 */

#ifndef _CLIENT_H
#define	_CLIENT_H
#include <string>
#include "Socket.h"

using std::string;

class Client :protected Socket
{
//	Socket my;
public:
	Client();
	virtual ~Client();

	void Connect (string adress, int port);
	int Receive (char* data, int max_size);
	void Confirm (int code);

};

#endif	/* _CLIENT_H */

