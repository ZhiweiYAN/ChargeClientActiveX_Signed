/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
// JavaScript Document
//2011-06-24
String.prototype.trim = function()//去空格
{
    return this.replace(/(^\s*)|(\s*$)/g, "");
}

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

//输入框清空
function $(ID) 
{
	return document.getElementById(ID);
}
function chg1(){
	$("setfocus1").value="";
}
function newChg1(){
    if($("setfocus1").value==""){
        $("setfocus1").value="缴费金额";
    }
    else{
		return $("setfocus1").value;
	}
}

function chg2(){
$("setfocus2").value="";
}
function newChg2(){
    if($("setfocus2").value==""){
        $("setfocus2").value="返销金额";
        }
    else{return $("setfocus2").value }
}

function chg3(){
$("setfocus3").value="";
}
function newChg3(){
    if($("setfocus3").value==""){
        $("setfocus3").value="流水号";
        }
    else{return $("setfocus3").value }
}

function chg4(){
$("setfocus4").value="";
}
function newChg4(){
    if($("setfocus4").value==""){
        $("setfocus4").value="账单日期(YYYYMM)";
        }
    else{return $("setfocus4").value }
}

function doit(num)
{
	controlname = "TextBox" + num;
}
function numClick(num){
	document.all(controlname).value=document.all(controlname).value + num;
}
function delClick(){
//document.all(controlname).value=document.all(controlname).value 
	var longnum=document.all(controlname).value.length;
	var num;
	num=document.all(controlname).value.substr(0,longnum-1);
	document.all(controlname).value=num;
}

function convertCurrency(currencyDigits) { //金额大写
// Constants: 
	var MAXIMUM_NUMBER = 99999999999.99; 
// Predefine the radix characters and currency symbols for output: 
	var CN_ZERO = "零"; 
	var CN_ONE = "壹"; 
	var CN_TWO = "贰"; 
	var CN_THREE = "叁"; 
	var CN_FOUR = "肆"; 
	var CN_FIVE = "伍"; 
	var CN_SIX = "陆"; 
	var CN_SEVEN = "柒"; 
	var CN_EIGHT = "捌"; 
	var CN_NINE = "玖"; 
	var CN_TEN = "拾"; 
	var CN_HUNDRED = "佰"; 
	var CN_THOUSAND = "仟"; 
	var CN_TEN_THOUSAND = "万"; 
	var CN_HUNDRED_MILLION = "亿"; 
	var CN_SYMBOL = " "; 
	var CN_DOLLAR = "元"; 
	var CN_TEN_CENT = "角"; 
	var CN_CENT = "分"; 
	var CN_INTEGER = "整"; 

// Variables: 
	var integral; // Represent integral part of digit number. 
	var decimal; // Represent decimal part of digit number. 
	var outputCharacters; // The output result. 
	var parts; 
	var digits, radices, bigRadices, decimals; 
	var zeroCount; 
	var i, p, d; 
	var quotient, modulus; 

// Validate input string: 
	currencyDigits = currencyDigits.toString(); 
	if (currencyDigits == "") { 
//alert("Empty input!"); 
		return ""; 
	} 
	if (currencyDigits.match(/[^,.\d]/) != null) { 
//alert("Invalid characters in the input string!"); 
		return ""; 
	} 
	if ((currencyDigits).match(/^((\d{1,3}(,\d{3})*(.((\d{3},)*\d{1,3}))?)|(\d+(.\d+)?))$/) == null) { 
//alert("Illegal format of digit number!"); 
		return ""; 
	} 

// Normalize the format of input digits: 
	currencyDigits = currencyDigits.replace(/,/g, ""); // Remove comma delimiters. 
	currencyDigits = currencyDigits.replace(/^0+/, ""); // Trim zeros at the beginning. 
// Assert the number is not greater than the maximum number. 
	if (Number(currencyDigits) > MAXIMUM_NUMBER) { 
		alert("您输入的金额太大，请重新输入!"); 
		return ""; 
	} 

// Process the coversion from currency digits to characters: 
// Separate integral and decimal parts before processing coversion: 
	parts = currencyDigits.split("."); 
	if (parts.length > 1) { 
		integral = parts[0]; 
		decimal = parts[1]; 
// Cut down redundant decimal digits that are after the second. 
		decimal = decimal.substr(0, 2); 
	} 
	else { 
		integral = parts[0]; 
		decimal = ""; 
	} 
// Prepare the characters corresponding to the digits: 
	digits = new Array(CN_ZERO, CN_ONE, CN_TWO, CN_THREE, CN_FOUR, CN_FIVE, CN_SIX, CN_SEVEN, CN_EIGHT, CN_NINE); 
	radices = new Array("", CN_TEN, CN_HUNDRED, CN_THOUSAND); 
	bigRadices = new Array("", CN_TEN_THOUSAND, CN_HUNDRED_MILLION); 
	decimals = new Array(CN_TEN_CENT, CN_CENT); 
// Start processing: 
	outputCharacters = ""; 
// Process integral part if it is larger than 0: 
	if (Number(integral) > 0) { 
		zeroCount = 0; 
		for (i = 0; i < integral.length; i++) { 
			p = integral.length - i - 1; 
			d = integral.substr(i, 1); 
			quotient = p / 4; 
			modulus = p % 4; 
			if (d == "0") { 
				zeroCount++; 
			} 
			else { 
				if (zeroCount > 0) 
				{ 
					outputCharacters += digits[0]; 
				} 
				zeroCount = 0; 
				outputCharacters += digits[Number(d)] + radices[modulus]; 
			} 
			if (modulus == 0 && zeroCount < 4) { 
				outputCharacters += bigRadices[quotient]; 
			} 
		} 
		outputCharacters += CN_DOLLAR; 
	} 
	// Process decimal part if there is: 
	if (decimal != "") { 
		for (i = 0; i < decimal.length; i++) { 
			d = decimal.substr(i, 1); 
			if (d != "0") { 
				outputCharacters += digits[Number(d)] + decimals[i]; 
			} 
		} 
	} 
	// Confirm and return the final output string: 
	if (outputCharacters == "") { 
		outputCharacters = CN_ZERO + CN_DOLLAR; 
	} 
	if (decimal == "") { 
		outputCharacters += CN_INTEGER; 
	} 
	outputCharacters = CN_SYMBOL + outputCharacters; 
	return outputCharacters; 
}

function strLeft(s, n) {//左起截取字符串
	var s2 = s.slice(0, n),
	i = s2.replace(/[^\x00-\xff]/g, "**").length;
	if (i <= n) return s2;
	i -= s2.length;
	switch (i) {
		case 0: return s2;
		case n: return s.slice(0, n>>1);
		default:
		var k = n - i,
		s3 = s.slice(k, n),
		j = s3.replace(/[\x00-\xff]/g, "").length;
		return j ? s.slice(0, k) + strLeft(s3, j) : s.slice(0, k);
	}
}
