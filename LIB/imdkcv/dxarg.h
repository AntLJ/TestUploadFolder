// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  dxarg.h (C++ Header File)                            HhHhH
// HhHhH                       Version  3. 0. 0                HhHhH
// HhHhH                                                       HhHhH
// HhHhH        Copyright (C)  1997-2000  Yosuke IMADA         HhHhH
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

#ifndef _IMD_DXARG_H
#define _IMD_DXARG_H 1

#ifndef NULL
  // NULL が定義されていないなら stdio.h をインクルードする．
  // NULL が定義されていないとコンパイルできないからです．
  #include <stdio.h>
#endif

class ImdCargl{
  // 文字列のリストのクラス
  public:
	ImdCargl	*next;	// 次のリスト
	int		df;	// s を delete する必要があるかどうかのフラグ
	char		*s;	// 文字列

	void  kill(void);	// 削除
	int   set(char *t);	// s を動的確保して文字列をコピーして格納
				// 動的確保失敗したら 0 を返す

	      ImdCargl(void){s = NULL; next = NULL; df = 0;}
	      ImdCargl(char *t){s = NULL; next = NULL; df = 0; set(t);}
	     ~ImdCargl(){kill();}
};

class ImdCarg{
  public:
	int		size;	// 持っている Cargl の数
	ImdCargl	*head,	// ImdCargl の先頭へのアドレス
			**pp;	// リストを繋ぐときに書き換える場所

	void  kill(void);	// 全てのリストを破棄
	void  add(char *t);	// t をコピーしてリストを作成する
	void  add2(char *t);	// t を指すリストを作成する
	char *get(int i);	// i 番目の文字列を得る (i: 0 以上 size 未満)
	int   getsize(void){return size;}

	      ImdCarg(void){size = 0; head = NULL; pp = &head;}
	     ~ImdCarg(){kill();}
};

// Windows ではシェルが * や ? を展開せずに渡す．
// それを UNIX のように展開させるのがこのクラスの init 関数の目的である．
// 単なる関数でも良いが，動的に確保したメモリを自動的に開放するためだけに
// クラスの形をとっている．
//   使い方:
//    1. dxarg.h をインクルードする
//	#include "dxarg.h"
//    2. main 関数を main_dx 関数にリネームする．
//	int main_dx(int argc,char **argv);
//
//    3. 次のように main 関数を定義する
//	int main(int dargc,char *dargv[]){
//		int		i,argc,nc;
//		char		**argv,**nv;
//		ImdDxArg	_imdda(dargc,dargv,&argc,&argv,&nc,&nv);
//
//		// no match 
//		for(i = 0; i < nc; i++){printf("no match %s\n",nv[i]);}
//		return main_dx(argc,argv);
//	}
//
class ImdDxArg{
  protected:
	char		**nwargv,	// 新しい argv の実メモリ用ポインタ
			**nwnomatch;	// マッチしなかったファイル
	ImdCarg		ca;		// 新しい argv の文字列リスト版

	void kill(void);
	void searchA(char *fn);		// *,? を展開する
	void search(char *t);		// 絶対パスにして searchA に渡す
	void init(int argc,char *argv[],int *xargc,char ***xargv,
			int *nNoMatch,char ***sNoMatch);
		// *,? 処理が必要なファイルを探して search に渡す

  public:
	     ImdDxArg(int argc,char *argv[],int *xargc,char ***xargv,
			int *nNoMatch = NULL,char ***sNoMatch = NULL)
			{init(argc,argv,xargc,xargv,nNoMatch,sNoMatch);}
	    ~ImdDxArg(){kill();}
};

#endif // _IMD_DXARG_H

