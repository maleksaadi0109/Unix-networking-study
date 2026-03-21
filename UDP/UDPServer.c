#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
  
int main() {
    char buff[MAX];
    int listenfd;
    socklen_t len; 
    struct sockaddr_in servaddr, cliaddr;

    bzero(&servaddr, sizeof(servaddr));
    
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenfd == -1) {
        printf("socket creation failed...\n"); 
        exit(0); 
    }

    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    if ((bind(listenfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    
    printf("UDP Server listening on port %d...\n", PORT);

    len = sizeof(cliaddr);
    int n = recvfrom(listenfd, buff, sizeof(buff) - 1, 0, (SA*)&cliaddr, &len);
    
    if (n >= 0) {
        buff[n] = '\0'; 
        printf("Client says: ");
        puts(buff);
    }
     
    
    sendto(listenfd, buff, strlen(buff), 0, (SA*)&cliaddr, len);
    printf("Reply sent to client.\n");

   
    close(listenfd);
    return 0;
}