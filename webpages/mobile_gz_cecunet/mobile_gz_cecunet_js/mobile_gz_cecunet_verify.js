function Verify_PassParameter() {
// ------ DEBUG INFO BEGIN ------
//If the debug flag is set "ON", the log will be wroten.
//After we finished the debug process, we should remove the lines.
//ChargeClientActiveX1.InputParameterDebugFlag="DEBUG_ON";
 //ChargeClientActiveX1.InputParameterDebugFlag="DEBUG_OFF";

// The flag "debugger" is set for VS 2005 studio
//debugger;
//------ DEBUG INFO END ------

//double check the input parameters
var la_sn = verify_la_sn.value; 
la_sn = la_sn.replace(/(^\s*)|(\s*$)/g, "");
if (0==la_sn.length){
 	alert("利安流水有误.，请检查。");
   return;
}
//assume the length of phone number >=8
charge_date = verify_charge_date.value;
charge_date = charge_date.replace(/(^\s*)|(\s*$)/g, "");
if ( 8!=charge_date.length) {
    //alert("There are something wrong with the phone number.");
	alert("缴费日期有误，请检查。");
    return;
}

// Get the name of company abbr.
ChargeClientActiveX1.InputParameterComName = "mobile_gz_cecunet";

// Get the name of transaction action.
ChargeClientActiveX1.InputParameterActName = "verify"; 


//Fill out the transaction packet Fields. 
var now = new Date();
ChargeClientActiveX1.InputFields(11) = now.format("mmddHHmmss"); 
ChargeClientActiveX1.InputFields(16) = la_sn;    
ChargeClientActiveX1.InputFields(18) = charge_date; 

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
//debugger;
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

// var backward_pkt_la_sn = ChargeClientActiveX1.OutputFields(17);
// backward_pkt_la_sn = backward_pkt_la_sn.replace(/(^\s*)|(\s*$)/g, "");

// var backward_pkt_cecunet_sn = ChargeClientActiveX1.OutputFields(18);
// backward_pkt_cecunet_sn = backward_pkt_cecunet_sn.replace(/(^\s*)|(\s*$)/g, "");

var backward_pkt_charge_date = ChargeClientActiveX1.OutputFields(21);
backward_pkt_charge_date = backward_pkt_charge_date.replace(/(^\s*)|(\s*$)/g, "");

var backward_pkt_verify_result =  ChargeClientActiveX1.OutputFields(22);
backward_pkt_verify_result = backward_pkt_verify_result.replace(/(^\s*)|(\s*$)/g, "");

//output to webpages
var td;
if("0"==backward_pkt_verify_result){
	td = document.getElementById("print_verify_result");
	td.innerText="成功";
}
td = document.getElementById("print_verify_phone_number");
td.innerText= backward_pkt_phone_number;

td = document.getElementById("print_verify_money");
td.innerText=backward_pkt_money;

td = document.getElementById("print_verify_charge_date");
td.innerText=backward_pkt_charge_date;

return;
}//end of function 'PassChargeParameters()'


function Clear_VerifyResults() {
    $('#print_reversal_result').empty(); 
    $('#print_reversal_phone_number').empty(); 
    $('#print_reversal_money').empty(); 
    $('#print_verify_charge_date').empty(); 
	
}
