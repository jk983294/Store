#ifndef SOCKET_UTILITY_H
#define SOCKET_UTILITY_H

class CSocket_utility
{
public:
	CSocket_utility(){}
	~CSocket_utility(){}
	/* �������ݵĹ��ߺ���*/ 
	/*ʹ��SOCKET.recv���ն�������*/
	int recvFixedLengthData(SOCKET sclient,char* targetbuf,int length);
	/*ʹ��SOCKET.recv���մ󳤶ȱ䳤����*/
	int recvVarLengthData(SOCKET sclient,char* targetbuf,int expectlen);
	/*ʹ��SOCKET.send��������*/
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