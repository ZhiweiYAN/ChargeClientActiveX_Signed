#2013-04-16
## 解决了一个大问题
- 春节前，Openssl的Verify功能不能用。使用了浑身解术，也不管用。
- 原来必须SHA后，签名前要加入PKCS标准的头；然后再签名。这才符合PKCS的标准。
- 两周前，在ActiveX上验证服务器的签名，提示长度不对。
- 因为，Openssl的签名是符合PKCS标准的。它也是在SHA1后加了PKCS标准头，后再签名的。
- 所以，SHA1签名，长度其实是35个字节（SHA1,20个，PKCS包头，15个）。

#2013-03-30
## 对USB Key的自检
- 加入取Ukey的ID,并显示在界面上
- 重新开放ActiveX的数据库操作功能

#2013-03-29
## 第三方库的头文件所在目录
1. USB KEY，C:\et199_pki_dll\Include\pkcs11;
2. Curl c:\curl-7.16.4\include;
3. Gas IC card， c:\gas_card;
4. Glog， C:\glog-0.3.1\windows;
5. Berkyley DB, C:\db-5.1.25\build_windows;

## 第三方库的LIB代码所在目录
1. "C:\et199_pki_dll\Lib\x86;"
2. "c:\curl-7.16.4\lib";
3. "c:\gas_card;"
3. "C:\glog-0.3.1";
4. "C:\db-5.1.25\build_windows\Win32\Release_static";
5. "C:\db-5.1.25\build_windows\Win32\Debug_static"

## 第三方库的LIB文件名Debug版
1. libglog.lib 
2. libdb_small51s.lib 
3. curllib.lib 
4. ws2_32.lib 
5. winmm.lib 
6. et199csp11.lib

## 第三方库的LIB文件名Release版
1. libglog_static.lib 
2. libdb_small51s.lib 
3. curllib.lib 
4. ws2_32.lib 
5. winmm.lib 
6. et199csp11.lib

## 预处理定义
### debug 
> WIN32;_WINDOWS;NDEBUG;_USRDLL;GOOGLE_GLOG_DLL_DECL=;CURL_STATICLIB
### Release
> GOOGLE_GLOG_DLL_DECL=;WIN32;_WINDOWS;_DEBUG;_USRDLL;CURL_STATICLIB

