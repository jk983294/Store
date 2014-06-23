#ifndef SOCKET_UTILITY_H
#define SOCKET_UTILITY_H

class CSocket_utility
{
public:
	CSocket_utility(){}
	~CSocket_utility(){}
	/* 接收数据的工具函数*/ 
	/*使用SOCKET.recv接收定长数据*/
	int recvFixedLengthData(SOCKET sclient,char* targetbuf,int length);
	/*使用SOCKET.recv接收大长度变长数据*/
	int recvVarLengthData(SOCKET sclient,char* targetbuf,int expectlen);
	/*使用SOCKET.send发送数据*/
	int sendFixedLengthData(SOCKET sclient,char* sourcebuf,int length);
	
	int pack_UDP_Header_packet(short type,char* source,int topic,char* paddr,u_short port,char flag,u_short reference);
	
	void unpack_UDP_Header_packet(short &type);

	int pack_TCP_Header_packet(char* source,short type,int topic,char* paddr,
							 u_short port,u_short len);
	void  unpack_TCP_Header_packet(short &type,char* tcp_packet,int& topic,
							   char*& paddr,u_short& port,u_short &len);
private:

};
#endif