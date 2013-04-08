#pragma once

class CBerkeleyDB
{
public:
	CBerkeleyDB(void);
public:
	~CBerkeleyDB(void);
public:
	int GetCStringBytesNumber(CString str);
	int RemoveNullChar(char *t, CString s);

	int ScanTemplateDB(char * db_file, CString &str);
	int CheckKeyDataStringValid(CString key, CString data);
	int GetDataByKeyFromDB(CString ckey, char * *data, int* data_length);

	int InsertRecordIntoDatabase(CString key, CString data);
	int WriteIntoDatabaseFile(char *key_buffer, int key_length , char* data_buffer, int data_length,char *db_path_file_name);
	
	int RemoveRecordFromDatabase(CString key);
	int RemoveFromDatabaseFile(char * key_buffer, int key_length,char * db_path_file_name);

public:
	// The content in memory into HEX String
	CString hex2str(char* hex, int hex_len );
public:
	int GetDbDownloadFlag(void);
public:
	int SetDbDownloadFlag(int down);
};
