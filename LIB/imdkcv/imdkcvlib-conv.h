// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  imdkcvlib-conv.h (C++ Header File)                   HhHhH
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


// ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
// ◆◆◆  ImdConvModule 派生型            ◆◆◆
// ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆

// ----------------------------------------------------------
// ここで宣言されているクラス
// ----------------------------------------------------------
// Imd_SJISNtoUCS		SJIS(Win系) -> UCS
// Imd_UCStoSJISN  		UCS         -> SJIS(Win系)
// Imd_SJISAtoUCS  		SJIS(Mac系) -> UCS
// Imd_UCStoSJISA  		UCS         -> SJIS(Mac系)
// Imd_SJISN_filter		Win系機種依存文字除去(SJIS用)
// Imd_SJISA_filter		Mac系機種依存文字除去(SJIS用)
//
// Imd_UCStoJIS			UCS         -> JIS
// Imd_JIStoUCS			JIS         -> UCS
// Imd_JISX0208Gaiji_filter	JISX0208外字除去 (JIS用)
// Imd_JISX0212_filter		JISX0212除去 (JIS用)
// Imd_ISO8859_filter		ISO-8859除去 (ISO-2022-JP   向)
// Imd_ISO8859_filter2		ISO-8859除去 (ISO-2022-JP-2 向)
//
// Imd_UCStoEUC			UCS         -> EUC
// Imd_EUCtoUCS			EUC         -> UCS
//
// Imd_X0208andASCII_passfilter	JISX0208+ASCIIのみ通す(UCS用)
// Imd_Replacement		無効文字を "・" 等に変換
// Imd_Remove			無効文字を消滅させる
// Imd_NLandLF_converter	改行変換
// Imd_JISX0201KANAtoJISX0208	半角カナを全角化(UCS用)
// Imd_JISX0201KANA_filter	半角カナを無効文字に(UCS用)
// ----------------------------------------------------------


#ifndef _IMDKCVLIB_CONV_H
#define _IMDKCVLIB_CONV_H 1

#ifndef _IMDKCVLIB_H
#include "imdkcvlib.h"
#endif

// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎
// ◎◎◎ SJIS 関連                        ◎◎◎
// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎

// SJISN : SJIS + NEC特殊文字 + NEC選定IBM拡張文字 +
//	   IBM選定IBM拡張文字 (= Windows で使える文字)
// SJISA : SJIS + Apple拡張文字 (= Mac で使える文字)
// 機種依存文字なしの SJIS を使いたい場合は各種フィルタを
// お使いください

class Imd_SJISNtoUCS : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // SJISN -> UCS 変換
	  // 変換できなかった文字は codetype = -2 になる
};

class Imd_UCStoSJISN : public ImdConvModule{
  public:
	int flags;
	  // 0x***0 包摂リカバリしない
	  // 0x***1 包摂リカバリする
	  // 入力が Windows 系機種依存文字を含む JIS,SJIS,EUC
	  // でない場合は必ず 0 にして下さい。
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // UCS -> SJISN 変換
	  // 変換できなかった文字は codetype = -2 になる
		 Imd_UCStoSJISN(void){flags = 0;}
};

class Imd_SJISAtoUCS : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // SJISA -> UCS 変換
	  // 変換できなかった文字は codetype = -2 になる
};

class Imd_UCStoSJISA : public ImdConvModule{
  public:
	int	flags;
	  // 0x***0 包摂リカバリしない
	  // 0x***1 包摂リカバリする
	  // 入力が Apple拡張文字を含む JIS,SJIS,EUC でない場合
	  // は必ず 0 にして下さい。
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // UCS -> SJISA 変換
	  // 変換できなかった文字は codetype = -2 になる
		 Imd_UCStoSJISA(void){flags = 0;}
};

// NEC特殊文字・NEC選定IBM拡張文字・IBM選定IBM拡張文字
// のどれかだったら codetype を -2(変換失敗) に変換するフィルタ
class Imd_SJISN_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// Apple拡張文字だったら codetype を -2(変換失敗) に変換する
// フィルタ
class Imd_SJISA_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};






// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎
// ◎◎◎ JIS 関連                         ◎◎◎
// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎

class Imd_UCStoJIS : public ImdConvModule{
  public:
	int	f208mode;
	  // NEC特殊文字とNEC選定IBM特殊文字、Apple拡張文字には
	  // 変換候補がJISX0208の外字領域にもJISX0212にもある文字
	  // が一部存在する。
	  // その変換をどう扱うかを f208mode 変数で指定する。
	  // デフォルト値は 0x1022
	  //
	  // 0x1***: Windows系の変換を行う
	  // 0x2***: Macintosh系の変換を行う
	  // どちらでもなければ機種依存文字の処理は無し
	  //
	  // ● Windows系
	  // ・JISX0208 13区(NEC特殊文字) の変換ポリシ
	  //  0x1**0: X0212 への変換のみ
	  //          (変換できないものは codetype = -2 にする)
	  //  0x1**1: X0212 優先 ( X0212,13区どちらへも変換できる
	  //          ものは X0212 に変換し、13区だけに変換できる
	  //          ものは 13区に変換
	  //  0x1**2: 13区への変換のみ
	  //   ( 13区優先 == 13区への変換のみ )
	  //   どれにも当てはまらない場合は 2 とみなす
	  //
	  // ・JISX0212 89-92区(NEC選定IBM拡張文字) の変換ポリシ
	  //  0x1*0*: X0212 への変換のみ
	  //  0x1*1*: X0212 優先
	  //  0x1*2*: 89-92区への変換のみ
	  //   ( 89-92区優先 == 89-92区への変換のみ )
	  //   どれにも当てはまらない場合は 1 とみなす
	  //
	  // ・UCS に変換される際に包摂された文字のリカバリを行うか
	  //  0x10**: リカバリは行わない(デフォルト)
	  //  0x11**: リカバリを行う
	  //   その他の場合は f208mode & 0x100 の結果による
	  //
	  // ●Macintosh系
	  // (Apple拡張文字) の変換ポリシ
	  //  0x2**0: X0212 への変換のみ
	  //  0x2**1: X0212 優先
	  //  0x2**2: JISX0208外字領域への変換のみ
	  //   ( JISX0208外字領域優先 == JISX0208外字領域への変換のみ )
	  //   どれにも当てはまらない場合は 2 とみなす
	  // 
	  // ・UCS に変換される際に包摂された文字のリカバリを行うか
	  //  0x20**: リカバリは行わない
	  //  0x21**: リカバリを行う
	  //   どれにも当てはならない場合は 1 とみなす

	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // UCS -> JIS 変換
	  // 変換できなかった文字は codetype = -2 になる

		 Imd_UCStoJIS(void){f208mode = 0x1022;}
};

class Imd_JIStoUCS : public ImdConvModule{
  public:
	int	f208mode;
	  // 1: JISX0208外字領域を Windows系 のものとして判断
	  // 2: 同 Macintosh系 のものとして判断
	  // その他は全部無効文字に変換
	  // 初期値 1

	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // JIS -> UCS 変換
	  // 変換できなかった文字は codetype = -2 になる

		 Imd_JIStoUCS(void){f208mode = 1;}
};

// JISX0208 の外字だったら codetype = -2 にするフィルタ
class Imd_JISX0208Gaiji_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// JISX0212 だったら codetype = -2 にするフィルタ
class Imd_JISX0212_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// ISO-8859-XX を除去
class Imd_ISO8859_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// ISO-8859-XX を除去 (1,7を除く)
class Imd_ISO8859_filter2 : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};





// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎
// ◎◎◎ EUC 関連                         ◎◎◎
// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎

class Imd_EUCtoJIS : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // EUC ->JIS 変換
	  // 変換できなかった文字は codetype = -2 になる
};

class Imd_JIStoEUC : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // JIS -> EUC 変換
	  // 変換できなかった文字は codetype = -2 になる
};






// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎
// ◎◎◎ 各種特殊フィルタ                 ◎◎◎
// ◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎

// ・Windows で表示できる文字だけ通す (その他は codetype = -2 に)
//   フィルタ -> Imd_UCStoSJISN か Imd_SJISNtoUCS で可能
//
// ・Mac で表示できる文字だけ通す (その他は codetype = -2 に)
//   フィルタ -> Imd_UCStoSJISA か Imd_SJISAtoUCS で可能
//
// ・一般的な UNIX で表示できる文字(JISX0208 + ASCII) だけ
//   通す  (その他は codetype = -2 に) フィルタ
//   -> Imd_UCStoJIS {fNecMode = 0} + Imd_JISX0212_filter    or
//      Imd_UCStoSJISN + Imd_SJISN_filter    or
//      Imd_UCStoSJISA + Imd_SJISA_filter    or
//      Imd_X0208andASCII_passfilter
//  (最終的に EUC,JIS,SJISに変換する場合は
//   Imd_X0208andASCII_passfileter より、上記のセットを使う方が
//   効率が良いです)

// 
class Imd_X0208andASCII_passfilter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};


// 変換に失敗した文字を別の文字に変換するフィルタ
// ( 例えば "・" に変換する )
// インスタンス生成後にメンバ変数 kcRep をいじれば良い
// ここでは UCS の "・" を初期値として定義している。
// kcRep.kcode のコードの値が正しければ入力コード
// ( UCS, SJIS, JIS, EUC ,...) は、どれでもかまわない
class Imd_Replacement : public ImdConvModule{
  public:
	ImdKcvCode	kcRep,kcRep0;
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc){
		if(-1000000 <= pkc->codetype && pkc->codetype <= -2){
			if(pkc->org_kcode < 0x100){
				pkc->kcode = kcRep0.kcode;
				pkc->codetype = kcRep0.codetype;
				  // org_kcode は破壊しない
			}
			else{
				pkc->kcode = kcRep.kcode;
				pkc->codetype = kcRep.codetype;
				  // org_kcode は破壊しない
			}
		}
		return pkc;
	}

		 Imd_Replacement(void){
		kcRep.kcode = 0x30FB; kcRep.codetype = 4;
		kcRep0.kcode = 0x20; kcRep0.codetype = 1;
		// "・" UCS:0x30FB  SJIS:0x8145 JIS:0x2126
	}
};

class Imd_Remove : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc){
		if(pkc->codetype == -2){delete pkc;pkc=NULL;}
		return pkc;
	}
};


// 半角カナ(JISX0201カナ) フィルタ
// 半角カナなら codetype = -2 に変換する
// 入力・出力とも UCS です。
class Imd_JISX0201KANA_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// 半角カナ(JISX0201カナ) -> 全角カナ 変換モジュール
// (Apple Macintosh 拡張, カタカナ／ひらがな出力 両対応)
// 入力・出力とも UCS です。
class Imd_JISX0201KANAtoJISX0208 : public ImdConvModule{
  protected:
	int	isv;
  public:
	int MaxOutputSize(void){return 2;}
	ImdKcvCode* convert(ImdKcvCode *pkc);

	int	bApple,bHiragana;
	  // bApple:  「"う"に濁点」「"ワ"に濁点」「"ヲ"に濁点」
	  //	    を有効にする場合 (SJIS-mac,UCS 出力向け)
	  // bHiragana:  カタカナでなくひらがなに変換

		 Imd_JISX0201KANAtoJISX0208(void){isv=bApple=bHiragana=0;}
};

// 改行コード変換モジュール
class Imd_NLandLF_converter : public ImdConvModule{
  protected:
	int		isv;
	ImdKcvCode*	_onls(int imode);
  public:
	int MaxOutputSize(void){return 2;}
	ImdKcvCode* convert(ImdKcvCode *pkc);

	int	mode;
	  // 出力 改行を全て消去 : mode = 0
	  //           0x0A      : mode = 1
	  //      0x0D           : mode = 2
	  //      0x0D 0x0A      : mode = 3
	  // 変換したくない場合はこのモジュールを使わなければ良い
	  // 入力が 0x0D 0x0A の場合、１つの改行とみなす

		 Imd_NLandLF_converter(void){isv=0;mode=1;}
};

#endif
