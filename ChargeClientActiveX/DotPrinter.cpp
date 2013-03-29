#include "StdAfx.h"
#include "DotPrinter.h"
#include <stdio.h>

#undef ERROR
#include <glog/logging.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ESC 0x1B //Init Char
#define FF 0x0C

#define PrinterPort "LPT1"
//1毫米=0.04英寸
#define PRINTER_LEN_UNIT 0.04 

#define PAGE_LENGTH 15 //inches
#define MAX_RETRY_TIMES 2



CDotPrinter::CDotPrinter(void)
{
	m_fp = NULL;
	m_invoice.clear();
}

CDotPrinter::~CDotPrinter(void)
{
	if(NULL!=m_fp){
		fclose(m_fp);
		m_fp = NULL;
	}
	m_invoice.clear();

}

int CDotPrinter::ReSetPrinter(void)
{
	fprintf(m_fp,"%c%c", ESC,'@');
	fflush(m_fp);
	return 0;

}
int CDotPrinter::InitPrinter(void)
{
	errno_t err = 0;
	int ret = 0;

	if(NULL==m_fp){
		err = fopen_s(&m_fp, PrinterPort,"wb");
		LOG_IF(INFO,err==0)<<"Printer Ready.";
		LOG_IF(ERROR,err!=0)<<"Printer Open --> failed.";
	}
	else{
		fclose(m_fp);
		err = fopen_s(&m_fp, PrinterPort,"wb");
		LOG(INFO)<<"Printer Close and reOpen and Ready.";
	}

	if(NULL==m_fp){
		ret = -1;
	}
	fflush(m_fp);

	return ret;
}

void CDotPrinter::PrintOut(unsigned char* lpcbData,int Length)
{     
	if (! Length) {
		fprintf(m_fp,"%s",lpcbData);
	}
	else do {
		fprintf(m_fp,"%c",*lpcbData++);
		Length--;
	} while(Length);
	fflush(m_fp);

}
int CDotPrinter::SetPageLength(int len_inches)
{
	CHECK(m_fp!=NULL)<<"Printer pointer == NULL";
	CHECK(len_inches<22)<<"Maximum_page_length should be < 22 inches.";
	fprintf_s(m_fp,"%c%c%c%c", ESC,'C',0x0,len_inches);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::SetPageFormatLeftMargin(int len_char_num)
{
	CHECK(m_fp!=NULL)<<"Printer pointer == NULL";
	fprintf_s(m_fp, "%c%c", ESC, 'P'); //10 CPI font
	fprintf_s(m_fp,"%c%c%c", ESC,'1',len_char_num);// n/10 inches
	fflush(m_fp);
	return 0;
}

int CDotPrinter::EnableChineseCharSet(void)
{
	CHECK(m_fp!=NULL)<<"Printer pointer == NULL";
	fprintf_s(m_fp, "%c%c", 0x1c,0x26);
	fflush(m_fp);
	return 0;
}



int CDotPrinter::DisableChineseCharSet(void)
{
	CHECK(m_fp!=NULL)<<"Printer pointer == NULL";
	fprintf_s(m_fp, "%c%c", 0x1c,0x2E);
	fflush(m_fp);

	return 0;
}

int CDotPrinter::EnableSongFont(void)
{
	fprintf_s(m_fp, "%c%c%c", 0x1c,0x6B, 0x0);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::EnableHeiFont(void)
{
	fprintf_s(m_fp, "%c%c%c", 0x1c,0x6B, 0x1);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::CarriageReturn(void)
{
	//Moves the print position to the left margin position
	fprintf_s(m_fp,"%c", 0x0D);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::ReturnHomePosition(void)
{
	fprintf_s(m_fp,"%c%c", ESC,0x3C);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::LineFeed(void)
{
	//Advances the vertical print position
	fprintf_s(m_fp,"%c", 0x0A);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::FormFeed(void)
{
	//Prints all data in the buffer
	//Ejects single-sheet paper
	fprintf_s(m_fp,"%c", 0x0C);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::MoveAbsoluteHorizontalPosition(double x_inches)
{
	int nl = 0, nh=0;
	//Moves the horizontal print position to the position specified by the following formula
	//(horizontal position) = ((nH *256) + nL) *(1/60inches) + (left margin)
	int x = 0;
	x =(int) (x_inches*60);

	nl= x%256;
	nh = (int)(x/256);

	fprintf_s(m_fp, "%c%c%c%c", ESC, 0x24, nl, nh);
	fflush(m_fp);
	return 0;
}

int CDotPrinter::AdvanceRelativeVerticalPosition(double y_inches)
{
	int max_step_once = 250;

	int step = (int)(y_inches*180);

	int m = step/max_step_once;
	int n = step%max_step_once;
	int i = 0;
	//每次走纸不能大于250，所以分多次走。
	for(i=0; i<m; i++){
		fprintf_s(m_fp, "%c%c%c", ESC, 'J', max_step_once );
		fflush(m_fp);
	}
	if(0!=n){
		fprintf_s(m_fp, "%c%c%c", ESC, 'J', n );
	}
	fflush(m_fp);
	return 0;
}

int CDotPrinter::BackRelativeVerticalPosition(double y_inches)
{
	int max_step_once = 250;

	int step = (int)(y_inches*180);

	int m = step/max_step_once;
	int n = step%max_step_once;
	int i = 0;

	//每次走纸不能大于250，所以分多次走。
	for(i=0; i<m; i++){
		fprintf_s(m_fp, "%c%c%c", ESC, 'j', max_step_once );
		fflush(m_fp);
	}
	if(0!=n){
		fprintf_s(m_fp, "%c%c%c", ESC, 'j', n );
	}
	fflush(m_fp);
	return 0;}

int  CDotPrinter:: PrintBarcode(char * data, int size)
{
	int len = size + 6;
	int n_h = (int)(len/256);
	int n_l = len%256;

	fprintf_s(m_fp, "%c%c%c", ESC, '(', 'B');
	fprintf_s(m_fp, "%c%c", (char)n_l, (char)n_h);
	fprintf_s(m_fp, "%c", 0x07);
	fprintf_s(m_fp, "%c", 0x02);
	fprintf_s(m_fp, "%c", 0x00);
	fprintf_s(m_fp, "%c%c", 0x00,0x00);
	fprintf_s(m_fp, "%c", 0x01);

	/*	fprintf_s(m_fp, "%c%c%c%c%c%c%c%c%c%c%c", 
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
	0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
	0x32, 0x33);
	*/	
	PrintOut((unsigned char*)data, size);
	fflush(m_fp);
	return 0;
}
/* 
x coordinate 0.1 inches
y coordinate 0.1 inches
*/
int CDotPrinter::PushInvoiceItem(int x, int y, int data_len, char* data)
{
	LOG_IF(ERROR,NULL==data)<<"pointer is NULL";
	LOG_IF(ERROR,data_len<=0)<<"data_len <= 0";

	if(data_len<=0||NULL==data){
		return -1;
	}
	if(data_len>MAX_LEN){
		data_len = MAX_LEN -1;
	}

	//Push a invoice item into MAP_INVOICE
	T_InvoiceItem *item = NULL;

	char keytmp[MAX_LEN];
	char datatmp[MAX_LEN];

	item = (T_InvoiceItem *)malloc(sizeof(T_InvoiceItem));
	CHECK(item!=NULL)<<"Malloc memory ---> failed.";

	memset(keytmp,0, MAX_LEN);
	sprintf_s(keytmp,MAX_LEN,"%05d%05d",y,x);
	string key(keytmp);
	item->x = x;
	item->y = y;

	item->data_len = data_len;

	memset(datatmp,0,MAX_LEN);
	item->data = NULL;
	item->data = (char*)malloc(item->data_len+1);
	CHECK(NULL!=item->data)<<"Malloc memory ---> failed.";
	memset(item->data, 0, data_len+1 );
	memcpy(item->data, data, item->data_len);

	//Push into the MAP_INVOICE
	m_invoice[key]=item;

	return 0;
}

int CDotPrinter::ClearInvoiceItem(void)
{
	Map_InvoiceItem::iterator pos;
	for(pos=m_invoice.begin();pos!=m_invoice.end();++pos){

		/*
		cout<<"key: "<<pos->first <<"\t"
		<<"x: "<<pos->second->x <<"\t"
		<<"y: "<<pos->second->y <<"\t"
		<<"data: " <<pos->second->data <<endl;
		*/
		free(pos->second->data);
		pos->second->data = NULL;
		free(pos->second);
		pos->second = NULL;
	}
	m_invoice.clear();
	return 0;
}

int CDotPrinter::DisplayInvoiceItem(void)
{
	Map_InvoiceItem::iterator pos;
	for(pos=m_invoice.begin();pos!=m_invoice.end();++pos){		
		LOG(INFO)<<"key: "<<pos->first <<"\t"
			<<"x: "<<pos->second->x <<"\t"
			<<"y: "<<pos->second->y <<"\t"
			<<"data len: "<<pos->second->data_len <<"\t"
			<<"data: " <<pos->second->data <<endl;
	}
	return 0;
}

int CDotPrinter::PrintInvoiceItem(void)
{
	double x = 0;
	double y = 0;
	int pre_item_y = 0;
	int cur_item_y = 0;
	int ret = 0;
	int retry_times = 0;

	/* temperately disable the testing of printer. 	
	for(retry_times = 0; retry_times< MAX_RETRY_TIMES; retry_times++){
		//检测打印机的状态
		ret = VerifyPrinter();
		if(-1==ret){
			LOG(ERROR)<<"Error on the port LPT1.";
		}
		else{
			LOG(INFO)<<"Ready on the port LPT1.";
			break;
		}
	}
	if(MAX_RETRY_TIMES==retry_times){
		AfxMessageBox(_T("打印机发生严重错误, 打印任务被取消. \
			\n请关闭计算机电源和打印机电源. \
			\n重新连接打印机的电源线和数据线后,重新开机试一下."),MB_OK, 0);
		LOG(ERROR)<<"Serious errors on the port LPT1.";
		return -1;
	}
	*/

	//检测打印机存在
	ret = InitPrinter();
	if(-1==ret){
		LOG(ERROR)<<"Cannot find the printer on the port LPT1.";
		return -1;
	}
	else{
		LOG(INFO)<<"Find the printer on the port LPT1.";
	}

	//打印机纸张初始化,打印头回左边界
	ret = ReSetPrinter();
	ret = SetPageLength(PAGE_LENGTH); // 5 inches.
	ret = SetPageFormatLeftMargin(1); // 1*0.1inches
	ret = EnableChineseCharSet(); 
	//ret = LineFeed();
	//ret = CarriageReturn();
	ret = LineFeed();
	ret = CarriageReturn();
	ret = EnableSongFont();

	Map_InvoiceItem::iterator pos;
	pos=m_invoice.begin();
	pre_item_y = pos->second->y;
	y = PRINTER_LEN_UNIT*pre_item_y;
	AdvanceRelativeVerticalPosition(y);
	for(pos=m_invoice.begin();pos!=m_invoice.end();++pos){		

		/*
		cout<<"key: "<<pos->first <<"\t"
		<<"x: "<<pos->second->x <<"\t"
		<<"y: "<<pos->second->y <<"\t"
		<<"data: " <<pos->second->data <<endl;
		*/
		cur_item_y = pos->second->y;
		if((cur_item_y-pre_item_y)>0){
			y = PRINTER_LEN_UNIT* (cur_item_y-pre_item_y);
			AdvanceRelativeVerticalPosition(y);
		}
		x = PRINTER_LEN_UNIT*pos->second->x;
		ret = MoveAbsoluteHorizontalPosition(x);

		int bar_code_len = 8;
		if(pos->second->data_len == (bar_code_len + 1) && 0==strncmp("BAR_CODE", (const char *)pos->second->data, bar_code_len)){
			CTime time = CTime::GetCurrentTime();
			CString time_stamp = time.Format("%m%d%H%M");
			USES_CONVERSION;
			PrintBarcode(T2A(time_stamp),time_stamp.GetLength());
		}else
		{
			PrintOut((unsigned char *)pos->second->data,pos->second->data_len);
		}
		pre_item_y = cur_item_y;
	}

	//Print the timestamp bar codes
	ret = LineFeed();
	ret = CarriageReturn();

	FormFeed();
	DisableChineseCharSet();

	ReSetPrinter();
	ClosePrinter();
	ClearInvoiceItem();
	return 0;
}

int CDotPrinter::ClosePrinter(void)
{
	if(NULL!=m_fp){
		fclose(m_fp);
		m_fp = NULL;
	}
	return 0;
}
int CDotPrinter::TestingPrinter(void)
{
	int err = 0;
	int ret = 0;
	int i = 0;
	int j = 0;
	err = ClearInvoiceItem();

	//length unit 0.1iches
	for(i=0; i<40; i=i+5){
		for(j=0; j<10; j=j+5){
			PushInvoiceItem(i,j,1,"+");
		}
	}
	PushInvoiceItem(0, 20, 35, "打印机正常工作, +号 列间距 约5毫米.");
	PushInvoiceItem(0, 30, 44, "第一行+号距纸上边约12毫米. 距纸左边约10毫米.");
	PushInvoiceItem(0, 40, 52, "如不是,请按照您的打印机说明书, 调整默认的初始进纸量.");

	err = PrintInvoiceItem();
	if(-1==err){
		ret = -1;
	}
	else{
		ret = 0;
	}
	return ret;
}



int CDotPrinter::VerifyPrinter(void)
{

	int ret = 0;
	HANDLE m_h_port = NULL;
	CString m_port_str;

	m_port_str = _T(PrinterPort);

	m_h_port = CreateFile(m_port_str, GENERIC_READ | GENERIC_WRITE,
		0, NULL, 
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_h_port == INVALID_HANDLE_VALUE)
	{
		ret = -1;
		AfxMessageBox(_T("并口（打印机LPT1） 端口打开错误!\n请打开打印机电源或放好纸张后, 按[OK]或[确定]."),MB_OK, 0);
		goto VerifyPrinter_END;
	}

	// get any early notifications
	SetCommMask(m_h_port, EV_RXCHAR ) ;

	// setup device buffers //设置端口缓冲
	SetupComm(m_h_port, 4096, 4096 ) ;

	COMMTIMEOUTS TimeOuts;
	memset(&TimeOuts,0,sizeof(TimeOuts));    
	TimeOuts.ReadIntervalTimeout = MAXDWORD; 
	TimeOuts.ReadTotalTimeoutMultiplier = 0; 
	TimeOuts.ReadTotalTimeoutConstant = 0; 
	TimeOuts.WriteTotalTimeoutMultiplier = 50;
	TimeOuts.WriteTotalTimeoutConstant = 2000; //必须2000ms以上!否则会超时错误

	//设置超时控制
	if (!SetCommTimeouts(m_h_port, &TimeOuts))
	{ 
		ret = -1;
		AfxMessageBox(_T("端口超时设置错误!\n或有其它设备驱动占用了该端口!"),MB_OK, 0); 
		
		goto VerifyPrinter_END;
	}

	PurgeComm(m_h_port, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);		//清空输出缓冲

	//如果打印机有错，例如脱机；立即返回。
	DWORD iBytesLength;
	char chInitCode[] = "\x1B\x40\x07";
	BOOL bSuccess;
	bSuccess = WriteFile(m_h_port, chInitCode, (DWORD)4L, &iBytesLength, NULL);
	if (!bSuccess || !iBytesLength)
	{
		AfxMessageBox(_T("打印机缺纸，脱机或其它错误发生!\n请打开打印机电源或放好纸张后, 按[OK]或[确定]."),MB_OK, 0);
		ret = -1;
	}
	PurgeComm(m_h_port, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);		//清空输出缓冲
VerifyPrinter_END:
	CloseHandle(m_h_port);
	m_h_port = NULL;
	return ret;
}

//用来去除空字符，也可直接用T2A这宏代替。
//Remove the NULL chars from a string. The function can alos be replaced by the Macro T2A
int CDotPrinter::RemoveNullChar(char *t, CString s)
{
	int ret = 0; 

	//检查输入参数
	//Check the input parameters
	CHECK(NULL!=t)<<"Input parameters";	

	//先取字符串的长度
	//get the length of the string.
	int len = 0;
	len = WideCharToMultiByte(CP_ACP,0,s,s.GetLength(),NULL,0,NULL,NULL);

	//转换字符串
	//Wide chars to Multi bytes 
	ret = WideCharToMultiByte(CP_ACP,0,s,s.GetLength(),(LPSTR)t,len,NULL,NULL);
	if(0==ret){
		LOG(ERROR)<<"Transform WildChars to Multi-bytes --> [Failed].";
		return -1;
	}

	return 0;
}


