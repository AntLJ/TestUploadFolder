// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  dxarg.cxx (C++ Source File)                          CcCcC
// CcCcC                       Version  3. 0. 0                CcCcC
// CcCcC                                                       CcCcC
// CcCcC        Copyright (C)  1997-2000  Yosuke IMADA         CcCcC
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include "dxarg.h"

// もし _IMD_DXARG_DOUBLEAST を 0 以外にすると \**\ を
// ０個以上の任意のディレクトリとしてみなす．
// ちなみにこの場合 \*\**\ と書けば１個以上の任意のディレクトリになる
#define _IMD_DXARG_DOUBLEAST 0

void ImdCargl::kill(void){
	if(df != 0){
		delete s; df = 0;
	}
	s = NULL;
}

int  ImdCargl::set(char *t){
	kill();
	s = new char[strlen(t)+1];
	if(s == NULL){return 0;}
	df = 1; strcpy(s,t);
	return 1;
}

void ImdCarg::kill(void){
	ImdCargl	*b;

	while(head != NULL){
		b = head; head = head->next;
		delete b;
	}
	size = 0; pp = &head;
}

void ImdCarg::add(char *t){
	// t をコピーしてリストを作成する
	*pp = new ImdCargl(t);
	pp = &((*pp)->next); size++;
}

void ImdCarg::add2(char *t){
	// リストは t 自体をさす．(コピーしない)
	// t は delete する必要がないので pp->df == 0 でリスト作成
	*pp = new ImdCargl();
	(*pp)->s = t; pp = &((*pp)->next); size++;
}

char *ImdCarg::get(int i){
	ImdCargl	*c = head;

	if(i < 0 || size <= i) return NULL;
	while(i){c = c->next; i--;}
	return c->s;
}

void ImdDxArg::kill(void){
	if(nwargv    != NULL){free(nwargv);    nwargv = NULL;}
	if(nwnomatch != NULL){free(nwnomatch); nwnomatch = NULL;}
}

#if _IMD_DXARG_DOUBLEAST
  // \**\ を０個以上の任意のディレクトリとみなすバージョン
  void ImdDxArg::searchA(char *fn){
	int			i,ifn,bmark,prevbs;
	char			s[1024];
	struct _finddata_t	fd;
	long			lh;

	ifn = 0;	// fn 用インジケータ
	bmark = 0;	// * や ? があったら 1 になる
	prevbs = 0;	// 最後の '\\' の位置

	for(;fn[ifn] != '\0';ifn++){
		if     ((fn[ifn] == '\\') && (bmark == 0)){
			// '\\' の位置のバックアップ
			prevbs = ifn;
		}else if(fn[ifn] == '\\'){
			// ディレクトリ部分に * や ? があった場合
			for(i=0;i <= prevbs;i++){s[i] = fn[i];}

			if((fn[ifn-3] == '\\') && (fn[ifn-2] == '*') &&
			   (fn[ifn-1] == '*')){
				// \**\ の場合
				// 例えば fn = c:\home\data\**\*.txt の場合
				// s = c:\home\data\ fn = c:\home\data\* にする
				s[prevbs + 1] = '\0'; fn[ifn - 1] = '\0';

				// fn で検索
				// \**\ を１個以上のディレクトリとしてみなす場合
				lh = _findfirst(fn,&fd); i = (int)lh;
				for(;i != -1; i = _findnext(lh,&fd)){
					// ディレクトリじゃなければ continue
					if(!(fd.attrib & _A_SUBDIR)){continue;}
					if(strcmp(fd.name,".") && strcmp(fd.name,"..")){
						// . でも .. でもなければ s に具体的な
						// 名前を入れて再起呼び出し
						// 例 s = c:\home\data\xyz\**\*.txt
						s[prevbs+1] = '\0';
						strcat(s,fd.name);
						strcat(s,"\\**\\");
						strcat(s,&(fn[ifn+1]));
						searchA(s);
				}	}
				_findclose(lh);

				// \**\ を０個のディレクトリとしてみなす場合
				// s = c:\home\data\*.txt
				strcat(s,&(fn[ifn+1])); searchA(s);
				fn[ifn - 1] = '*';  // fn を元に戻す
				return ;
			}else{
				// 例えば fn = c:\home\data\x?z\*.txt の場合
				// s = c:\home\data\ fn = c:\home\data\x?z にする
				fn[ifn] = '\0';

				// fn で検索
				lh = _findfirst(fn,&fd); i = (int)lh;
				for(;i != -1; i = _findnext(lh,&fd)){
					// ディレクトリじゃなければ continue
					if(!(fd.attrib & _A_SUBDIR)){continue;}
					if(strcmp(fd.name,".") && strcmp(fd.name,"..")){
						// . でも .. でもなければ s に具体的な
						// 名前を入れて再起呼び出し
						// 例 s = c:\home\data\xyz\*.txt
						s[prevbs+1] = '\0';
						strcat(s,fd.name);
						strcat(s,"\\");
						strcat(s,&(fn[ifn+1]));
						searchA(s);
				}	}
				_findclose(lh);
				fn[ifn] = '\\';  // fn を元に戻す
				return ;
			}
		}
		else if(fn[ifn] == '*' || fn[ifn] == '?'){bmark = 1;}
	}

	// ファイル名に *,? がある場合 (例 c:\data\*.txt )
	// ファイル名以外を s に格納 (s = c:\data\ )
	for(i = 0; i <= prevbs; i++){s[i] = fn[i];}

	lh = _findfirst(fn,&fd); i = (int)lh;
	for(;i != -1; i = _findnext(lh,&fd)){
		// ファイルじゃない場合は continue
		if(fd.attrib & _A_SUBDIR){continue;}

		// s に具体的な名前を入れる
		s[prevbs + 1] = '\0';	// (s == c:\data\ )
		strcat(s,fd.name);	// (fd.name == a.txt )
		ca.add(s);
	}
	_findclose(lh);
  }

#else // _IMD_DXARG_DOUBLEAST
  // \**\ を０個以上の任意のディレクトリとみなさないバージョン

  void ImdDxArg::searchA(char *fn){
	int			i,ifn,bmark,prevbs;
	char			s[1024];
	struct _finddata_t	fd;
	long			lh;

	ifn = 0;	// fn 用インジケータ
	bmark = 0;	// * や ? があったら 1 になる
	prevbs = 0;	// 最後の '\\' の位置

	for(;fn[ifn] != '\0';ifn++){
		if     ((fn[ifn] == '\\') && (bmark == 0)){
			// '\\' の位置のバックアップ
			prevbs = ifn;
		}else if(fn[ifn] == '\\'){
			// ディレクトリ部分に * や ? があった場合

			// 例えば fn = c:\home\data\x?z\*.txt の場合
			// s = c:\home\data\, fn = c:\home\data\x?z にする．
			for(i=0;i <= prevbs;i++){s[i] = fn[i];}
			fn[ifn] = '\0';

			// fn で検索
			lh = _findfirst(fn,&fd); i = (int)lh;
			for(;i != -1; i = _findnext(lh,&fd)){
				// ディレクトリじゃなければ continue
				if(!(fd.attrib & _A_SUBDIR)){continue;}
				if(strcmp(fd.name,".") && strcmp(fd.name,"..")){
					// . でも .. でもなければ s に具体的な
					// 名前を入れて再起呼び出し
					// 例 s = c:\home\data\xyz\*.txt
					s[prevbs+1] = '\0';
					strcat(s,fd.name);
					strcat(s,"\\");
					strcat(s,&(fn[ifn+1]));
					searchA(s);
			}	}
			_findclose(lh);
			fn[ifn] = '\\';  // fn を元に戻す
			return ;
		}
		else if(fn[ifn] == '*' || fn[ifn] == '?'){bmark = 1;}
	}

	// ファイル名に *,? がある場合 (例 c:\data\*.txt )
	// ファイル名以外を s に格納 (s = c:\data\ )
	for(i = 0; i <= prevbs; i++){s[i] = fn[i];}

	lh = _findfirst(fn,&fd); i = (int)lh;
	for(;i != -1; i = _findnext(lh,&fd)){
		// ファイルじゃない場合は continue
		if(fd.attrib & _A_SUBDIR){continue;}

		// s に具体的な名前を入れる
		s[prevbs+1] = '\0';	// ( s == c:\data\ )
		strcat(s,fd.name);	// ( fd.name == a.txt )
		ca.add(s);
	}
	_findclose(lh);
  }

#endif // _IMD_DXARG_DOUBLEAST


void ImdDxArg::search(char *t){
	char		c,c_prev;
	int		i,j,slen,slenN,nDot,tlen,bAbsPath;
	char		s[1024];

	// この関数では * ? はまだ展開しない
	// まずファイル名を相対パスからドライブ名付絶対パスに変換する
	// (c:data.txt -> c:\tmp\data.txt の変換など)

	tlen = strlen(t);
	bAbsPath = 0;	// s に絶対パスを格納したら 1 する

	if(t[0] == '\\'){
		// [1] 絶対パスでドライブ名なし
		// ドライブ名を (例えば C: など) を付加する
		s[0] = (char)(_getdrive() + 'A' - 1);
		s[1] = ':';
		strcpy(&(s[2]),t);
		// これで s は絶対パスになった
		bAbsPath = 1;
	}
	else if(2 < tlen){
		if(t[1] == ':'){
			if(t[2] == '\\'){
				// [2] 絶対パスでドライブ名あり
				// 特に処理なし
				// ドライブ名は大文字である必要がある
				strcpy(s,t); s[0] = (char)toupper(s[0]);
			}
			else{
				// [3] 相対パスでドライブ名あり
				// そのドライブのカレントディレクトリを得る
				_getdcwd(toupper(t[0]) - 'A' + 1,s,1024);
				slen = strlen(s);
				// 得たカレントディレクトリの最後が '\\' じゃ
				// なかったら それを追加
				if(s[slen - 1] != '\\'){
					s[slen] = '\\'; s[slen+1] = '\0';
				}
				// t を s に接続すれば絶対パスの出来上がり
				strcat(s,&(t[2]));
			}
			bAbsPath = 1;
	}	}

	if(bAbsPath == 0){
		// [4] 相対パスでドライブ名なし
		_getcwd(s,1024); j = strlen(s);
		if(s[j-1] != '\\'){s[j] = '\\'; s[j+1] = '\0';}
		strcat(s,t);
	}

	// この時点で s にはドライブ名付絶対パスが格納されている
	// が， c:\tmp\..\text\\data.txt みたいな形になっているかもしれない
	// ので， . や .. の変換，\\ の変換を行う．
	// なお，ルートディレクトリの親はルートディレクトリとする

	// s[] を書き換えて s[] に書き込む．
	// 短くなるだけなので同じバッファに書き込むことが可能．
	// 読み込み用のインジケータが slen で書き込み用が slenN になる．

	slen = slenN = 0; nDot = 0;

	// 一文字前
	c_prev = '\0';

	while((c = s[slen]) != '\0'){
		if     ((c == '\\') && (c_prev == '\\')){
			// \ が２つ続く場合は１つにマージ
			// slenN の方を更新しないことで１文字ずれる
			slen++; continue;
		}else if((c == '.') && (c_prev == '\\')){
			// \. 
			nDot = 1;
		}else if((c == '.') && (nDot != 0)){
			// \.. や \... などの時
			nDot++;
		}else if((c == '\\') && (nDot != 0)){
			// \.. のドット数が確定した場合 (\..\)
			// 戻る位置を探す
			// 例えば magic\data\imada\home\...\a.txt
			// なら data の後ろの \ の位置を slenN に格納
			i = slenN - 1; slenN = -1;
			while(0 <= i){
				if(s[i] == '\\'){
					nDot--;
					if(nDot == 0){slenN = i; break;}
				}
				i--;
			}
			if(slenN == -1){
				// ルートディレクトリの親はルートディレクトリ
				// ドライブ名が必ずついているので
				// 先頭は C:\ のような形になっている
				slenN = 2;
			}
			// magic\data\imada\home\...\a.txt は
			// magic\data\a.txt で， s[slen] が a.txt の前の \
			// s[j] が data の後ろの \ になる．

			// s[slenN] も s[slen] も '\\' だから
			slen++; slenN++; c_prev = '\\';
			continue;
		}else if(nDot != 0){
			// \.emacs のような場合だったとき
			nDot = 0;
		}

		// 一文字前の更新
		s[slenN] = s[slen]; slenN++; slen++;
		c_prev = c;
	}
	s[slenN] = '\0';

	// この時点で s は正規化されたドライブ名付絶対パス

	searchA(s);
}

void ImdDxArg::init(int argc,char *argv[],int *xargc,char ***xargv,
			int *nNoMatch,char ***sNoMatch){
	int		i,j,bmark,iSizeBack;
	ImdCarg		caNoMatch;


	// 確定したものを ca に格納していく
	// argv[0] は確定
	ca.add2(argv[0]);

	for(i = 1; i < argc; i++){
		j = bmark = 0;
		// *,? が含まれれば bmark を 0 以外にする
		while(argv[i][j] != '\0'){
			if(argv[i][j] == '*' || argv[i][j] == '?'){bmark = 1;}
			j++;
		}
		// 引数に * か ? が含まれていなければ そのまま ca に格納
		if(bmark == 0){ca.add2(argv[i]);}
		// 含まれていれば展開する
		else{
			// 現在確定している argc をバックアップ
			iSizeBack = ca.getsize();
			search(argv[i]);
			if(iSizeBack == ca.getsize()){
				// search() の結果 argc が増えなかった場合
				// つまり argv[i] にマッチするものが1つも
				// ない場合
				caNoMatch.add2(argv[i]);
	}	}	}

	// 新しい argc の値を算出
	*xargc = j = ca.getsize();

	// 新しい argv を作成する  ( サイズ j の (char *) 配列 )
	nwargv = *xargv = (char **)malloc(sizeof(char *) * j);
	for(i=0;i < j;i++){
		(*xargv)[i] = ca.get(i);
	}

	if(sNoMatch != NULL && nNoMatch != NULL){
		// マッチしなかった文字列をそれぞれ格納する
		*nNoMatch = j = caNoMatch.getsize();
		nwnomatch = *sNoMatch = (char **)malloc(sizeof(char *) * j);
		for(i=0;i < j;i++){
			(*sNoMatch)[i] = caNoMatch.get(i);
		}
	}else{
		nwnomatch = NULL;
	}
}

