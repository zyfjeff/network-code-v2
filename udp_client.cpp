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
#include <string.h>
#include <errno.h>

void echo_client(int sockfd)
{
    char buf[4096] = {0};
    struct sockaddr_in peeraddr;
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(9999);
    int ret = inet_pton(AF_INET,"127.0.0.1",(void*)&peeraddr.sin_addr);
    if (ret != 1)
        perror("trandfomr ip addr\n");
    socklen_t len = sizeof(peeraddr);
    connect(sockfd,(struct sockaddr*)&peeraddr,sizeof(peeraddr));
    while(fgets(buf,sizeof(buf),stdin) != NULL)
    {

        int ret = sendto(sockfd,buf,strlen(buf),0,
                            (struct sockaddr*)&peeraddr,len);
        

        if (ret == -1) {
            if (errno == EINTR)
                continue;
            perror("sendto1:");
            exit(1);
        }

        ret = sendto(sockfd,buf,strlen(buf),0,
                            (struct sockaddr*)&peeraddr,len);
        
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            perror("sendto2:");
            exit(1);
        }

        ret = recvfrom(sockfd,buf,sizeof(buf),0,
                            (struct sockaddr*)&peeraddr,&len);
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            perror("recvfrom:");
            exit(1);
        }
        buf[ret] = '\0';
        printf("response:%s\n",buf);
    }
}

int main()
{
    //udp socket
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    echo_client(sockfd);
    close(sockfd);
    return 0;
}

