function Charge_PassParameter() {
// ------ DEBUG INFO BEGIN ------
//If the debug flag is set "ON", the log will be wroten.
//After we finished the debug process, we should remove the lines.
// ChargeClientActiveX1.InputParameterDebugFlag="DEBUG_ON";
 //ChargeClientActiveX1.InputParameterDebugFlag="DEBUG_OFF";

// The flag "debugger" is set for VS 2005 studio
//debugger;
//------ DEBUG INFO END ------

//double check the input parameters
var money = charge_money.value; 
money = money.replace(/(^\s*)|(\s*$)/g, "");
if (0.001>=parseFloat(money)){
    //alert("The value of money must be greater than ZERO.");
	alert("金额有误，请检查。");
   return;
}
//assume the length of phone number >=8
phone_number = charge_phone_number.value;
phone_number = phone_number.replace(/\D/g, "");
if ( 8>phone_number.length) {
    //alert("There are something wrong with the phone number.");
	alert("号码有误，请检查。");
    return;
}

// Get the name of company abbr.
ChargeClientActiveX1.InputParameterComName = "mobile_gz_cecunet";

// Get the name of transaction action.
ChargeClientActiveX1.InputParameterActName = "charge"; 

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
if("00"!=error_code_ocx && "  "!=error_code_ocx){
    error_info = ChargeClientActiveX1.OutputFields(9);
    error_info = error_info.replace(/(^\s*)|(\s*$)/g, ""); 
    error_info = "Data Center:" + error_info + "\n";    
    alert(error_info);
    return;
}
//debuger;
var transaction_response_code = ChargeClientActiveX1.OutputFields(14);
if ("0000"!= transaction_response_code){
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

td = document.getElementById("print_charge_result");
td.innerText="成功";
td = document.getElementById("print_charge_phone_number");
td.innerText= backward_pkt_phone_number;
td = document.getElementById("print_charge_money")
td.innerText=backward_pkt_money;
td = document.getElementById("print_charge_la_sn");
td.innerText= backward_pkt_la_sn;
td = document.getElementById("print_charge_cecunet_sn");
td.innerText=backward_pkt_cecunet_sn;

return;
//Print charge information on an invoice paper.
//location flag.
ChargeClientActiveX1.PrepareInvoiceItem(0,0, "+", 1);

var x = 30;
var y = 10;
var y_step = 30;

ChargeClientActiveX1.PrepareInvoiceItem(x, y, backward_pkt_phone_number, backward_pkt_phone_number.length);
ChargeClientActiveX1.PrepareInvoiceItem(x, y+y_step*1, backward_pkt_money, backward_pkt_money.length);
ChargeClientActiveX1.PrepareInvoiceItem(x, y+y_step*2, backward_pkt_la_sn, backward_pkt_la_sn.length);
ChargeClientActiveX1.PrepareInvoiceItem(x, y+y_step*3, backward_pkt_cecunet, backward_pkt_cecunet.length);

//time stamp
var time_stamp = ChargeClientActiveX1.OutputFields(15);
time_stamp = time_stamp.replace(/(^\s*)|(\s*$)/g, "");
ChargeClientActiveX1.PrepareInvoiceItem(x, y+y_step*4, time_stamp, time_stamp.length);

//terminal_id and worker_id
ChargeClientActiveX1.PrepareInvoiceItem(x, y+y_step*5, ChargeClientActiveX1.OutputFields(3), 8);
ChargeClientActiveX1.PrepareInvoiceItem(x, y+y_step*6, ChargeClientActiveX1.OutputFields(3), 4);

//send print command to LPT port
ChargeClientActiveX1.PrintInvoice();

}//end of function 'PassChargeParameters()'

//section
function SegmentPhoneNumber(){
//debugger;
var number = charge_phone_number.value;
var num_length = number.length;
var formated_phone_number = "00";
if(num_length==11){
	var obj_reg_exp = /(\d{3})(\d{4})(\d*)/;
	formated_phone_number = number.replace(obj_reg_exp, "$1-$2-$3");
    charge_phone_number.value = formated_phone_number;	
}

}

function Clear_ChargeResults() {
    $('#print_charge_result').empty(); 
    $('#print_charge_phone_number').empty(); 
    $('#print_charge_money').empty(); 
    //$('#print_charge_la_sn').empty(); 
    $('#print_charge_cecunet_sn').empty(); 
}
