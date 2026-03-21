#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> // Added for inet_addr()
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
    int sockfd, n; 
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n"); 
        exit(0); 
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

   
    if ((connect(sockfd, (SA*)&servaddr, sizeof(servaddr))) < 0) { 
        printf("socket connection failed...\n"); 
        exit(0); 
    } 

   
    char *msg = "malek";
    send(sockfd, msg, strlen(msg), 0); 
    printf("Message sent!\n");

    
    n = recv(sockfd, buff, sizeof(buff) - 1, 0);
    
    if (n >= 0) {
        buff[n] = '\0'; 
        printf("Server reply: ");
        puts(buff);
    }

    close(sockfd);
    return 0;
}