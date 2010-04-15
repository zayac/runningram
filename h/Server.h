/*
 * File:   Server.h
 * Author: necto
 *
 * Created on March 29, 2010, 1:52 AM
 */

#ifndef _SERVER_H
#define	_SERVER_H

#include "Socket.h"
#include "Game_manager.h"
#include "Transmitted.h"
#include <list>
#include <vector>

using std::list;
using std::vector;

class Server :public list <Transmitted*>
{
	char buffer[Buffer_size];

	Socket listening;

	struct pkg_data
	{
		int size;
		char* data;
	};

	struct package: public UniId<pkg_data>
	{
		package (int size, char* data);
		package (Transmitted* from);

		int copy_to (char* dst, int max_size);
	protected:
		void Delete_data();

	};

	struct Clie_sock :public Socket
	{
		vector <package> undelivered;
		Clie_sock (const Socket& from) :Socket (from){}
	};
	list<Clie_sock> clients;

	void Add_pack (package imp);

public:
	Server (int port);
	virtual ~Server();

	void accept_one();

	void send (char* data, int size);
	void send_next();
	void send_important();

//	void Add_important

};

#endif	/* _SERVER_H */

