#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include "page.h"



page
request(page web, int error){
    int sockfd = 0;
    int request_len;
    struct sockaddr_in serv_addr;
    struct hostent * server;
    char buffer[BUFSIZ];
    char request_info[1024];
    
    

    
    struct protoent *protoent;
    ssize_t nbytes_total, nbytes_last;
    unsigned short server_port = 80;
    char *hostname =  web.my_hostname;
    char *path = web.my_path;
    char *user_agent = "yinzhou1";
    if(error == 401){
        char request_template[] = "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\nAuthorization: Base eWluemhvdTE6cGFzc3dvcmQ=\r\n\r\n";
        char *Authorization = "";
        request_len = snprintf(request_info, 1024, request_template, path, hostname,user_agent,Authorization);
    }else{
        char request_template[] = "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n";
        request_len = snprintf(request_info, 1024, request_template, path, hostname,user_agent);
    }
    
    
    

    
    if (request_len >= 1024){
        fprintf(stderr, "request length large: %d\n", request_len);
        exit(EXIT_FAILURE);
    }
    
    /* Translate host name into peer's IP address */
    
    server = gethostbyname(hostname);
    if (server == NULL){
        fprintf(stderr, "error: gethostbyname(\"%s\")\n", hostname);
        exit(EXIT_FAILURE);
    }

    /* build TCP socket */
    protoent = getprotobyname("tcp");
    if (protoent == NULL){
        perror("getprotobyname");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, protoent->p_proto);
    if (sockfd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(server_port);

   

    /* Actually connect. */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    /* Send HTTP request. */
    nbytes_total = 0;
    while (nbytes_total < request_len){
        nbytes_last = write(sockfd, request_info + nbytes_total, request_len - nbytes_total);
        if (nbytes_last < 0) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        nbytes_total += nbytes_last;
    }

    /* Read the response. */
    while ((nbytes_total = read(sockfd, buffer, BUFSIZ)) > 0){
        char temp[MAX_RESPONSE];
        snprintf(temp, MAX_RESPONSE, "%s", buffer);
        if(strlen(web.response)+strlen(temp) > MAX_RESPONSE){break;}
        strcat(web.response,temp);
    }
    

    if (nbytes_total < 0){
        perror("read");
        exit(EXIT_FAILURE);
    }
     
    /* parse error code */
    char *str = web.response;
    char *temp = malloc(sizeof(char) * (4));
    memcpy(temp, str+9, 3);
    temp[3] = '\0';
    web.error_code =  temp;

    close(sockfd);
    
    return web;
}
