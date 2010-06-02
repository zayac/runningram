// Implementation of the Socket class.


#include "Socket.h"
#include "Exception.h"
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
	destroy();
}
//--------------------------------------------------------------------------------------------------
void Socket::deleteData()
{
	if (isValid ())
	{
		::close (data()->m_sock);
		delete data();
	}
}
//--------------------------------------------------------------------------------------------------
bool Socket::create()
{
	data()->m_sock = socket (AF_INET, SOCK_STREAM, 0);

	if (!isValid ()) return false;

	// TIME_WAIT - argh
	int on = 1;
	if (setsockopt (data()->m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) & on, sizeof ( on)) == -1)
		return false;

	return true;

}
//--------------------------------------------------------------------------------------------------
bool Socket::bind (const int port)
{
	if (!isValid ())
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
	if (!isValid ())
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
		throw Exception ("couldn\'t send right package", __LINE__, __FILE__);//return -1;

	return size;
}
//--------------------------------------------------------------------------------------------------
int Socket::recv (char* buffer, int max_size) const
{
	if (data()->unreceived_size != 0)
	{
		if (data()->unreceived_size > max_size) return -1;

		int recved = ::recv (data()->m_sock, buffer, data()->unreceived_size, 0);//recv the old packet
		if (-1 == recved)//data not ready yet
			return -1;
		GODFORBIDlf(recved != data()->unreceived_size, "Not provided situation: received size deffers from expected");
		data()->unreceived_size = 0;
		return recved;
	}
	int size = 0;
	if (-1 == :: recv (data()->m_sock, &size, sizeof (size), 0))
		return -1;
	if (size > max_size)
	{
		data()->unreceived_size = size;
		return -1;
	}
	int recved = ::recv (data()->m_sock, buffer, size, 0);
	if (-1 == recved)
	{
		data()->unreceived_size = size;
		return -1;
	}
	GODFORBIDlf(recved != size, "Not provided situation: received size deffers from expected");

	return size;
}
//--------------------------------------------------------------------------------------------------
bool Socket::connect (const std::string& host, const int port)
{
	if (!isValid ()) return false;

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

//--------------------------------------------------------------------------------------------------
Socket::package::package (int size_, char* data_)
:UniId<pkg_data>(new pkg_data, 0)
{
	data()->size = size_;
	data()->data = new char[size_];
	GODFORBIDlf(data()->data == 0, "can\'t allocate memory");
	memcpy (data()->data, data_, data()->size);
}
//--------------------------------------------------------------------------------------------------
Socket::package::package (Transmitted* from)
:UniId<pkg_data>(new pkg_data, 0)
{
	char buffer[Buffer_size];
	buffer[0] = from->id ();
	data()->size = from->exp (buffer + 1, Buffer_size - 1) + 1;

	//if size == 0, export returned -1 = error
	GODFORBIDlf(0 == data()->size, "can\'t export to buffer");

	data()->data = new char[data()->size];
	GODFORBIDlf(data()->data == 0, "can\'t allocate memory");

	memcpy (data()->data, buffer, data()->size);
}
//--------------------------------------------------------------------------------------------------
int Socket::package::copy_to (char* dst, int max_size)
{
	GODFORBIDlf(max_size < data()->size, "package is too big for this small buffer");
	memcpy (dst, data()->data, data()->size);
	return data()->size;
}
//--------------------------------------------------------------------------------------------------
void Socket::package::deleteData()
{
	if (data()->data) delete [] data()->data;
	delete data();
}
//--------------------------------------------------------------------------------------------------
