#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>//包含struct sockaddr_in
#include <string.h>//包含memset()
 
#define SERV_IP "127.0.0.1"
#define SERV_PORT 8888
 
int main(void)
{
	int clie_fd;
	struct sockaddr_in serv_addr, clie_addr;
	char buf[BUFSIZ];//BUFSIZ是内置的宏，专门用来指定buf大小
	int n;
 
	clie_fd = socket(AF_INET, SOCK_STREAM, 0);
 
	memset(&serv_addr, 0, sizeof(serv_addr));//把这一块内存空间全部清成0；memset()用来为新申请的内存做初始化
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);//host to network short转成网络字节序
	inet_pton(clie_fd, SERV_IP, &serv_addr.sin_addr.s_addr);//IP字符串转成网络字节序；
	
	connect(clie_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
 
	while(1){
		fgets(buf, sizeof(buf), stdin);//参数1：目的缓冲区指针，参数2：缓冲区大小，参数3：源数据流；stdin：标准输入
		write(clie_fd, buf, strlen(buf));//键盘输入hello/n，fgets后buf里的是hello/n/0,strlen(buf)取的是/0之前的长度
		n = read(clie_fd, buf, sizeof(buf));//写完之后发给服务端了，服务端会响应回来，现在该读了
		write(STDOUT_FILENO, buf, n);//把读到的服务器的响应写到屏幕
	}
 
	close(clie_fd);
 
	return 0;
}
