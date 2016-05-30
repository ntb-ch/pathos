#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>      /* inet_ntoa() to format IP address */
    #include <stdbool.h>
      
#define PORTNO 9876

#define TRUE 1
#define FALSE 0
#define NUM_ID 48

#include <array>

    
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
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
     serv_addr.sin_port = htons(PORTNO);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
  
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     printf("\n- Connected to client IP --> %s !", inet_ntoa(serv_addr.sin_addr) );
 
     listen(sockfd,1);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,  &clilen);
        if (newsockfd < 0)     error("ERROR on accept");
        //else printf("\n- Connected to %s ", argv[1] );
 
 int count = 0;	
 int id;
 int idx;
 bool Detection[NUM_ID];
 
 std::array<bool,NUM_ID> detection;
 
	while(1)
	{
		//recieving packet
		n = recv(newsockfd,(char*) buffer, sizeof(char)*1024, 0);
		if(n ==0 || n ==-1 )
		{
			close(newsockfd); 
			break;
		}
		
		//RECIEVE_DATA_LEN = sizeof(buffer);
		memset(Detection,0,sizeof(bool)*NUM_ID);


		for(id=0;id<NUM_ID;id++)
		{
		     idx = id/8;
				unsigned char d;
				switch(id%8)
				{
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
				{
					Detection[id] = TRUE;
				}
				else
				{
					Detection[id] = FALSE;
				}
		}
		
		
			printf("\n");
			printf("%03d:",count);
			int i;
			for(i=0;i<NUM_ID;i++)
			{
				//if(Detection[i]==TRUE)
			//	{

					printf("[%d]", Detection[i]);
			//	}
			}
			count++;
			printf("\n");

	}
  
      close(newsockfd);
      close(sockfd);
      return 0; 
}
