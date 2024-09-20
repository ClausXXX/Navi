function CodeMetrics() {
	 this.metricsArray = {};
	 this.metricsArray.var = new Array();
	 this.metricsArray.fcn = new Array();
	 this.metricsArray.fcn["Compute_AzPar_Eq_2"] = {file: "C:\\Users\\zurnmar\\Documents\\2_SW_Devops\\Prova+21042022\\NTCM_mbd\\slprj\\ert\\NTCM_G\\NTCM_G.c",
	stack: 8,
	stackTotal: 8};
	 this.metricsArray.fcn["Compute_Eq_29_30_31"] = {file: "C:\\Users\\zurnmar\\Documents\\2_SW_Devops\\Prova+21042022\\NTCM_mbd\\slprj\\ert\\NTCM_G\\NTCM_G.c",
	stack: 16,
	stackTotal: 16};
	 this.metricsArray.fcn["Compute_Eq_5_6_7_9_10_13_14"] = {file: "C:\\Users\\zurnmar\\Documents\\2_SW_Devops\\Prova+21042022\\NTCM_mbd\\slprj\\ert\\NTCM_G\\NTCM_G.c",
	stack: 16,
	stackTotal: 16};
	 this.metricsArray.fcn["Compute_F_factors"] = {file: "C:\\Users\\zurnmar\\Documents\\2_SW_Devops\\Prova+21042022\\NTCM_mbd\\slprj\\ert\\NTCM_G\\NTCM_G.c",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["NTCM_G"] = {file: "C:\\Users\\zurnmar\\Documents\\2_SW_Devops\\Prova+21042022\\NTCM_mbd\\slprj\\ert\\NTCM_G\\NTCM_G.c",
	stack: 152,
	stackTotal: 168};
	 this.metricsArray.fcn["Sun_declination_Eq_28"] = {file: "C:\\Users\\zurnmar\\Documents\\2_SW_Devops\\Prova+21042022\\NTCM_mbd\\slprj\\ert\\NTCM_G\\NTCM_G.c",
	stack: 8,
	stackTotal: 8};
	 this.metricsArray.fcn["asin"] = {file: "C:\\Program Files\\MATLAB\\R2021a\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["cos"] = {file: "C:\\Program Files\\MATLAB\\R2021a\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["exp"] = {file: "C:\\Program Files\\MATLAB\\R2021a\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["sin"] = {file: "C:\\Program Files\\MATLAB\\R2021a\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["sqrt"] = {file: "C:\\Program Files\\MATLAB\\R2021a\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.getMetrics = function(token) { 
		 var data;
		 data = this.metricsArray.var[token];
		 if (!data) {
			 data = this.metricsArray.fcn[token];
			 if (data) data.type = "fcn";
		 } else { 
			 data.type = "var";
		 }
	 return data; }; 
	 this.codeMetricsSummary = '<a href="NTCM_G_metrics.html">Global Memory: 0(bytes) Maximum Stack: 152(bytes)</a>';
	}
CodeMetrics.instance = new CodeMetrics();
