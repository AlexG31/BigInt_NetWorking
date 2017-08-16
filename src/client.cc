#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "10086"	// the port users will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once 

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int UDP_get(char* server_address) {
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(server_address, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

    char msg[] = "client udp message";
	if ((numbytes = sendto(sockfd, msg, strlen(msg), 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

    // Receive
	char s[INET6_ADDRSTRLEN];
    const int max_buffer_length = MAXDATASIZE;
    char Buffer[max_buffer_length];
    char *buf = Buffer;
    sprintf(Buffer, "Hello from UDP");
	printf("listener: waiting to recvfrom...\n");

	/*unsigned int addr_len = fromlen;*/
	if ((numbytes = recvfrom(sockfd, Buffer, max_buffer_length - 1, 0,
		p->ai_addr, &(p->ai_addrlen))) == -1) {
		perror("recvfrom");
		exit(1);
	}

	buf[numbytes] = '\0';
    FILE *fout = fopen("./received.dat", "wb");
    fwrite(buf, sizeof(char), numbytes, fout);
    fclose(fout);
	printf("listener: received data saved as received.dat\n", buf);
    
    
	freeaddrinfo(servinfo);

	printf("talker: sent %d bytes to %s\n", numbytes, server_address);
	close(sockfd);

	return 0;
    
}
int TCP_get(char* server_address) {
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server_address, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); 

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    
    
    FILE *fout = fopen("./received.dat", "wb");
    fwrite(buf, sizeof(char), numbytes, fout);
    fclose(fout);

    printf("client: data saved as received.dat\n");

    close(sockfd);

    return 0;
}

int main(int argc, char *argv[])
{

    if (argc != 3) {
        fprintf(stderr,"usage: client hostname [TCP/UDP]\n");
        exit(1);
    }
    if (strcmp(argv[2], "TCP") == 0) {
        TCP_get(argv[1]);
    } else if (strcmp(argv[2], "UDP") == 0){
        UDP_get(argv[1]);
    }

    return 0;
}
