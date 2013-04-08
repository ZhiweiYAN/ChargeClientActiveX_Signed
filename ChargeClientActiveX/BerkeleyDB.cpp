#include "StdAfx.h"
#include "BerkeleyDB.h"
#include "MacroDefinition.h"
#include <db.h>

#undef ERROR
#include <glog/logging.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBerkeleyDB::CBerkeleyDB(void)
{
}

CBerkeleyDB::~CBerkeleyDB(void)
{
}

int CBerkeleyDB::ScanTemplateDB(char * db_file, CString &str)
{
	DB *dbp;
	DBC *dbcp;
	DBT key, data;
	int close_db, close_dbc, ret;
	close_db = close_dbc = 0;
	CString s;

	/* Open the database. */
	if ((ret = db_create(&dbp, NULL, 0)) != 0) {
		LOG(ERROR)<<"Init DB --- [failed].";
		return -1;
	}
	close_db = 1;
	/* Turn on additional error output. */
	//dbp->set_errfile(dbp, stderr);
	//dbp->set_errpfx(dbp, progname);
	/* Open the database. */
	if ((ret = dbp->open(dbp, NULL, db_file, NULL,
		DB_UNKNOWN, DB_RDONLY, 0)) != 0) {
			LOG(ERROR)<<"OPEN DB --- [failed].";
			goto err;
	}
	/* Acquire a cursor for the database. */
	if ((ret = dbp->cursor(dbp, NULL, &dbcp, 0)) != 0) {
		LOG(ERROR)<<"OPEN DB Cursor --- [failed].";
		goto err;
	}
	close_dbc = 1;
	/* Initialize the key/data return pair. */
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));

	/* Walk through the database and print out the key/data pairs. */
	char db_record[MAX_STRING_LENGTH];

	while ((ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT)) == 0){
		memset(db_record, 0, MAX_STRING_LENGTH);
		sprintf_s(db_record, MAX_STRING_LENGTH, "%.*s: %.*s",	
			(int)key.size, (char *)key.data,(int)data.size, (char *)data.data);
		s = db_record;
		str = str +  s + HTTP_RETURN;
	}
	if (ret != DB_NOTFOUND) {
		LOG(ERROR)<<"OPEN DB Cursor -get --- [failed].";
		goto err;
	}
err: if (close_dbc && (ret = dbcp->c_close(dbcp)) != 0)
		 LOG(ERROR)<<"Close DB Cursor --- [failed].";;
	 if (close_db && (ret = dbp->close(dbp, 0)) != 0)
		 LOG(ERROR)<<"Close DB --- [failed].";
	 return (0);
}

int CBerkeleyDB::CheckKeyDataStringValid(CString key, CString data)
{
	int ret = 0;
	int err = 0;

	// Check key string
	CString letter_set;
	int len = 0;
	letter_set.Empty();
	letter_set = DB_KEY_ALPHA_BETA_LETTER_SET;
	len = key.GetLength();

	for(int i=0; i<len; i++){
		char c = (char)key.GetAt(i);
		ret = letter_set.Find(c);
		LOG_IF(ERROR, -1==ret)<<"The character "<<c<<" NOT in "<< DB_KEY_ALPHA_BETA_LETTER_SET;
		if(-1 == ret){
			err = -1;
			//DisplayBackToWebPage(_T("字母不合法. 只能是英文小写a-z,0-9 和 下划线_."),DISPLAY_WEB_POSITION);
			break;
		}
	}

	// Check data string
	letter_set.Empty();
	letter_set = DB_DATA_ALPHA_BETA_LETTER_SET;
	len = data.GetLength();

	for(int i=0; i<len; i++){
		char c = (char)data.GetAt(i);
		ret = letter_set.Find(c);
		LOG_IF(ERROR, -1==ret)<<"The character "<<c<<" NOT in "<< DB_DATA_ALPHA_BETA_LETTER_SET;
		if(-1 == ret){
			err = -1;
			//DisplayBackToWebPage(_T("字母不合法. 只能是英文大小写A-Z,a-z,0-9 和星号*."),DISPLAY_WEB_POSITION);
			break;
		}
	}

	return err;
}

//插入一条记录到数据库中去, insert a record into the db file.
int CBerkeleyDB::GetDataByKeyFromDB(CString ckey, char * *data, int* data_length)
{
	int ret = 0; 
	int err = 0;

	//检查输入参数
	//Check the input parameters
	if(NULL==data_length||ckey.IsEmpty()){
		LOG(ERROR)<<"Input parameters errors.";
		return -1;

	}

	//去除NULL字符
	//Remove the NULL chars from the key and data
	int key_length = 0;	
	key_length = GetCStringBytesNumber(ckey) + STRING_TAIL_NULL;

	char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);
	CHECK(NULL!=key_buffer)<<"Allocate memory --> [Not Enough Mem].";
	memset(key_buffer,0,key_length);

	RemoveNullChar(key_buffer,ckey);

	//初始化一个DB的使用句柄
	//Init the handle of db
	DB *dbp = NULL; 
	err = db_create(&dbp, NULL, 0);
	if (err != 0) {
		/* Error handling goes here */
		LOG(ERROR)<<"Init DB handle --> [failed].";
	}
	LOG(INFO)<<"Init DB handle --> [OK].";

	err = dbp->open(dbp, /* DB structure pointer */
		NULL, /* Transaction pointer */
		CHARGE_CLIENT_DB_FILE_NAME, /* On-disk file that holds the database. */
		NULL, /* Optional logical database name */
		DB_BTREE, /* Database access method */
		DB_RDONLY, /* Open flags */
		0); /* File mode (using defaults) */
	if (err != 0) 
	{
		LOG(ERROR)<<"Open DB file --> [Failed].";
		ret = -1;
		if (NULL!=key_buffer)
		{
			free(key_buffer);
			key_buffer = NULL;
		}
		return ret;
	}
	LOG(INFO)<<"Open DB file --> [OK].";

	//初始化DBT的结构
	//Init the structures of DB
	DBT get_key;
	DBT get_data;
	memset( &get_key, 0, sizeof(DBT) );
	memset( &get_data, 0, sizeof(DBT) );

	//检测一下数据库中存有此KEY的数据
	//Find the data via the key
	get_key.size = key_length;
	get_key.data = key_buffer;

	err = dbp->get( dbp, NULL, &get_key, &get_data, 0);
	if( DB_NOTFOUND == err){
		LOG(WARNING)<<"Find in DB via KEY --> [No Found].";
		LOG(WARNING)<<"KEY="<<key_buffer;
		ret = -1;
	}else{
		LOG(INFO)<<"Find in DB via KEY --> [OK].";
		LOG(INFO)<<"KEY="<<key_buffer;
		LOG(INFO)<<"DATA="<<get_data.size<<"|"<<(char*)(get_data.data)<<"|";
		*data = (char *)malloc(get_data.size);
		CHECK(NULL!=data)<<"Allocate memory --> [Not Enough Mem].";
		memcpy(*data,get_data.data, get_data.size);
		*data_length = get_data.size;
	}

	// When we're done with the database, close it. O
	// 关闭数据库
	if (dbp != NULL){
		err = dbp->close(dbp, 0);
		CHECK(err==0)<<"Close DB --> [Failed]";
		dbp = NULL;
	}
	LOG(INFO)<<"Close DB --> [OK]";

	// Free memory
	if (NULL!=key_buffer)
	{
		free(key_buffer);
		key_buffer = NULL;
	}
	return ret;
}

//插入一条记录到数据库中
//Inset a record(key, data0 into the db file
int CBerkeleyDB::InsertRecordIntoDatabase(CString key, CString data)
{
	int ret = 0;

	//检查输入参数
	//Check the input parameters
	if(key.IsEmpty()||data.IsEmpty()){
		LOG(ERROR)<<"The length of key or data MUST NOT be NULL";
		return -1;

	}

	//去除NULL字符
	//Remove the NULL chars from the key and data
	int key_length = 0;
	int data_length = 0;
	key_length = GetCStringBytesNumber(key)+STRING_TAIL_NULL;
	data_length = GetCStringBytesNumber(data)+STRING_TAIL_NULL;

	char * key_buffer = NULL;
	char * data_buffer = NULL;
	key_buffer = (char *)malloc(key_length);
	CHECK(NULL!=key_buffer)<<"Allocate key memory --> [Not Enough Mem].";
	memset(key_buffer,0,key_length);
	data_buffer = (char*)malloc(data_length);
	CHECK(NULL!=data_buffer)<<"Allocate data memory --> [Not Enough Mem].";
	memset(data_buffer,0,data_length);

	RemoveNullChar(key_buffer,key);
	RemoveNullChar(data_buffer,data);

	//写入数据
	//Write Data
	ret = WriteIntoDatabaseFile(key_buffer,key_length, data_buffer, data_length, CHARGE_CLIENT_DB_FILE_NAME);
	CHECK(0==ret)<<"Writing DB --> [failed].";

	free(key_buffer);
	free(data_buffer);
	key_buffer = NULL;
	data_buffer = NULL;

	return 0;
}

int CBerkeleyDB::WriteIntoDatabaseFile(char *key_buffer, int key_length , char* data_buffer, int data_length,char *db_path_file_name)
{
	int ret = 0; 

	//检查输入参数
	//Check the input parameters
	CHECK(NULL!=key_buffer) << "Input parameters";
	CHECK(NULL!=data_buffer) <<  "Input parameters";
	CHECK(NULL!=db_path_file_name) <<  "Input parameters";

	//初始化一个DB的使用句柄
	//Init the handle
	DB *dbp = NULL; 
	ret = db_create(&dbp, NULL, 0);
	CHECK(0==ret)<<"Init DB handle --> [Failed].";
	LOG_IF(INFO,0==ret)<<"Init DB handle --> [OK].";

	ret = dbp->open(dbp, /* DB structure pointer */
		NULL, /* Transaction pointer */
		db_path_file_name, /* On-disk file that holds the database. */
		NULL, /* Optional logical database name */
		DB_BTREE, /* Database access method */
		DB_CREATE, /* Open flags */
		0); /* File mode (using defaults) */
	CHECK(0==ret)<<"Open DB file --> [Failed].";
	LOG_IF(INFO,0==ret)<<"Open DB file --> [OK].";

	//初始化DBT的结构
	//init the structures of DB
	DBT key, put_data;
	DBT get_data;
	memset( &key, 0, sizeof(DBT) );
	memset( &get_data, 0, sizeof(DBT) );
	memset( &put_data, 0, sizeof(DBT) );

	//先检测一下数据库中存有此KEY的数据,无则直接插入,有则先删除.
	//Check the existed data with key berfore inserting the data and key.
	key.size = key_length;
	key.data = key_buffer;

	ret = dbp->get( dbp, NULL, &key, &get_data, 0);
	if( DB_NOTFOUND == ret){
		LOG(WARNING)<<"KEY="<<key_buffer<<"Find in DB via KEY --> [No Found].";
	}else{
		LOG(INFO)<<"KEY="<<key_buffer<<"Find in DB via KEY --> [OK].";
		LOG(INFO)<<"DATA_LEN="<<get_data.size<<"  Content="<<(char*)(get_data.data);
		ret = dbp->del( dbp, NULL, &key, 0);
		LOG_IF(ERROR, 0!=ret)<<"Delete item in DB --> [Failed].";
		LOG_IF(INFO, 0==ret)<<"Delete item in DB --> [OK].";
	}

	//插入数据到数据库中
	//Insert thd dat into the DB
	put_data.size = data_length;
	put_data.data = data_buffer;

	LOG(INFO)<<"Insert_KEY="<<(char*)(key.data);
	LOG(INFO)<<"Insert_DATA="<<(char*)(put_data.data);


	ret = dbp->put(dbp, NULL, &key, &put_data, DB_OVERWRITE_DUP);
	if(0!=ret){
		LOG(ERROR)<<"Insert DB --> [Failed].";
	}else{
		LOG(INFO)<<"Insert DB --> [OK].";
	}


	/* When we're done with the database, close it. */
	if (dbp != NULL){
		ret = dbp->close(dbp, 0);
		CHECK(ret==0)<<"Close DB --> [Failed]";
		dbp = NULL;
	}
	LOG(INFO)<<"Close DB --> [OK]";
	return 0;
}

int CBerkeleyDB::GetCStringBytesNumber(CString str)
{
	int len = 0; 

	//检查输入参数
	//Check the input parameters
	if(str.IsEmpty()){
		LOG(ERROR)<<"Input string is NULL.";
		return -1;
	}	
	len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	return len;
}

//用来去除空字符，也可直接用T2A这宏代替。
//Remove the NULL chars from a string. The function can alos be replaced by the Macro T2A
int CBerkeleyDB::RemoveNullChar(char *t, CString s)
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

int CBerkeleyDB::RemoveRecordFromDatabase(CString key)
{
	int ret = 0;

	//检查输入参数
	//Check the input parameters
	if(key.IsEmpty()){
		LOG(ERROR)<<"The length of key or data must not be less than zero";
		return -1;

	}

	//去除NULL字符
	//Remove the NULL chars from the key and data
	int key_length = 0;
	key_length = GetCStringBytesNumber(key)+STRING_TAIL_NULL;

	char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);
	LOG_IF(ERROR, NULL==key_buffer)<<"Allocate memory --> [Not Enough Mem].";
	memset(key_buffer,0,key_length);

	RemoveNullChar(key_buffer,key);

	//从数据库中删除
	//Remove from the DB
	ret = RemoveFromDatabaseFile(key_buffer,key_length, CHARGE_CLIENT_DB_FILE_NAME);
	LOG_IF(ERROR,0!=ret)<<"While writing into database file, error happened.";

	free(key_buffer);
	key_buffer = NULL;

	return 0;
}

int CBerkeleyDB::RemoveFromDatabaseFile(char * key_buffer, int key_length,char * db_path_file_name)
{
	int ret = 0; 

	//检查输入参数
	//Check the input parameters
	if(NULL==key_buffer||NULL==db_path_file_name){
		LOG(ERROR)<<"Input parameters errors.";
		return -1;
	}

	//打开一个Berkeley DB 数据库,用于存放本地的数据
	//初始化一个DB的使用句柄
	DB *dbp = NULL; 
	ret = db_create(&dbp, NULL, 0);
	if (ret != 0) {
		/* Error handling goes here */
		LOG(ERROR)<<"Init DB handle --> [Failed].";
	}
	LOG(INFO)<<"Init DB handle --> [OK].";

	ret = dbp->open(dbp, /* DB structure pointer */
		NULL, /* Transaction pointer */
		db_path_file_name, /* On-disk file that holds the database. */
		NULL, /* Optional logical database name */
		DB_BTREE, /* Database access method */
		DB_CREATE, /* Open flags */
		0); /* File mode (using defaults) */
	if (ret != 0) {
		LOG(ERROR)<<"Open DB file --> [Failed].";
	}
	LOG(INFO)<<"Open DB file --> [OK].";

	//初始化DBT的结构
	//Init  the structure of DB
	DBT key;
	DBT get_data;
	memset( &key, 0, sizeof(DBT) );
	memset( &get_data, 0, sizeof(DBT) );

	//先检测一下数据库中存有此KEY的数据,无则直接插入,有则先删除.
	//Check the existed data with key berfore inserting the data and key.
	key.size = key_length;
	key.data = key_buffer;

	ret = dbp->get( dbp, NULL, &key, &get_data, 0);
	if( DB_NOTFOUND == ret){
		LOG(ERROR)<<"KEY="<<key_buffer<<"Find in DB via KEY --> [No Found].";
	}else{
		LOG(INFO)<<"KEY="<<key_buffer<<"Find in DB via KEY --> [OK].";
		LOG(INFO)<<"DATA_LEN="<<get_data.size<<"  Content="<<"|"<<(char*)(get_data.data)<<"|";
		ret = dbp->del( dbp, NULL, &key, 0);
		LOG_IF(ERROR, 0!=ret)<<"Delete item in DB --> [Failed].";
		LOG_IF(INFO, 0==ret)<<"Delete item in DB --> [OK].";
	}
	/* When we're done with the database, close it. */
	if (dbp != NULL){
		ret = dbp->close(dbp, 0);
		CHECK(ret==0)<<"Close DB --> [Failed]";
		dbp = NULL;
	}
	LOG(INFO)<<"Close DB --> [OK]";
	return 0;
}


// The content in memory into HEX String
CString CBerkeleyDB::hex2str(char* hex, int hex_len)
{
	CString ret;
    int i = 0;
//	assert(NULL!=hex);

	USES_CONVERSION;

	//unsigned char* str = NULL;
	//int str_len = 0;


	if(hex_len<=0 || NULL==hex){
		ret = A2T("HEX2Str: hex_len < =0, or, HEX == NULL");
		LOG(ERROR)<<"HEX2Str: hex_len < =0, or, HEX == NULL";
		return ret;
	}

	//以NULL来结尾
    //str_len = 2 * hex_len + 2;
	//str = (unsigned char *)malloc(str_len);
	//CHECK(NULL!=str)<<"Malloc Memory.";

	//memset(str, 0, str_len);

	ret.Empty();
	char str[MAX_STRING_LENGTH];
	for(i=0; i<hex_len; i++){
		memset(str, 0, MAX_STRING_LENGTH);
		sprintf_s(str, MAX_STRING_LENGTH, "%02X", (( unsigned char *) hex)[i] );
		ret = ret+A2T((char*)str);
	}

	return ret;

}
// If the value of return clause equals 1, it will conduct a download action.
int CBerkeleyDB::GetDbDownloadFlag(void)
{
	int ret = 0;
	int len = 0;
	char *data = NULL;
	USES_CONVERSION;

	data = (char*)malloc(MAX_STRING_LENGTH);
	CHECK(NULL!=data)<<"Malloc Mem --> [failed].";
	memset(data, 0, MAX_STRING_LENGTH);

	GetDataByKeyFromDB(_T(DB_DOWNLOAD_FLAG_KEY), &data, &len);

	if(len>=1){
		CString str = _T("取本地数据库模板标记 成功.");
		str = str + A2T(data);

		str = A2T(data);
		if(0==str.CompareNoCase(_T(DB_DOWNLOAD_TRUE))){
			ret = 1;
		}
		else{
			ret = -1;
		}
	}else{
		ret = -1;
	}

	free(data);
	data = NULL;
	return ret;
}

int CBerkeleyDB::SetDbDownloadFlag(int down)
{
	int ret = 0;
	if(1 == down){
		ret = InsertRecordIntoDatabase(_T(DB_DOWNLOAD_FLAG_KEY),_T(DB_DOWNLOAD_TRUE));
	}
	else{
		ret = InsertRecordIntoDatabase(_T(DB_DOWNLOAD_FLAG_KEY),_T(DB_DOWNLOAD_FALSE));
	}
	return 0;
}
