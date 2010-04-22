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
#include <vector>
#include "Transmitted.h"
#include "Socket.h"

using std::string;
using std::list;
using std::vector;
class Player_manager;

class Client :protected Socket, public list <Transmitted*>
{
//	char buffer[Buffer_size];

	Player_manager* pm;
	vector<package> undelivered;

	int Combine_undelivered (char* buf, int size);

public:
	Client();
	virtual ~Client();

	void Connect (string adress, int port);
	int Receive (char* data, int max_size);
	void Check_events (char* buffer, int bsize);

	void Set_pm (Player_manager* pm_) {pm = pm_;}

	void Send_reaction (char* buffer, int bsize);
	void Receive_next (char* buffer, int bsize);

};

#endif	/* _CLIENT_H */

