#include "./../lib/unp.h"

int main(int argc, char *argv[])
{
	int listen_fd = 0;
	int conn_fd = 0;

	struct sockaddr_in serv_addr;

	listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

	// 将对应字节全部置0
	bzero(&serv_addr, sizeof(serv_addr));

	// 协议族
	serv_addr.sin_family = AF_INET;

	// INADDR_ANY 指定0.0.0.0的地址，表示本机所有IP
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 端口指定
	serv_addr.sin_port = htons(9876);

	// 对套接字进行地址和端口绑定
	Bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	Listen(listen_fd, LISTENQ);

	for (;;)
	{

	}

	return 0;
}
