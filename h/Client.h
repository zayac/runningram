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

	int combineUndelivered (char* buf, int size);

public:
	Client();
	virtual ~Client();

	void connect (string adress, int port);
	int receive (char* data, int max_size);
	void checkEvents (char* buffer, int bsize);

	void setPM (Player_manager* pm_) {pm = pm_;}

	void sendReaction (char* buffer, int bsize);
	void receiveNext (char* buffer, int bsize);

};

#endif	/* _CLIENT_H */

