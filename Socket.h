// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include "constants.h"


#define TCP true
#define UDP false
#define MORE true

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 20;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization

  bool create(bool protocol);
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;
  void close();
  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string s ) const;			//TCP command
  bool send (const std::string s, const bool FLAG ) const;	//TCP command
  bool sendTo ( const std::string s, std::string host, 
				const int port );		//UDP command
  bool sendTo(const std::string s, int host,
	  const int port);		//UDP command
  int recv ( std::string& ) const;
  int recvFrom(std::string&);		      //UDP command

  void set_non_blocking ( const bool );	      //TCP command

  bool is_valid() const { return m_sock != -1; }

  //PRE: assumes that remaddr has a value (AKA recvMessage called)
  //POST: returns the IP address of the remote address
  int getRemoteIP() ;

  //same as above except return the string formatted IP and butts
  std::string getRemoteIP(int dummy);
  
 private:

  //Error flag
  int m_sock;
  struct sockaddr_in m_addr;

  //the recieving socket
  struct sockaddr_in remaddr;
  //length of the addresses
  socklen_t addrlen = sizeof(remaddr);
};


#endif
