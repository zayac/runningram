// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <string>
#include <arpa/inet.h>
#include "UniId.h"
#include "Transmitted.h"

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

const int max_packets_in_net = 5;

struct soc_data
{
	int unreceived_size;

	int m_sock;
	sockaddr_in m_addr;
	soc_data (int sock);
};

class Socket: public UniId <soc_data>
{
public:
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
		void deleteData();
	public:
		~package()
		{
			destroy();
		}

	};

protected:
    void deleteData();

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
	int send (char* data, int size) const;		//-1 means error occured
	int recv (char* buffer, int max_size) const;//-1 means error occured


	void set_non_blocking (bool);

	bool isValid() const
	{
		return data() !=0 && data()->m_sock != -1;
	}
};


#endif
