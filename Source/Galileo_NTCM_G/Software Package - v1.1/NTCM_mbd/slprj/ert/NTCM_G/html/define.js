function CodeDefine() { 
this.def = new Array();
this.def["Compute_AzPar_Eq_2"] = {file: "NTCM_G_c.html",line:54,type:"fcn"};
this.def["Compute_Eq_29_30_31"] = {file: "NTCM_G_c.html",line:95,type:"fcn"};
this.def["Compute_Eq_5_6_7_9_10_13_14"] = {file: "NTCM_G_c.html",line:149,type:"fcn"};
this.def["Compute_F_factors"] = {file: "NTCM_G_c.html",line:226,type:"fcn"};
this.def["Sun_declination_Eq_28"] = {file: "NTCM_G_c.html",line:262,type:"fcn"};
this.def["NTCM_G"] = {file: "NTCM_G_c.html",line:288,type:"fcn"};
this.def["int8_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:61,type:"type"};
this.def["uint8_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:62,type:"type"};
this.def["int16_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:63,type:"type"};
this.def["uint16_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:64,type:"type"};
this.def["int32_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:65,type:"type"};
this.def["uint32_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:66,type:"type"};
this.def["real32_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:67,type:"type"};
this.def["real64_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:68,type:"type"};
this.def["real_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:74,type:"type"};
this.def["time_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:75,type:"type"};
this.def["boolean_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:76,type:"type"};
this.def["int_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:77,type:"type"};
this.def["uint_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:78,type:"type"};
this.def["ulong_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:79,type:"type"};
this.def["ulonglong_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:80,type:"type"};
this.def["char_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:81,type:"type"};
this.def["uchar_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:82,type:"type"};
this.def["byte_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:83,type:"type"};
this.def["int64_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:84,type:"type"};
this.def["uint64_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:85,type:"type"};
this.def["int8_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:95,type:"type"};
this.def["uint8_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:96,type:"type"};
this.def["int16_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:97,type:"type"};
this.def["uint16_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:98,type:"type"};
this.def["int32_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:99,type:"type"};
this.def["uint32_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:100,type:"type"};
this.def["int64_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:101,type:"type"};
this.def["uint64_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:102,type:"type"};
this.def["real32_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:103,type:"type"};
this.def["real64_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:104,type:"type"};
this.def["real_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:110,type:"type"};
this.def["time_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:111,type:"type"};
this.def["boolean_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:112,type:"type"};
this.def["int_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:113,type:"type"};
this.def["uint_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:114,type:"type"};
this.def["ulong_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:115,type:"type"};
this.def["ulonglong_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:116,type:"type"};
this.def["char_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:117,type:"type"};
this.def["uchar_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:118,type:"type"};
this.def["byte_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:119,type:"type"};
this.def["pointer_T"] = {file: "../../_sharedutils/html/rtwtypes_h.html",line:142,type:"type"};
}
CodeDefine.instance = new CodeDefine();
var testHarnessInfo = {OwnerFileName: "", HarnessOwner: "", HarnessName: "", IsTestHarness: "0"};
var relPathToBuildDir = "../ert_main.c";
var fileSep = "\\";
var isPC = true;
function Html2SrcLink() {
	this.html2SrcPath = new Array;
	this.html2Root = new Array;
	this.html2SrcPath["NTCM_G_c.html"] = "../NTCM_G.c";
	this.html2Root["NTCM_G_c.html"] = "NTCM_G_c.html";
	this.html2SrcPath["NTCM_G_h.html"] = "../NTCM_G.h";
	this.html2Root["NTCM_G_h.html"] = "NTCM_G_h.html";
	this.html2SrcPath["NTCM_G_private_h.html"] = "../NTCM_G_private.h";
	this.html2Root["NTCM_G_private_h.html"] = "NTCM_G_private_h.html";
	this.html2SrcPath["NTCM_G_types_h.html"] = "../NTCM_G_types.h";
	this.html2Root["NTCM_G_types_h.html"] = "NTCM_G_types_h.html";
	this.html2SrcPath["rtwtypes_h.html"] = "../rtwtypes.h";
	this.html2Root["rtwtypes_h.html"] = "../../_sharedutils/html/rtwtypes_h.html";
	this.getLink2Src = function (htmlFileName) {
		 if (this.html2SrcPath[htmlFileName])
			 return this.html2SrcPath[htmlFileName];
		 else
			 return null;
	}
	this.getLinkFromRoot = function (htmlFileName) {
		 if (this.html2Root[htmlFileName])
			 return this.html2Root[htmlFileName];
		 else
			 return null;
	}
}
Html2SrcLink.instance = new Html2SrcLink();
var fileList = [
"NTCM_G_c.html","NTCM_G_h.html","NTCM_G_private_h.html","NTCM_G_types_h.html","rtwtypes_h.html"];
