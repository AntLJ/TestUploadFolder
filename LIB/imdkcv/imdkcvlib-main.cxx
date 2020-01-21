// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcvlib-main.cxx (C++ Source File)                 CcCcC
// CcCcC                                                       CcCcC
// CcCcC        Copyright (C)  1999-2000  Yosuke IMADA         CcCcC
// CcCcC                                                       CcCcC
// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// This library is free software; you can redistribute it and/or
// modify it under the terms of the LIBRARY LICENSE IMADA-1 TYPE
// as published by Yosuke IMADA.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// LIBRARY LICENSE IMADA-1 TYPE for more details.

#include "imdkcvlib.h"

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvCode メンバ関数                           ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

void ImdKcvCode::cat(ImdKcvCode **pphead){
	while(*pphead != NULL){
		pphead = &((*pphead)->next);
	}
	*pphead = this;
}

ImdKcvCode* ImdKcvCode::copy(void){
	ImdKcvCode	*pi,*pre;

	if(next != NULL){pi = next->copy();}
	else{pi = NULL;}

	pre = new ImdKcvCode(kcode,org_kcode,codetype);
	pre->housetsu = housetsu;
	if(pre == NULL){delete pi; return NULL;}
	  // new に失敗した場合はこれまでコピーした分を削除し
	  // NULL を返す
	pre->next = pi;

	return pre;
}

ImdKcvCode::ImdKcvCode(void){
	next=NULL;
	kcode = org_kcode = codetype = housetsu = 0;
}

ImdKcvCode::ImdKcvCode(int vkcode,int vorg_kcode,
		int vcodetype){
	next = NULL;
	kcode = vkcode; org_kcode = vorg_kcode;
	codetype = vcodetype; housetsu = 0;
}

ImdKcvCode::ImdKcvCode(ImdKcvCode **pphead,int vkcode,int vorg_kcode,
		int vcodetype){
	next = NULL;
	kcode = vkcode; org_kcode = vorg_kcode;
	codetype = vcodetype; housetsu = 0;
	cat(pphead);
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdConvModuleList メンバ関数                    ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* ImdConvModuleList::execute(ImdKcvCode *pkc){
	int		cdtype;
	ImdKcvCode	*pkci,*pkcj,*pkck,*pkcre;

	pkcre = NULL;
	cdtype = pkc->codetype;
	  // convert 関数を呼ぶと pkc の値が無効になるため

	pkci =	pcm->convert(pkc); // 第一段階

	if(cdtype == -1){
		for(pkcj = pkci; pkcj != NULL; pkcj = pkcj->next){
			if(pkcj->codetype == -1){cdtype = 0;}
		}
		if(cdtype == -1){
			  // EOF を入力したにもかかわらず、
			  // 第一段階が EOF を出力しなかった場合
			new ImdKcvCode(&pkci,-1,-1,-1);
			  // 第一段階の結果の最後尾に接続
	}	}

	if(next == NULL){pkcre = pkci;}
	else{
		pkcj = pkci;
		while(pkcj != NULL){
			pkck = pkcj; pkcj = pkck->next;
			pkck->next = NULL;
			  // 第一段階の結果を一つ一つ切り出す
			pkci = next->execute(pkck); // 第二段階
			  // この時点で pkck の中身は無効
			  // delete されているか、pkci に流用
			  // されているかのどちらか。
			  // 最後に第二段階の結果だけを出力するが
			  // 第一段階の結果を delete する必要は無い
			pkci->cat(&pkcre);
			  // pkcre の後ろに pkci を繋ぐ
	}	}

	return pkcre;
}

int ImdConvModuleList::concat(ImdConvModuleList **pphead){
	// リストの最後尾にこのリストを接続

	if(pphead == NULL){return -1;}
	while(*pphead != NULL){pphead = &((*pphead)->next);}
	*pphead = this; next = NULL;
	return 0;
}

void ImdConvModuleList::kill_all(void){
	if(next != NULL){delete next;}
	if(pcm  != NULL){delete pcm ;}
}

int ImdConvModuleList::MaxOutputSize(void){
	int		re = 1;

	if(pcm  != NULL){re *=  pcm->MaxOutputSize();}
	if(next != NULL){re *= next->MaxOutputSize();}
	return re;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKanjiCodeConverter メンバ関数                ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// 戻り値:前のデコーダ or NULL
ImdKcvDecoder* ImdKanjiCodeConverter::SetDecoder(ImdKcvDecoder *vpkd){
	ImdKcvDecoder	*re = pkd;
	pkd = vpkd; return re;
}

// 戻り値:前のデコーダ or NULL
ImdKcvEncoder* ImdKanjiCodeConverter::SetEncoder(ImdKcvEncoder *vpke){
	ImdKcvEncoder	*re = pke;
	pke = vpke; return re;
}

// 戻り値 -1:エンコーダ・デコーダがセットされていない
int ImdKanjiCodeConverter::execute(int i,char *s,int *piOffset){
	ImdKcvCode		*pkci,*pkcj,*pkck,*pkcm;
	int			re = 0;

	if(pkd == NULL || pke == NULL){
		  // デコーダ・エンコーダがセットされていない
		return -1;
	}

	pkci = pkd->decode(i);
	  // デコード

	if(phead != NULL){
		pkck = NULL;
		while(pkci != NULL){
			pkcm = pkci;pkci = pkci->next;
			pkcm->next = NULL;
			pkcj = phead->execute(pkcm);
			if(pkcj != NULL){pkcj->cat(&pkck);}
		}
		  // コンバータに一文字一文字渡していく
		  // ( エンコーダが複数文字返す可能性があるから )
		pkci = pkck;
	}

	for(pkcj = pkci; pkcj != NULL; pkcj = pkcj->next){
		re = re + pke->encode(pkcj,s,piOffset);
	}

	delete pkci;

	  // エンコーダ
	  // コンバータの結果 or デコーダの結果(コンバータが無い
	  // 場合) を入力

	return re;
}

int ImdKanjiCodeConverter::MaxOutputSize(void){
	int		re = 1;

	if(pkd == NULL){re = 0;}
	else{re *= pkd->MaxOutputSize();}
	  // デコーダ

	if(phead != NULL){
		re *= phead->MaxOutputSize();
	}
	  // コンバータ

	if(pke == NULL){re = 0;}
	else{re *= pke->MaxOutputSize();}
	  // エンコーダ

	return re;
}

int ImdKanjiCodeConverter::ConcatinateModule(ImdConvModule *pcm){
	ImdConvModuleList	*pkcvlist;

	if(pcm == NULL){return -1;}
	pkcvlist = new ImdConvModuleList(&phead,pcm);
	if(pkcvlist == NULL){return -2;}
	return 0;
}

void ImdKanjiCodeConverter::DeleteModules(void){
	if(phead != NULL){delete phead; phead = NULL;}
}

ImdKanjiCodeConverter::ImdKanjiCodeConverter(void){
	pkd = NULL; pke = NULL; phead = NULL;
}

ImdKanjiCodeConverter::~ImdKanjiCodeConverter(){
	if(pkd != NULL){delete pkd;}
	if(phead != NULL){delete phead;}
	if(pke != NULL){delete pke;}
}
