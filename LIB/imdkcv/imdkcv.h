// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  imdkcv.h (C++ Header File)                           HhHhH
// HhHhH                                                       HhHhH
// HhHhH        Copyright (C)  1999-2000  Yosuke IMADA         HhHhH
// HhHhH                                                       HhHhH
// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// This program is free software; you can redistribute it and/or
// modify it under the terms of the LICENSE IMADA-2 TYPE
// as published by Yosuke IMADA.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// LICENSE IMADA-2 TYPE for more details.

#ifndef _IMDKCV_H
#define _IMDKCV_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imdkcvlib.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

extern char	szBuf[512];

// #define _WIN32 1
// Visual C++'s compiler predefine "_WIN32"
#ifdef _WIN32
  #define DIRSEP '\\'
#else
  #define DIRSEP '/'
#endif
class Imd_StringList{
  public:
	Imd_StringList		*next;
	char			*s;

	int SetString(char *vs);

	void ListCat(Imd_StringList **pphead);

	void mkext(Imd_StringList *pisl,Imd_StringList *pext);

		 Imd_StringList(void){s = NULL; next = NULL;}
		 Imd_StringList(char *vs){
			s = NULL; next = NULL;
			SetString(vs);
		 }
		 Imd_StringList(Imd_StringList **pphead,char *vs = NULL){
			s = NULL; next = NULL;
			SetString(vs); ListCat(pphead);
		 }
		~Imd_StringList(){
			if(s    != NULL){delete    s; s    = NULL;}
			if(next != NULL){delete next; next = NULL;}
		 }
};

// -A        : 入力コード自動判別
// -R		 : 入力コード報告のみ
// -J   -j   : JISコード    読書
// -Sn  -sn  : SJIS (win系) 読書
// -Sa  -sa  : SJIS (mac系) 読書
// -E   -e   : EUC-jp       読書
// -U4  -u4  : UCS-4        読書
// -U7  -u7  : UTF-7        読書
// -U8  -u8  : UTF-8        読書
// -U16 -u16 : UTF-16       読書

//             ( 以下３つを指定した場合 NLandLF filter を有効化 )
//      -d   : 改行コードを 0x0D      で統一
//      -a   : 改行コードを      0x0A で統一
//      -w   : 改行コードを 0x0D-0x0A で統一

// -hkf      : Imd_JISX0201KANAtoJISX0208 有効化
// -hka      : Imd_JISX0201KANAtoJISX0208 でアップル拡張文字有りに
// -hk2f     : Imd_JISX0201KANA_filter 有効化
// -necf     : Imd_SJISN_filter を有効化
// -applef   : Imd_SJISA_filter を有効化
// -208gf    : Imd_JISX0208Gaiji_filter を有効化
// -212f     : Imd_JISX0212_filter を有効化
// -208pf    : Imd_X0208andASCII_passfilter を有効化
// -8859a    : Imd_ISO-8859_filter を有効化
// -8859b    : Imd_ISO-8859_filter2 を有効化

// -appleji  : Imd_JIStoUCS で JISX0208外字領域を Apple 拡張文字
//             として扱う ( 指定しない場合 Windows系機種依存文字 )
// -applejo  : Imd_UCStoJIS で JISX0208外字領域へのマッピングを
//             Apple 拡張文字 に使う ( 指定しない場合 Windows系
//             機種依存文字に使う )
// -212p     : Imd_UCStoJIS で機種依存文字の変換を JISX0212 優先に

// -ltendi   : UCS-4, UTF-16 入力で強制的にリトルエンディアンに
// -bgendi   : UCS-4, UTF-16 入力で強制的にビッグエンディアンに
// -ltendo   : UCS-4, UTF-16 出力でリトルエンディアンに
// -bgendo   : UCS-4, UTF-16 出力でビッグエンディアンに

// -ov       : オーバーライト
// -o  [fn]  : [fn] に出力
// -xo [ext] : 出力ファイルの拡張子を [ext] に
// -xi [ext] : 入力ファイルの拡張子を [ext] に

class ImdKcvArgs{
  protected:
	int	inm;
	  // 0:入力待ち無し
	  // 2-4: outmode の入力待ち

  public:
	int	incode,outcode;
	  // 0:指定無(デフォルト)  1:自動認識 2:自動・報告
	  // 11:JIS  12:EUC-jp
	  // 21:SJIS-N  22:SJIS-A
	  // 31:UCS-4  32:UTF-7  33:UTF-8  34:UTF-16
	int	nl;
	  // 0:変換無(デフォルト)
	  // 1:     0x0A
	  // 2:0x0D
	  // 3:0x0D-0x0A
	Imd_StringList	*phfn;
	  // NULL:      標準入力から入力(デフォルト)
	  // otherwise: 入力ファイル名のリスト
	int	outmode;
	  // 0:標準出力へ出力(デフォルト)
	  // 1:各ファイルをオーバーライト
	  // 2:outstで指定されたファイルに出力
	  // 3:outstで指定された拡張子を出力ファイルに付加
	  // 4:outstで指定された拡張子を入力ファイルに付加
	Imd_StringList	outst;
	int	filter;
	  // 各フィルタを使うかどうかなど
	  // 0x000001 : Imd_JISX0201KANAtoJISX0208 有効化
	  // 0x000002 : Imd_JISX0201KANAtoJISX0208::bApple = 1
	  // 0x000004 : Imd_JISX0201KANA_filter 有効化
	  // 0x000008 : Imd_SJISN_filter を有効化
	  // 0x000010 : Imd_SJISA_filter を有効化
	  // 0x000020 : Imd_JISX0208Gaiji_filter を有効化
	  // 0x000040 : Imd_JISX0212_filter を有効化
	  // 0x000080 : Imd_X0208andASCII_passfilter を有効化
	  // 0x000100 : Imd_JIStoUCS::f208mode = 0x0002
	  // 0x000200 : Imd_UCStoJIS::f208mode = 0x2***
	  // 0x000400 : Imd_UCStoJIS::f208mode = 0x**22
	  // 0x000800 : ImdKcvDecoder::endian = 2 (Big)
	  // 0x001000 : ImdKcvDecoder::endian = 3 (Little)
	  // 0x002000 : ImdKcvEncoder::endian = 0 (Big)
	  // 0x004000 : ImdKcvEncoder::endian = 1 (Little)
	  // 0x008000 : Imd_ISO8859_filter を有効化
	  // 0x010000 : Imd_ISO8859_filter2 を有効化

	char* add(char *s,int *pi);
	  // 引数を入力
	  // 出力はエラーコード(ワーニングは無し)

	char* endadd(int *pi);
	  // 全引数入力終了後実行
	  // 出力はエラーコード(ワーニングは無し)

		 ImdKcvArgs(void){
			incode = outcode = outmode = nl = 0;
			phfn = NULL; filter = 0; inm = 0;
		 }
};

// Error strings and usage. 
extern char RB[],WB[],FS[],cannotopen[],cannottmp[],pressany[],
	dupincode[],dupoutcode[],dupnlcode[],exhk[],exout[],
	dupout[],outexc[],waitst[],nooutcode[],multiinerr[],
	sznecf[],szapplef[],sz208gf[];

extern char sz212f[],sz208pf[],sz212p[],szappleji[],szapplejo[],
	dupiendian[],dupoendian[],szltendi[],szbgendi[],
	szltendo[],szbgendo[],ex8859[],sz8859a[],sz8859b[];

extern char usage0[],usage1[],usage2[],usage3[],
	usage4[],usage5[];

// end of "ifndef _IMDKCV_H"
#endif
