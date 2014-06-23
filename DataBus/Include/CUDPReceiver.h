
/**************************************************	
	��    ����Receiver
	����˵����
	  ���߳�ִ�й�����ѭ����ȡ��Ӧ�˿ڣ����ҽ����ݷ�װΪ
	  ��Ӧ�Ľṹ������յ�ԭʼ���ݣ���ԭʼ���ݴ洢�ڻ�����
**************************************************/
class CUDPReceiver:public CReceiver
{
public:
	CUDPReceiver(SOCKET s );


	~CUDPReceiver();
	/*
	 *	���øýӿڵõ��洢��buffer_�е�ԭʼ���ݡ�ͨ���ź���sema_�Զ�д����
	 *	����ͬ��
	 */
	int Receive(TopicID& topic, char*& data , int& length);

	/*
	 UDP���鲥��ʼ��
	 */
	int Init_multicast();
private:

	//SOCKET���
	SOCKET s_;


	//�洢��ǰ��ԭʼ���ݵĻ���
	char buffer_[UDP_HEADER_LEN];
	//��ǰ����ĳ���
	int buffer_len_;

	struct sockaddr_in from;//�Է���Ϣ
	int addrlen;//sockaddr_in length

};
