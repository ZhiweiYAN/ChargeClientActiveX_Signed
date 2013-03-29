// ChargeClientActiveXCtrl.cpp : CChargeClientActiveXCtrl ActiveX �ؼ����ʵ�֡�
// 2012-12-04
#include "stdafx.h"
#include "ChargeClientActiveX.h"
#include "ChargeClientActiveXCtrl.h"
#include "ChargeClientActiveXPropPage.h"
#include "afxsock.h"
#include "io.h"

#include <db.h>

#undef ERROR
#include <glog/logging.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MacroDefinition.h"

IMPLEMENT_DYNCREATE(CChargeClientActiveXCtrl, COleControl)


// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CChargeClientActiveXCtrl, COleControl)
	ON_MESSAGE(OCM_COMMAND, &CChargeClientActiveXCtrl::OnOcmCommand)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CChargeClientActiveXCtrl, COleControl)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "InputParameterA", dispidInputParameterA, m_InputParameterA, OnInputParameterAChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "InputParameterDebugFlag", dispidInputParameterDebugFlag, m_InputParameterDebugFlag, OnInputParameterDebugFlagChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "OutputParameterA", dispidOutputParameterA, m_OutputParameterA, OnOutputParameterAChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "InputParameterComName", dispidInputParameterComName, m_InputParameterComName, OnInputParameterComNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "InputParameterActName", dispidInputParameterActName, m_InputParameterActName, OnInputParameterActNameChanged, VT_BSTR)
	DISP_PROPERTY_PARAM_ID(CChargeClientActiveXCtrl, "InputFields", dispidInputFields, GetInputFields, SetInputFields, VT_BSTR, VTS_I2)
	DISP_PROPERTY_PARAM_ID(CChargeClientActiveXCtrl, "OutputFields", dispidOutputFields, GetOutputFields, SetOutputFields, VT_BSTR, VTS_I2)
	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "LoadParameter", dispidLoadParameter, LoadParameter, VT_EMPTY, VTS_NONE)
	//	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "AddTemplate", dispidAddTemplate, AddTemplate, VT_EMPTY, VTS_BSTR VTS_I4 VTS_BSTR VTS_I4)
	//	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "RemoveTemplate", dispidRemoveTemplate, RemoveTemplate, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "WebOrder", dispidWebOrder, m_WebOrder, OnWebOrderChanged, VT_BSTR)
	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "TestingFunction", dispidTestingFunction, TestingFunction, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "PrepareInvoiceItem", dispidPrepareInvoiceItem, PrepareInvoiceItem, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "PrintInvoice", dispidPrintInvoice, PrintInvoice, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "ActivexErrorCode", dispidActivexErrorCode, m_ActivexErrorCode, OnActivexErrorCodeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CChargeClientActiveXCtrl, "ActivexErrorInfo", dispidActivexErrorInfo, m_ActivexErrorInfo, OnActivexErrorInfoChanged, VT_BSTR)
	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "GetTerminalID", dispidGetTerminalID, GetTerminalID, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CChargeClientActiveXCtrl, "GetUserID", dispidGetUserID, GetUserID, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CChargeClientActiveXCtrl, COleControl)
	EVENT_CUSTOM_ID("ParameterLoaded", eventidParameterLoaded, FireParameterLoaded, VTS_NONE)
	EVENT_CUSTOM_ID("DisplayActivexWorking", eventidDisplayActivexWorking, FireDisplayActivexWorking, VTS_NONE)
	EVENT_CUSTOM_ID("DisplayLocalDbWorking", eventidDisplayLocalDbWorking, FireDisplayLocalDbWorking, VTS_NONE)
	EVENT_CUSTOM_ID("DisplayInternetWorking", eventidDisplayInternetWorking, FireDisplayInternetWorking, VTS_NONE)
	EVENT_CUSTOM_ID("DisplayAtPosition", eventidDisplayAtPosition, FireDisplayAtPosition, VTS_I4)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CChargeClientActiveXCtrl, 1)
PROPPAGEID(CChargeClientActiveXPropPage::guid)
END_PROPPAGEIDS(CChargeClientActiveXCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CChargeClientActiveXCtrl, "CHARGECLIENTACTI.ChargeClientActiCtrl.1",
					   0x40a92d87, 0x2fd0, 0x40f7, 0x95, 0xb0, 0xa3, 0xee, 0x1e, 0xe8, 0xb7, 0xb6)



					   // ����� ID �Ͱ汾

					   IMPLEMENT_OLETYPELIB(CChargeClientActiveXCtrl, _tlid, _wVerMajor, _wVerMinor)



					   // �ӿ� ID

					   const IID BASED_CODE IID_DChargeClientActiveX =
{ 0xF5B4C55F, 0xAE4F, 0x4F6B, { 0x90, 0xE6, 0x6F, 0xF8, 0x61, 0x0, 0x49, 0x34 } };
const IID BASED_CODE IID_DChargeClientActiveXEvents =
{ 0x1893A798, 0x6B36, 0x4A5D, { 0xB1, 0xEE, 0xFF, 0xC0, 0xA5, 0xE7, 0x56, 0xA9 } };



// �ؼ�������Ϣ

static const DWORD BASED_CODE _dwChargeClientActiveXOleMisc =
OLEMISC_ACTIVATEWHENVISIBLE |
OLEMISC_SETCLIENTSITEFIRST |
OLEMISC_INSIDEOUT |
OLEMISC_CANTLINKINSIDE |
OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CChargeClientActiveXCtrl, IDS_CHARGECLIENTACTIVEX, _dwChargeClientActiveXOleMisc)



// CChargeClientActiveXCtrl::CChargeClientActiveXCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CChargeClientActiveXCtrl ��ϵͳע�����

BOOL CChargeClientActiveXCtrl::CChargeClientActiveXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
		AfxGetInstanceHandle(),
		m_clsid,
		m_lpszProgID,
		IDS_CHARGECLIENTACTIVEX,
		IDB_CHARGECLIENTACTIVEX,
		afxRegApartmentThreading,
		_dwChargeClientActiveXOleMisc,
		_tlid,
		_wVerMajor,
		_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CChargeClientActiveXCtrl::CChargeClientActiveXCtrl - ���캯��

CChargeClientActiveXCtrl::CChargeClientActiveXCtrl()
: m_tcp_recv_buff(_T(""))
, m_tcp_send_buff(_T(""))
{
	InitializeIIDs(&IID_DChargeClientActiveX, &IID_DChargeClientActiveXEvents);

	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	int ret = 0;
	int err = 0;
	m_ActivexErrorInfo.Empty();
	m_ActivexErrorCode = 0;
	m_InputParameterDebugFlag.Empty();


	//the directory for saving the log files
	//��C�̽���һ��Ŀ¼���ڴ�ű������ݿ��ļ���
	//���Ŀ¼�����ڣ��򴴽�һ��������Ѿ����ڣ�����ʲô��
	ret = ::CreateDirectory(_T(CHARGE_CLIENT_INSTALL_DIR),0);

	// Set the prefix name of log_files
	// ����LOG�ļ���ǰ׺������ǿ��ַ�������ʱ������������LOG��
	//google::SetLogDestination(google::INFO,(CHARGE_CLIENT_LOG_INFO_FILE));
	//google::SetLogDestination(google::INFO,(""));
	google::SetLogDestination(google::ERROR,(CHARGE_CLIENT_LOG_ERROR_FILE));
	//google::SetLogDestination(google::WARNING,(CHARGE_CLIENT_LOG_WARNING_FILE));
	google::SetLogDestination(google::WARNING,"");

	/*if(0==init_UKInit_once)
	{
	m_usbkey.InitInstance();
	}
	else
	{
	LOG(INFO)<<"UK have InitInstanced -->[cancel].";
	}
	init_UKInit_once++;*/

	// Guarantee the google-log running once
	// ��֤IE���к�GLOGֻ�ܳ�ʼ��һ�Σ���ȫ�ֱ��������Ƶġ�
	if(0==init_glog_once){
		google::InitGoogleLogging("");
		init_glog_once ++; 
		LOG(INFO)<<"ChargeClient LOG System Work -->[Begin].";

		//for debug		
		CString version_info_str(_T(FUNCTION_INFO));
		version_info_str = version_info_str 
			+_T("\n\n��������: ")_T(__DATE__)
			+_T("\n����ʱ��: ") + _T(__TIME__);
		AfxMessageBox(version_info_str,MB_OK, 0);	

	}

	//make sure that the db file exists.
	ret = _access_s(CHARGE_CLIENT_DOWNLOAD_DB_FILE_NAME,0);
	if(0 ==ret){
		LOG(INFO)<<"Great, the DB file exists.";
		if(-1 == GetDbVersionID())
		{
			LOG(ERROR)<<"Check local DB version ID, [failed]";
			DisplayDebugInfoToWebPage(_T("�����ݿ���,��ȡ�������ݿ�汾ID,����.�����Զ����ر�� ���"));
			download_db_flag = 1;//��OnDraw�����У�������ع�����

		}
		else
		{
			DisplayDebugInfoToWebPage( _T("�������ݿ��ļ��������.") );
		}

	}
	else{
		LOG(INFO)<<"Sorry, the DB file does not exist. We set the download flag.";
		DisplayDebugInfoToWebPage( _T("�������ݿ��ļ���� �쳣; �����Զ����ر�� ���.") );
		download_db_flag = 1;
		//��OnDraw�����У�������ع�����
	}
}



// CChargeClientActiveXCtrl::~CChargeClientActiveXCtrl - ��������

CChargeClientActiveXCtrl::~CChargeClientActiveXCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
	/*if(1==init_UKInit_once)
	{
	m_usbkey.ExitInstance();
	}
	init_UKInit_once--;*/

	LOG(INFO)<<"ChargeClient LOG System Work -->[End]..";
	google::ShutdownGoogleLogging();
}



// CChargeClientActiveXCtrl::OnDraw - ��ͼ������

void CChargeClientActiveXCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	int err = 0;
	if (!pdc)
		return;

	DoSuperclassPaint(pdc, rcBounds);

	m_MainDialog.MoveWindow(rcBounds, TRUE);
	CBrush brBackGnd(TranslateColor(AmbientBackColor()));
	pdc->FillRect(rcBounds, &brBackGnd);

	//��ˢ��ʱ,���ر��ص����ݰ��������ݿ��ļ�,
	//Download the local file of packet configuration.
	if(1==download_db_flag)
	{
		CString web_url;
		web_url.Empty();

		err = m_net.GetCurrentUrl(web_url, (this));
		err = m_sys.UpdateDbFile(web_url, 0);
		if(0 == err)
		{
			DisplayDebugInfoToWebPage( _T("�Զ� ���ر������ݿ��ļ� ����.") );
		}
		else
		{
			DisplayDebugInfoToWebPage( _T("�Զ� ���ر������ݿ��ļ� ����.") );
		}

		download_db_flag++;
		//������ص�db�ļ��Ƿ���ȷ
		if(-1 == GetDbVersionID())
		{
			LOG(ERROR)<<" Check local DB(downloaded) version ID, [failed]";
			//DisplayDebugInfoToWebPage(_T("�����ݿ���,��ȡ�������ݿ�汾ID,����.����Ƿ����سɹ�."));
			AfxMessageBox(ERRO_DOWNLOAD_DB);
			return;
		};
	}
}



// CChargeClientActiveXCtrl::DoPropExchange - �־���֧��

void CChargeClientActiveXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}


// CChargeClientActiveXCtrl::GetControlFlags -
// �Զ��� MFC �� ActiveX �ؼ�ʵ�ֵı�־��
//
DWORD CChargeClientActiveXCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// �ڻ�Ͳ��״̬֮�����ת��ʱ��
	// �������»��ƿؼ���
	dwFlags |= noFlickerActivate;
	return dwFlags;
}



// CChargeClientActiveXCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CChargeClientActiveXCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CChargeClientActiveXCtrl::PreCreateWindow - �޸� CreateWindowEx �Ĳ���

BOOL CChargeClientActiveXCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("STATIC");
	return COleControl::PreCreateWindow(cs);
}



// CChargeClientActiveXCtrl::IsSubclassedControl - ����һ������ؼ�

BOOL CChargeClientActiveXCtrl::IsSubclassedControl()
{
	return TRUE;
}



// CChargeClientActiveXCtrl::OnOcmCommand - ����������Ϣ

LRESULT CChargeClientActiveXCtrl::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN32
	WORD wNotifyCode = HIWORD(wParam);
#else
	WORD wNotifyCode = HIWORD(lParam);
#endif

	// TODO: �ڴ˽�ͨ wNotifyCode��

	return 0;
}



// CChargeClientActiveXCtrl ��Ϣ�������

int CChargeClientActiveXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_MainDialog.Create(IDD_MAINDIALOG, this);

	return 0;
}

void CChargeClientActiveXCtrl::OnInputParameterAChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CChargeClientActiveXCtrl::OnInputParameterDebugFlagChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CChargeClientActiveXCtrl::OnOutputParameterAChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}



void CChargeClientActiveXCtrl::OnInputParameterComNameChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CChargeClientActiveXCtrl::OnInputParameterActNameChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

/*
This function contains the main process of constructing a package and sending it.
InputFields[]: web parameters --> activex varaiable
OutputFields[]: ActiveX variables --> web parameters
*/
void CChargeClientActiveXCtrl::LoadParameter(void)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//Enable Glog INFO output from javascript codes on the web pages.
	if(0==m_InputParameterDebugFlag.Compare(_T(DEBUG_INFO_ON))){
		google::SetLogDestination(google::INFO,(CHARGE_CLIENT_LOG_INFO_FILE));
	}

	int err = 0;
	int ret = 0;
	CString res_str_info;
	USES_CONVERSION;

	//�������õ�����,
	//Empty the array contents for the output of web pages.
	ClearOutputFields();
	m_tcp_send_buff.Empty();
	m_tcp_recv_buff.Empty();
	m_ActivexErrorCode = 0;
	m_ActivexErrorInfo.Empty();

	//��ʾһ���������ݿ�İ汾��,������ʾ��.
	//Display the version id of the local db file.
	if(-1 == GetDbVersionID()){
		LOG(ERROR)<<"Check local DB version ID, [failed]";
		DisplayDebugInfoToWebPage(_T("�����ݿ���,��ȡ�������ݿ�汾ID,����.����Ƿ����سɹ�."));
		AfxMessageBox(ERRO_DOWNLOAD_DB);
		m_sys.ExitProc();
		return;
	};

	//����(����)���ݰ� = ������ͷ + ��ҵ����(����)���ݰ�.
	//up(down)-link packet = common pkt header + business pkt (forward pkt or backward pkt)
	CString sending_packet_data; 

	//ȡ�ô˴β������õ������ݰ���ģ��(�����ݿ���)
	//Get the all templates which are used this time from database.
	CString common_packet_header_template;

	CString forward_packet_template;
	CString backward_packet_template;

	CString sending_packet_template;
	CString recving_packet_template;

	CString key ;
	char *field_data_pointer = NULL;
	int field_data_length = 0;

	//ȡ������ͷģ��,��������ر�����IE.
	//Get Common packet header template, close IE if failed.
	DisplayDebugInfoToWebPage(_T("ȡ������ͷ."));
	key.Empty();
	key = _T(COMMON_PKT_HEADR_TEMPLATE);
	err = m_db.GetDataByKeyFromDB(key, &field_data_pointer, &field_data_length);
	CHECK(0==err)<<"Find common_packet_header_template in DB --> [NO Found].";
	common_packet_header_template = field_data_pointer;
	if(NULL!=field_data_pointer){
		//DisplayDebugInfoToWebPage( A2T(field_data_pointer) );
		free(field_data_pointer);
		field_data_pointer = NULL;
		field_data_length = 0;
	}

	//ȡ������ҵ���ݰ�ģ��,KeyҪ����ҵ�������붯��(��WEB���ݹ���)����ƴ��,��������ر�����IE.
	//Reassemble the up-link packet template through name and action of company passed from web pages.
	//Example:"unicom_xa_query_packet_forward_template"
	DisplayDebugInfoToWebPage(_T("ȡ������ҵ���ݰ�ģ��."));
	key.Empty();
	key = m_InputParameterComName +_T("_")
		+ m_InputParameterActName +_T("_")
		+ _T(COM_PKT_UP_PKT_TEMPLATE);
	err = m_db.GetDataByKeyFromDB(key, &field_data_pointer, &field_data_length);
	if(-1==err)
	{
		LOG(INFO)<< "Find packet_forward_template in DB --> [NO Found].";
		//������ҵ��ʱ���ھ����ݿ����޷���ѯ��ָ����ҵ���ݰ�ģ��
		LOG(ERROR)<<"Find packet_forward_template in DB --> [NO Found]";
		DisplayDebugInfoToWebPage(_T("�ھ����ݿ����޷���ѯ��ָ����ҵ���ݰ�ģ��.���ֶ����������ݿ�"));
		download_db_flag=1;
		AfxMessageBox(ERRO_DOWNLOAD_DB);
		m_sys.ExitProc();
		return;
	}	
	if(NULL!=field_data_pointer)
	{
		forward_packet_template = field_data_pointer;
		free(field_data_pointer);
		field_data_pointer = NULL;
		//field_data_length = 0;
	}


	//�������������ݰ��Ĺ���
	//reassemble the real up-link packet template.
	DisplayDebugInfoToWebPage(_T("����������ݰ�ģ��."));
	sending_packet_template = common_packet_header_template + forward_packet_template;
	LOG(INFO)<<"The whole forward_packet template:"<<T2A(sending_packet_template);

	//�Ѵ�ҳ���ϴ��������ݻ������ݿ�洢�Ĺ̶������ݰ�ģ��Ҫ������ģ��
	//Collect the data from user interface and push them into Input Buffer;
	//Default: the data from keyboard have put into the input Buffer;

	//�����������ݰ����ֶθ���,��������ر�����IE.
	//Count the fields included in the up-link packet.
	int field_num = GetPacketTemplateFieldNumberEx(sending_packet_template);
	if(-1==field_num)
	{
		LOG(ERROR)<<"Sending Packet field number is not valid";
		AfxMessageBox(ERRO_DOWNLOAD_DB);
		m_sys.ExitProc();
		return;
	}

	//���ȴ����ݿ���,��ȡ�̶�������.ͬ����,Ҫ�ȸ�����ҵ���붯����ƴ�ӹؼ���.
	//Collect the data from the local database first according to the key of db
	//(company name and action name).
	DisplayDebugInfoToWebPage(_T("�����ݿ���,��ȡ�̶�������."));
	CString field_id;
	for(int i=0; i<field_num; i++){
		key.Empty();
		field_data_pointer = NULL;
		field_data_length = 0;
		field_id.Empty();

		field_id.Format(_T("%d"),i);
		key = m_InputParameterComName +_T("_")
			+ m_InputParameterActName +_T("_")
			+ _T(COM_PKT_UP_PKT_DATA) +_T("_")
			+ field_id;
		err = m_db.GetDataByKeyFromDB(key, &field_data_pointer, &field_data_length);
		if(0==err){
			m_InputFieldBuffer[i] = field_data_pointer;
		}else{
			LOG(WARNING)<<"The key="<<T2A(key)<<" ,searching in DB ---> [NO FOUND].";
		}
		if(NULL!=field_data_pointer){
			free(field_data_pointer);
			field_data_pointer = NULL;
			field_data_length = 0;
		}
	}

	//�˴�,��ʽ����������ݰ�.
	//Form the packet according to the sending packet template
	//and all the data from users and local DBs.
	DisplayDebugInfoToWebPage(_T("��ʽ����������ݰ�."));

	//��USB KEY ��ȡ���ն˺�ID��������̶�������λ�õ�����(��ϸ��Э���ж��壩
	//�˴�Ҫ�󣬱������USB KEY, ����ᱨ���ء�
	CString terminal_id;
	CString user_id;
	CString err_info;

	terminal_id.Empty();
	user_id.Empty();
	m_usbkey.InitInstance();
	terminal_id = m_usbkey.GetTerminalID(err_info);
	user_id = m_usbkey.GetUserID(err_info);
	m_usbkey.ExitInstance();

	if(TRUE==terminal_id.IsEmpty()|| TRUE==user_id.IsEmpty()){
		LOG(ERROR)<<"While Get USB Key GetTokenName, failed. Sorry.";
		SetErrorInfo4Web(OCX_ERR_USB_KEY_CODE);
		return;		
	}
	else{
		m_InputFieldBuffer[TERMINAL_ID_POSITION] = terminal_id;
		m_InputFieldBuffer[USER_ID_POSITION] = user_id;
	}

	//////////////////////////////////////////////////////////////////

	err = FormSendingPacket(sending_packet_data, sending_packet_template, field_num);
	LOG(INFO)<<"Length of Sending Data="<<sending_packet_data.GetLength();
	LOG(INFO)<<"Content of Sending Data="<<T2A(sending_packet_data);

	//���������ͻ�����ȥ.ע��˴����ǿɼ����ַ���,����Ƕ����Ƶ�����(ָ�����м�
	//���ڿ��ַ�,�˴���������.
	//Pass the packet into the buffer of sending. Note: Here is the string packet not
	//binary packet. You should be careful about it.
	m_tcp_send_buff.Empty();
	m_tcp_send_buff = sending_packet_data;
	//#ifdef DRUMTM_DEVELOP
	//#endif
	//���ҳ���ݲ�Ҫ����,�򷵻�.Ŀ����,ҳ��Ҫ��һ��������װ������ݰ�.
	//If web pages need to process the whole packet contents, just return.
	if(0==m_WebOrder.Compare(_T(ORDER_LOOP_BACK))){
		LOG(INFO)<<"Packet has been ready, and return to Web for next command.";
		m_OutputParameterA.Empty();
		m_OutputParameterA = sending_packet_data;
		return;
	}

	//ҳ��Ҫ����(�ֳɶ���,ǩ�����ݻ���δǩ������.)
	//If web pages need to send un_signature pkt to the proxy server.
	m_WebOrder="SIGN_SEND";
	if(0 ==m_WebOrder.Compare(_T(ORDER_PACK_SEND)))
	{
		LOG(INFO)<<"Unsigned Packet Sending.";
		DisplayDebugInfoToWebPage(_T("δǩ�� ���� ���ͺͽ���."));
		err = m_net.SendRecvPacketToProxyServer(m_tcp_send_buff,m_tcp_recv_buff,res_str_info);
		if(0==err){
			DisplayDebugInfoToWebPage(res_str_info);
		}
		else{
			SetErrorInfo4Web(OCX_ERR_NETWORK_CODE);
			DisplayDebugInfoToWebPage(res_str_info);
			return;
		}
	}
	else if(0==m_WebOrder.Compare(_T(ORDER_SIGN_SEND))){
		if(SIGNATUER_SWITCH){

			//���͸�������ǩ�������ݵ������
			//ԭʼ����
			//Prepare the original data packet
			char* msg = T2A(sending_packet_data);
			int msg_len = sending_packet_data.GetLength();

			//����ǩ������ܵ�Ԫ����
			//����ǩ�������ܴ�������ݵĳ��Ȼ��б仯��
			//���ԣ���ǩ����Ԫ���ڲ������ڴ�(malloc signed_crypted_pkt, uncrypted_unsigned_pkt)��
			//�ⲿ�������ͷ�(free signed_crypted_pkt, uncrypted_unsigned_pkt)���мǣ�
			//The USB signature unit will tranform the original data into the cryptogram space
			//after adding signature and cryptic operation.
			//For the view of a programer, we have to take the close relation with invoking functions.
			//Thus, we should take care the application and release of memory, because the malloc and free operation 
			//might be implemented in the different functions.
			char* signed_crypted_send_pkt= NULL;
			int signed_crypted_send_pkt_len = 0;
			char* signed_crypted_recv_pkt = NULL;
			int signed_crypted_recv_pkt_len = 0;
			char *uncrypted_unsigned_recv_pkt = NULL;
			int uncrypted_unsigned_recv_pkt_len = 0;
			char ret_state=' ';

			//ǩ�������ܴ���(�ɾ����ĳ�����
			//The following function will transform the msg into signed_crypted_pkt).
			//This means that the signed_crypted_pkt and signed_crypted_pkt_len in fact are output parameters.
			//
			//add here!
			//Function_signed_crypted(char *msg, int msg_len, char **signed_crypted_send_pkt, int *signed_crypted_send_pkt_len);
			m_usbkey.InitInstance();
			BOOL ret_usb_key = m_usbkey.SignedEncrypt((unsigned char *)msg, msg_len, (unsigned char **) &signed_crypted_send_pkt,  signed_crypted_send_pkt_len);
			m_usbkey.ExitInstance();
			LOG(INFO)<<"original send pkt_len: "<<msg_len<<"original pkt: "<<msg;


			//���������
			//Check the result of the transform operation with signature and crypt.
			if(NULL!= signed_crypted_send_pkt && 0 < signed_crypted_send_pkt_len){
				LOG(INFO)<<"Add USB_KEY signture and transfer packet into cryptogam space, [Success]";
				DisplayDebugInfoToWebPage(_T("ǩ�����������ݣ�[�ɹ�]."));
				LOG(INFO)<<"signed_crypted_send_pkt_len: "<<signed_crypted_send_pkt_len;
				LOG(INFO)<<"signed_crypted_send_pkt: "<<T2A(m_db.hex2str(signed_crypted_send_pkt,signed_crypted_send_pkt_len));
			}
			else{
				LOG(ERROR)<<"Add USB_KEY signture and transfer packet into cryptogam space, [Failed]";
				DisplayDebugInfoToWebPage(_T("ǩ�����������ݣ�[ʧ��]."));

				if(NULL!=signed_crypted_send_pkt){
					LOG(INFO)<<"Free memory, signed_crypted_send_pkt";
					free(signed_crypted_send_pkt);
					signed_crypted_send_pkt = NULL;
					signed_crypted_send_pkt_len = 0;
				}
				return;
			}

			//����ǩ�����ܺ�����ݰ���Ϊ���������ݣ����ַ����͵����ݲ�ͬ��
			//���ԣ�������д��һ����������պ�����2012-12-04��
			//Since the tranformed data are binary, we re-write the send and receive functions.
			LOG(INFO)<<"Send signed pkt and recv signed pkt";
			CString res_str;
			err = m_net.SendRecvPacketToSignatureServer(signed_crypted_send_pkt, signed_crypted_send_pkt_len, 
				(char **)& signed_crypted_recv_pkt, &signed_crypted_recv_pkt_len, res_str);
			DisplayDebugInfoToWebPage( res_str );

			//�ͷż��ܼ�ǩ�������ݿռ�
			//release the memory space of signed_crypted_send_pkt, after sending operation.
			if(NULL!=signed_crypted_send_pkt){
				LOG(INFO)<<"Free memory, signed_crypted_send_pkt";
				free(signed_crypted_send_pkt);
				signed_crypted_send_pkt  = NULL;
				signed_crypted_send_pkt_len = 0;
			}

			//���ܣ���ǩ�� ���ɾ����ĳ�����)
			//Check the TCP recv buffer, and unsign and uncrypt packet.
			if(NULL != signed_crypted_recv_pkt && signed_crypted_recv_pkt_len >0){

				LOG(INFO)<<"Tranform the singed and crypted data to unsigned and uncryted data";
				LOG(INFO)<<"signed_crypted_recv_pkt_len"<<signed_crypted_recv_pkt_len;
				LOG(INFO)<<"signed_crypted_recv_pkt"<<T2A(m_db.hex2str(signed_crypted_recv_pkt,signed_crypted_recv_pkt_len));

				//add here!
				//err = Function_uncrypted_unsigned(char* signed_crypted_recv_pkt, int signed_crypted_recv_pkt_len, 
				//	char* *uncrypted_unsigned_recv_pkt, int* uncrypted_unsigned_recv_pkt_len);
				m_usbkey.InitInstance();
				BOOL ret_usb_key = m_usbkey.DecryptVerify((unsigned char*)signed_crypted_recv_pkt, signed_crypted_recv_pkt_len,
					ret_state,
					(unsigned char* * )&uncrypted_unsigned_recv_pkt,uncrypted_unsigned_recv_pkt_len,res_str);

				m_usbkey.ExitInstance();
				LOG(INFO)<<"uncrypted_unsigned_recv_pkt_len"<<uncrypted_unsigned_recv_pkt_len;
				LOG(INFO)<<"uncrypted_unsigned_recv_pkt"<<T2A(m_db.hex2str(uncrypted_unsigned_recv_pkt,uncrypted_unsigned_recv_pkt_len));

				//Here, we can release the recv buffer.
				if(NULL!= signed_crypted_recv_pkt){
					LOG(INFO)<<"Free memory, signed_crypted_recv_pkt";
					free(signed_crypted_recv_pkt);
					signed_crypted_recv_pkt = NULL;
					signed_crypted_recv_pkt_len = 0;
				}


			}
			else{
				//������������⣬ֱ�ӷ��ء�
				return;
			}

			//�������,��ǩ���ɹ������ʱ
			//Check the result of unsigned and uncrypted operation.
			if( 0 == err && NULL!=uncrypted_unsigned_recv_pkt && uncrypted_unsigned_recv_pkt_len>0)
			{
				LOG(INFO)<<"Unsigned and Uncrypted the data, [Success]";
				DisplayDebugInfoToWebPage(_T("���ͺͽ���ǩ���ͼ��ܽ��ܣ� �ɹ�."));

				//��������ǩ������ݷ��ڻ����У��ȴ���һ���ķ�����
				//Put the downlink data into the buffer for next split operation.
				m_tcp_recv_buff = A2T(uncrypted_unsigned_recv_pkt);
				LOG(INFO)<<"Recv original packet:"<<T2A(m_tcp_recv_buff);

				if(NULL!=uncrypted_unsigned_recv_pkt)
				{
					LOG(INFO)<<"Free memory, uncrypted_unsigned_recv_pkt";
					free(uncrypted_unsigned_recv_pkt);
					uncrypted_unsigned_recv_pkt = NULL;
					uncrypted_unsigned_recv_pkt_len = 0;
				}
				else{
					LOG(ERROR)<<"Unsigned and Uncrypted the data, [Failed]";
					SetErrorInfo4Web(OCX_ERR_NETWORK_CODE);
					DisplayDebugInfoToWebPage(_T("���ͺͽ���ǩ���ͼ��ܽ��� ����."));

					return;
				}
			}
			else
			{
				LOG(ERROR)<<"Unsigned and Uncrypted the data, [Failed]";
				SetErrorInfo4Web(OCX_ERR_Work_State_CODE);
				DisplayDebugInfoToWebPage(_T("���ͺͽ���ǩ���ͼ��ܽ��� ����."));
				return;
			}
		}

	} 
	else{
		DisplayDebugInfoToWebPage(_T("Require A Order?"));
		return;
	}



	//�����������ݰ�.���������ݰ�����,����ģ��,�ٷ���ʵ���յ��İ�.
	//Parse down-link packets similar to up-link packets. Get the template of packets and analyze the real packets.
	//Example:"unicom_xa_query_backward_packet_template"
	DisplayDebugInfoToWebPage(_T("�����������ݰ�."));
	key.Empty();
	key = m_InputParameterComName +_T("_")
		+ m_InputParameterActName +_T("_")
		+ _T(COM_PKT_DOWN_PKT_TEMPLATE);
	err = m_db.GetDataByKeyFromDB(key, &field_data_pointer, &field_data_length);
	//CHECK(0==err)<<"Find backward_packet_template in DB --> [NO Found].";
	if(-1==err)
	{
		LOG(INFO)<< "Find backward_packet_template in DB --> [NO Found].";
		//������ҵ��ʱ���ھ����ݿ����޷���ѯ��ָ����ҵ���ݰ�ģ��
		LOG(ERROR)<<"Find backward_packet_template in DB --> [NO Found]";
		DisplayDebugInfoToWebPage(_T("�ھ����ݿ����޷���ѯ��ָ����ҵ���ݰ�ģ��.���ֶ����������ݿ�"));
		download_db_flag=1;
		AfxMessageBox(ERRO_DOWNLOAD_DB);
		m_sys.ExitProc();
		return;
	}	
	backward_packet_template = field_data_pointer;
	if(NULL!=field_data_pointer){
		free(field_data_pointer);
		field_data_pointer = NULL;
		field_data_length = 0;
	}

	//���������еĹ�����ͷ�ṹ��һ����.
	//up-link packets and down-link packets have same structures.
	recving_packet_template = common_packet_header_template + backward_packet_template;
	LOG(INFO)<<"The whole BACKward_packet template:"<<T2A(recving_packet_template);

	field_num = GetPacketTemplateFieldNumberEx(recving_packet_template);
	//CHECK(-1!=field_num)<<"Receiving Packet field number is not valid";
	if(-1==field_num)
	{
		LOG(INFO)<< "Receiving Packet field number is not valid.";
		LOG(ERROR)<<"Receiving Packet field number is not valid";
		DisplayDebugInfoToWebPage(_T("��ҵ���ݰ�ģ�治������ɽ���ʧ��.���ֶ����������ݿ�"));
		download_db_flag=1;
		AfxMessageBox(ERRO_DOWNLOAD_DB);
		m_sys.ExitProc();
		return;
	}	

	//�����������ݰ�.
	//analyze the down-link packets.
	err = SplitRecvingPacket(m_tcp_recv_buff,recving_packet_template, field_num);
	if(0==err){
		DisplayDebugInfoToWebPage(_T("�����������ݰ����� �ɹ�."));
	}
	else{
		DisplayDebugInfoToWebPage(_T("�����������ݰ����� ����."));
		return;
	}
	//ˢ����־�ļ�
	//Flush the log messages into its log files.
	google::FlushLogFiles(google::INFO);
	google::FlushLogFiles(google::WARNING);
	google::FlushLogFiles(google::ERROR);

	DisplayDebugInfoToWebPage(_T("���һ�ι���."));

	return;
}



int CChargeClientActiveXCtrl::GetCStringBytesNumber(CString str)
{
	int len = 0; 

	//����������
	//Check the input parameters
	if(str.IsEmpty()){
		LOG(ERROR)<<"Input string is NULL.";
		return -1;
	}	
	len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	return len;
}



int CChargeClientActiveXCtrl::GetDbVersionID(void)
{
	int ret = 0;
	int len = 0;
	char *data = NULL;
	USES_CONVERSION;

	data = (char*)malloc(MAX_STRING_LENGTH);
	CHECK(NULL!=data)<<"Malloc Mem --> [failed].";
	memset(data, 0, MAX_STRING_LENGTH);

	m_db.GetDataByKeyFromDB(_T(DB_VERSION_ID_KEY), &data, &len);

	if(len>=1){
		CString str = _T("ҵ��ģ�����ݿ���� �ɹ�. ���ݰ汾ʱ�����");
		str = str + A2T(data);
		DisplayDebugInfoToWebPage(str);
		ret = 1;
	}else{
		DisplayDebugInfoToWebPage(_T("ҵ��ģ�����ݿ���� ʧ��."));
		ret = -1;
	}

	free(data);
	data = NULL;
	return ret;
}

//
//void CChargeClientActiveXCtrl::AddTemplate(LPCTSTR key, LONG key_len, LPCTSTR data, LONG data_Len)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	// TODO: �ڴ���ӵ��ȴ���������
//	int ret = 0;
//	CString input_key(key);
//	CString input_data(data);
//	input_key.Trim();
//	input_data.Trim();
//
//	// Add the DB timestamp
//	// ����ʱ���
//	CTime time = CTime::GetCurrentTime();
//	CString db_time_stamp = time.Format("%Y.%m.%d.%H.%M.%S");
//
//	ret = m_db.CheckKeyDataStringValid(input_key,input_data);
//	if(-1==ret){
//		DisplayBackToWebPage(_T("���ֲ��Ϸ��ַ�, ģ�����ʧ��."),DISPLAY_WEB_POSITION);
//	}else{
//		m_db.InsertRecordIntoDatabase(_T(DB_TIME_STAMP_KEY),db_time_stamp);
//		m_db.InsertRecordIntoDatabase(input_key, input_data);
//	}
//	CString res_str;
//	res_str.Empty();
//	m_db.ScanTemplateDB(CHARGE_CLIENT_DB_FILE_NAME, res_str);
//	return ;
//
//}
//
//void CChargeClientActiveXCtrl::RemoveTemplate(LPCTSTR key, LONG key_len)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	// TODO: �ڴ���ӵ��ȴ���������
//
//	CString s = key;
//	s.Trim();
//	if(!s.IsEmpty()){
//
//		//����ʱ���
//		// Add the DB timestamp
//		CTime time = CTime::GetCurrentTime();
//		CString db_time_stamp = time.Format("%Y.%m.%d.%H.%M.%S");
//		m_db.InsertRecordIntoDatabase(_T(DB_TIME_STAMP_KEY),db_time_stamp);
//
//		//ɾ����¼�����ݿ���
//		//Delete the records from the DB
//		m_db.RemoveRecordFromDatabase(s);
//	}
//	s.Empty();
//	m_db.ScanTemplateDB(CHARGE_CLIENT_DB_FILE_NAME,s);
//	DisplayDebugInfoToWebPage(s);
//	return ;
//
//}
//
////��ʾ���ݿ�����������
////Display the all data and key in the DB.
//int CChargeClientActiveXCtrl::ScanTemplateDB(char * database)
//{
//	 CString res_str;
//	 res_str.Empty();
//
//	 m_db.ScanTemplateDB(database, res_str);
//	 m_OutputParameterA.Empty();
//	 m_OutputParameterA = res_str;
//	 FireDisplayAtPosition(911L);
//	 return (0);
//}


int CChargeClientActiveXCtrl::GetPacketTemplateFieldNumberEx(CString pkt_template)
{
	int err = 0;
	int ret = 0;
	int field_num = 0;

	if(pkt_template.IsEmpty()){
		LOG(ERROR)<<"The string is empty.";
		return -1;
	}

	USES_CONVERSION;
	char *str = T2A(pkt_template);

	int i = 0;
	int j = 0;
	char *p = NULL;

	char c = '\000';
	int len = 1;
	p = str;

	while(*p!=NULL && *(p+1)!=NULL){
		c = *p;
		p = p+1;
		if(c == *p){
			len ++;
		}
		else{
			if(VAR_LEN_FIELD_LETTER==c){
				len = -1;
			}
			//Output j and len
			j = j+1;
			len = 1;
		}
	}

	field_num = j + 1;
	return field_num;
}

int CChargeClientActiveXCtrl::GetPacketTemplateFieldLength(CString pkt_template, int field_num, int *len_array)
{
	int err = 0;
	int ret = 0;
	int field_num_inner = 0;

	if(pkt_template.IsEmpty()){
		LOG(ERROR)<<"The string is empty.";
		return -1;
	}

	field_num_inner = GetPacketTemplateFieldNumberEx(pkt_template);
	if(field_num_inner!=field_num){
		LOG(ERROR)<<"The string Field Number do not match.";
		return -1;
	}

	USES_CONVERSION;
	char *str = T2A(pkt_template);

	int i = 0;
	int j = 0;
	char *p = NULL;

	char c = '\000';
	int len = 1;
	p = str;

	while(*p!=NULL && *(p+1)!=NULL){
		c = *p;
		p = p+1;
		if(c == *p){
			len ++;
		}
		else{
			if(VAR_LEN_FIELD_LETTER==c){
				len = -1;
			}
			//Output j and len
			*(len_array+j) = len;

			j = j+1;
			len = 1;
		}
	}
	//�����пɱ䳤���ֶ�,�ø�1��ʾ.
	if(VAR_LEN_FIELD_LETTER==*p){
		len = -1;
	}
	*(len_array+j) = len;
	return 0;
}

//���ӡ���ĺ���
//Printer Functions
void CChargeClientActiveXCtrl::PrepareInvoiceItem(LONG x, LONG y, LPCTSTR data, LONG data_len)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	CString item_str(data);
	item_str.Trim();

	//Remove the NULL chars from the item
	int key_length = 0;
	key_length = GetCStringBytesNumber(item_str)+STRING_TAIL_NULL;

	char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);
	CHECK(NULL!=key_buffer)<<"Allocate key memory --> [Not Enough Mem].";
	memset(key_buffer,0,key_length);
	m_dot_printer.RemoveNullChar(key_buffer,item_str);
	m_dot_printer.PushInvoiceItem(x,y,key_length,key_buffer);

	free(key_buffer);
	key_buffer = NULL;
}

void CChargeClientActiveXCtrl::PrintInvoice(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	m_dot_printer.PrintInvoiceItem();
}


//���ݰ����������
//Reassemble and Parse the packet.
int CChargeClientActiveXCtrl::FormSendingPacket(CString &pkt_data, CString pkt_template,int field_num)
{
	int ret = 0;
	int err = 0;
	USES_CONVERSION;

	LOG_IF(ERROR, pkt_template.IsEmpty())<<"Packet template is empty.";
	LOG_IF(ERROR, field_num<=0)<<"Field number is less than zero.";

	//��ÿ���ֶεĳ���
	//Analyze each field number and length
	int* each_field_len = NULL;
	each_field_len = (int *)malloc(sizeof(int)*(field_num+1));
	CHECK(NULL!=each_field_len)<<"Allocate memory --> [Not Enough Mem].";
	memset(each_field_len, 0 , sizeof(int)*(field_num+1));

	err = GetPacketTemplateFieldLength(pkt_template, field_num, each_field_len );
	CHECK(err==0)<<"Analyze the length of each field in the packet";

	LOG(INFO)<<"Analyze each field number and length";

	//-1��ʾ�ǿɱ䳤�ȵġ�
	for(int i = 0; i<field_num; i++){
		if(-1 == *(each_field_len+i)){
			LOG(INFO)<<"Field: "<<i<<","<<"\tLen: "<<"varible length";
		}
		else{
			LOG(INFO)<<"Field: "<<i<<","<<"\tLen: "<<*(each_field_len+i);
		}
	}

	//Generate Packet for sending
	wchar_t format_string[MAX_STRING_LENGTH];
	wchar_t value_string[MAX_STRING_LENGTH];

	//���ո�ʽ���γ�ȫ���ֶε�ֵ
	for(int i=0;i<field_num;i++){
		memset(format_string, 0, MAX_STRING_LENGTH);
		memset(value_string, 0, MAX_STRING_LENGTH);
		if(0<*(each_field_len+i)){
			swprintf_s(format_string, MAX_STRING_LENGTH, L"%%-%d.%ds", *(each_field_len+i),*(each_field_len+i));
		}else{
			swprintf_s(format_string, MAX_STRING_LENGTH, L"%%s", *(each_field_len+i),*(each_field_len+i));
		}
		//if(m_InputFieldBuffer[i].IsEmpty()){
		//	swprintf_s(value_string, MAX_STRING_LENGTH, format_string," ");
		//}else{
		swprintf_s(value_string, MAX_STRING_LENGTH, format_string, m_InputFieldBuffer[i] );

		//}
		m_FieldValueBuffer[i]= value_string;
		LOG(INFO)<<"Field ["<<i<<"] Value: |"<<T2A(value_string)<<"|";
	}

	//ƴ��ȫ���ֶ�
	pkt_data.Empty();
	for(int i=0;i<field_num;i++){
		pkt_data  = pkt_data + m_FieldValueBuffer[i];

	}
	LOG(INFO)<<"Form Packet ---> [OK].";

	//�ͷ��ڴ�
	free(each_field_len);
	each_field_len = NULL;

	return 0;
}

//����յ������ݰ���
int CChargeClientActiveXCtrl::SplitRecvingPacket(CString pkt_data, CString pkt_template, int field_num)
{
	int ret = 0;
	int err = 0;
	int i = 0;
	int j = 0;

	//���û���յ����ݰ�.
	if(pkt_data.IsEmpty()){
		return -1;
	}
	USES_CONVERSION;

	//��ÿ���ֶεĳ���
	//Analyze each field number and length
	int* each_field_len = NULL;
	each_field_len = (int *)malloc(sizeof(int)*(field_num+1));
	CHECK(NULL!=each_field_len)<<"Allocate memory --> [Not Enough Mem].";
	memset(each_field_len, 0 , sizeof(int)*(field_num+1));

	err = GetPacketTemplateFieldLength(pkt_template, field_num,each_field_len );
	CHECK(err==0)<<"Analyze the length of each field in the packet";

	LOG(INFO)<<"Analyze each field number and length";
	for(int i = 0; i<field_num; i++){
		if(-1 == *(each_field_len+i)){
			LOG(INFO)<<"Field: "<<i<<","<<"\tLen: "<<"varible length";
		}
		else{
			LOG(INFO)<<"Field: "<<i<<","<<"\tLen: "<<*(each_field_len+i);
		}
	}


	//Split Packet 
	char *p = T2A(pkt_data);
	char *q = p;
	int len = (int)strlen(p);
	int data_len = len;
	int template_len = pkt_template.GetLength();

	//���ģ�峤�ȴ��������յ������ݳ���,��Ϊ������ͷ�ǿ����յ���.
	//�����������������PROXY�رգ������������ĵļ�������û�з��͵���ҵ��������
	err = pkt_template.Find('*');
	if(-1==err){
		if(template_len >data_len){
			//Only analyze the common header that is guaranteed.
			//�����ֻ������ͷ�����յ���ͷ����Ķ���ŵ����һ��������
			field_num = COMMON_PACKET_HEADER_FIELD_NUM;
		}
	}
	else{
		if(template_len >(data_len+1)){
			//Only analyze the common header that is guaranteed.
			//�����ֻ������ͷ�����յ���ͷ����Ķ���ŵ����һ��������
			field_num = COMMON_PACKET_HEADER_FIELD_NUM;
		}

	}


	char value_string[MAX_STRING_LENGTH];
	int last_idx = 0;
	for(j=0;j<field_num;j++){
		if(-1==*(each_field_len+j)){
			//����Ǳ䳤���ֶ�.
			memset(value_string, 0, MAX_STRING_LENGTH);
			memcpy(value_string, q, data_len-template_len+1);
			m_OutputFieldBuffer[j].Empty();
			m_OutputFieldBuffer[j]= value_string;
			q = q + (data_len-template_len+1);
			len = len - (data_len-template_len+1);
		}
		else if(len>=*(each_field_len+j)){
			memset(value_string, 0, MAX_STRING_LENGTH);
			memcpy(value_string, q, *(each_field_len+j));
			m_OutputFieldBuffer[j].Empty();
			m_OutputFieldBuffer[j]= value_string;
			q = q + *(each_field_len+j);
			len = len - *(each_field_len+j);
		}else{
			//because of the the length of template more than the length of packet.
			LOG(ERROR)<<"The packet template does NOT match the recv packet.";
			LOG(ERROR)<<"the length of template more than the length of packet";
			LOG(ERROR)<<"Recv Pkt: "<<p;
			LOG(ERROR)<<"Template: "<<T2A(pkt_template);

			memset(value_string, 0, MAX_STRING_LENGTH);
			memcpy(value_string, q, len);
			m_OutputFieldBuffer[j].Empty();
			m_OutputFieldBuffer[j]= value_string;
			len = 0;
			break;
		}
		LOG(INFO)<<"Field ["<<j<<"] Value: |"<<value_string<<"|";
	}
	//���������,ģ�峤��С�����ݰ�����
	//because of the the length of template less than the length of packet.
	if(len>0){
		memset(value_string, 0, MAX_STRING_LENGTH);
		memcpy(value_string, q, len);
		m_OutputFieldBuffer[j].Empty();
		m_OutputFieldBuffer[j]= value_string;
		LOG(INFO)<<"Field ["<<j<<"] Value: |"<<value_string<<"|";
		LOG(ERROR)<<"The packet template does NOT match the recv packet.";
		LOG(ERROR)<<"The length of the template less than the length of packet, "<<len;
		LOG(ERROR)<<"Incoming Pkt: "<<p;
		LOG(ERROR)<<"Template is : "<<T2A(pkt_template);

	}
	// SPLIT_PKT_END:
	LOG(INFO)<<"Split Packet ---> [OK].";
	free(each_field_len );
	each_field_len = NULL;
	return 0;
}


//��Webҳ��Ľ���
BSTR CChargeClientActiveXCtrl::GetOutputFields(SHORT field_index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	// �ṩ��Webҳ��Ŀ�����Ա����ֱ���ø�ֵ�İ취��ActiveX��ȡ��ֵ����
	// Provide the web developers, OutFieldsBuffer[index] , array.
	if (field_index > MAX_PACKET_FIELD_NUM){
		LOG(ERROR)<<"Output Field Index > Max Packet field Number.  --> [Invalid]";
		strResult = "Output field_index must exceed the maximum value 100. DrumTm Products.";
	}
	else{
		strResult = m_OutputFieldBuffer[field_index];
	}


	return strResult.AllocSysString();
}
void CChargeClientActiveXCtrl::SetOutputFields(SHORT field_index, LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}
void CChargeClientActiveXCtrl::ClearOutputFields(void)
{
	for(int i = 0; i < MAX_PACKET_FIELD_NUM; i++){
		m_OutputFieldBuffer[i].Empty();
	}
}

void CChargeClientActiveXCtrl::OnWebOrderChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

BSTR CChargeClientActiveXCtrl::GetInputFields(SHORT field_index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	if (field_index > MAX_PACKET_FIELD_NUM){
		LOG(WARNING)<<"Input Field Index > Max Packet field Number.  --> [Invalid]";
		strResult = "Input field_index must exceed the maximum value. DrumTm Products.";

	}
	else{
		strResult = m_InputFieldBuffer[field_index];
	}

	// TODO: �ڴ���ӵ��ȴ���������
	return strResult.AllocSysString();
}

void CChargeClientActiveXCtrl::SetInputFields(SHORT field_index, LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������
	if (field_index > MAX_PACKET_FIELD_NUM){
		LOG(WARNING)<<"Input Field Index > Max Packet field Number.  --> [Invalid]";
	}
	else{
		m_InputFieldBuffer[field_index] = newVal;
	}

	SetModifiedFlag();
}

void CChargeClientActiveXCtrl::DisplayBackToWebPage(CString str, long positon)
{
	m_OutputParameterA.Empty();
	m_OutputParameterA = _T("<li>")+str+_T("</li>");
	FireDisplayAtPosition(positon);
}


//ά�����
void CChargeClientActiveXCtrl::TestingFunction(LONG f)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ������
	int ret = 0;
	int err = 0;
	CString info;
	CString cur_url;
	info.Empty();
	cur_url.Empty();
	//CString sss=GetTerminalID();

	CString debug_flag;
	debug_flag = m_InputParameterDebugFlag;
	m_InputParameterDebugFlag=_T("DEBUG_ON");
	CString token_name;

	SysMainTestOptions test_opt = (SysMainTestOptions)f;

	switch(test_opt){
		case SysMain_ClearLog:
			m_sys.ClearLogFiles();
			DisplayDebugInfoToWebPage(_T("LOG�ļ�������ɹ�."));
			DisplayDebugInfoToWebPage(_T(__TIMESTAMP__));
			break;
		case SysMain_AccessLocalDb:
			GetDbVersionID();
			break;
		case SysMain_Printer:
			ret = m_dot_printer.TestingPrinter();
			if(-1==ret){
				DisplayDebugInfoToWebPage(_T("��ӡ����, ʧ��."));
			}
			else{
				DisplayDebugInfoToWebPage(_T("��ӡ�����Ѿ� �ɹ� ����."));
			}
			break;

		case SysMain_DigiSign:
			//ret = m_signature.CreateKeyPair();
			m_usbkey.InitInstance();
			if(false==m_usbkey.SelfCheck(info)){
				//DisplayDebugInfoToWebPage(_T("����ǩ��USB״̬, ʧ��."));
				DisplayDebugInfoToWebPage(info);
				SetErrorInfo4Web(OCX_ERR_USB_KEY_CODE);
			}
			else{
				//DisplayDebugInfoToWebPage(_T("USB ID Ϊ��")+token_name);
				DisplayDebugInfoToWebPage(_T("����ǩ��USB KEY ����, ����, �ɹ�."));
			}
			m_usbkey.ExitInstance();
			break;

		case SysMain_Tcp:
			m_net.CheckRemoteNetWorking(info);
			DisplayDebugInfoToWebPage(info);
			break;
		case SysMain_UpdateLocalDb:
			err = m_net.GetCurrentUrl(cur_url, this);
			ret = m_sys.UpdateDbFile(cur_url,1);
			if(-1==ret){
				DisplayDebugInfoToWebPage(_T("ҵ�����ݿ�����, ʧ�ܣ���������һ��."));
			}
			else{
				DisplayDebugInfoToWebPage(_T("ҵ�����ݿ�����, �ɹ�."));
			}
			GetDbVersionID();
			break;
		case SysMain_RestoreFactory:
			m_sys.ResetSystem();
			break;
		default:
			break;
	}

	m_InputParameterDebugFlag = debug_flag;
	return;
}


//�����ĺ���
void CChargeClientActiveXCtrl::OnActivexErrorCodeChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

void CChargeClientActiveXCtrl::OnActivexErrorInfoChanged(void)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}


void CChargeClientActiveXCtrl::SetErrorInfo4Web(long err_code)
{
	switch(err_code){
		case OCX_ERR_NETWORK_CODE:
			m_ActivexErrorCode = OCX_ERR_NETWORK_CODE;
			m_ActivexErrorInfo = _T( OCX_ERR_NETWORK_INFO );
			DisplayDebugInfoToWebPage(_T( OCX_ERR_NETWORK_INFO ));
			break;
		case OCX_ERR_USB_KEY_CODE:
			m_ActivexErrorCode = OCX_ERR_USB_KEY_CODE;
			m_ActivexErrorInfo = _T( OCX_ERR_USB_KEY_INFO );
			DisplayDebugInfoToWebPage(_T( OCX_ERR_USB_KEY_INFO ));
			break;
		default:
			break;
	}
	return;
}


void CChargeClientActiveXCtrl::DisplayDebugInfoToWebPage(CString  info)
{
	int ret = 0;
	int err = 0;
	CString str = info;
	if(0==m_InputParameterDebugFlag.Compare(_T( DEBUG_INFO_ON ))){
		DisplayBackToWebPage(str, DISPLAY_WEB_POSITION);
	}
	return;

}

BSTR CChargeClientActiveXCtrl::GetTerminalID(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	CString err_info;

	strResult.Empty();
	m_usbkey.InitInstance();
	strResult = m_usbkey.GetTerminalID(err_info);
	//strResult = m_usbkey.GetUserID(err_info);
	m_usbkey.ExitInstance();

	if(TRUE==strResult.IsEmpty()){
		LOG(ERROR)<<"While Get USB Key GetTokenName, failed. Sorry.";
		SetErrorInfo4Web(OCX_ERR_USB_KEY_CODE);		
	}
	else{
		m_InputFieldBuffer[TERMINAL_ID_POSITION] = strResult;
		//m_InputFieldBuffer[USER_ID_POSITION] = strResult;
	}
	return strResult.AllocSysString();
}

BSTR CChargeClientActiveXCtrl::GetUserID(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	CString err_info;
	strResult.Empty();
	m_usbkey.InitInstance();
	//strResult = m_usbkey.GetTerminalID(err_info);
	strResult = m_usbkey.GetUserID(err_info);
	m_usbkey.ExitInstance();

	if(TRUE==strResult.IsEmpty()){
		LOG(ERROR)<<"While Get USB Key GetTokenName, failed. Sorry.";
		SetErrorInfo4Web(OCX_ERR_USB_KEY_CODE);			
	}
	else{
		//m_InputFieldBuffer[TERMINAL_ID_POSITION] = strResult;
		m_InputFieldBuffer[USER_ID_POSITION] = strResult;
	}
	return strResult.AllocSysString();
}
