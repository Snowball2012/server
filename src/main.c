/* test server, just for integer */


#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>


#define PORT_NUM 0x401 /*1025*/

struct ClientNode {
	int fd;
	struct ClientNode * next;
};

/*ACHTUNG!!!!!!!!!! GLOBAL VARIABLE!!!!!*/
int counter = 0;

/* Creates a new listening socket */
int CreateSocket(int port)
{
	int ls;
	struct sockaddr_in addr;
	ls = socket(AF_INET,SOCK_STREAM,0);
	if(ls == -1) {
		perror("socket");
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if(0 != bind(ls, (struct sockaddr *) &addr, sizeof(addr))) {
		perror("bind");
		return -1;
	}
	if(-1 == listen(ls, 5)) {
		perror("listen");
		return -1;
	}
	return ls;
}

/*Adds new client to the client list*/
struct ClientNode * AddClient(int lsock, struct ClientNode * list) 
{
	struct ClientNode * newcl = (struct ClientNode *)malloc(sizeof(*newcl));
	newcl->fd = accept(lsock, NULL, NULL);
	if (newcl->fd == -1) {
		perror("accept");
		free(newcl);
		return list;
	}
	newcl->next = list;
	return newcl;
}


struct ClientNode * DeleteClient(struct ClientNode * cl, 
				struct ClientNode * prev, struct ClientNode ** clients)
{
	if(cl == prev) {
		*clients = (*clients)->next;
		free(cl);
		return *clients;
	} else {
		prev->next = cl->next;
		free(cl);
		return prev;
	}
}
			


int ProcessClient(struct ClientNode * cl)
{
	char buff;
	const char * message = "wrong command, try +, - or d\n";
	if(!read(cl->fd, &buff, sizeof(char))) {
		shutdown(cl->fd, 2);
		close(cl->fd);
		return 1;
	}
	switch(buff) {
		case 'd': 
			write(cl->fd, &counter, sizeof(int));
			break;
		case '+':
			counter++;
			write(cl->fd, &counter, sizeof(int));
			break;
		case '-':
			counter--;
			write(cl->fd, &counter, sizeof(int));
			break;
		default:
			/*write(cl->fd, message, strlen(message));*/
			break;
	}
	return 0;
}


int main(int argc, char ** argv)
{
	int lsock;
	struct ClientNode * clients = NULL;
	int port;
	if(argc == 1) 
		port = PORT_NUM;
	else 
		return 1;
	printf("    Server v0.01\n");
	lsock = CreateSocket(port);
	if (lsock == -1)
		return 1;
	for(;;) { /**/
		fd_set readfds;
		int maxDs = lsock;
		int res;
		FD_ZERO(&readfds);
		FD_SET(lsock, &readfds);
		struct ClientNode * cl, *prev;
		for (cl = clients; cl!=NULL; cl = cl->next) {
			FD_SET(cl->fd, &readfds);
			if(cl->fd > maxDs)
				maxDs = cl->fd;
		}
		res = select(maxDs+1, &readfds, NULL, NULL, NULL);
		if(res<1) {
			/*handle error*/
		}
		if(FD_ISSET(lsock, &readfds)) {	
			clients = AddClient(lsock, clients);
			printf("new client connected\n");
		}
		prev = clients;
		for(cl = clients; cl != NULL; cl = cl->next) {
			if(FD_ISSET(cl->fd, &readfds)) {
				/*read data, process it*/
				int status;
				printf("new data arrives from client\n");
				status = ProcessClient(cl);
				if(1 == status) { 
					cl = DeleteClient(cl, prev, &clients);
					if(cl == NULL)
						break;
				}
			}
			prev = cl;
		}
	}
	return 0;
}
