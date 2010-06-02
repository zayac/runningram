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
#include "Client.h"
#include <list>
#include <vector>

using std::list;
using std::vector;
class Player_manager;

class Server :public list <Transmitted*>
{
	char buffer[Buffer_size];

	Socket listening;

	struct Clie_sock :public Socket
	{
		vector <package> undelivered;
		int packets_in_net;
		Clie_sock (const Socket& from) :Socket (from), packets_in_net(0){}

		//void Check_confirmation();
		void receiveAnswer (Player_manager* pm);
		bool sendIfPossible (char* data, int size, Player_manager* pm = 0);

		void trySendUndelivered (char* buffer, Player_manager* pm = 0);
	};
	list<Clie_sock> clients;

	Player_manager* pm;

	void addPack (Socket::package imp);

public:
	Server (int port);
	virtual ~Server();

	void acceptOne();

	void send (char* data, int size);
	void sendNext();
	void sendImportant();

	void setPM (Player_manager* _pm) {pm = _pm;}

};

#endif	/* _SERVER_H */

