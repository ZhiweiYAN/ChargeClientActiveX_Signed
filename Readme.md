#2013-04-16
## �����һ��������
- ����ǰ��Openssl��Verify���ܲ����á�ʹ���˻��������Ҳ�����á�
- ԭ������SHA��ǩ��ǰҪ����PKCS��׼��ͷ��Ȼ����ǩ������ŷ���PKCS�ı�׼��
- ����ǰ����ActiveX����֤��������ǩ������ʾ���Ȳ��ԡ�
- ��Ϊ��Openssl��ǩ���Ƿ���PKCS��׼�ġ���Ҳ����SHA1�����PKCS��׼ͷ������ǩ���ġ�
- ���ԣ�SHA1ǩ����������ʵ��35���ֽڣ�SHA1,20����PKCS��ͷ��15������

#2013-03-30
## ��USB Key���Լ�
- ����ȡUkey��ID,����ʾ�ڽ�����
- ���¿���ActiveX�����ݿ��������

#2013-03-29
## ���������ͷ�ļ�����Ŀ¼
1. USB KEY��C:\et199_pki_dll\Include\pkcs11;
2. Curl c:\curl-7.16.4\include;
3. Gas IC card�� c:\gas_card;
4. Glog�� C:\glog-0.3.1\windows;
5. Berkyley DB, C:\db-5.1.25\build_windows;

## ���������LIB��������Ŀ¼
1. "C:\et199_pki_dll\Lib\x86;"
2. "c:\curl-7.16.4\lib";
3. "c:\gas_card;"
3. "C:\glog-0.3.1";
4. "C:\db-5.1.25\build_windows\Win32\Release_static";
5. "C:\db-5.1.25\build_windows\Win32\Debug_static"

## ���������LIB�ļ���Debug��
1. libglog.lib 
2. libdb_small51s.lib 
3. curllib.lib 
4. ws2_32.lib 
5. winmm.lib 
6. et199csp11.lib

## ���������LIB�ļ���Release��
1. libglog_static.lib 
2. libdb_small51s.lib 
3. curllib.lib 
4. ws2_32.lib 
5. winmm.lib 
6. et199csp11.lib

## Ԥ������
### debug 
> WIN32;_WINDOWS;NDEBUG;_USRDLL;GOOGLE_GLOG_DLL_DECL=;CURL_STATICLIB
### Release
> GOOGLE_GLOG_DLL_DECL=;WIN32;_WINDOWS;_DEBUG;_USRDLL;CURL_STATICLIB

