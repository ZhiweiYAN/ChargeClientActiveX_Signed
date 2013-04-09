#ifndef _MACRODEFINITION_H
#define _MACRODEFINITION_H


//程序的功能简介, the introduction of the program.
#define FUNCTION_INFO "\n欢迎使用「广东利安 便民软件平台」！\n Welcome you to [Lian Platform]."


#define MAX_PACKET_FIELD_NUM 128
#define MAX_STRING_LENGTH 65534
#define MAX_RECV_TCP_BUFFER 8192*64
#define MAX_SIGNATURE_LEN 4096

//调试信息所用的宏
#define DEBUG_INFO_ON "DEBUG_ON"
#define DEBUG_INFO_OFF "DEBUG_OFF"

//目录与文件, folders and files
#define CHARGE_CLIENT_INSTALL_DIR "c:\\charge_client\0\0"
#define CHARGE_CLIENT_LOG_FILES   "c:\\charge_client\\log-*\0\0\0\0"
#define CHARGE_CLIENT_LOG_INFO_FILE "c:\\charge_client\\log-info-"
#define CHARGE_CLIENT_LOG_ERROR_FILE "c:\\charge_client\\log-erro-"
#define CHARGE_CLIENT_LOG_WARNING_FILE "c:\\charge_client\\log-warn-"

#define CHARGE_CLIENT_DOWNLOAD_DIR "c:\\Windows\\Downloaded Program Files\\"
#define CHARGE_CLIENT_DOWNLOAD_DB_FILE_NAME "c:\\charge_client\\charge_client_local_db.db"
#define CHARGE_CLIENT_DB_FILE_NAME "c:\\charge_client\\charge_client_local_db.db"

#define STRING_TAIL_NULL 1

#define LEFT_START _T(" |")
#define RIGHT_END  _T("| ")
#define HTTP_RETURN _T("<br />")
#define VAR_LEN_FIELD_LETTER '*'
#define ALPHA_BETA_GAMMA "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define DB_TEMPLATE_ALPHA_BETA_LETTER_SET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*"
#define DB_KEY_ALPHA_BETA_LETTER_SET "0123456789abcdefghijklmnopqrstuvwxyz_"
#define DB_DATA_ALPHA_BETA_LETTER_SET "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*."
#define COMMON_PACKET_HEADER_FIELD_NUM 10
#define ORDER_LOOP_BACK "LOOP_BACK"
#define ORDER_PACK_SEND "PACK_SEND"
#define ORDER_SIGN_SEND "SIGN_SEND"
#define SIGNATUER_SWITCH 1
#define TERMINAL_ID_POSITION 3
#define USER_ID_POSITION 4

//网络配置参数, network setup parameters
#define NETWORK_SETUP_INNTER_FIXED 0 //1 or 0:load from db
#define PROXY_SERVER_IP "10.176.191.6"
#define PROXY_SERVER_PORT 26314




#define PROXY_SERVER_SIGNATURE_IP  "123.139.158.146"
#define PROXY_SERVER_SIGNATURE_PORT 7501

#define DB_PROXY_SERVER_IP "proxy_server_ip"
#define DB_PROXY_SERVER_PORT "proxy_server_port"
#define DB_PROXY_SERVER_SIGNATURE_IP "verify_server_ip"
#define DB_PROXY_SERVER_SIGNATURE_PORT "verify_server_port"

//定义出错的显示信息, define the error informations
#define ERRO_DOWNLOAD_DB _T("本地数据库不完整.将关闭浏览器,请重启浏览器后手动下载数据库！")
#define OCX_ERR_NETWORK_CODE 11
#define OCX_ERR_NETWORK_INFO "网络通信故障,Socket返回出错. 可能原因是网络不通.\
\n(Network failed, and return errors after invoking socket functions.)"
#define OCX_ERR_USB_KEY_CODE 12
#define OCX_ERR_USB_KEY_INFO " USB KEY 出错. 可能原因是系统中没有发现合法的USB KEY.\
\n(USB Key operations failed, and invalid usb keys installed.)"

#define OCX_ERR_Work_State_CODE 13
#define OCX_ERR_Work_State_INFO "发送和接收签名和加密解密, 出错."

//页面的显示位置, the position label of web pages
#define DISPLAY_WEB_POSITION 911L 

//数据库的固定字段, the necessary data record in the local database file. 
#define DB_TIME_STAMP_KEY "DB_TIME_STAMP"
#define DB_VERSION_ID_KEY DB_TIME_STAMP_KEY
#define DB_DOWNLOAD_FLAG_KEY "DB_DOWNLOAD_FLAG"
#define DB_DOWNLOAD_TRUE "1"
#define DB_DOWNLOAD_FALSE "0"
#define COMMON_PKT_HEADR_TEMPLATE "common_packet_header_template"
#define COM_PKT_UP_PKT_TEMPLATE "forward_packet_template"
#define COM_PKT_UP_PKT_DATA "forward_packet_data"
#define COM_PKT_DOWN_PKT_TEMPLATE "backward_packet_template"
//#define TESTING_NETWORK_PKT "    02                                                                                                                  select version();"
#define TESTING_NETWORK_PKT "00"
#define INFO_DB_FILE_EXIST "交费的业务配置数据不在指定目录中,请重新下载. \
\n(The local DB file is not the pre-installed directory. Download it at first, please.)"


//维护类用的宏
#define TureURL "TURE_URL"
#define OCX_FILE "c:\\Windows\\Downlo~1\\ChargeClientActiveX.ocx"
#define DB_FILE "c:\\charge_client\\charge_client_local_db.db"
#define DB_BACKUP_FILE "c:\\charge_client\\charge_client_local_db.bak"
#define DB_URL "TURE_URL/la_client/db_repository/charge_client_local_db.db"
#define INF_FILE "c:\\Windows\\Downlo~1\\ChargeClientActiveX.inf"
#define ICCARD_FILE_1 "c:\\Windows\\System32\\Mwic_32.dll"
#define ICCARD_FILE_2 "c:\\Windows\\System32\\TrqLADll.dll"

#define SYSTEM_LOG_FILES "c:\\charge_client\\log_*.*\0\0"

#define TIME_OUT_INTERVAL 7000

#define CMD_SHUTDOWN "shutdown -f -s -t 15\0"
#define CMD_REMOVE_LOGS "del -s -q c:\\charge_client\\*.*0\0"
#endif
