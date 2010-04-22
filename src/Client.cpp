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
void Client::Connect (string adress, int port)
{
	if (!Socket::connect (adress, port))
		throw Exception ("Could not connect.");
	set_non_blocking (true);

// not needed	for (int i = max_packets_in_net; i-- > 0;) Confirm(1);//allow sever max_packets_in_net packages to send
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
void Client::Receive_next (char* buffer, int bsize)
{
	Check_events (buffer, bsize);

	int received = Receive (buffer, bsize);
	if (received == 0)
	{
		return;
	}
	Send_reaction (buffer + received, bsize - received);//allow next package
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
			if ((**i).Id () == id)
			{
				int size = (**i).Import (buffer + readed, received - readed);
				if (-1 == size) return;//!!!throw Exception ("Can\'t load package");
				readed += size;
				break;
			}
	}
}
//--------------------------------------------------------------------------------------------------
void Client::Check_events (char* buffer, int bsize)
{
	if (pm)
	{
		int len = 0;
		*(buffer + len++) = 'e';
		int writed = pm->Export_events (buffer + len, bsize - len);
		GODFORBID(-1 == writed, "can\'t export_events");
		len += writed;
		if (len > sizeof (int))//there real events
			undelivered.push_back (package (len, buffer));
	}
}
//--------------------------------------------------------------------------------------------------
void Client::Send_reaction (char* buffer, int bsize)
{
	int offset = 0;
	offset = 0;
	int undelivered_size = Combine_undelivered (buffer + offset, bsize - offset);
	offset += undelivered_size;

	*(buffer + offset++) = 'c';//"confirmation" id
	*(int*)(buffer + offset) = 1;//confirmation code
	offset += 4;


	if (-1 == send (buffer, offset))
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			undelivered.push_back (package (undelivered_size, buffer + 1));//package undelivered
		else
			throw Exception ("Could not send", __LINE__, __FILE__);
	}
}
//--------------------------------------------------------------------------------------------------
int Client::Combine_undelivered (char* buf, int bsize)
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
//void Client::Confirm (int code)
//{
//	if (!Socket::send ((char*)&code, sizeof(code))) throw Exception ("could not confirm.");
//}
//--------------------------------------------------------------------------------------------------
