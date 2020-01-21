// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcv.cxx (C++ Source File)                         CcCcC
// CcCcC                                                       CcCcC
// CcCcC        Copyright (C)  1999-2000  Yosuke IMADA         CcCcC
// CcCcC                                                       CcCcC
// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// This program is free software; you can redistribute it and/or
// modify it under the terms of the LICENSE IMADA-2 TYPE
// as published by Yosuke IMADA.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// LICENSE IMADA-2 TYPE for more details.

#include "imdkcv.h"
// Visual C++'s compiler predefine "_WIN32"
// if you want define _WIN32 manualy, define it in imdkcv.h.
#ifdef _WIN32
  #include "DxArg.h"
  #include "conio.h"
    // 引数の＊や？を展開するためのクラス(main関数先頭みよ)
    // 実はソースファイルなので複数のファイルから include
    // 出来ません (^^;
    // main 関数のあるソースファイルのみで include して下さい
#endif

// 各種グローバル変数
char	szTmpFname[1024],szBuf[512];

int	bTmpUsed;

char RB[] = "rb", WB[] = "wb", FS[] = "%s",
     cannotopen[] = "imdkcv : Cannot open/create file %s\n",
	 cannottmp[] = "imdkcv : Cannot create temporary file.\n",
	 pressany[] = "[press any key to continue...]:";

char dupincode[]="Duplicated Input Code Specification.",
     dupoutcode[]="Duplicated Output Code Specification.",
     dupnlcode[]="Duplicated NL-LF Convert Specification.",
     exhk[]="hkf and hkf2 options are exclusive.",
     exout[]="ov, o, oe and ie options are exclusive",
     dupout[]="You can specify ov, o, oe, or ie option only once.";

char outexc[]="You cannot specify -ov, -oe, or -ie options when read STDIN.",
     waitst[]="No string is specified after -o, -oe, or -ie.",
     nooutcode[]="Output code is not specified.",
     multiinerr[]="You cannot specify -o option for multiple input files.";

char sznecf[]="necf option was ignored (output code is not SJIS-N).",
     szapplef[]="applef option was ignored (output code is not SJIS-A).",
     sz208gf[]="208gf option was ignored (output code is not JIS nor EUC).";

char sz212f[]="212f option was ignored (output code is not JIS nor EUC).",
     sz208pf[]="208pf option was ignored (output code is not UCS).",
     sz212p[]="212p option was ignored (output code is not JIS nor EUC).";

char szappleji[]="appleji option was ignored (input code is not AUTO, JIS nor EUC).",
     szapplejo[]="applejo option was ignored (output code is not JIS nor EUC).",
     dupiendian[]="ltendi and bgendi options are exclusive.",
     dupoendian[]="ltendo and bgendo options are exclusive.";

char szltendi[]="ltendi was ignored (input code is not UCS).",
     szbgendi[]="bgendi was ignored (input code is not UCS).",
     szltendo[]="ltendo was ignored (output code is not UCS).",
     szbgendo[]="bgendo was ignored (output code is not UCS).";

char ex8859[]="8859a and 8859b options are exclusive.",
     sz8859a[]="8859a option was ignored (output code is not JIS nor EUC).",
     sz8859b[]="8859b option was ignored (output code is not JIS nor EUC).";

char usage0[] = "imdkcv:infomation: -help option is available.\n";

char usage1[] = "\
Imada's Japanese Kanji Code Converter Version 1.0.6\n\
    Copyright 1999-2000 Yosuke IMADA\n\
\n\
usage: imdkcv (-A | -Sn | -Sa | -U7 | -U8 |\n\
               (-J | -E)[-appleji] | (-U4 | -U16)[-ltendi | -bgendi])\n\
              [-d | -a | -w] [-hkf [-hka] | -hk2f]\n\
              (-sn [-necf] | -sa [-applef] |\n\
               (-j | -e)[-applejo][-212p][-212f][-208gf][-8859a|-8859b] |\n\
               ((-u4 | -u16)[-ltendo | -bgendo] | -u7 | -u8)[-208pf])\n\
              [-ov | -o output_file_name | -xo output_file_name's_extension |\n\
               -xi extension_for_rename_input_file]  [input_file_names...]\n\
\n\
       imdkcv -R [input_file_names...]\n\
       imdkcv -help\n\
\n\
  (...) means you have to specify one of them.\n\
  [...] means you can specify optinally.\n\n";

char usage2[] = "\
Input file's  Kanji code specifiers and options for them:\n\
  -R   : Auto detect and report what kind of code, but not output.\n\
  -A   : Auto detect what kind of code is used.\n\
  -Sn  : Shift_JIS ( + Windows dependent charcters (or WinDepChars) )\n\
  -Sa  : Shift_JIS ( + Apple   dependent charcters (or AppleDepChars) )\n\
  -J   : ISO-2022 (JIS-Code) ( US-ASCII,JISX0201,208,212,ISO-8859-1to10 )\n\
  -E   : EUC-jp ( G2 for JISX0201Kana and G3 for JISX0212 )\n\
    -appleji : Regard JISX208 Gaiji area as AppleDepChars.\n\
     ( If you do not specify this, regard as Windows dependent chars. )\n\
  -U7  : UTF-7        -U8  : UTF-8\n\
  -U4  : UCS-4        -U16 : UTF-16\n\
    -ltendi : Forcedly decode input as Little Endian.\n\
    -bgendi : Forcedly decode input as Big Endian.\n\
    ( If you do not specify neither, decode as Big Endian or follow 0xFEFF )\n\n\
";

char usage3[] = "\
New Line and Line Feed converter specifiers:\n\
  -d : convert 0x0D, 0x0A and 0x0D-0x0A to 0x0D.\n\
  -a : convert to 0x0A        -w : convert to 0x0D-0x0A\n\
\n\
JISX0201Kana ( Hankaku-Kana ) converter specifiers:\n\
  -hkf  : Convert JISX0201-Kana to JISX0208 Chars.\n\
  -hka  : Convert JISX0201-Kana to JISX0208 Chars + AppleDepChars.\n\
  -hk2f : Convert JISX0201-Kana to ' ' (0x20:Space).\n\n\
";

char usage4[] = "\
Output file's Kanji code specifiers and options for them:\n\
  -sn  : Shift_JIS (+WinDepChars)        -sa : Shift_JIS (+AppleDepChars)\n\
  -j   : ISO-2022 ( Only use GL and SS2(ESC N) )\n\
  -e   : Euc-jp ( G2 for JISX0201Kana and G3 for JISX0212 )\n\
    -applejo : Use JISX0208 Gaiji area for AppleDepChars.\n\
     ( If you do not specify this, use for WinDepChars. )\n\
    -212p  : Convert JISX0208 Gaiji Chars to JISX0212 if possible.\n\
    -212f  : Remove JISX0212 Chars.\n\
    -208gf : Remove JISX0208 Gaiji Chars.\n\
    -8859a : Remove all ISO-8859 Chars.\n\
    -8859b : Remove ISO-8859 Chars except ISO-8859-1 and ISO-8859-7.\n\
  -U7  : UTF-7        -U8  : UTF-8\n\
  -U4  : UCS-4        -U16 : UCS-4\n\
    -ltendo : Output byte-order is Little Endian.\n\
    -bgendo : Output byte-order is Big Endian. ( default )\n\
    -208pf  : Remove all chars except US-ASCII, JISX0201-Latin, and JISX0208.\n\n\
";

char usage5[] = "\
Output file's specifiers:\n\
  -ov : Over write input files.\n\
  -xo ext : If input file name is \"abc.txt\" and ext is \"jis\", then output\n\
            file name will be \"abc.txt.jis\".\n\
  -xi ext : If input file name is \"abc.txt\" and ext is \"old\", then input\n\
            file name will rename to \"abc.txt.old\" and\n\
            output file name will be \"abc.txt\".\n\
  -o output_file_name : specify output file name.\n\
  If you specify none of above, STDOUT will be used for output.\n\
\n\
Input file:\n\
  input_file_names : You can specify plural input files.\n\
  If you specify no input file, STDIN will be used for input.\n\
\n\
Misc:\n\
  -help : Print short instruction document.\n\n\n\
This is free soft ware, and you are welcome to redistribute it\n\
under 'LICENSE IMADA-2 TYPE' which comes with this package.\n\n\
";

// 各種 static 変数
static char szCODEs[][64] = {
    "Shift-JIS(+NEC)","Shift-JIS(+Apple)", "JIS(+NEC)","JIS(+Apple)",
    "EUC(+NEC)","EUC(+Apple)","UCS-4 Big-endian","UCS-4 Little-endian",
    "UTF-7","UTF-8","UTF-16 Big-endian","UTF-16 Little-endian"};
static char szSTDIN[] = "(STDIN)";

// 自動検出実行部
void which_code(FILE *f, int *pincode,int *pfilter){
	Imd_AutoInputCodeDetecter	iacd;
	int				i;

	do{
		i = getc(f);
		iacd.read(i);
	}while(i != EOF);
	i = iacd.guess();

	if     (i ==  1){
		*pincode = 21;
	}else if(i ==  2){
		*pincode = 22;
	}else if(i ==  3){
		*pincode = 11;
		*pfilter = *pfilter & 0xFFFFFEFF;
	}else if(i ==  4){
		*pincode = 11; *pfilter = *pfilter | 0x0100;
	}else if(i ==  5){
		*pincode = 12; *pfilter = *pfilter & 0xFFFFFEFF;
	}else if(i ==  6){
		*pincode = 12; *pfilter = *pfilter | 0x0100;
	}else if(i ==  7){
		*pincode = 31;	*pfilter = *pfilter | 0x0800;
	}else if(i ==  8){*pincode = 31; 
		*pfilter = *pfilter | 0x1000;
	}else if(i ==  9){
		*pincode = 32;
	}else if(i == 10){
		*pincode = 33;
	}else if(i == 11){
		*pincode = 34;
		*pfilter = *pfilter | 0x0800;
	}else if(i == 12){
		*pincode = 34;
		*pfilter = *pfilter | 0x1000;
	}
}

// 下の report_code の補助関数
static void _sub_report(int i,int ia,int ik,int iv){
	double	dv,dk;

	if(ia == 0){dv = dk = 0.0;}
	else{
		dv = iv * 100.0 / ia;
		if(iv == 0){dk = 0.0;}
		else{dk = ik * 100.0 / iv;}
	}

	fprintf(stdout,"  %24s %8d/%8d/%8d = %8.4f%% %8.4f%%\n",
		szCODEs[i],ik,iv,ia,dk,dv);
}

// 自動認識で、各漢字コードとして読み込んだ場合の
// 不正でない(コード表から外れない)文字の割合と漢字の割合表示
int report_code(char *szf){
	Imd_AutoInputCodeDetecter	iacd;
	int				i;
	FILE			*f;
	char			*s;

	if(szf == NULL){f = stdin; s = szSTDIN;}
	else{
		s = szf;
		f = fopen(szf,"rb");
		if(f == NULL){return 1;}
	}

	do{
		i = getc(f);
		iacd.read(i);
	}while(i != EOF);
	i = iacd.guess();

	fprintf(stdout," %s : %s\n",s,szCODEs[i-1]);
	fprintf(stdout,
	  "                              kanji    valid      all       (k/v)     (v/a)\n");

	_sub_report( 0, iacd.sjisn_all, iacd.sjisn_k, iacd.sjisn_v);
	_sub_report( 1, iacd.sjisa_all, iacd.sjisa_k, iacd.sjisa_v);
	_sub_report( 2,  iacd.jisn_all,  iacd.jisn_k,  iacd.jisn_v);
	_sub_report( 3,  iacd.jisa_all,  iacd.jisa_k,  iacd.jisa_v);
	_sub_report( 4,  iacd.eucn_all,  iacd.eucn_k,  iacd.eucn_v);
	_sub_report( 5,  iacd.euca_all,  iacd.euca_k,  iacd.euca_v);
	_sub_report( 6, iacd.ucs4b_all, iacd.ucs4b_k, iacd.ucs4b_v);
	_sub_report( 7, iacd.ucs4l_all, iacd.ucs4l_k, iacd.ucs4l_v);
	_sub_report( 8,  iacd.utf7_all,  iacd.utf7_k,  iacd.utf7_v);
	_sub_report( 9,  iacd.utf8_all,  iacd.utf8_k,  iacd.utf8_v);
	_sub_report(10,iacd.utf16b_all,iacd.utf16b_k,iacd.utf16b_v);
	_sub_report(11,iacd.utf16l_all,iacd.utf16l_k,iacd.utf16l_v);

	if(szf == NULL){fclose(f);}
	return 0;
}

// 既にオープンされたファイルを受け取り変換実行
int exec01(FILE *f,int incode,FILE *g,int outcode,int nl,int filter){
	ImdKanjiCodeConverter	ikcc;
	ImdConvModule		*pcm;
	Imd_Remove		*prm;
	Imd_Replacement		*prp;
	int			i,j,k,outx,len;
	char			*s;

	outx = 0;

	// 全てに共通の 無効文字処理モジュール
	prm = new Imd_Remove;
	if(prm == NULL){return 1;}
	prp = new Imd_Replacement;
	if(prp == NULL){delete prm; return 1;}

	// デコーダ ～ UCSへの変換まで
	if(incode == 11 || incode == 12){
		if(incode == 11){ // JIS デコーダ
			ImdKcvDecoder_JIS *pdjis = new ImdKcvDecoder_JIS;
			if(pdjis == NULL){return 1;}
			ikcc.SetDecoder(pdjis);
			if(ikcc.ConcatinateModule(prm) != 0)
				 {delete prm; return 1;}
		}
		else{ // EUC デコーダ
			ImdKcvDecoder_EUC *pdeuc = new ImdKcvDecoder_EUC;
			if(pdeuc == NULL){return 1;}
			ikcc.SetDecoder(pdeuc);
			if(ikcc.ConcatinateModule(prm) != 0)
				 {delete prm; return 1;}
			pcm = new Imd_EUCtoJIS;
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}

		Imd_JIStoUCS *pju = new Imd_JIStoUCS;
		if(pju == NULL){return 1;}
		if((filter & 0x100)!=0){pju->f208mode=0x0002;}
		if(ikcc.ConcatinateModule(pju) != 0)
			 {delete pju; return 1;}

	}
	else if(incode == 21 || incode == 22){ // SJIS デコーダ
		ImdKcvDecoder_SJIS *pdsj = new ImdKcvDecoder_SJIS;
		if(pdsj == NULL){return 1;}
		ikcc.SetDecoder(pdsj);
		if(ikcc.ConcatinateModule(prm) != 0)
			 {delete prm; return 1;}

		if(incode == 21){pcm = new Imd_SJISNtoUCS;}
		else{pcm = new Imd_SJISAtoUCS;}
		if(pcm == NULL){return 1;}
		if(ikcc.ConcatinateModule(pcm) != 0)
			 {delete pcm; return 1;}
	}
	else if(31 <= incode && incode <= 34){ // UCS デコーダ
		ImdKcvDecoder_UCS *pducs = new ImdKcvDecoder_UCS;
		if(pducs == NULL){return 1;}
		if     ((filter & 0x0800) != 0){pducs->endian = 2;}
		else if((filter & 0x1000) != 0){pducs->endian = 3;}
		if     (incode == 31){pducs->mode = 0;}
		else if(incode == 32){pducs->mode = 7;}
		else if(incode == 33){pducs->mode = 8;}
		else{pducs->mode = 16;}
		ikcc.SetDecoder(pducs);

		if(ikcc.ConcatinateModule(prm) != 0)
			 {delete prm; return 1;}
	}

	// UCS -> UCS フィルタ(改行・半角カナ)
	if(nl != 0){
		Imd_NLandLF_converter *pnl =
			 new Imd_NLandLF_converter;
		if(pnl == NULL){return 1;}
		pnl->mode = nl;
		if(ikcc.ConcatinateModule(pnl) != 0)
			 {delete pnl; return 1;}
	}
	if((filter & 3) != 0){
		Imd_JISX0201KANAtoJISX0208 *phz = 
			 new Imd_JISX0201KANAtoJISX0208;
		if(phz == NULL){return 1;}
		if((filter & 2) != 0){phz->bApple = 1;}
		if(ikcc.ConcatinateModule(phz) != 0)
			 {delete phz; return 1;}
	}
	if((filter & 4) != 0){
		pcm = new Imd_JISX0201KANA_filter;
		if(pcm != NULL){return 1;}
		if(ikcc.ConcatinateModule(pcm) != 0)
			 {delete pcm; return 1;}
	}

	// UCSからの変換～各種フィルタ～エンコーダ
	if(outcode == 11 || outcode == 12){
		Imd_UCStoJIS *puj = new Imd_UCStoJIS;
		outx = 1;
		if(puj == NULL){return 1;}
		if((filter & 0x0200) != 0){
			puj->f208mode = puj->f208mode & 0xFFFF0FFF | 0x2000;
			if((incode == 11 || incode == 12) &&
			 ((filter & 0x000100) != 0) || incode == 22){
				puj->f208mode = puj->f208mode & 0xFFFFF0FF | 0x100;
		}	}
		else if((incode == 11 || incode == 12) &&
			 ((filter & 0x000100) == 0) || incode == 21){
			puj->f208mode = puj->f208mode & 0xFFFFF0FF | 0x100;
		}

		if((filter & 0x0400) != 0){
			puj->f208mode = puj->f208mode & 0xFFFFFF00 | 0x11;
		}

		if(ikcc.ConcatinateModule(puj) != 0)
			 {delete puj; return 1;}

		if((filter & 0x0020) != 0){
			pcm = new Imd_JISX0208Gaiji_filter;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}
		if((filter & 0x0040) != 0){
			pcm = new Imd_JISX0212_filter;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}
		if((filter & 0x08000) != 0){
			pcm = new Imd_ISO8859_filter;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}
		if((filter & 0x10000) != 0){
			pcm = new Imd_ISO8859_filter2;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}

		if(outcode == 11){ // JIS エンコーダ
			prp->kcRep.kcode = 0x2126;
			if(ikcc.ConcatinateModule(prp) != 0)
				 {delete prp; return 1;}
			ImdKcvEncoder_JIS *pejis = new ImdKcvEncoder_JIS;
			if(pejis == NULL){return 1;}
			ikcc.SetEncoder(pejis);
		}
		else{ // EUC エンコーダ
			pcm = new Imd_JIStoEUC;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}

			prp->kcRep.kcode = 0xA1A6;
			if(ikcc.ConcatinateModule(prp) != 0)
				 {delete prp; return 1;}
			ImdKcvEncoder_EUC *peeuc = new ImdKcvEncoder_EUC;
			if(peeuc == NULL){return 1;}
			ikcc.SetEncoder(peeuc);
	}	}
	else if(outcode == 21 || outcode == 22){ // SJIS エンコーダ
		outx = 1;
		if(outcode == 21){
			Imd_UCStoSJISN	*pusn = new Imd_UCStoSJISN;
			if((incode == 11 || incode == 12) && 
				(filter & 0x0100) == 0){i = 1;}
			else if(incode == 21){i = 1;}
			else{i = 0;}
			pusn->flags = i;
			pcm = pusn;
		}
		else{
			Imd_UCStoSJISA	*pusa = new Imd_UCStoSJISA;
			if((incode == 11 || incode == 12) && 
				(filter & 0x0100) != 0){i = 1;}
			else if(incode == 22){i = 1;}
			else{i = 0;}
			pusa->flags = i;
			pcm = pusa;
		}
		if(pcm == NULL){return 1;}
		if(ikcc.ConcatinateModule(pcm) != 0)
			 {delete pcm; return 1;}

		if((filter & 0x08) != 0){
			pcm = new Imd_SJISN_filter;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}
		else if((filter & 0x10) != 0){
			pcm = new Imd_SJISA_filter;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}

		prp->kcRep.kcode = 0x8145;
		if(ikcc.ConcatinateModule(prp) != 0)
			 {delete prp; return 1;}

		ImdKcvEncoder_SJIS *pesj = new ImdKcvEncoder_SJIS;
		if(pesj == NULL){return 0x1003;}
		ikcc.SetEncoder(pesj);
	}
	else if(31 <= outcode && outcode <= 34){ // UCS エンコーダ
		if((filter & 0x80) != 0){
			pcm = new Imd_X0208andASCII_passfilter;
			if(pcm == NULL){return 1;}
			if(ikcc.ConcatinateModule(pcm) != 0)
				 {delete pcm; return 1;}
		}

		prp->kcRep.kcode = 0x30FB;
		if(ikcc.ConcatinateModule(prp) != 0)
			 {delete prp; return 1;}

		ImdKcvEncoder_UCS *peucs = new ImdKcvEncoder_UCS;
		if(peucs == NULL){return outcode - 27 + 0x1000;}
		if     ((filter & 0x2000) != 0){peucs->endian = 0;}
		else if((filter & 0x4000) != 0){peucs->endian = 1;}
		if     (outcode == 31){peucs->mode = 0;}
		else if(outcode == 32){peucs->mode = 7;}
		else if(outcode == 33){peucs->mode = 8;}
		else{peucs->mode = 16;}
		ikcc.SetEncoder(peucs);
	}

	// ここでようやくコンバータ完成

	len = ikcc.MaxOutputSize() + 16;
	if(len < 128){len = 128;}
	s = new char[len];
	if(s == NULL){return 1;}

	do{
		i = getc(f); j = 0;
		len = ikcc.execute(i,s,&j);
		if(outx == 1){s[len] = '\0'; fprintf(g,"%s",s);}
		else{for(k=0;k<len;k++){putc((int)s[k],g);}}
	}while(i != EOF);

	return 0;
}

// ファイル名を元にファイルをオープンする所まで
// 0:正常 1: szf が開けない 2: szg が開けない
// 3:作業ファイルが開けない
int exec00(char *szf,int incode,char * szg,int outcode,
           int nl,int outmode,int filter){
	FILE		*f,*g;
	int		i,bStdinUsed = 0;

	if(incode == 1){
		if(szf == NULL){
			  // リワインドできない stdin のために
			  // stdin を作業ファイルにセーブする
			f = stdin;
			g = fopen(szTmpFname,"wb");
			if(g == NULL){return 3;}
			bTmpUsed = bStdinUsed = 1;
			while((i = getc(f)) != EOF){putc(i,g);}
			fclose(g); fclose(f);
			f = fopen(szTmpFname,"rb");
			if(f == NULL){return 3;}
		}
		else{
			f = fopen(szf,"rb");
			if(f == NULL){return 1;}
		}
		which_code(f,&incode,&filter);
		fclose(f);
	}
	else if(incode == 2){return report_code(szf);}

	if(outmode == 0 || outmode == 2 || outmode == 3){
		  // 一般的な形
		if(szf == NULL){
			if(bStdinUsed == 0){f = stdin;}
			else if((f = fopen(szTmpFname,"rb")) == NULL){
				return 3;
		}	}
		else if((f = fopen(szf,"rb")) == NULL){return 1;}
		if(szg == NULL){g = stdout;}
		else if((g = fopen(szg,"wb")) == NULL){
			if(f != stdin){fclose(f);} return 2;
		}
		exec01(f,incode,g,outcode,nl,filter);
		if(g != stdout){fclose(g);}
		if(f != stdin){fclose(f);}
	}
	else if(outmode == 1){
		  // オーバーライト
		if((f = fopen(szf,"rb")) == NULL){return 1;}
		if((g = fopen(szTmpFname,"wb")) == NULL){
			fclose(f); return 3;
		}
		bTmpUsed = 1;
		exec01(f,incode,g,outcode,nl,filter);
		fclose(g); fclose(f);
		if((f = fopen(szTmpFname,"rb")) == NULL){return 3;}
		if((g = fopen(szf,"wb")) == NULL){
			fclose(f); return 2;
		}
		while((i = getc(f)) != EOF){putc(i,g);}
		fclose(g); fclose(f);
	}
	else if(outmode == 4){
		  // 入力ファイルの方を書きかえる
		if((f = fopen(szf,"rb")) == NULL){return 1;}
		if((g = fopen(szg,"wb")) == NULL){
			fclose(f); return 2;
		}
		while((i = getc(f)) != EOF){putc(i,g);}
		fclose(g); fclose(f);
		if((f = fopen(szg,"rb")) == NULL){return 2;}
		if((g = fopen(szf,"wb")) == NULL){
			fclose(f); return 1;
		}
		exec01(f,incode,g,outcode,nl,filter);
		fclose(g); fclose(f);
	}

	return 0;
}


  #ifdef _WIN32
static void imdwin32_tmpnam(char *stfn){
	char	*st;
	int	len,i;

	st = getenv("TEMP");
	if(st == NULL){
		st = getenv("TMP");
	}

	if(st == NULL){len = 0;}
	else{
		len = strlen(st);
		strcpy(stfn,st);
		i = len - 1;
		if(stfn[i] == '\\'){len = i;}
	}
	tmpnam(&(stfn[len]));
}

void usage(FILE *f){
	int i;

	// Windows の場合 24 行ずつ表示
	// 25 行ずつにすると漢字変換起動中の場合に一行切れるため
	fprintf(f,FS,usage1);
	for(i=0;i<5;i++){putc('\n',f);}
	fprintf(f,pressany);
	_getch();putc('\n',f);

	fprintf(f,FS,usage2);
	for(i=0;i<8;i++){putc('\n',f);}
	fprintf(f,pressany);
	_getch();putc('\n',f);

	fprintf(f,FS,usage3);
	for(i=0;i<14;i++){putc('\n',f);}
	fprintf(f,pressany);
	_getch();putc('\n',f);

	fprintf(f,FS,usage4);
	for(i=0;i<6;i++){putc('\n',f);}
	fprintf(f,pressany);
	_getch();putc('\n',f);

	fprintf(f,FS,usage5);
	for(i=0;i<2;i++){putc('\n',f);}
}

  #else
void usage(FILE *f){
	fprintf(f,FS,usage1);
	fprintf(f,FS,usage2);
	fprintf(f,FS,usage3);
	fprintf(f,FS,usage4);
	fprintf(f,FS,usage5);
}

  #endif


int main(int dargc,char *dargv[]){
	ImdKcvArgs	ika;
	int		i,j,e;
	char		*strerr,*szf,*szg;
	Imd_StringList	*pisl,sl;

	int		argc;
	char		**argv;

  #ifdef _WIN32
	int		nc;
	char		**nv;
	ImdDxArg	mainDxArg(dargc,dargv,&argc,&argv,&nc,&nv);
	  // Windows で＊や？を展開
	for(i = 0; i < nc; i++){printf("imdkcv: no match %s\n",nv[i]);}
	  // no match を表示
  #else
	argc = dargc; argv = dargv;
  #endif

	j = 0;
	for(i=1;i<argc;i++){
		strerr = ika.add(argv[i],&e);
		if(e == 1){
			fprintf(stderr,"imdkcv:%s\n",strerr);
			j++;
		}
		else if(e == 2){
			usage(stdout); return 1;
		}
	}
	strerr = ika.endadd(&i);
	if(strerr != NULL){
		fprintf(stderr,"imdkcv:%s\n",strerr);
		if(i != 0){j++;}
	}
	if(j != 0){fprintf(stderr,FS,usage0); return 1;}

	bTmpUsed = 0;

  #ifdef _WIN32
	imdwin32_tmpnam(szTmpFname);
  #else
	tmpnam(szTmpFname);
  #endif

	if(ika.phfn == NULL){
		szf = NULL;
		if(ika.outmode == 0){szg = NULL;}
		else if(ika.outmode == 2){szg = ika.outst.s;}
		exec00(szf,ika.incode,szg,ika.outcode,
			 ika.nl,ika.outmode,ika.filter);
	}
	else{
		for(pisl = ika.phfn; pisl != NULL; pisl = pisl->next){
			szf = pisl->s;
			if     (ika.outmode == 0){szg = NULL;}
			else if(ika.outmode == 1){szg = szf;}
			else if(ika.outmode == 2){szg = ika.outst.s;}
			else if(ika.outmode == 3 || ika.outmode == 4){
				sl.mkext(pisl,&(ika.outst));
				szg = sl.s;
			}
			i = exec00(szf,ika.incode,szg,ika.outcode,
				 ika.nl,ika.outmode,ika.filter);
			if     (i==1){fprintf(stderr,cannotopen,szf);}
			else if(i==2){fprintf(stderr,cannotopen,szg);}
			else if(i==3){fprintf(stderr,cannottmp);}
		}
	}

	if(bTmpUsed != 0){
		remove(szTmpFname);
	}

	return 0;
}

