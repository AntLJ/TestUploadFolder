// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcvlib-jis1.cxx (C++ Source File)                 CcCcC
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
//  ■■■  Imd_UCStoJIS クラス                             ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_UCStoJIS::convert(ImdKcvCode *pkc){
	int		iUcs = pkc->kcode,iJis,inec,inecibm,
			iapple,iwa,i;
	ImdKcvCode	*pkci;

	inec    = f208mode & 0xF;
	iapple  = inec;
	inecibm = (f208mode >> 4 ) & 0xF;
	iwa     = (f208mode >> 12) & 0xF;

	i = f208mode & 0x100;
	if(i != 0 && pkc->housetsu != 0){ // 包摂リカバリ
		if(iwa == 1){
			i = fImd_SJISrecover_NEC(pkc->housetsu);
			if(i != 0){
				i = fImd_SJIStoJIS_X(pkc->housetsu);
				if(i != -1){
					pkc->kcode = i;
					if(pkc->housetsu < 0x87FD){pkc->codetype = 5;}
					else{pkc->codetype = 7;}
					return pkc;
		}	}	}
		else if(iwa==2 && (0xFF < pkc->housetsu)){
			i = fImd_SJISrecover_APPLE(pkc->housetsu);
			if(i != 0){
				pkc->kcode = fImd_SJIStoJIS_X(pkc->housetsu);
				pkc->codetype = 8;
				return pkc;
	}	}	}

	if(pkc->codetype == -3){ // 非 UCS 文字リカバリ
		if(iwa == 2 && 1 <= iapple){ // JISX0212 はない
			pkc->codetype = 8; pkci = pkc->copy();
			if(fImd_SJIStoUCS_APPLE(pkci) == 2){
				pkc->kcode = fImd_SJIStoJIS_X(pkc->kcode);
				delete pkci; return pkc;
		}	}
		pkc->codetype = -2; return pkc;
	}

	if(pkc->codetype < 0){return pkc;}
	if(fImd_UCStoJISX0201(pkc) != 0){return pkc;}
	if(fImd_UCStoJISX0208(iUcs,&iJis) != 0){
		pkc->kcode = iJis;
		pkc->codetype = 4;
		return pkc;
	}

	if(iwa == 1){
		if(2 <= inec){ // X0208外字領域に変換する場合(NEC特殊)
			if(fImd_UCStoJIS_NEC(pkc) != 0){return pkc;}
		}
		if(2 <= inecibm){ // X0208外字領域に変換する場合(NEC選定IBM)
			if(fImd_UCStoJIS_NECIBM(pkc) != 0){return pkc;}
	}	}
	else if(iwa == 2){
		if(2 <= iapple){ // X0208外字領域に変換する場合(Apple拡張)
			if(fImd_UCStoJIS_APPLE(pkc) != 0){return pkc;}
	}	}

	if(fImd_UCStoJISX0212(iUcs,&iJis) != 0){
		pkc->kcode = iJis;
		pkc->codetype = 9;
		return pkc;
	}

	if(iwa == 1){
		if(1 == inec){ // X0212を優先させる場合(NEC特殊)
			if(fImd_UCStoJIS_NEC(pkc) != 0){return pkc;}
		}
		if(1 == inecibm){ // X0212を優先させる場合(NEC選定IBM)
			if(fImd_UCStoJIS_NECIBM(pkc) != 0){return pkc;}
	}	}
	else if(iwa == 2){
		if(1 == iapple){ // X0212を優先させる場合(Apple拡張)
			if(fImd_UCStoJIS_APPLE(pkc) != 0){return pkc;}
	}	}

	if(fImd_UCSto8859(pkc) != 0){return pkc;}

	pkc->codetype = -2; return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_JIStoUCS クラス                             ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_JIStoUCS::convert(ImdKcvCode *pkc){
	int		iUcs,iJis = pkc->kcode;

	if(pkc->codetype < 0){return pkc;}
	else if(pkc->codetype == 9){
	  // コード番号だけでは 208 と 212 の区別が不可能
		if(fImd_JISX0212toUCS(iJis,&iUcs) != 0){
			pkc->kcode = iUcs;
		}
		else{pkc->codetype = -2;}
		return pkc;
	}
	else if(fImd_JISX0201toUCS(pkc) != 0){return pkc;}
	else if(fImd_8859toUCS(pkc) != 0){return pkc;}
	else if(fImd_JISX0208toUCS(iJis,&iUcs) != 0){
		pkc->kcode = iUcs;
		pkc->codetype = 4;
		return pkc;
	}

	// JISX0208外字領域
	if(f208mode == 1){
		if     (fImd_JIStoUCS_NEC(pkc) != 0){return pkc;}
		  // 13区(NEC特殊文字)の場合
		else if(fImd_JIStoUCS_NECIBM(pkc) != 0){return pkc;}
		  // 89-92区(NEC選定IBM拡張文字)の場合
		  // 
		  // IBM選定IBM拡張文字は JISX0208外字領域への
		  // マッピングは無い
		else{pkc->codetype = -2; return pkc;}
	}
	else if(f208mode == 2){
		if     (fImd_JIStoUCS_APPLE(pkc) != 0){return pkc;}
		  // Apple拡張文字の場合
		else{pkc->codetype = -2; return pkc;}
	}

	else{pkc->codetype = -2;}

	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_JISX0208Gaiji_filter クラス                 ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_JISX0208Gaiji_filter::convert(ImdKcvCode *pkc){
	int	i,ku,ten;

	// codetype == 0 も処理したい所だが、JISX0208かJISX0212か
	// 判別できないため codetype == 0 はフィルタしない
	// ( そもそもオリジナルのデコーダ・コンバータはJISコード
	//   に変換する際に codetype は 0 にならない )

	if(pkc->codetype < 0){}
	else if(4 <= pkc->codetype && pkc->codetype <= 8){
		i = pkc->kcode - 0x2020;
		ku = (i >> 8) & 0xFFFF;
		ten = i & 0xFF;
		if(1<=ku && ku<=94 && 1<=ten && ten<=94){
			  // 1～94 区の 1～94 点の場合
			if     (ku == 2 && (15<=ten && ten<=25 ||
			 34<=ten && ten<=41 || 49<=ten && ten<=59 ||
			 75<=ten && ten<=81 || 90<=ten && ten<=93))
				{pkc->codetype = -2;}
			else if(ku == 3 && (1<=ten && ten<=15 ||
			 26<=ten && ten<=32 || 59<=ten && ten<=64 ||
			 91<=ten)){pkc->codetype = -2;}
			else if(ku == 4 && 84<=ten)
				{pkc->codetype = -2;}
			else if(ku == 5 && 87<=ten)
				{pkc->codetype = -2;}
			else if(ku == 6 && (25<=ten && ten<=32 ||
			 57<=ten)){pkc->codetype = -2;}
			else if(ku == 7 && (34<=ten && ten<=48 ||
			 82<=ten)){pkc->codetype = -2;}
			else if(ku == 8 && 33<=ten)
				{pkc->codetype = -2;}
			else if(9<=ku && ku <=15 ||
			 85<=ku && ku<=94)
				{pkc->codetype = -2;}
			else if(ku == 47 && 52<=ten)
				{pkc->codetype = -2;}
			else if(ku == 84 && 6<=ten)
				{pkc->codetype = -2;}

		}
		else{pkc->codetype = -2;}
	}
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_JISX0212_filter クラス                      ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_JISX0212_filter::convert(ImdKcvCode *pkc){
	if(pkc->codetype == 9){
		pkc->codetype = -2;
	}

	return pkc;
}


