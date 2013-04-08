/*************************************************************     
Author      : Yanfu Xie <xieyanfu@yahoo.com.cn>     
Url     : http://YITU.org       
Description : 正确截取单字节和双字节混和字符串     
String str  : 要截取的字符串
Number start: 截取的起始位置(字节为单位)
Number len  : 截取长度(字节为单位)  
*************************************************************/
function sub_str(str, start, len) {
	if(!str || !len){ 
		return ''; 
	}
	//预期计数：中文2字节，英文1字节     
	var a = 0;       
	//循环计数     
	var i = 0;       
	//临时字串     
	var temp = ''; 
	//debugger;
	//计算以汉字为单位的start
	for (i=0;i<start;i++){         
		if (str.charCodeAt(i)>255){             
		//按照预期计数减1             
			start--;         
		}         
	}     
	for (i=start;i<str.length;i++){         
		if (str.charCodeAt(i)>255){             
			//按照预期计数增加2             
			a+=2;         
		}else{             
			a++;         
		}         
		//如果增加计数后长度大于限定长度，就直接返回临时字符串         
		if(a > len) { 
			return temp; 
		}           
		//将当前内容加到临时字符串         
		temp += str.charAt(i);     
	}     
	//如果全部是单字节字符，就直接返回源字符串     
	return temp; 
} 

/*************************************************************     
Author      : Zhang Lei      
Url     :        
Description : 以分为单位的转成以元为单位的     
string1     : 以分为单位的钱的字符串
*************************************************************/

function money_change(string1)//0100-->1.00
{
	while(string1.indexOf("0")==0){
		if(string1.length==3)
			break;
		string1 = string1.replace("0","");
	}
	string1 = string1.substr(0, string1.length-2) + "." + string1.substr(string1.length-2, 2);
	return string1;
}

/*************************************************************     
Author      : Zhang Lei      
Url     :        
Description : 去空格     
*************************************************************/
String.prototype.trim = function()//去空格
{
    return this.replace(/(^\s*)|(\s*$)/g, "");
}

