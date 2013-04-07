#include "StdAfx.h"
#include "Network.h"

#include "afxsock.h"
#include "MacroDefinition.h"

#undef ERROR
#include <glog/logging.h>

#define SERVER_IP "10.176.191.6"
#define SERVER_PORT 26314
#define BUFFER_SIZE 8192

CNetwork::CNetwork(void)
{
}

CNetwork::~CNetwork(void)
{
}


int CNetwork::SendPacketToProxyServer(unsigned char* packet, int pkt_size)
{
	int ret = 0;
	int err = 0;
	int sd_bytes = 0;
	int bytes = 0;
	int rv_bytes = 0;

	unsigned char recv_buff[BUFFER_SIZE];
	unsigned char recv_pkt[BUFFER_SIZE];

	USES_CONVERSION;
	AfxSocketInit();
	CSocket ConnectSocket;

	//Create a socket.
	ConnectSocket.Create();
	err = ConnectSocket.Connect(_T(SERVER_IP), SERVER_PORT);
	LOG(INFO)<<"IP="<<SERVER_IP<<" Port="<<SERVER_PORT;
	LOG_IF(WARNING,0==err)<<"TCP Connection to Proxy Server. --> [Failed].";

	//Sending packet now.
	LOG(INFO)<<"TCP Send_length="<<pkt_size;
	LOG(INFO)<<"TCP Send_data="<<packet;
	sd_bytes = ConnectSocket.Send(packet, pkt_size);
	if(0<sd_bytes){
		LOG(INFO)<<"TCP Send ---> [OK].";
	}
	else{
		LOG(WARNING)<<"TCP Send ---> [Failed].";
	}

	//Receiving packet now.
	do{
		memset(recv_buff, 0, sizeof(recv_buff));
		bytes = ConnectSocket.Receive(recv_buff, sizeof(recv_buff));
		if (bytes>0){
			memcpy(recv_pkt + rv_bytes, recv_buff, bytes);
			rv_bytes = rv_bytes + bytes;
		}
	}while(bytes>0);

	if(0<rv_bytes){
		LOG(INFO)<<"TCP Recv_length="<<rv_bytes;
		LOG(INFO)<<"TCP Recv_data="<<recv_pkt;
		LOG(INFO)<<"TCP Recv ---> [OK].";
		ret = 0;
	}else{
		LOG(WARNING)<<"TCP Recv ---> [Failed].";
		ret = -1;
	}

	ConnectSocket.Close();
	LOG(INFO)<<"TCP close.";
	return ret;
}

int CNetwork::GetCurrentUrl(CString &pure_url, COleControl *h)
{
	int ret = 0;
	int err = 0;

	CComPtr<IOleClientSite> spClientSite;
	spClientSite = (h)->GetClientSite();
	LOG_IF(ERROR,NULL==spClientSite)<<"GetCurrentUrl GetClientSite --> ERROR.";

	CComPtr <IOleContainer> spCont; 
	spClientSite->GetContainer(&spCont);
	LOG_IF(ERROR,NULL==spClientSite)<<"GetCurrentUrl GetContainer --> ERROR.";

	IServiceProvider *pISP;
	spCont->QueryInterface (IID_IServiceProvider,(void**)&pISP);
	LOG_IF(ERROR,NULL==spClientSite)<<"GetCurrentUrl QueryInterface --> ERROR.";

	CComQIPtr<IWebBrowser2> m_WebBrowser2;
	pISP-> QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&m_WebBrowser2);
	LOG_IF(ERROR,NULL==spClientSite)<<"GetCurrentUrl QueryService --> ERROR.";

	LPOLESTR lpURL = NULL;
	m_WebBrowser2->get_LocationURL(&lpURL);
	char *strurl;
	CComVariant varURL(lpURL);
	varURL.ChangeType(VT_BSTR);
	USES_CONVERSION;
	strurl = OLE2A(varURL.bstrVal);

	CString WebUrl(strurl);
	int pos = 0;
	pos = WebUrl.Find('/',7); 		//http://
	if(!WebUrl.IsEmpty()){
		pure_url = WebUrl.Left(pos);
		ret = 0;
	}
	else{
		ret = -1;
	}

	return ret;
}

int CNetwork::RecvPacketFromProxyServer(unsigned char* packet, int pkt_size)
{
	return 0;
}
int CNetwork::CheckRemoteNetWorking(CString &res_str)
{
	int ret = 0;
	int err = 0;

	char *server_ip = NULL;
	char *server_port = NULL;

	int len = 0;

	res_str.Empty();

	USES_CONVERSION;

	//构造一个查询包,去数据库查版本号
	//reassemble the testing packet to get the pgsql version.
	CString tcp_send_buff(_T(TESTING_NETWORK_PKT));
	char *packet = T2A(tcp_send_buff);
	int length = tcp_send_buff.GetLength();

	//初始化WinSocket
	AfxSocketInit();
	CSocket ConnectSocket;

	//Create a socket.
	ConnectSocket.Create();

	//从数据库中取出 数字签名IP与Port
	if(0==NETWORK_SETUP_INNTER_FIXED){
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_IP), &server_ip, &len);
		if(-1==err)
		{
			ret=-1;
			LOG(ERROR)<<"db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_IP) is not valid";
			AfxMessageBox(ERRO_DOWNLOAD_DB);
			m_sys.ExitProc();
			return -1;
		}
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_PORT), &server_port, &len);
		if(-1==err)
		{
			ret=-1;
			LOG(ERROR)<<"db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_PORT) is not valid";
			AfxMessageBox(ERRO_DOWNLOAD_DB);
			m_sys.ExitProc();
			return -1;
		}
		err = ConnectSocket.Connect(A2T(server_ip), atoi(server_port));
		LOG(INFO)<<"IP="<<server_ip<<"Port="<<server_port;

		if(NULL!=server_ip){
			free(server_ip);
			server_ip = NULL;
		}
		if(NULL!=server_port){
			free(server_port);
			server_port = NULL;
		}
		len = 0;
	}else{
		err = ConnectSocket.Connect(_T(PROXY_SERVER_SIGNATURE_IP), PROXY_SERVER_SIGNATURE_PORT);
		LOG(INFO)<<"IP="<<PROXY_SERVER_SIGNATURE_IP<<"Port="<<PROXY_SERVER_SIGNATURE_PORT;
		LOG_IF(ERROR,0==err)<<"TCP Connection to Proxy Server. --> [Failed]."
		<<"IP="<<PROXY_SERVER_SIGNATURE_IP<<"Port="<<PROXY_SERVER_SIGNATURE_PORT;
	}


	int bytes = 0;
	char buff[MAX_STRING_LENGTH];
	memset(buff,0,MAX_STRING_LENGTH);

	//Sending packet now.
	LOG(INFO)<<"Testing TCP Send_length="<<length;
	LOG(INFO)<<"Testing TCP Send_data="<<packet;
	bytes = ConnectSocket.Send(packet, length);
	if(0<bytes){
		res_str= (_T("测试发送数据到数据中心机房 成功."));
		LOG(INFO)<<"TCP Send ---> [OK].";

	}
	else{
		res_str = (_T("测试发送数据到数据中心机房 失败."));
		LOG(WARNING)<<"TCP Send ---> [Failed].";
	}

	//Receiving packet now.
	int total_len = 0;
	CString tcp_recv_buff;
	tcp_recv_buff.Empty();
	CString str;
	do{
		memset(buff, 0, sizeof(buff));
		bytes = ConnectSocket.Receive(buff, sizeof(buff));
		str = buff;
		tcp_recv_buff = tcp_recv_buff + str;
		total_len = total_len + bytes;
	}while(bytes>0);

	if(0<total_len){
		LOG(INFO)<<"Testing TCP Recv_length="<<total_len;
		LOG(INFO)<<"Testing TCP Recv_data="<<T2A(tcp_recv_buff);
		LOG(INFO)<<"Testing TCP Recv ---> [OK].";
		res_str = res_str + (_T("测试接收数据从数据中心机房 成功。\n."));
		res_str = res_str + tcp_recv_buff;
	}else{
		LOG(WARNING)<<"TCP Recv ---> [Failed].";
		res_str = res_str + (_T("测试接收数据从数据中心机房 失败。."));
		ret = -1;
	}

	ConnectSocket.Close();

	LOG(INFO)<<"TCP close.";
	//DisplayBackToWebPage(OutputInfo,DISPLAY_WEB_POSITION);
	return ret;
}
//发送并接收 未 附加数字签名的数据包。
//Send the data to a proxy server.
//成员变量，m_tcp_send_buff和m_tcp_recv_buff
//Member variable: m_tcp_send_buff, m_tcp_recv_buff
int CNetwork::SendRecvPacketToProxyServer(CString &tcp_send_buff, CString &tcp_recv_buff, CString &res_str)
{
	int ret = 0;
	int err = 0;

	int bytes = 0;

	int total_len = 0;
	char *recv_buff = NULL;
	CString m_tcp_send_buff;
	CString m_tcp_recv_buff;

	m_tcp_send_buff.Empty();
	m_tcp_recv_buff.Empty();

	m_tcp_send_buff = tcp_send_buff;


	USES_CONVERSION;
	char *packet = T2A(m_tcp_send_buff);
	int length = m_tcp_send_buff.GetLength();

	CString str;

	char *server_ip = NULL;
	char *server_port = NULL;

	int len = 0;

	//创建一个用于发送 没有签名 通信的Socket
	//Create a socket.
	AfxSocketInit();
	CSocket ConnectSocket;
	ConnectSocket.Create();

	res_str = (_T( "连接代理机." ));
	
	//从数据库中取出 无需 数字签名IP与Port
	if(0==NETWORK_SETUP_INNTER_FIXED){
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_IP), &server_ip, &len);
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_PORT), &server_port, &len);
		err = ConnectSocket.Connect(A2T(server_ip), atoi(server_port));
		LOG(INFO)<<"IP="<<server_ip<<"Port="<<server_port;
		LOG_IF(INFO,0==err)<<"TCP Connection to Proxy Server. --> [Failed].";

		if(NULL!=server_ip){
			free(server_ip);
			server_ip = NULL;
		}
		if(NULL!=server_port){
			free(server_port);
			server_port = NULL;
		}
		len = 0;
	}
	else{
		err = ConnectSocket.Connect(_T(PROXY_SERVER_IP), PROXY_SERVER_PORT);
		LOG(INFO)<<"IP="<<PROXY_SERVER_IP<<"Port="<<PROXY_SERVER_PORT;
		LOG_IF(INFO,0==err)<<"TCP Connection to Proxy Server. --> [Failed].";
	}

	if(0==err){
		ret = -1;
		goto Socket_End;
	}


	bytes = 0;

	//发送数据
	//Sending packet now.
	res_str = res_str + (_T( "发送数据开始." ));
	LOG(INFO)<<"TCP Send_length="<<length;
	LOG(INFO)<<"TCP Send_data="<<packet;
	bytes = ConnectSocket.Send(packet, length);
	if(0<bytes){
		res_str = res_str + (_T("发送数据到数据中心机房 成功."));
		LOG(INFO)<<"TCP Send ---> [OK].";
		ret = 0;
	}
	else{
		res_str = res_str + (_T("发送数据到数据中心机房 失败."));
		LOG(ERROR)<<"TCP Send ---> [Failed].";
		LOG(ERROR)<<"IP="<<PROXY_SERVER_IP<<"Port="<<PROXY_SERVER_PORT;
		ret = -1;
		goto Socket_End;
	}

	//接收数据
	//Receiving packet now.
	total_len = 0;
	recv_buff = NULL;
	recv_buff = (char *)malloc(MAX_RECV_TCP_BUFFER);
	memset(recv_buff,0,MAX_RECV_TCP_BUFFER);

	//此处用循环来收包，是担心一个下行数据包太大。
	//We use a loop to receive a larger packet.
	m_tcp_recv_buff.Empty();
	str.Empty();
	do{
		memset(recv_buff, 0, sizeof(recv_buff));
		bytes = ConnectSocket.Receive(recv_buff, MAX_RECV_TCP_BUFFER);
		str = recv_buff;
		m_tcp_recv_buff = m_tcp_recv_buff + str;
		total_len = total_len + bytes;
	}while(bytes>0);

	//接收会出现错误
	//If errors happened during the receiving process.
	if(0<total_len){
		LOG(INFO)<<"RecvLength="<<total_len<<"RecvData="<<T2A(m_tcp_recv_buff);
		LOG(INFO)<<"TCP Recv ---> [OK].";
		res_str = res_str + (_T( "接收数据 正常." ));
		ret = 0;
	}else{
		LOG(ERROR)<<"TCP Recv ---> [Failed].";
		res_str = res_str + (_T( "接收数据 出错." ));
		ret = -1;
	}
Socket_End:
	//关闭网络连接
	//Close Socket of the network connections
	ConnectSocket.Close();
	LOG(INFO)<<"TCP close.";

	//释放动态申请的内存
	//Free the memory of malloc functions
	if(NULL!=recv_buff){
		free(recv_buff);
		recv_buff = NULL;
	}

	//设置出错标记，Web页面可以读到这个值
	//Set the error flag that can be accessed by Web pages.
	if(0!=ret){
		//SetErrorInfo4Web(OCX_ERR_NETWORK_CODE);
		res_str = res_str + (_T( "设置网络出错标记." ));
	}
	tcp_recv_buff = m_tcp_recv_buff;
	return ret;

}
//Send the data attached digital signature.
//成员变量，m_tcp_send_buff和m_tcp_recv_buff
//Member variable: m_tcp_send_buff, m_tcp_recv_buff
int CNetwork::SendRecvPacketToProxyServer(CString &tcp_send_buff, CString &tcp_recv_buff, CString &res_str, char* padding_data, int padding_data_len)
{
	int ret = 0;
	int err = 0;
	int bytes = 0;

	CHECK(NULL!=padding_data)<<"Input parameters";
	CHECK(0<=padding_data_len)<<"Input parameters";

	CString m_tcp_send_buff;
	CString m_tcp_recv_buff;

	m_tcp_send_buff.Empty();
	m_tcp_recv_buff.Empty();

	m_tcp_send_buff = tcp_send_buff;

	CString str;

	USES_CONVERSION;
	char *packet = T2A(m_tcp_send_buff);
	int length = m_tcp_send_buff.GetLength();
	int signed_packet_len = length + padding_data_len;

	char *signed_packet = NULL;
	signed_packet = (char*)malloc(signed_packet_len +1);
	CHECK(NULL!=signed_packet)<<"Allocate Memory Error.";

	int total_len = 0;
	char *recv_buff = NULL;

	memset(signed_packet, 0, signed_packet_len +1 );
	memcpy(signed_packet, packet, length);
	memcpy(signed_packet+length, padding_data, padding_data_len );


	//创建一个用于通信的Socket
	//Create a socket.
	AfxSocketInit();
	CSocket ConnectSocket;
	ConnectSocket.Create();

	char *server_ip = NULL;
	char *server_port = NULL;

	int len = 0;
	//连接数字签名代理机
	//Connect to a proxy server
	res_str = res_str +(_T( "连接数字签名代理机." ));

	//从数据库中取出 数字签名IP与Port
	if(0==NETWORK_SETUP_INNTER_FIXED){
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_IP), &server_ip, &len);
		if(-1==err)
		{
			ret=-1;
			LOG(ERROR)<<"db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_IP) is not valid";
			AfxMessageBox(ERRO_DOWNLOAD_DB);
			m_sys.ExitProc();
			return -1;
		}
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_PORT), &server_port, &len);
		if(-1==err)
		{
			ret=-1;
			LOG(ERROR)<<"db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_PORT) is not valid";
			AfxMessageBox(ERRO_DOWNLOAD_DB);
			m_sys.ExitProc();
			return -1;
		}
		err = ConnectSocket.Connect(A2T(server_ip), atoi(server_port));
		LOG(INFO)<<"IP="<<server_ip<<"Port="<<server_port;

		if(NULL!=server_ip){
			free(server_ip);
			server_ip = NULL;
		}
		if(NULL!=server_port){
			free(server_port);
			server_port = NULL;
		}
		len = 0;
	}else{
		err = ConnectSocket.Connect(_T(PROXY_SERVER_SIGNATURE_IP), PROXY_SERVER_SIGNATURE_PORT);
		LOG(INFO)<<"IP="<<PROXY_SERVER_SIGNATURE_IP<<"Port="<<PROXY_SERVER_SIGNATURE_PORT;
		LOG_IF(ERROR,0==err)<<"TCP Connection to Proxy Server. --> [Failed]."
		<<"IP="<<PROXY_SERVER_SIGNATURE_IP<<"Port="<<PROXY_SERVER_SIGNATURE_PORT;
	}
	if(0==err){
		ret = -1;
		goto Socket_Sign_End;
	}
	//发送数字签名数据
	//Sending packet now.
	res_str = res_str +(_T( "发送数字签名数据." ));
	LOG(INFO)<<"SendLength="<<signed_packet_len<<"SendData="<<signed_packet;
	bytes = ConnectSocket.Send(signed_packet, signed_packet_len);
	if(0<bytes){
		LOG(INFO)<<"TCP Send ---> [OK].";
		res_str = res_str +(_T( "发送数字签名数据 正常." ));
		ret = 0;
	}
	else{
		LOG(ERROR)<<"TCP Send ---> [Failed].";
		LOG(ERROR)<<"SendLength="<<signed_packet_len<<"SendData="<<signed_packet;
		res_str = res_str +(_T( "发送数字签名数据 错误." ));
		ret = -1;
	}

	//接收数据
	//Receiving packet now.
	res_str = res_str +(_T( "接收数据." ));
	recv_buff = (char*)malloc(MAX_RECV_TCP_BUFFER);
	CHECK(NULL!=recv_buff)<<"Malloc recv_buffer";
	memset(recv_buff,0,MAX_RECV_TCP_BUFFER);

	//此处用循环来收包，是担心一个下行数据包太大。
	//We use a loop to receive a larger packet.
	m_tcp_recv_buff.Empty();
	do{
		memset(recv_buff, 0, sizeof(recv_buff));
		bytes = ConnectSocket.Receive(recv_buff, MAX_RECV_TCP_BUFFER);
		str = recv_buff;
		m_tcp_recv_buff = m_tcp_recv_buff + str;
		total_len = total_len + bytes;
	}while(bytes>0);

	//接收会出现错误
	//If errors happened during the receiving process.
	if(0<total_len){
		LOG(INFO)<<"RecvLength="<<total_len<<"RecvData="<<T2A(m_tcp_recv_buff);
		LOG(INFO)<<"TCP Recv ---> [OK].";
		res_str = res_str +(_T( "接收数据 正常." ));
		ret = 0;
	}else{
		LOG(ERROR)<<"TCP Recv ---> [Failed].";
		res_str = res_str +(_T( "接收数据 错误." ));
		ret = -1;
	}
Socket_Sign_End:
	//关闭网络连接
	//Close Socket of the network connections
	ConnectSocket.Close();
	LOG(INFO)<<"TCP close.";

	//释放动态申请的内存
	//Free the memory of malloc functions
	if(NULL!=signed_packet){
		free(signed_packet);
		signed_packet = NULL;
	}
	if(NULL!=recv_buff){
		free(recv_buff);
		recv_buff = NULL;
	}

	//设置出错标记，Web页面可以读到这个值
	//Set the error flag that can be accessed by Web pages.
	if(0!=ret){
		//SetErrorInfo4Web(OCX_ERR_NETWORK_CODE);
		res_str = res_str +(_T( "设置网络出错标记." ));

	}
	tcp_recv_buff = m_tcp_recv_buff;

	return ret;
}
CString CNetwork::GetServerIP(CString server_type)
{
	return CString();
}

int CNetwork::GetServerPort(CString server_type)
{
	return 0;
}

// Send and receive binary packet to the Signature Server
int CNetwork::SendRecvPacketToSignatureServer(char* send_pkt , int send_pkt_len, char**recv_pkt, int *recv_pkt_len, CString &res_str)
{
	int ret = 0;
	int err = 0;
	int sd_bytes = 0;
	int bytes = 0;
	int rv_bytes = 0;

	//检查输入
	if(NULL==send_pkt || 0>send_pkt_len|| NULL!=*recv_pkt || 0!=*recv_pkt_len){
		LOG(ERROR)<<"Send binary packet to signature server, input parameters, [Wrong]";
		return -1;
	}

	char *recv_buff = NULL;
	char *recv_pkt_buff = NULL;

	USES_CONVERSION;

	//创建一个用于通信的Socket
	//Create a socket.
	AfxSocketInit();
	CSocket ConnectSocket;
	ConnectSocket.Create();

	char *server_ip = NULL;
	char *server_port = NULL;

	int len = 0;
	//连接数字签名代理机
	//Connect to a proxy server
	res_str = res_str +(_T( "连接数字签名代理机." ));

	//从数据库中取出 数字签名IP与Port
	if(0==NETWORK_SETUP_INNTER_FIXED){
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_IP), &server_ip, &len);
		if(-1==err)
		{
			ret=-1;
			LOG(ERROR)<<"db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_IP) is not valid";
			AfxMessageBox(ERRO_DOWNLOAD_DB);
			m_sys.ExitProc();
			return -1;
		}
		err = db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_PORT), &server_port, &len);
		if(-1==err)
		{
			ret=-1;
			LOG(ERROR)<<"db.GetDataByKeyFromDB(_T(DB_PROXY_SERVER_SIGNATURE_PORT) is not valid";
			AfxMessageBox(ERRO_DOWNLOAD_DB);
			m_sys.ExitProc();
			return -1;
		}
		
		err = ConnectSocket.Connect(A2T(server_ip), atoi(server_port));
		LOG(INFO)<<"IP="<<server_ip<<"Port="<<server_port;
		LOG_IF(ERROR,0==err)<<"TCP Connection to Proxy Server. --> [Failed].";

		if(NULL!=server_ip){
			free(server_ip);
			server_ip = NULL;
		}
		if(NULL!=server_port){
			free(server_port);
			server_port = NULL;
		}
		len = 0;
	}else{
		err = ConnectSocket.Connect(_T(PROXY_SERVER_SIGNATURE_IP), PROXY_SERVER_SIGNATURE_PORT);
		LOG(INFO)<<"IP="<<PROXY_SERVER_SIGNATURE_IP<<"Port="<<PROXY_SERVER_SIGNATURE_PORT;
		LOG_IF(ERROR,0==err)<<"TCP Connection to verify Server. --> [Failed]."
		<<"IP="<<PROXY_SERVER_SIGNATURE_IP<<"Port="<<PROXY_SERVER_SIGNATURE_PORT;
	}
	if(0==err){
		ret = -1;
		res_str = res_str +(_T( "连接签名服务器，出错." ));
		goto Socket_Sign_End;
	}
	//发送数字签名数据
	//Sending packet now.
	res_str = res_str +(_T( "发送数字签名数据." ));
	LOG(INFO)<<"SendLength="<<send_pkt_len<<"SendData="<<T2A(db.hex2str(send_pkt,send_pkt_len));
	sd_bytes = ConnectSocket.Send(send_pkt, send_pkt_len);
	if(0<sd_bytes){
		LOG(INFO)<<"TCP Send ---> [OK].";
		res_str = res_str +(_T( "发送数字签名数据 正常." ));
		ret = 0;
	}
	else{
		LOG(ERROR)<<"TCP Send ---> [Failed].";
		LOG(ERROR)<<"SendLength="<<send_pkt_len<<"SendData="<<T2A(db.hex2str(send_pkt,send_pkt_len));
		res_str = res_str +(_T( "发送数字签名数据 错误." ));
		ret = -1;
	}

	//接收数据
	//Receiving packet now.
	res_str = res_str +(_T( "接收数据." ));
	recv_buff = (char*)malloc(MAX_RECV_TCP_BUFFER);
	CHECK(NULL!=recv_buff)<<"Malloc recv_buffer";
	memset(recv_buff,0,MAX_RECV_TCP_BUFFER);

	recv_pkt_buff = (char*)malloc(MAX_RECV_TCP_BUFFER);
	CHECK(NULL!=recv_pkt_buff)<<"Malloc recv_buffer";
	memset(recv_pkt_buff,0,MAX_RECV_TCP_BUFFER);

	//此处用循环来收包，是担心一个下行数据包太大。
	//We use a loop to receive a larger packet.
	int total_pkt_len = 0;
	do{
		memset(recv_buff,0,MAX_RECV_TCP_BUFFER);
		rv_bytes = 0;
		rv_bytes = ConnectSocket.Receive(recv_buff, MAX_RECV_TCP_BUFFER);
		if (rv_bytes>0 && total_pkt_len < MAX_RECV_TCP_BUFFER){
			memcpy(recv_pkt_buff + total_pkt_len, recv_buff, rv_bytes);
			total_pkt_len = total_pkt_len + rv_bytes;
		}
	}while(rv_bytes>0);

	//接收会出现错误
	//If errors happened during the receiving process.
	if(0<total_pkt_len){
		LOG(INFO)<<"RecvLength="<<total_pkt_len<<"RecvData="<<T2A(db.hex2str(recv_pkt_buff, total_pkt_len));
		LOG(INFO)<<"TCP Recv ---> [OK].";
		res_str = res_str +(_T( "接收数据 正常." ));
		
		//将接收到的数据包传出
		//注意，是指针的指针
		*recv_pkt = (char*)malloc(total_pkt_len);
		CHECK(NULL!=*recv_pkt)<<"Malloc recv_buffer";
		memset(*recv_pkt,0,total_pkt_len);
		memcpy(*recv_pkt, recv_pkt_buff, total_pkt_len);
		*recv_pkt_len = total_pkt_len;
		ret = 0;
	}else{
		LOG(ERROR)<<"TCP Recv ---> [Failed].";
		res_str = res_str +(_T( "接收数据 错误." ));
		ret = -1;
	}
Socket_Sign_End:
	//关闭网络连接
	//Close Socket of the network connections
	ConnectSocket.Close();
	LOG(INFO)<<"TCP close.";

	//释放动态申请的内存
	//Free the memory of malloc functions
	if(NULL!=recv_buff){
		free(recv_buff);
		recv_buff = NULL;
	}
	if(NULL!=recv_pkt_buff){
		free(recv_pkt_buff);
		recv_pkt_buff = NULL;
	}

	//设置出错标记，Web页面可以读到这个值
	//Set the error flag that can be accessed by Web pages.
	if(0!=ret){
		//SetErrorInfo4Web(OCX_ERR_NETWORK_CODE);
		res_str = res_str +(_T( "设置网络出错标记." ));

	}
	return ret;
}
