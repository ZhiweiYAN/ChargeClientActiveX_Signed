#pragma once
#include "stdio.h"
#include <iostream>
#include <string>
#include <map>
#include "systemmaintenance.h"
using namespace std;
#define MAX_LEN 4096

typedef struct{
	int x;
	int y;
	char* data;
	int data_len;
}T_InvoiceItem;
typedef map<string,T_InvoiceItem *> Map_InvoiceItem;

class CDotPrinter
{
public:
	CDotPrinter(void);
public:
	~CDotPrinter(void);
public:
	FILE *m_fp;
	Map_InvoiceItem m_invoice;
public:
	int InitPrinter(void);
	int ReSetPrinter(void);
	void PrintOut(unsigned char* lpcbData,int Length);
	int SetPageLength(int inches);
	int SetPageFormatLeftMargin(int len_char_num);
	int EnableChineseCharSet(void);
	int DisableChineseCharSet(void);
	int EnableSongFont(void);
	int EnableHeiFont(void);
	int CarriageReturn(void);
	int LineFeed(void);
	int FormFeed(void);
	int MoveAbsoluteHorizontalPosition(double x_inches);
	int BackRelativeVerticalPosition(double y_inches);
	int AdvanceRelativeVerticalPosition(double y_inches);
	int PrintBarcode(char * data, int size);
public:
	int PushInvoiceItem(int x, int y, int data_len, char* data);
public:
	int ClearInvoiceItem(void);
public:
	int DisplayInvoiceItem(void);
public:
	int PrintInvoiceItem(void);
public:
	int TestingPrinter(void);
public:
	int ReturnHomePosition(void);
public:
	int VerifyPrinter(void);
public:
	int ClosePrinter(void);
	int RemoveNullChar(char *t, CString s);
public:
	CSystemMaintenance m_sys_worker;
};
