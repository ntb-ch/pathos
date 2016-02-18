#include "SensorsThread.hpp"
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>      /* inet_ntoa() to format IP address */
      
#include <iostream>
#include <array>

using namespace pathos::peepingpanel;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

SensorsThread::SensorsThread() : read_ptr(&detection1) {
	running = false;
	detection1.fill(false);
}

SensorsThread::~SensorsThread() {
	join();
}

void SensorsThread::stop() {
	running = false;
}

bool SensorsThread::isRunning() {
	return running;
}

const SensorsThread::Array& SensorsThread::getBuffer() {
	return *read_ptr;
}

SensorsThread::Array& SensorsThread::getWriteBuffer() {
	auto p = read_ptr.load();
	if (p == &detection1) return detection2;
	else if (p == &detection2) return detection3;
	else if (p == &detection3) return detection1;
}

void SensorsThread::flip() {
	read_ptr = &getWriteBuffer();
}

void SensorsThread::run() { 
	std::cout << "Sensors thread started" << std::endl;
	
	int i = 0;
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[1024];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_port = htons(9876);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	printf("\n- Connected to client IP --> %s \n!", inet_ntoa(serv_addr.sin_addr) );
		
	listen(sockfd,1);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,  &clilen);
	if (newsockfd < 0)     error("ERROR on accept");
	
	running = true;
	
	int count = 0;
	int id;
	int idx;
	
	auto *detection = &getWriteBuffer();
	
	while(running) {
		//recieving packet
		n = recv(newsockfd,(char*) buffer, sizeof(char)*1024, 0);
		if(n == 0 || n == -1 ) {  // TODO
			stop();
			break;             
		}
		
		detection->fill(false);
		
		for(id=0;id<NUM_ID;id++) {
			idx = id/8;
			unsigned char d;
			switch(id%8) {
				case 0:
					d= 0x80;
					break;
				case 1:
					d= 0x40;
					break;
				case 2:
					d= 0x20;
					break;
				case 3:
					d= 0x10;
					break;
				case 4:
					d= 0x08;
					break;
				case 5:
					d= 0x04;
					break;
				case 6:
					d= 0x02;
					break;
				case 7:
					d= 0x01;
					break;
			}
			
			if(buffer[idx]&d) 
				(*detection)[id] = true;
			else 
				(*detection)[id] = false;
		}
		
// 		for(int i=0;i<NUM_ID;i++)
// 			printf("[%d]", detection[i]);
		
		flip();
		detection = &getWriteBuffer();

		count++;
	}
	
	close(newsockfd);
	close(sockfd);
}
