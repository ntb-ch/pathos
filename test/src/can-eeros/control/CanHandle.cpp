#include "CanHandle.hpp"
#include <fcntl.h>

using namespace eeros::control;

CanHandle::CanHandle(std::string can_iface)
{
      /* Create the socket */
	if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		throw eeros::EEROSException("Error: Failed to create can socket.");
		return;
	}
	 
	/* Locate the interface you wish to use */
	can_iface.copy(ifr.ifr_name, can_iface.length(), 0);
	if(ioctl(sock, SIOCGIFINDEX, &ifr) < 0){ 		// ifr.ifr_ifindex gets filled with that device's index
		throw eeros::EEROSException("Error: Failed to locate can interface");
		return;
	}
	 
	// Select that CAN interface, and bind the socket to it
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
		throw eeros::EEROSException("Error: Failed to bind can socket");
		return;
	}
	int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	
	init_can_nodes(sock);
}

CanHandle::~CanHandle()
{
	close(sock);
}

int CanHandle::getSocket()
{
	return sock;
}
