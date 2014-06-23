
/**************************************************	
	类    名：Receiver
	功能说明：
	  在线程执行过程中循环读取相应端口，并且将数据封装为
	  相应的结构。如果收到原始数据，则将原始数据存储在缓存中
**************************************************/
class CUDPReceiver:public CReceiver
{
public:
	CUDPReceiver(SOCKET s );


	~CUDPReceiver();
	/*
	 *	调用该接口得到存储在buffer_中的原始数据。通过信号量sema_对读写过程
	 *	进行同步
	 */
	int Receive(TopicID& topic, char*& data , int& length);

	/*
	 UDP的组播初始化
	 */
	int Init_multicast();
private:

	//SOCKET句柄
	SOCKET s_;


	//存储当前的原始数据的缓存
	char buffer_[UDP_HEADER_LEN];
	//当前缓存的长度
	int buffer_len_;

	struct sockaddr_in from;//对方信息
	int addrlen;//sockaddr_in length

};
