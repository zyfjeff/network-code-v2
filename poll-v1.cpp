/*
 =====================================================================================
        Filename:  poll-v1.cpp
     	Description:  第一版 poll
        Version:  1.0
        Created:  05/07/2015 07:37:23 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <poll.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


//一个编程习惯或是技巧吧，错误处理的一种写法
#define ERR_EXIT(m) do {                        \
        perror(m);                              \
        exit(EXIT_FAILURE);                     \
}while(0)


// 在C语言中使用结构体数组，这里使用vector更加方面
typedef std::vector<struct pollfd> poll_fd_list;

int main(void)
{
    // SIGPIPE信号的忽略 需要理解SIGPIPE信号的产生原因
    signal(SIGPIPE,SIG_IGN);
    signal(SIGCHLD,SIG_IGN);

    //下面都是网络编程的惯例,socket bind listen
    int listenfd;
    if((listenfd = socket(PF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,
                        IPPROTO_TCP)) < 0)
        ERR_EXIT("create socket");
    
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    if (setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
        ERR_EXIT("setsockopt reuseaddr");

    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
        ERR_EXIT("bind addr");

    if (listen(listenfd,SOMAXCONN) < 0)
        ERR_EXIT("listen");
    
    //poll的部分，填充poll,注意POLLOUT这个事件,理解什么时候该设置这个事件
    //这个事件是什么原因导致的，盲目设置这个事件导致的问题是什么
    struct pollfd pfd;
    pfd.fd = listenfd;
    pfd.events = POLLIN;
    
    //放到vector中,
    poll_fd_list pollfds;
    pollfds.push_back(pfd);

    int nready; 
    //存放客户端的IP信息
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int connfd; //存在连接过来的fd

    while(1)
    {
        nready = poll(&*pollfds.begin(),pollfds.size(),-1);
        if (nready == -1)
        {
            //自重启
            if (errno == EINTR)
                continue;
            ERR_EXIT("poll");
        }

        if (nready == 0)
            continue;
        //判断监听描述符的读事件
        if (pollfds[0].revents & POLLIN)
        {
            //accept客户端,获取客户端地址进行打印,并填充poll结构体
            peerlen = sizeof(peeraddr);
            connfd = accept4(listenfd,(struct sockaddr*)&peeraddr,&peerlen,
                            SOCK_NONBLOCK | SOCK_CLOEXEC);
            if (connfd == -1)
                ERR_EXIT("accept4");
/*
            if (connfd == -1) {
                if (errno == EMFILE) {
                    close(idlefd);
                    idlefd = accept(listenfd,NULL,NULL);
                    close(idlefd);
                    idlefd = open("/dev/null",O_RDONLY | O_CLOEXEC);
                    continue;
                } else {
                    ERR_EXIT("accept4");   
                }
            }
*/
            pfd.fd = connfd;
            pfd.events = POLLIN;
            pfd.revents = 0;
            pollfds.push_back(pfd);
            --nready;

            //连接成功
            std::cout <<"ip=" << inet_ntoa(peeraddr.sin_addr) <<
                "port=" << ntohs(peeraddr.sin_port) << std::endl;
            if (nready == 0)
                continue;
        }
        
        //是其他描述符的读事件
        for(poll_fd_list::iterator it = pollfds.begin()+1;
            it != pollfds.end() && nready > 0;++it)
        {
            //处理读事件
            if (it->revents & POLLIN) {
               --nready;
               connfd = it->fd;
               char buf[1024] = {0};
               int ret = read(connfd,buf,1024);
               if (ret == -1)
                   ERR_EXIT("read data");
            //如果客户端关闭描述符,则从pollfds中去除
               if (ret == 0)
               {
                   std::cout<<"client close"<<std::endl;
                   pollfds.erase(it);
                   it--;
                   close(connfd);
                   continue;
               }
               std::cout << buf;
               write(connfd,buf,sizeof(buf));
            }
            
        }
    }
}
