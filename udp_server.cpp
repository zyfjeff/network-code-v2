/*
 =====================================================================================
        Filename:  udp_server.cpp
     	Description:  udp echo server
        Version:  1.0
        Created:  05/25/2015 10:03:20 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>



void echo_server(int sockfd)
{
    char buf[4096] = {0};
    struct sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    while(1)
    {

        int ret = recvfrom(sockfd,buf,sizeof(buf),0,
                            (struct sockaddr*)&peeraddr,&len);
        buf[ret] = '\0';
        printf("recvdata:%s\n",buf);
        sendto(sockfd,buf,ret,0,
                            (struct sockaddr*)&peeraddr,len);
    }
}

int main()
{
    //udp socket
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(9999);
    int ret = inet_pton(AF_INET,"127.0.0.1",(void*)&bindaddr.sin_addr);
    if (ret != 1)
        perror("trandfomr ip addr\n");
    bind(sockfd,(struct sockaddr*)&bindaddr,sizeof(bindaddr));
    echo_server(sockfd);
    close(sockfd);
    return 0;
}

