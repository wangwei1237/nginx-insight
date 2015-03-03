/*
 * tcp_client.c
 *
 *  Created on: 2015年1月14日
 *      Author: wangwei
 */
#include "tcp_client.h"

void tcp_client()
{
	struct sockaddr_in pin;
	struct hostent *nlp_host;
	int sd;
	char host_name[256];
	int port;
	char dst[256];
	char send[1024];
	char recv[1024];

	strcpy(host_name, "127.0.0.1");
	port = 8080;

	if ((nlp_host = gethostbyname(host_name)) == 0) {
		printf("解析域名失败！\n");
	} else {
		printf("the ip is %s:%s\n", nlp_host->h_name, inet_ntop(nlp_host->h_addrtype, nlp_host->h_addr_list[0], dst, 256));
	}

	bzero(&pin, sizeof(pin));
	pin.sin_family = AF_INET;
	pin.sin_addr.s_addr = htonl(INADDR_ANY);
	pin.sin_addr.s_addr = ((struct in_addr *)(nlp_host->h_addr))->s_addr;
	pin.sin_port = htons(port);

	//创建tcp连接
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sd, (struct sockaddr *)&pin, sizeof(pin)) == -1) {
		printf("创建socket套接字失败！\n");
	} else {
		printf("the socket is : %d\n", sd);
	}

	memset(send, 0, sizeof(send));
	memset(recv, 0, sizeof(recv));

	char *send1 = "GET /tests/test/index HTTP/1.1\r\nHost: 127.0.0.1:8080\r\n";
	strcpy(send, send1);

//	int j = strlen(send);
//	for(int i =j; i< 1124; ++i) {
//		send[i] = 'A';
//	}

	//写入请求数据
	write(sd, send, strlen(send));
	printf("the sizeof of the send: %lu\n", sizeof(send));

	//写入\r\n，结束请求首部.
	strcpy(send, "\r\n");
	write(sd, send, strlen(send));

	//读取nginx响应数据.
	read(sd, recv, 1000);
	printf("the response of the server is: \n%s", recv);

	//关闭连接
	close(sd);
	printf("the connection is close.\n");
}
