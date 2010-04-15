/* 
 * File:   Client.cpp
 * Author: necto
 * 
 * Created on March 29, 2010, 2:00 AM
 */

#include <errno.h>

#include "Client.h"
#include "Exception.h"

Client::Client ()
{
	if (!Socket::create ())
		throw Exception ("Could not create client socket.");
}
//--------------------------------------------------------------------------------------------------
Client::~Client () { }
//--------------------------------------------------------------------------------------------------
void Client::Connect (string adress, int port)
{
	if (!Socket::connect (adress, port))
		throw Exception ("Could not connect.");
	set_non_blocking (true);
}
//--------------------------------------------------------------------------------------------------
int Client::Receive (char* data, int max_size)
{
	int rez = Socket::recv (data, max_size);
	if (rez == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 0;
		else throw Exception ("can\'t read from socket");
	}
	return rez;
}
//--------------------------------------------------------------------------------------------------
void Client::Receive_next()
{
	static bool confirmed = false;
	if (!confirmed)		Confirm (1);

	int received = Receive (buffer, Buffer_size);

	if (received == 0)
	{
		confirmed = false;
		return;
	}
	int readed = 0;

	while (readed < received)
	{
		char id = *(buffer + readed++);
		if (id == 0) break;
		for (iterator i = begin(); i != end(); ++i)
			if ((**i).Id () == id)
			{
				int size = (**i).Import (buffer + readed, received - readed);
				if (-1 == size) return;//!!!throw Exception ("Can\'t load package");
				readed += size;
				break;
			}
	}
	confirmed = true;
}
//--------------------------------------------------------------------------------------------------
void Client::Confirm (int code)
{
	if (!Socket::send ((char*)&code, sizeof(code))) throw Exception ("could not confirm.");
}
//--------------------------------------------------------------------------------------------------
