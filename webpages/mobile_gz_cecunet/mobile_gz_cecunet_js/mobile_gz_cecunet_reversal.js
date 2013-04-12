function Reversal_PassParameter() {
// ------ DEBUG INFO BEGIN ------
//If the debug flag is set "ON", the log will be wroten.
//After we finished the debug process, we should remove the lines.
//ChargeClientActiveX1.InputParameterDebugFlag="DEBUG_ON";
 //ChargeClientActiveX1.InputParameterDebugFlag="DEBUG_OFF";

// The flag "debugger" is set for VS 2005 studio
//debugger;
//------ DEBUG INFO END ------

//double check the input parameters
var money = reversal_money.value; 
money = money.replace(/(^\s*)|(\s*$)/g, "");
if (0.001>=parseFloat(money)){
    //alert("The value of money must be greater than ZERO.");
	alert("金额有误，请检查。");
   return;
}
//assume the length of phone number >=8
phone_number = reversal_phone_number.value;
phone_number = phone_number.replace(/\D/g, "");
if ( 8>phone_number.length) {
    //alert("There are something wrong with the phone number.");
	alert("号码有误，请检查。");
    return;
}

var cecunet_la = reversal_cecunet_sn.value;
cecunet_la = cecunet_la.replace(/(^\s*)|(\s*$)/g, "");
if(32!=cecunet_la.length){
	//alert("Reversal Serial Number is wrong!");
	alert("流水号有误，请检查。");
	return;
}
// Get the name of company abbr.
ChargeClientActiveX1.InputParameterComName = "mobile_gz_cecunet";

// Get the name of transaction action.
ChargeClientActiveX1.InputParameterActName = "reversal"; 

//Fill out the common packet Fields 
ChargeClientActiveX1.InputFields(6) = phone_number;

//Convert the value of money from YUAN2 format to FEN1 format. 
var money = charge_money.value;
var money_str = jQuery.sprintf("%010d",parseFloat(money)*100);
ChargeClientActiveX1.InputFields(7) = money_str;

//Fill out the transaction packet Fields. 
var now = new Date();
ChargeClientActiveX1.InputFields(11) = now.format("mmddHHmmss"); 
ChargeClientActiveX1.InputFields(18) =  phone_number;    
money_str = jQuery.sprintf("%012d", parseFloat(money)*100);
ChargeClientActiveX1.InputFields(19) = money_str; 


ChargeClientActiveX1.InputFields(20) = cecunet_la; 

//Send the packet to verify server.
ChargeClientActiveX1.WebOrder="SIGN_SEND";
ChargeClientActiveX1.LoadParameter();

//Waiting for the packet from the verify server.
//Waiting ...

//There are three kinds of error codes.
var error_info =" ";
var error_code_ocx = ChargeClientActiveX1.ActivexErrorCode; 
if(0!=error_code_ocx){
    //alert("OCX Control Error! Read the log file for the details.");
	error_info = ChargeClientActiveX1.ActivexErrorInfo;
	alert(error_info);
    return;
}

var error_code_cmn_pkt_hdr = ChargeClientActiveX1.OutputFields(8); 
if("00"!==error_code_cmn_pkt_hdr && "  "!==error_code_cmn_pkt_hdr){
    error_info = ChargeClientActiveX1.OutputFields(9);
    error_info = error_info.replace(/(^\s*)|(\s*$)/g, ""); 
    error_info = "Data Center:" + error_info + "\n";    
    alert(error_info);
    return;
}
//debuger;
var transaction_response_code = ChargeClientActiveX1.OutputFields(14);
if ("0000"!== transaction_response_code){
    error_info = "Cecunet error codes:" + transaction_response_code;
    alert(error_info);
    return;
}

//Dispay the results when the charge operation is done.
var backward_pkt_phone_number = ChargeClientActiveX1.OutputFields(19);

//remove redundant blank spaces.
backward_pkt_phone_number = backward_pkt_phone_number.replace(/(^\s*)|(\s*$)/g, "");

var backward_pkt_money = ChargeClientActiveX1.OutputFields(20);
backward_pkt_money = (parseFloat("0"+backward_pkt_money)/100).toString();

var backward_pkt_la_sn = ChargeClientActiveX1.OutputFields(17);
backward_pkt_la_sn = backward_pkt_la_sn.replace(/(^\s*)|(\s*$)/g, "");

var backward_pkt_cecunet_sn = ChargeClientActiveX1.OutputFields(18);
backward_pkt_cecunet_sn = backward_pkt_cecunet_sn.replace(/(^\s*)|(\s*$)/g, "");

td = document.getElementById("print_reversal_result");
td.innerText="成功";
td = document.getElementById("print_reversal_phone_number");
td.innerText= backward_pkt_phone_number;
td = document.getElementById("print_reversal_money")
td.innerText=backward_pkt_money;

return;
}//end of function 'PassChargeParameters()'


function Clear_ReversalResults() {
    $('#print_reversal_result').empty(); 
    $('#print_reversal_phone_number').empty(); 
    $('#print_reversal_money').empty(); 
}


//打开或显示返销功能的界面
$(document).ready(function(){
$(".flip").click(function(){
    	$("table.reversal_result").toggle();	
		$("div.reversal").toggle();
  });
});
