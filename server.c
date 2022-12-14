#include <stdio.h>
#include <stdlib.h>//包含exit()
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>//包含struct sockaddr_in
#include <ctype.h>//包含toupper()
#include <strings.h>//包含bzero()
 
#define SERV_IP INADDR_ANY  //INADDR_ANY是192.168.x.x内部地址
#define SERV_PORT 8888
 
int main(void)
{
	int serv_fd, clie_fd;
	struct sockaddr_in serv_addr, clie_addr;
	socklen_t clie_addr_len;
	char buf[BUFSIZ], clie_ip_buf[BUFSIZ];//BUFSIZ是内置的宏，专门用来指定buf大小
	int n, i;
 
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
 
	bzero(&serv_addr, sizeof(serv_addr));//类似于memeset()，bzero()是直接初始化为0
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);//host to network short转成网络字节序
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//host to network long转成网络字节序；
	bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
 
	listen(serv_fd, 128);
	
	clie_addr_len = sizeof(clie_addr);
	clie_fd = accept(serv_fd, (struct sockaddr *)&clie_addr, &clie_addr_len);//参数3是传入传出参数，因为传入，所以初始化，因为传出，所以取地址
	printf("client IP: %s,  client port: %d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_ip_buf, sizeof(clie_ip_buf)), ntohs(clie_addr.sin_port));
	while(1){	
		n = read(clie_fd, buf, sizeof(buf));//从客户socket里读，读到buf里，读的空间是sizeof(buf)这么大，读到了n个字符
		for(i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		write(clie_fd, buf, n);//往clie_fd里写，写的是buf里的东西，写n个字符；
	}
 
	close(serv_fd);
	close(clie_fd);
 
	return 0;
}
