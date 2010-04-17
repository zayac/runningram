// Implementation of the Socket class.


#include "Socket.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

Socket::Socket() :UniId <soc_data> (new soc_data (-1), 0)
{
}
//--------------------------------------------------------------------------------------------------
Socket::~Socket()
{
}
//--------------------------------------------------------------------------------------------------
void Socket::Delete_data()
{
	if (is_valid ())
	{
		::close (data()->m_sock);
		delete data();
	}
}
//--------------------------------------------------------------------------------------------------
bool Socket::create()
{
	data()->m_sock = socket (AF_INET, SOCK_STREAM, 0);

	if (!is_valid ()) return false;

	// TIME_WAIT - argh
	int on = 1;
	if (setsockopt (data()->m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) & on, sizeof ( on)) == -1)
		return false;

	return true;

}
//--------------------------------------------------------------------------------------------------
bool Socket::bind (const int port)
{
	if (!is_valid ())
		return false;

	data()->m_addr.sin_family = AF_INET;
	data()->m_addr.sin_addr.s_addr = INADDR_ANY;
	data()->m_addr.sin_port = htons (port);

	if (-1 == ::bind (data()->m_sock, (struct sockaddr *) & data()->m_addr, sizeof ( data()->m_addr)))
		return false;

	return true;
}
//--------------------------------------------------------------------------------------------------
bool Socket::listen () const
{
	if (!is_valid ())
		return false;

	if (-1 == ::listen (data()->m_sock, MAXCONNECTIONS))
		return false;

	return true;
}
//--------------------------------------------------------------------------------------------------
bool Socket::accept (Socket& new_socket) const
{
	int addr_length = sizeof ( data()->m_addr);
	new_socket.data()->m_sock = ::accept (data()->m_sock, (sockaddr *) & data()->m_addr, (socklen_t *) & addr_length);

	if (new_socket.data()->m_sock <= 0)
		return false;
	else
		return true;
}
//--------------------------------------------------------------------------------------------------
int Socket::send (char* dat, int size) const
{
	if (size <= 0) return -1;
	if (-1 == ::send (data()->m_sock, &size, sizeof (size), MSG_NOSIGNAL))
		return -1;
	if (-1 == ::send (data()->m_sock, dat, size, MSG_NOSIGNAL))
		return -1;

	return size;
}
//--------------------------------------------------------------------------------------------------
int Socket::recv (char* buffer, int max_size) const
{
	if (data()->unreceived_size != 0)
	{
		char dev_null[1024];
		while (data()->unreceived_size > 1024)
		{
			if (-1 == ::recv (data()->m_sock, dev_null, 1024, 0))//trash old packet
				return -1;
			data()->unreceived_size -= 1024;
		}
		if (-1 == ::recv (data()->m_sock, dev_null, data()->unreceived_size, 0))//trash old packet
			return -1;
		data()->unreceived_size = 0;
	}
	int size = 0;
	if (-1 == :: recv (data()->m_sock, &size, sizeof (size), 0))
		return -1;
	if (size > max_size)
	{
		data()->unreceived_size = size;
		return -1;
	}
	if (-1 == ::recv (data()->m_sock, buffer, size, 0))
	{
		data()->unreceived_size = size;
		return -1;
	}

	return size;
}
//--------------------------------------------------------------------------------------------------
bool Socket::connect (const std::string& host, const int port)
{
	if (!is_valid ()) return false;

	data()->m_addr.sin_family = AF_INET;
	data()->m_addr.sin_port = htons (port);

	int status = inet_pton (AF_INET, host.c_str (), &data()->m_addr.sin_addr);

	if (errno == EAFNOSUPPORT) return false;

	status = ::connect (data()->m_sock, (sockaddr *) & data()->m_addr, sizeof ( data()->m_addr));

	if (status == 0) return true;
	else			 return false;
}
//--------------------------------------------------------------------------------------------------
void Socket::set_non_blocking (bool b)
{
	int opts = fcntl (data()->m_sock, F_GETFL);

	if (opts < 0) return;

	if (b)
		opts = (opts | O_NONBLOCK);
	else
		opts = (opts & ~O_NONBLOCK);

	fcntl (data()->m_sock, F_SETFL, opts);
}
//--------------------------------------------------------------------------------------------------
soc_data::soc_data (int sock)
:m_sock(sock), unreceived_size (0)
{
	memset (&m_addr, 0, sizeof (m_addr));
}
//--------------------------------------------------------------------------------------------------
