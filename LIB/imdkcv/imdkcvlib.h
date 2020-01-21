// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  imdkcvlib.h (C++ Header File)                        HhHhH
// HhHhH                                                       HhHhH
// HhHhH        Copyright (C)  1999-2000  Yosuke IMADA         HhHhH
// HhHhH                                                       HhHhH
// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// This library is free software; you can redistribute it and/or
// modify it under the terms of the LIBRARY LICENSE IMADA-1 TYPE
// as published by Yosuke IMADA.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// LIBRARY LICENSE IMADA-1 TYPE for more details.

// ----------------------------------------------------------
// ここで宣言されているクラス
// ----------------------------------------------------------
// ImdKcvCode			imdkcvlib 内部の文字コード
//				(一文字分・リスト形式)
//
// ImdKcvDecoder		デコーダ抽象クラス
// ImdKcvEncoder		エンコーダ抽象クラス
// ImdConvModule		コンバータ抽象クラス
// 
// ImdConvModuleList		コンバータ用リスト
// ImdKanjiCodeConverter	コードコンバータ全体
//				デコーダ、エンコーダ、コンバータを
//				管理する
//
// Imd_AutoInputCodeDetecter	コードの種類(SJISなのか～など)を
//				自動検出する関数
//
// (その他内部向け補助関数の宣言)
// ----------------------------------------------------------

// デコーダ・エンコーダの派生クラスは imdkcvlib-code.h
// コンバータの派生クラスは imdkcvlib-conv.h
// でそれぞれ宣言されている
// ( このファイルの途中でこれらのファイルを読みこんでいる )


#ifndef _IMDKCVLIB_H
#define _IMDKCVLIB_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef NULL
#define NULL 0
#endif

class ImdKcvCode{
  public:
	ImdKcvCode	*next;		// リスト用
	int		kcode;		// 入力コードの値
	int		org_kcode;	// もともとの値
	int		housetsu;	// 包摂情報
	int		codetype;
	  // codetype の値
	  // -2:変換に失敗した文字
	  // -1:EOF(この場合 kcode は無効)
	  // 0:Unknown 1:US-ASCII 2:JISX0201-Latin 3:JISX0201-Kana
	  // 4:JISX0208 5:NEC特殊文字 6:NEC選定IBM拡張文字
	  // 7:IBM選定IBM拡張文字 8:Apple拡張文字
	  // 9:JISX0212
	  // 885901-885999: ISO-8859-XX (現在 1 - 10 まで使用可)
	  // 10 ～1000000 : システムリザーブ
	  // -4 ～ -1000000 : システムリザーブ
	  // -3 : Mac トンネル ( UCS に変換できない機種依存文字 )
	  //      kcode の値は SJIS 値

	void	cat(ImdKcvCode **pphead);
	  // *pphead の後ろに自分を接続

	ImdKcvCode* copy(void);
	  // 自分と自分の後ろをコピー

	void	kill_all(void)
		{if(next != NULL){delete next; next = NULL;}}
	  // next 以降を全て delete

		 ImdKcvCode(void);
		 ImdKcvCode(int vkcode,int vorg_kcode,
				int vcodetype);
		 ImdKcvCode(ImdKcvCode **pphead,int vkcode,int vorg_kcode,
					int vcodetype);
		~ImdKcvCode(void)
			{kcode=org_kcode=codetype=housetsu=0;kill_all();}
};

// JISのエスケープシーケンスの処理やUTF-7,8,16のUCS4へのデコード
// などを行い、一文字一文字切り出すためのモジュールの基本抽象クラス
// ２バイト文字の場合 decodeを２度呼び出して初めて一文字得る
// このとき１バイト目の処理結果は NULL ポインタ
class ImdKcvDecoder{
  public:
	virtual int MaxOutputSize(void) = 0;
	  // ImdConvModule の同名の関数と意味は同じ
	virtual ImdKcvCode *decode(int i) = 0;
};

// JISにエスケープシーケンスを付加したり、UCS4 の UTF-? へのエン
// コード等を行う。
class ImdKcvEncoder{
  public:
	virtual int MaxOutputSize(void) = 0;
	  // ImdConvModule の同名の関数と意味は同じ
	virtual int encode(ImdKcvCode *pkc,char *s,int *piOffset) = 0;
};

// 変換モジュールの抽象クラス
class ImdConvModule{
  public:
	virtual int MaxOutputSize(void) = 0;
	  // 最大出力文字
	  // 例えば"♪"を入力として受け取ったときに
	  // "お" と "ん" と "ぷ" を出力する場合は return 3;
	virtual ImdKcvCode* convert(ImdKcvCode *pkc) = 0;
	  // 変換モジュール
};

class ImdConvModuleList{
  protected:
	ImdConvModuleList	*next;
	ImdConvModule		*pcm;
  public:
	ImdKcvCode*	execute(ImdKcvCode *pkcvc);

	int	concat(ImdConvModuleList **pphead);
        int	MaxOutputSize(void);
	void	kill_all(void);

		 ImdConvModuleList(void)
			{next = NULL;pcm = NULL;}
		 ImdConvModuleList(ImdConvModule *vpcm)
			{next = NULL; pcm=vpcm;}
		 ImdConvModuleList(ImdConvModuleList **pphead,
			ImdConvModule *vpcm){pcm=vpcm;concat(pphead);}
		~ImdConvModuleList(){kill_all();}
};

class ImdKanjiCodeConverter{
  protected:
	ImdKcvDecoder		*pkd;
	ImdConvModuleList	*phead;
	ImdKcvEncoder		*pke;

  public:
	ImdKcvDecoder* SetDecoder(ImdKcvDecoder *vpkd);
	ImdKcvEncoder* SetEncoder(ImdKcvEncoder *vpke);
	int ConcatinateModule(ImdConvModule *pcm);
	void DeleteModules(void);

	int	execute(int i,char *s,int *piOffset);
	  // s に書きこんだ文字数を返します
	int	MaxOutputSize(void);
	  // execute の s に最低限必要な空バイト数 を返す

		 ImdKanjiCodeConverter(void);
		~ImdKanjiCodeConverter();
};




// デコーダ・エンコーダの派生クラス

#ifndef _IMDKCVLIB_CODE_H
#include "imdkcvlib-code.h"
#endif



// コンバータの派生クラス

#ifndef _IMDKCVLIB_CONV_H
#include "imdkcvlib-conv.h"
#endif




class Imd_AutoInputCodeDetecter{
  protected:
	ImdKcvDecoder_UCS	ducs4b,ducs4l,dutf7,dutf8,
				dutf16b,dutf16l;
	ImdKcvDecoder_JIS	djis;
	ImdKcvDecoder_EUC	deuc;
	ImdKcvDecoder_SJIS	dsjis;

	Imd_JIStoUCS		jn_u,ja_u,en_u,ea_u;
	Imd_SJISNtoUCS		sn_u;
	Imd_SJISAtoUCS		sa_u;

  public:
	// 各コードとして読みこんだときの
	// 無効文字でない有効な文字の数と文字総数
	int	 ucs4b_v, ucs4b_all, ucs4b_k,  ucs4l_v, ucs4l_all, ucs4l_k;
	int	  utf7_v,  utf7_all,  utf7_k,   utf8_v,  utf8_all,  utf8_k;
	int	utf16b_v,utf16b_all,utf16b_k, utf16l_v,utf16l_all,utf16l_k;
	int	  jisn_v,  jisn_all,  jisn_k,   jisa_v,  jisa_all,  jisa_k;
	int	  eucn_v,  eucn_all,  eucn_k,   euca_v,  euca_all,  euca_k;
	int	 sjisn_v, sjisn_all, sjisn_k,  sjisa_v, sjisa_all, sjisa_k;

	void read(int i);
	int guess(void);
	  // 予想されるコードのタイプを返す

		 Imd_AutoInputCodeDetecter(void);
};
// コードのタイプ
//  1: SJISN (SJIS + Windows系機種依存文字)
//  2: SJISA (SJIS + Macintosh系機種依存文字)
//  3: JISN  (JIS  + Windows系機種依存文字(JISX0208外字領域))
//  4: JISA  (JIS  + Macintosh系機種依存文字(JISX0208外字領域))
//  5: EUCN  (EUC  + Windows系機種依存文字(JISX0208外字領域))
//  6: EUCA  (EUC  + Macintosh系機種依存文字(JISX0208外字領域))
//  7: UCS-4 Big-endian
//  8: UCS-4 Little-endian
//  9: UTF-7
// 10: UTF-8
// 11: UTF-16 Big-endian
// 12: UTF-16 Little-endian
//
// (3-6は -appleji オプションを付けるか付けないかに対応)







//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  補助関数宣言                                    ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


//  ●●● imdkcvlib-misc.cxx で定義されている主な関数      ●●●
// (非UCS) <-> UCS US-ASCII,JISX0201-{ラテン,カナ}
int fImd_JISX0201toUCS(ImdKcvCode *pkc);
int fImd_UCStoJISX0201(ImdKcvCode *pkc);
int fImd_UCSto8859(ImdKcvCode *pkc);
int fImd_8859toUCS(ImdKcvCode *pkc);

//  ●●● imdkcvlib-jis2.cxx で定義されている主な関数      ●●●
// JIS <-> UCS 変換
int fImd_UCStoJISX0208(int iUcs,int *piJis208);
int fImd_JISX0208toUCS(int iJis208,int *piUcs);
int fImd_UCStoJISX0212(int iUcs,int *piJis212);
int fImd_JISX0212toUCS(int iJis212,int *piUcs);

// NEC特殊文字・{NEC|IBM}選定IBM拡張文字 用関数 (JIS版)
// JISX0208外字領域(13,89-91区) <-> UCS のマッピング
int fImd_JIStoUCS_NEC(ImdKcvCode *pkc);
int fImd_UCStoJIS_NEC(ImdKcvCode *pkc);
int fImd_JIStoUCS_NECIBM(ImdKcvCode *pkc);
int fImd_UCStoJIS_NECIBM(ImdKcvCode *pkc);

// Apple拡張文字用関数 (JIS版)
int fImd_JIStoUCS_APPLE(ImdKcvCode *pkc);
int fImd_UCStoJIS_APPLE(ImdKcvCode *pkc);


//  ●●● imdkcvlib-sjis.cxx で定義されている主な関数      ●●●
// JISX0208 範囲(機種依存文字を除く)にあるかどうか
int fImd_IsJISX0208(int jis);   //  JISコード版
int fImd_IsSJIS(int sjis);      // SJISコード版

// JISX0208 範囲内に限った JIS<->SJIS 変換
// 戻り値 0:JISX0208範囲外  1:変換成功
int fImd_JIStoSJIS(int iJis, int *piSjis);
int fImd_SJIStoJIS(int iSjis, int *piJis);
// 外字も含めたバージョン -1 が戻り値なら範囲外
int fImd_JIStoSJIS_X(int iJis);
int fImd_SJIStoJIS_X(int iSjis);

// NEC特殊文字・{NEC|IBM}選定IBM拡張文字 用関数 (SJIS版)
int fImd_SJIStoUCS_NEC(ImdKcvCode *pkc);
int fImd_UCStoSJIS_NEC(ImdKcvCode *pkc);
int fImd_SJIStoUCS_NECIBM(ImdKcvCode *pkc);
int fImd_UCStoSJIS_NECIBM(ImdKcvCode *pkc);
int fImd_SJIStoUCS_IBMIBM(ImdKcvCode *pkc);
int fImd_UCStoSJIS_IBMIBM(ImdKcvCode *pkc);
int fImd_SJISrecover_NEC(int i);

// Apple拡張文字用関数 (SJIS版)
int fImd_SJIStoUCS_APPLE(ImdKcvCode *pkc);
int fImd_UCStoSJIS_APPLE(ImdKcvCode *pkc);
int fImd_SJISrecover_APPLE(int i);

#endif // _IMDKCVLIB_H
