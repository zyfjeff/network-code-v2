/*
 =====================================================================================
        Filename:  epoll-v1.cpp
     	Description: epoll版本的实现
        Version:  1.0
        Created:  05/07/2015 10:52:22 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//存放返回的描述符的epoll的封装
typedef std::vector<struct epoll_event> event_list;

//错误处理
#define ERR_EXIT(m) do {               \
        perror(m);                     \
        exit(EXIT_FAILURE);            \
}while(0)

int main(void)
{
    signal(SIGPIPE,SIG_IGN); //SIGPEPE产生的原因
    signal(SIGCHLD,SIG_IGN);
    //用于处理EMFILE错误 
    int idlefd = open("/dev/null",O_RDONLY | O_CLOEXEC);
    int listenfd;
    if ((listenfd = socket(PF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,
        IPPROTO_TCP)) < 0)
        ERR_EXIT("craete socket");
    
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int on = 1;
    if (setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
        ERR_EXIT("setsockopt reuseaddr");

    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
        ERR_EXIT("bind socket");
    
    if (listen(listenfd,SOMAXCONN) < 0)
        ERR_EXIT("listen");

    std::vector<int> clients;
    
    //创建epolll描述符,添加监听套接字到这个描述符中
    int epollfd;
    //epoll_create 和epoll_create1的区别和联系
    epollfd = epoll_create1(EPOLL_CLOEXEC);
    struct epoll_event event;
    event.data.fd = listenfd;
    event.events = EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&event);

    //存放返回的描述符的epoll结构体
    event_list events(100);

    //存放客户端连接信息
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int connfd;

    int nready;
    while(1)
    {
        nready = epoll_wait(epollfd,&*events.begin(),
                            static_cast<int>(events.size()),-1);
        if (nready == -1) {
            if (errno == EINTR) //自重启
                continue;
            ERR_EXIT("epoll_wait");
        }

        if (nready == 0)
            continue;

        //调整epoll结构体向量的大小
        if ((size_t)nready == events.size())
            events.resize(events.size()*2);
        
        for(int i = 0;i < nready; ++i)
        {
            if (events[i].data.fd == listenfd) {
                peerlen = sizeof(peeraddr);
                connfd = accept4(listenfd,(struct sockaddr*)&peeraddr,
                                &peerlen,SOCK_NONBLOCK|SOCK_CLOEXEC);
                if (connfd == -1) {
                //处理EMFILE错误
                    if (errno == EMFILE) {
                        
                        close(idlefd);
                        idlefd = accept(listenfd,NULL,NULL);
                        close(idlefd);
                        idlefd = open("/dev/null",O_RDONLY | O_CLOEXEC);
                        continue;
                    } else {
                        ERR_EXIT("accept4");   
                    } // end else
                } //end if
                    std::cout << "ip=" << inet_ntoa(peeraddr.sin_addr) <<
                                "port=" << ntohs(peeraddr.sin_port) << std::endl;
                    clients.push_back(connfd);
                    event.data.fd = connfd;
                    event.events = EPOLLIN;
                   epoll_ctl(epollfd,EPOLL_CTL_ADD,connfd,&event);
              } else if (events[i].events & EPOLLIN) {
                        connfd = events[i].data.fd;
                        if (connfd < 0)
                            continue;
                        char buf[1024] = {0};
                        int ret = read(connfd,buf,1024);
                        if (ret == -1)
                            ERR_EXIT("read");
                        if (ret == 0) { 
                            std::cout << "client close" << std::endl; 
                            close(connfd);
                            event = events[i];
                            epoll_ctl(epollfd,EPOLL_CTL_DEL,connfd,&event);
                            //remove和erase组合使用,remove只是将符合条件的元素
                            //移动到末尾,最后还需要使用erase来删除
                            clients.erase(std::remove(clients.begin(),clients.end(),
                                            connfd),clients.end());
                            continue;
                        
                        }
                        std::cout << buf << std::endl;
                        write(connfd,buf,strlen(buf));
              } //end if
            } //end for
        }//end while
        return 0;
}



