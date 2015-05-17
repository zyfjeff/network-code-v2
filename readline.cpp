/*
 =====================================================================================
        Filename:  readline.cpp
     	Description:  按行读取
        Version:  1.0
        Created:  05/17/2015 11:35:23 PM
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>

ssize_t readn(int sockfd,void *buf,size_t len)
{
    size_t nleft = len;
    char *p_buf = (char*)buf;
    ssize_t nread = 0;
    while(nleft > 0)
    {
        nread = recv(sockfd,p_buf,len,0);
        if (nread < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (nread == 0)
            break;
        nleft -= nread;
        p_buf +=nread;
    }
    return (len-nleft);
}


ssize_t writen(int sockfd,void *buf,size_t len)
{
    size_t nleft;
    ssize_t nwrite =0;
    char *p_buf = (char*)buf;
    while(nleft > 0)
    {
        nwrite = send(sockfd,buf,len,0);
        if (nwrite < 0) {
            if (errno == EINTR)
                nwrite = 0;
            else
                return -1;
        } else {
            nleft -= nwrite;
            p_buf += nwrite;
        }
    }
    return len-nleft;
    
}

ssize_t recv_peek(int sockfd,void *buf,size_t len)
{
    while(1)
    {
        int ret = recv(sockfd,buf,len,MSG_PEEK);
        if (ret == -1 && errno == EINTR)
            continue;
        return ret;
    }
}

ssize_t readline(int sockfd,void *buf,size_t maxline)
{
    int ret;
    int nread;
    char *bufp = (char*)buf;
    int nleft = maxline;
    while(1)
    {
        ret = recv_peek(sockfd,bufp,nleft);//一次性嗅探bufp个数据
        if (ret < 0) //发送错误
            return ret;
        else if (ret == 0)
            return ret;
        nread = ret;
        int i;
        for(i = 0;i < nread;i++) //查找是否有\n
        {
            if (bufp[i] == '\n')  //读取i+1个数据
            {
                ret = readn(sockfd,bufp,i+1);
                if (ret != i+1)
                    return -1; //发生错误
                return ret;
            }
        }
        if (nread > nleft) //读取错误
            return -1;
        nleft -= nread;  //减去读到的元素
        ret = readn(sockfd,bufp,nread); //把数据取走
        if (ret <= 0)
            return ret;
        bufp += ret;
    }
    return -1;
}
