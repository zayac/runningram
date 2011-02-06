/* 
 * File:   Client.cpp
 * Author: necto
 * 
 * Created on March 29, 2010, 2:00 AM
 */

#include <errno.h>

#include "Client.h"
#include "Exception.h"
#include "Player_manager.h"

Client::Client ()
{
	if (!Socket::create ())
		throw Exception ("Could not create client socket.");
}
//--------------------------------------------------------------------------------------------------
Client::~Client () { }
//--------------------------------------------------------------------------------------------------
void Client::connect (string adress, int port)
{
	if (!Socket::connect (adress, port))
		throw Exception ("Could not connect.");
	set_non_blocking (true);

// not needed	for (int i = max_packets_in_net; i-- > 0;) Confirm(1);//allow sever max_packets_in_net packages to send
}
//--------------------------------------------------------------------------------------------------
int Client::receive (char* data, int max_size)
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
void Client::receiveNext (char* buffer, int bsize)
{
	checkEvents (buffer, bsize);

	int received = receive (buffer, bsize);
	if (received == 0)
	{
		return;
	}
	sendReaction (buffer + received, bsize - received);//allow next package
	int readed = 0;

	while (readed < received)
	{
		char id = *(buffer + readed++);
		if (id == 0) break;
		if (id == 'N')
		{
//			std::cerr <<"pid = " << *(int*)(buffer + readed) <<std::endl;
			readed += sizeof (int);
		}
		for (iterator i = begin(); i != end(); ++i)
			if ((**i).id () == id)
			{
				int size = (**i).imp (buffer + readed, received - readed);
				if (-1 == size) return;//!!!throw Exception ("Can\'t load package");
				readed += size;
				break;
			}
	}
}
//--------------------------------------------------------------------------------------------------
void Client::checkEvents (char* buffer, int bsize)
{
	if (pm)
	{
		int len = 0;
		*(buffer + len++) = 'e';
		int writed = pm->exportEvents (buffer + len, bsize - len);
		GODFORBID(-1 == writed, "can\'t export_events");
		len += writed;
		if (len > sizeof (int))//there real events
			undelivered.push_back (package (len, buffer));
	}
}
//--------------------------------------------------------------------------------------------------
void Client::sendReaction (char* buffer, int bsize)
{
	int offset = 0;
	offset = 0;
	int undelivered_size = combineUndelivered (buffer + offset, bsize - offset);
	offset += undelivered_size;

	*(buffer + offset++) = 'c';//"confirmation" id
	*(int*)(buffer + offset) = 1;//confirmation code
	offset += sizeof(int);


	if (-1 == send (buffer, offset))
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			undelivered.push_back (package (undelivered_size, buffer + 1));//package undelivered
		else
			throw Exception ("Could not send", __LINE__, __FILE__);
	}
}
//--------------------------------------------------------------------------------------------------
int Client::combineUndelivered (char* buf, int bsize)
{
	if (undelivered.empty ()) return 0;

	int writed = 0;
	for (vector <package>::iterator j = undelivered.begin (); j != undelivered.end (); ++j)
	{
		int len = j->copy_to (buf + writed, bsize - writed);
		GODFORBIDlf (-1 == len, "can\'t write important packet")
		writed += len;
	}
	undelivered.clear ();
	return writed;
}
//--------------------------------------------------------------------------------------------------
