copy ..\release\ChargeClientActiveX.ocx .\ChargeClientActiveX.ocx 
regsvr32 /u ChargeClientActiveX.ocx
signtool sign /v /s my /t http://timestamp.wosign.com/timestamp et199csp11.dll
signtool sign /v /s my /t http://timestamp.wosign.com/timestamp ChargeClientActiveX.ocx
cabarc -s 6144 N ChargeClientActiveX.cab ^
	ChargeClientActiveX.ocx  ^
	ChargeClientActiveX.inf ^
        et199csp11.dll ^   
signtool sign /v /s my /t http://timestamp.wosign.com/timestamp ChargeClientActiveX.cab 
signtool verify /v /pa  ChargeClientActiveX.cab
copy ChargeClientActiveX.cab "D:\WebDir\la_client\system\ocx"
 




