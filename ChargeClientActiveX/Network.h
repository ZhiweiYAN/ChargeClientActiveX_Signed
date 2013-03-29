#pragma once
#include "berkeleydb.h"
#include "systemmaintenance.h"
class CNetwork
{
public:
	CNetwork(void);
public:
	~CNetwork(void);
public:
	int SendPacketToProxyServer(unsigned char* packet, int pkt_size);
	int RecvPacketFromProxyServer(unsigned char* packet, int pkt_size);
	int GetCurrentUrl(CString &pure_url, COleControl * h);
	int CheckRemoteNetWorking(CString &res_str);
	int SendRecvPacketToProxyServer(CString &tcp_send_buff, CString &tcp_recv_buff, CString &res_str);
	int SendRecvPacketToProxyServer(CString &tcp_send_buff, CString &tcp_recv_buff, CString &res_str, char* padding_data, int padding_data_len);

public:
	CString GetServerIP(CString ServerType);
	int GetServerPort(CString server_type);
	CBerkeleyDB db;
	CSystemMaintenance m_sys;
public:
	// Send and receive binary packet to the Signature Server
int CNetwork::SendRecvPacketToSignatureServer(char* send_pkt , int send_pkt_len, char* *recv_pkt, int *recv_pkt_len, CString &res_str);
};
