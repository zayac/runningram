// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <string>
#include <arpa/inet.h>
#include "UniId.h"

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

struct soc_data
{
	int unreceived_size;

	int m_sock;
	sockaddr_in m_addr;
	soc_data (int sock);
};

class Socket: public UniId <soc_data>
{//public:
//	int m_sock;
//	sockaddr_in m_addr;

protected:
    void Delete_data();

public:
	Socket();
	virtual ~Socket();

	// Server initialization
	bool create();
	bool bind (const int port);
	bool listen() const;
	bool accept (Socket&) const;

	// Client initialization
	bool connect (const std::string& host, const int port);

	// Data Transimission
	bool send (const std::string&) const;
	int recv (std::string&) const;

	int send (char* data, int size) const;
	int recv (char* buffer, int max_size) const;


	void set_non_blocking (bool);

	bool is_valid() const
	{
		return data() !=0 && data()->m_sock != -1;
	}
};


#endif
