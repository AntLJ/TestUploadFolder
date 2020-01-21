// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcvlib-sjis.cxx (C++ Source File)                 CcCcC
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
//  ■■■  補助関数・定数群                                ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// 定義されている関数
// 
// JISX0208 範囲(機種依存文字を除く)にあるかどうか
// int fImd_IsJISX0208(int jis);	//  JISコード版
// int fImd_IsSJIS(int sjis);		// SJISコード版
// 
// JISX0208 範囲内に限った JIS<->SJIS 変換
// int fImd_JIStoSJIS(int iJis, int *piSjis);
// int fImd_SJIStoJIS(int iSjis, int *piJis);
// 
// NEC特殊文字・{NEC|IBM}選定IBM拡張文字 用関数
// int fImd_SJIStoUCS_NEC(ImdKcvCode *pkc);
// int fImd_UCStoSJIS_NEC(ImdKcvCode *pkc);
// int fImd_SJIStoUCS_NECIBM(ImdKcvCode *pkc);
// int fImd_UCStoSJIS_NECIBM(ImdKcvCode *pkc);
// int fImd_SJIStoUCS_IBMIBM(ImdKcvCode *pkc);
// int fImd_UCStoSJIS_IBMIBM(ImdKcvCode *pkc);
// Apple拡張文字用関数
// int fImd_SJIStoUCS_APPLE(ImdKcvCode *pkc);
// int fImd_UCStoSJIS_APPLE(ImdKcvCode *pkc);

int fImd_IsJISX0208(int jis){
	int	u,l,re;

	re = 0;
	u = (jis >> 8) & 0xFF; l = jis & 0xFF;

	if(u == 0x22){
		if((0x21<=l && l<=0x2E)||(0x3A<=l && l<=0x41)||
		   (0x4A<=l && l<=0x50)||(0x5C<=l && l<=0x6A)||
		   (0x72<=l && l<=0x79)||(l == 0x7E)){re = 1;}
	}
	else if(u == 0x23){
		if((0x30<=l && l<=0x39)||(0x41<=l && l<=0x5A)||
		   (0x61<=l && l<=0x7A)){re = 1;}
	}
	else if(u == 0x24){if(0x21<=l && l<=0x73){re = 1;}}
	else if(u == 0x25){if(0x21<=l && l<=0x76){re = 1;}}
	else if(u == 0x26){
		if((0x21<=l && l<=0x38)||
			(0x41<=l && l<=0x58)){re = 1;}
	}
	else if(u == 0x27){
		if((0x21<=l && l<=0x41)||
			(0x51<=l && l<=0x71)){re = 1;}
	}
	else if(u == 0x28){if(0x21<=l && l<=0x40){re = 1;}}
	else if(u == 0x4F){if(0x21<=l && l<=0x53){re = 1;}}
	else if(u == 0x74){if(0x21<=l && l<=0x26){re = 1;}}
	else if((u == 0x21)||(0x30<=u && u<=0x4E)||
			     (0x50<=u && u<=0x73)){
		if(0x21 <=l && l<=0x7E){re = 1;}
	}

	return re;
}

int fImd_IsSJIS(int sjis){
	int	u,l,re;

	re = 0;
	u = (sjis >> 8) & 0xFF; l = sjis & 0xFF;
	if(u==0x81){
		if((0x40<=l && l<=0x7E)||(0x80<=l && l<=0xAC)||
		   (0xB8<=l && l<=0xBF)||(0xC8<=l && l<=0xCE)||
		   (0xDA<=l && l<=0xE8)||(0xF0<=l && l<=0xF7)||
		   (l==0xFC)){re = 1;}
	}
	else if(u==0x82){
		if((0x4F<=l && l<=0x58)||(0x60<=l && l<=0x79)||
		   (0x81<=l && l<=0x9A)||(0x9F<=l && l<=0xF1))
			{re = 1;}
	}
	else if(u==0x83){
		if((0x40<=l && l<=0x7E)||(0x80<=l && l<=0x96)||
		   (0x9F<=l && l<=0xB6)||(0xBF<=l && l<=0xE6))
			{re = 1;}
	}
	else if(u==0x84){
		if((0x40<=l && l<=0x60)||(0x70<=l && l<=0x7E)||
		   (0x80<=l && l<=0x91)||(0x9F<=l && l<=0xBE))
			{re = 1;}
	}
	else if(u==0x88){
		if(0x9F<=l && l<=0xFC){re = 1;}
	}
	else if((0x89<=u && u<=0x97)||(0x99<=u && u<=0x9F)||
		(0xE0<=u && u<=0xE9)){
		if((0x40<=l && l<=0x7E)||(0x80<=l && l<=0xFC))
			{re = 1;}
	}
	else if(u==0x98){
		if((0x40<=l && l<=0x72)||(0x9F<=l && l<=0xFC))
			{re = 1;}
	}
	else if(u==0xEA){
		if((0x40<=l && l<=0x7E)||(0x80<=l && l<=0xA4))
			{re = 1;}
	}

	return re;
}

// 外字も含めて JIS->SJIS 変換
int fImd_JIStoSJIS_X(int iJis){
	int		u,l;
	u = (iJis >> 8) & 0xFF; l = iJis & 0xFF;
	if(0x21<=u&&u<=0x7E&&0x21<=l&&l<=0x7E){
		iJis = iJis - 0x2121;
		if((iJis & 0x100) != 0){iJis += 0x9E;}
		else{iJis += 0x40;}
		l = iJis & 0xFF;
		if(0x7F <= l){iJis += 1;}
		u = (((iJis >> 8) & 0xFF) >> 1) + 0x81;
		iJis = ((u << 8) & 0xFF00) | (iJis & 0xFF);
		if(0xA000 <= iJis){iJis += 0x4000;}
	}
	else{iJis = -1;}
	return iJis;
}

int fImd_JIStoSJIS(int iJis, int *piSjis){
	int	re,u,l,iSjis;

	re = fImd_IsJISX0208(iJis);
	if(re != 0){
		iSjis = iJis - 0x2121;
		if((iSjis & 0x100) != 0){iSjis += 0x9E;}
		else{iSjis += 0x40;}
		l = iSjis & 0xFF;
		if(0x7F <= l){iSjis += 1;}
		u = (((iSjis >> 8) & 0xFF) >> 1) + 0x81;
		iSjis = ((u << 8) & 0xFF00) | (iSjis & 0xFF);
		if(0xA000 <= iSjis){iSjis += 0x4000;}
		*piSjis = iSjis;
	}
	return re;
}

// 外字も含めて SJIS -> JIS 変換
int fImd_SJIStoJIS_X(int iSjis){
	int		u,l;
	u = (iSjis >> 8) & 0xFF; l = iSjis & 0xFF;
	if(0x81<=u&&u<=0xEF&&0x40<=l&&l<=0xFC){
		if(0xE000 <= iSjis){iSjis -= 0x4000;}
		u = ((((iSjis >> 8) & 0xFF) - 0x81) << 1) & 0xFF;
		iSjis = ((u << 8) & 0xFF00) | (iSjis & 0xFF);
		if((iSjis & 0x80) != 0){iSjis--;}
		l = iSjis & 0xFF;
		if(0x9E <= l){iSjis += 0x100; iSjis -= 0x9E;}
		else{iSjis -= 0x40;}
		iSjis += 0x2121;
	}
	else{iSjis = -1;}
	return iSjis;
}

int fImd_SJIStoJIS(int iSjis, int *piJis){
	int	re,u,l,iJis;

	re = fImd_IsSJIS(iSjis);
	if(re != 0){
		iJis = iSjis;
		if(0xE000 <= iJis){iJis -= 0x4000;}
		u = ((((iJis >> 8) & 0xFF) - 0x81) << 1) & 0xFF;
		iJis = ((u << 8) & 0xFF00) | (iJis & 0xFF);
		if((iJis & 0x80) != 0){iJis--;}
		l = iJis & 0xFF;
		if(0x9E <= l){iJis += 0x100; iJis -= 0x9E;}
		else{iJis -= 0x40;}
		iJis += 0x2121;
		*piJis = iJis;
	}
	return re;
}

static int iTblSJIStoUCS_NEC[] = {
	0x2460,0x2461,0x2462,0x2463,0x2464,0x2465,0x2466,0x2467,
	0x2468,0x2469,0x246A,0x246B,0x246C,0x246D,0x246E,0x246F,
	0x2470,0x2471,0x2472,0x2473,0x2160,0x2161,0x2162,0x2163,
	0x2164,0x2165,0x2166,0x2167,0x2168,0x2169,    -1,0x3349,
	0x3314,0x3322,0x334D,0x3318,0x3327,0x3303,0x3336,0x3351,
	0x3357,0x330D,0x3326,0x3323,0x332B,0x334A,0x333B,0x339C,
	0x339D,0x339E,0x338E,0x338F,0x33C4,0x33A1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,0x337B,    -1,
	0x301D,0x301F,0x2116,0x33CD,0x2121,0x32A4,0x32A5,0x32A6,
	0x32A7,0x32A8,0x3231,0x3232,0x3239,0x337E,0x337D,0x337C,
	0x2252,0x2261,0x222B,0x222E,0x2211,0x221A,0x22A5,0x2220,
	0x221F,0x22BF,0x2235,0x2229,0x222A};

// NEC特殊文字(SJIS) のとき UCS に変換して 1 を返す
int fImd_SJIStoUCS_NEC(ImdKcvCode *pkc){
	int	i = pkc->kcode;

	if(0x8740 <= i && i <= 0x879C){
		if((i==0x8790)||(i==0x8791)||(i==0x8792)||
		   (i==0x8795)||(i==0x8796)||(i==0x8797)||
		   (i==0x879A)||(i==0x879B)||(i==0x879C)){
			pkc->housetsu = i;
		}
		i = iTblSJIStoUCS_NEC[i - 0x8740];
		if(i != -1){
			pkc->kcode = i;
			pkc->codetype = 5;
			return 1;
	}	}
	return 0;
}

// NEC特殊文字(をUCSに変換したもの) のとき SJIS に変換して 1 を返す
int fImd_UCStoSJIS_NEC(ImdKcvCode *pkc){
	int	re = 0,ucs = pkc->kcode,i;

	for(i = 0; i <= 0x5C; i++){
		if(iTblSJIStoUCS_NEC[i] == ucs){
			pkc->kcode = i + 0x8740;
			pkc->codetype = 5; re = 1; break;
		}
	}

	return re;
}

static int iTblSJIStoUCS_NECIBM1[] = {
	0x7E8A,0x891C,0x9348,0x9288,0x84DC,0x4FC9,0x70BB,0x6631,
	0x68C8,0x92F9,0x66FB,0x5F45,0x4E28,0x4EE1,0x4EFC,0x4F00,
	0x4F03,0x4F39,0x4F56,0x4F92,0x4F8A,0x4F9A,0x4F94,0x4FCD,
	0x5040,0x5022,0x4FFF,0x501E,0x5046,0x5070,0x5042,0x5094,
	0x50F4,0x50D8,0x514A,0x5164,0x519D,0x51BE,0x51EC,0x5215,
	0x529C,0x52A6,0x52C0,0x52DB,0x5300,0x5307,0x5324,0x5372,
	0x5393,0x53B2,0x53DD,0xFA0E,0x549C,0x548A,0x54A9,0x54FF,
	0x5586,0x5759,0x5765,0x57AC,0x57C8,0x57C7,0xFA0F,    -1,
	0xFA10,0x589E,0x58B2,0x590B,0x5953,0x595B,0x595D,0x5963,
	0x59A4,0x59BA,0x5B56,0x5BC0,0x752F,0x5BD8,0x5BEC,0x5C1E,
	0x5CA6,0x5CBA,0x5CF5,0x5D27,0x5D53,0xFA11,0x5D42,0x5D6D,
	0x5DB8,0x5DB9,0x5DD0,0x5F21,0x5F34,0x5F67,0x5FB7,0x5FDE,
	0x605D,0x6085,0x608A,0x60DE,0x60D5,0x6120,0x60F2,0x6111,
	0x6137,0x6130,0x6198,0x6213,0x62A6,0x63F5,0x6460,0x649D,
	0x64CE,0x654E,0x6600,0x6615,0x663B,0x6609,0x662E,0x661E,
	0x6624,0x6665,0x6657,0x6659,0xFA12,0x6673,0x6699,0x66A0,
	0x66B2,0x66BF,0x66FA,0x670E,0xF929,0x6766,0x67BB,0x6852,
	0x67C0,0x6801,0x6844,0x68CF,0xFA13,0x6968,0xFA14,0x6998,
	0x69E2,0x6A30,0x6A6B,0x6A46,0x6A73,0x6A7E,0x6AE2,0x6AE4,
	0x6BD6,0x6C3F,0x6C5C,0x6C86,0x6C6F,0x6CDA,0x6D04,0x6D87,
	0x6D6F,0x6D96,0x6DAC,0x6DCF,0x6DF8,0x6DF2,0x6DFC,0x6E39,
	0x6E5C,0x6E27,0x6E3C,0x6EBF,0x6F88,0x6FB5,0x6FF5,0x7005,
	0x7007,0x7028,0x7085,0x70AB,0x710F,0x7104,0x715C,0x7146,
	0x7147,0xFA15,0x71C1,0x71FE,0x72B1};

static int iTblSJIStoUCS_NECIBM2[] = {
	0x72BE,0x7324,0xFA16,0x7377,0x73BD,0x73C9,0x73D6,0x73E3,
	0x73D2,0x7407,0x73F5,0x7426,0x742A,0x7429,0x742E,0x7462,
	0x7489,0x749F,0x7501,0x756F,0x7682,0x769C,0x769E,0x769B,
	0x76A6,0xFA17,0x7746,0x52AF,0x7821,0x784E,0x7864,0x787A,
	0x7930,0xFA18,0xFA19,0xFA1A,0x7994,0xFA1B,0x799B,0x7AD1,
	0x7AE7,0xFA1C,0x7AEB,0x7B9E,0xFA1D,0x7D48,0x7D5C,0x7DB7,
	0x7DA0,0x7DD6,0x7E52,0x7F47,0x7FA1,0xFA1E,0x8301,0x8362,
	0x837F,0x83C7,0x83F6,0x8448,0x84B4,0x8553,0x8559,    -1,
	0x856B,0xFA1F,0x85B0,0xFA20,0xFA21,0x8807,0x88F5,0x8A12,
	0x8A37,0x8A79,0x8AA7,0x8ABE,0x8ADF,0xFA22,0x8AF6,0x8B53,
	0x8B7F,0x8CF0,0x8CF4,0x8D12,0x8D76,0xFA23,0x8ECF,0xFA24,
	0xFA25,0x9067,0x90DE,0xFA26,0x9115,0x9127,0x91DA,0x91D7,
	0x91DE,0x91ED,0x91EE,0x91E4,0x91E5,0x9206,0x9210,0x920A,
	0x923A,0x9240,0x923C,0x924E,0x9259,0x9251,0x9239,0x9267,
	0x92A7,0x9277,0x9278,0x92E7,0x92D7,0x92D9,0x92D0,0xFA27,
	0x92D5,0x92E0,0x92D3,0x9325,0x9321,0x92FB,0xFA28,0x931E,
	0x92FF,0x931D,0x9302,0x9370,0x9357,0x93A4,0x93C6,0x93DE,
	0x93F8,0x9431,0x9445,0x9448,0x9592,0xF9DC,0xFA29,0x969D,
	0x96AF,0x9733,0x973B,0x9743,0x974D,0x974F,0x9751,0x9755,
	0x9857,0x9865,0xFA2A,0xFA2B,0x9927,0xFA2C,0x999E,0x9A4E,
	0x9AD9,0x9ADC,0x9B75,0x9B72,0x9B8F,0x9BB1,0x9BBB,0x9C00,
	0x9D70,0x9D6B,0xFA2D,0x9E19,0x9ED1,    -1,    -1,0x2170,
	0x2171,0x2172,0x2173,0x2174,0x2175,0x2176,0x2177,0x2178,
	0x2179,0xFFE2,0xFFE4,0xFF07,0xFF02};

// NEC選定IBM拡張文字 のとき UCS に変換して 1 を返す
int fImd_SJIStoUCS_NECIBM(ImdKcvCode *pkc){
	int	i = pkc->kcode,j;

	if     (0xED40 <= i && i <= 0xEDFC){
		j = iTblSJIStoUCS_NECIBM1[i - 0xED40];
	}
	else if(0xEE40 <= i && i <= 0xEEFC){
		j = iTblSJIStoUCS_NECIBM2[i - 0xEE40];
	}
	else{return 0;}

	if(j != -1){
		if(pkc->kcode == 0xEEF9){
			pkc->housetsu = i;
		}
		pkc->kcode = j;
		pkc->codetype = 6;
		return 1;
	}
	return 0;
}

// NEC選定IBM拡張文字(をUCSに変換したもの) のとき
// SJIS に変換して 1 を返す
int fImd_UCStoSJIS_NECIBM(ImdKcvCode *pkc){
	int	sjis = -1,ucs = pkc->kcode,i;

	for(i = 0; i <= 0xBC; i++){
		if     (iTblSJIStoUCS_NECIBM1[i] == ucs){
			sjis = 0xED40 + i; break;
		}
		else if(iTblSJIStoUCS_NECIBM2[i] == ucs){
			sjis = 0xEE40 + i; break;
		}
	}

	if(sjis != -1){
		pkc->kcode = sjis;
		pkc->codetype = 6;
		return 1;
	}
	return 0;
}

static int iTblSJIStoUCS_IBMIBM1[] = {
	0x2170,0x2171,0x2172,0x2173,0x2174,0x2175,0x2176,0x2177,
	0x2178,0x2179,0x2160,0x2161,0x2162,0x2163,0x2164,0x2165,
	0x2166,0x2167,0x2168,0x2169,0xFFE2,0xFFE4,0xFF07,0xFF02,
	0x3231,0x2116,0x2121,0x2235,0x7E8A,0x891C,0x9348,0x9288,
	0x84DC,0x4FC9,0x70BB,0x6631,0x68C8,0x92F9,0x66FB,0x5F45,
	0x4E28,0x4EE1,0x4EFC,0x4F00,0x4F03,0x4F39,0x4F56,0x4F92,
	0x4F8A,0x4F9A,0x4F94,0x4FCD,0x5040,0x5022,0x4FFF,0x501E,
	0x5046,0x5070,0x5042,0x5094,0x50F4,0x50D8,0x514A,    -1,
	0x5164,0x519D,0x51BE,0x51EC,0x5215,0x529C,0x52A6,0x52C0,
	0x52DB,0x5300,0x5307,0x5324,0x5372,0x5393,0x53B2,0x53DD,
	0xFA0E,0x549C,0x548A,0x54A9,0x54FF,0x5586,0x5759,0x5765,
	0x57AC,0x57C8,0x57C7,0xFA0F,0xFA10,0x589E,0x58B2,0x590B,
	0x5953,0x595B,0x595D,0x5963,0x59A4,0x59BA,0x5B56,0x5BC0,
	0x752F,0x5BD8,0x5BEC,0x5C1E,0x5CA6,0x5CBA,0x5CF5,0x5D27,
	0x5D53,0xFA11,0x5D42,0x5D6D,0x5DB8,0x5DB9,0x5DD0,0x5F21,
	0x5F34,0x5F67,0x5FB7,0x5FDE,0x605D,0x6085,0x608A,0x60DE,
	0x60D5,0x6120,0x60F2,0x6111,0x6137,0x6130,0x6198,0x6213,
	0x62A6,0x63F5,0x6460,0x649D,0x64CE,0x654E,0x6600,0x6615,
	0x663B,0x6609,0x662E,0x661E,0x6624,0x6665,0x6657,0x6659,
	0xFA12,0x6673,0x6699,0x66A0,0x66B2,0x66BF,0x66FA,0x670E,
	0xF929,0x6766,0x67BB,0x6852,0x67C0,0x6801,0x6844,0x68CF,
	0xFA13,0x6968,0xFA14,0x6998,0x69E2,0x6A30,0x6A6B,0x6A46,
	0x6A73,0x6A7E,0x6AE2,0x6AE4,0x6BD6,0x6C3F,0x6C5C,0x6C86,
	0x6C6F,0x6CDA,0x6D04,0x6D87,0x6D6F};

static int iTblSJIStoUCS_IBMIBM2[] = {
	0x6D96,0x6DAC,0x6DCF,0x6DF8,0x6DF2,0x6DFC,0x6E39,0x6E5C,
	0x6E27,0x6E3C,0x6EBF,0x6F88,0x6FB5,0x6FF5,0x7005,0x7007,
	0x7028,0x7085,0x70AB,0x710F,0x7104,0x715C,0x7146,0x7147,
	0xFA15,0x71C1,0x71FE,0x72B1,0x72BE,0x7324,0xFA16,0x7377,
	0x73BD,0x73C9,0x73D6,0x73E3,0x73D2,0x7407,0x73F5,0x7426,
	0x742A,0x7429,0x742E,0x7462,0x7489,0x749F,0x7501,0x756F,
	0x7682,0x769C,0x769E,0x769B,0x76A6,0xFA17,0x7746,0x52AF,
	0x7821,0x784E,0x7864,0x787A,0x7930,0xFA18,0xFA19,    -1,
	0xFA1A,0x7994,0xFA1B,0x799B,0x7AD1,0x7AE7,0xFA1C,0x7AEB,
	0x7B9E,0xFA1D,0x7D48,0x7D5C,0x7DB7,0x7DA0,0x7DD6,0x7E52,
	0x7F47,0x7FA1,0xFA1E,0x8301,0x8362,0x837F,0x83C7,0x83F6,
	0x8448,0x84B4,0x8553,0x8559,0x856B,0xFA1F,0x85B0,0xFA20,
	0xFA21,0x8807,0x88F5,0x8A12,0x8A37,0x8A79,0x8AA7,0x8ABE,
	0x8ADF,0xFA22,0x8AF6,0x8B53,0x8B7F,0x8CF0,0x8CF4,0x8D12,
	0x8D76,0xFA23,0x8ECF,0xFA24,0xFA25,0x9067,0x90DE,0xFA26,
	0x9115,0x9127,0x91DA,0x91D7,0x91DE,0x91ED,0x91EE,0x91E4,
	0x91E5,0x9206,0x9210,0x920A,0x923A,0x9240,0x923C,0x924E,
	0x9259,0x9251,0x9239,0x9267,0x92A7,0x9277,0x9278,0x92E7,
	0x92D7,0x92D9,0x92D0,0xFA27,0x92D5,0x92E0,0x92D3,0x9325,
	0x9321,0x92FB,0xFA28,0x931E,0x92FF,0x931D,0x9302,0x9370,
	0x9357,0x93A4,0x93C6,0x93DE,0x93F8,0x9431,0x9445,0x9448,
	0x9592,0xF9DC,0xFA29,0x969D,0x96AF,0x9733,0x973B,0x9743,
	0x974D,0x974F,0x9751,0x9755,0x9857,0x9865,0xFA2A,0xFA2B,
	0x9927,0xFA2C,0x999E,0x9A4E,0x9AD9};

static int iTblSJIStoUCS_IBMIBM3[] = {
	0x9ADC,0x9B75,0x9B72,0x9B8F,0x9BB1,0x9BBB,0x9C00,0x9D70,
	0x9D6B,0xFA2D,0x9E19,0x9ED1};

// IBM選定IBM拡張文字 のとき UCS に変換して 1 を返す
int fImd_SJIStoUCS_IBMIBM(ImdKcvCode *pkc){
	int	i = pkc->kcode,j;

	if     (0xFA40 <= i && i <= 0xFAFC){
		if(i == 0xFA54 || i == 0xFA5B){
			pkc->housetsu = i;
		}
		j = iTblSJIStoUCS_IBMIBM1[i - 0xFA40];
	}
	else if(0xFB40 <= i && i <= 0xFBFC){
		j = iTblSJIStoUCS_IBMIBM2[i - 0xFB40];
	}
	else if(0xFC40 <= i && i <= 0xFC4B){
		j = iTblSJIStoUCS_IBMIBM3[i - 0xFC40];
	}
	else{return 0;}

	if(j != -1){
		pkc->housetsu = pkc->kcode;
		pkc->kcode = j;
		pkc->codetype = 7;
		return 1;
	}
	return 0;
}

// NEC選定IBM拡張文字(をUCSに変換したもの) のとき
// SJIS に変換して 1 を返す
int fImd_UCStoSJIS_IBMIBM(ImdKcvCode *pkc){
	int	sjis = -1,ucs = pkc->kcode,i;

	for(i = 0; i <= 0xBC; i++){
		if     (iTblSJIStoUCS_IBMIBM1[i] == ucs){
			sjis = 0xFA40 + i; break;
		}
		else if(iTblSJIStoUCS_IBMIBM2[i] == ucs){
			sjis = 0xFB40 + i; break;
		}
		else if(i <= 0x0B){
			if(iTblSJIStoUCS_IBMIBM3[i] == ucs){
				sjis = 0xFC40 + i; break;
	}	}	}

	if(sjis != -1){
		pkc->kcode = sjis;
		pkc->codetype = 7;
		return 1;
	}
	return 0;
}

static int iTblSJIStoUCS_APPLE1[] = {
	0x339C,0x339F,0x339D,0x33A0,0x33A4,0xFF4D,0x33A1,0x33A5,
	0x339E,0x33A2,0x338E,0xFF47,0x338F,0x33C4,0x3396,0x3397,
	0xFF4C,0x3398,0x33B3,0x33B2,0x33B1,0x33B0,0x2109,0x33D4,
	0x33CB,0x3390,0x3385,0x3386,0x3387,0x0000,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,0x2116,0x33CD,0x2121,0x0000,0x2664,
	0x2667,0x2661,0x2662,0x2660,0x2663,0x2665,0x2666,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,0x3020,0x260E,0x3004,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,0x261E,
	0x261C,0x261D,0x261F,0x21C6,0x21C4,0x21C5,0x0000,0x21E8,
	0x21E6,0x21E7,0x21E9,0x0000,0x0000,0x0000,0x0000};

static int iTblSJIStoUCS_APPLE2[] = {
	0x3230,0x322A,0x322B,0x322C,0x322D,0x322E,0x322F,0x3240,
	0x3237,0x3242,0x3243,0x3239,0x323A,0x3231,0x323E,0x3234,
	0x3232,0x323B,0x3236,0x3233,0x3235,0x323C,0x323D,0x323F,
	0x3238,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	0x337B,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,0x0000,0x0000,0x32A4,0x32A5,0x32A6,0x32A7,0x32A8,
	0x32A9,0x3296,0x329D,0x3298,0x329E,0x0000,0x3299,0x3349,
	0x3322,0x334D,0x3314,0x3316,0x3305,0x3333,0x334E,0x3303,
	0x3336,0x3318,0x3315,0x3327,0x3351,0x334A,0x3339,0x3357,
	0x330D,0x3342,0x3323,0x3326,0x333B,0x332B,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,0x3300,0x331E,0x332A,
	0x3331,0x3347,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,0x337E,0x337D,0x337C,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
	    -1,    -1,0x337F,0x0000,0x0000};

// Apple 拡張文字 のとき UCS に変換して 1 を返す
// 但し対応する UCS がなければ変換せずに 2 を返す
int fImd_SJIStoUCS_APPLE(ImdKcvCode *pkc){
	int	sjis = pkc->kcode,ucs = 0,re,x;

	re = 0;
	if(sjis < 0x0100){
		if     (0x00FD==sjis){ucs=0x00A9;}
		else if(0x00FE==sjis){ucs=0x2122;}
		else if(0x00FF==sjis){ucs=0x2026; pkc->housetsu = sjis;}
	}
	else if(sjis < 0x85FF){
		if     (0x8540<=sjis&&sjis<=0x8553){ucs=sjis-0x60E0;}
		else if(0x855E<=sjis&&sjis<=0x8571){ucs=sjis-0x60EA;}
		else if(0x857C<=sjis&&sjis<=0x857E){ucs=sjis-0x5E06;}
		else if(0x8580<=sjis&&sjis<=0x8585){ucs=sjis-0x5E07;}
		else if(0x8591==sjis){re = 2;}
		else if(0x8592<=sjis&&sjis<=0x859A){ucs=sjis-0x610A;}
		else if(0x859F<=sjis&&sjis<=0x85AA){ucs=sjis-0x643F;}
		else if(0x85AB<=sjis&&sjis<=0x85AD){re = 2;}
		else if(0x85B2<=sjis&&sjis<=0x85BD){ucs=sjis-0x6442;}
		else if(0x85BF<=sjis&&sjis<=0x85C1){re = 2;}
		else if(0x85DB<=sjis&&sjis<=0x85F4){ucs=sjis-0x613F;}
	}
	else if(0x8645==sjis){ucs=0xFF4D; pkc->housetsu = sjis;}
	else if(0x864B==sjis){ucs=0xFF47; pkc->housetsu = sjis;}
	else if(0x8650==sjis){ucs=0xFF4C; pkc->housetsu = sjis;}
	  // 包摂情報を org_kcode に保存
	else if(0x8640<=sjis&&sjis<=0x86D6){
		x = iTblSJIStoUCS_APPLE1[sjis-0x8640];
		if(x == 0){re = 2;}
		else if(x != -1){ucs=x;}
	}
	else if(0x8740<=sjis&&sjis<=0x87FC){
		x = iTblSJIStoUCS_APPLE2[sjis-0x8740];
		if(x == 0){re = 2;}
		else if(x != -1){ucs=x;}
	}
	else if(sjis <= 0x886D){
		if     (0x8840==sjis){ucs=0x222E;}
		else if(0x8841==sjis){ucs=0x221F;}
		else if(0x8842==sjis){ucs=0x22BF;}
		else if(0x8854==sjis){ucs=0x301D;}
		else if(0x8855==sjis){ucs=0x301F;}
		else if(0x8868==sjis){ucs=0x3094;}
		else if(0x886A==sjis){ucs=0x30F7;}
		else if(0x886B==sjis){ucs=0x30F8;}
		else if(0x886C==sjis){ucs=0x30F9;}
		else if(0x886D==sjis){ucs=0x30FA;}
	}
	else{
		if     (0xEB41==sjis){re = 2;}
		else if(0xEB42==sjis){re = 2;}
		else if(0xEB50==sjis){re = 2;}
		else if(0xEB51==sjis){ucs=0xFE33;}
		else if(0xEB5B==sjis){re = 2;}
		else if(0xEB5C==sjis){ucs=0xFE31;}
		else if(0xEB5D==sjis){re = 2;}
		else if(0xEB60==sjis){re = 2;}
		else if(0xEB61==sjis){re = 2;}
		else if(0xEB62==sjis){re = 2;}
		else if(0xEB63==sjis){re = 2;}
		else if(0xEB64==sjis){ucs=0xFE30;}
		else if(0xEB69==sjis){ucs=0xFE35;}
		else if(0xEB6A==sjis){ucs=0xFE36;}
		else if(0xEB6B==sjis){ucs=0xFE39;}
		else if(0xEB6C==sjis){ucs=0xFE3A;}
		else if(0xEB6D==sjis){re = 2;}
		else if(0xEB6E==sjis){re = 2;}
		else if(0xEB6F==sjis){ucs=0xFE37;}
		else if(0xEB70==sjis){ucs=0xFE38;}
		else if(0xEB71==sjis){ucs=0xFE3F;}
		else if(0xEB72==sjis){ucs=0xFE40;}
		else if(0xEB73==sjis){ucs=0xFE3D;}
		else if(0xEB74==sjis){ucs=0xFE3E;}
		else if(0xEB75==sjis){ucs=0xFE41;}
		else if(0xEB76==sjis){ucs=0xFE42;}
		else if(0xEB77==sjis){ucs=0xFE43;}
		else if(0xEB78==sjis){ucs=0xFE44;}
		else if(0xEB79==sjis){ucs=0xFE3B;}
		else if(0xEB7A==sjis){ucs=0xFE3C;}
		else if(0xEC9F==sjis){re = 2;}
		else if(0xECA1==sjis){re = 2;}
		else if(0xECA3==sjis){re = 2;}
		else if(0xECA5==sjis){re = 2;}
		else if(0xECA7==sjis){re = 2;}
		else if(0xECE1==sjis){re = 2;}
		else if(0xECE3==sjis){re = 2;}
		else if(0xECE5==sjis){re = 2;}
		else if(0xECEC==sjis){re = 2;}
		else if(0xED40==sjis){re = 2;}
		else if(0xED42==sjis){re = 2;}
		else if(0xED44==sjis){re = 2;}
		else if(0xED46==sjis){re = 2;}
		else if(0xED48==sjis){re = 2;}
		else if(0xED83==sjis){re = 2;}
		else if(0xED85==sjis){re = 2;}
		else if(0xED87==sjis){re = 2;}
		else if(0xED8E==sjis){re = 2;}
		else if(0xED95==sjis){re = 2;}
		else if(0xED96==sjis){re = 2;}
	}

	if(ucs != 0){
		re = 1;
		pkc->kcode = ucs;
		pkc->codetype = 8;
	}
	return re;
}

// Apple 拡張文字(をUCSに変換したもの) のとき
// SJIS に変換して 1 を返す
int fImd_UCStoSJIS_APPLE(ImdKcvCode *pkc){
	int	ucs = pkc->kcode,sjis = -1,i,re;

	if(ucs == 0 || pkc->codetype < 0){return 0;}

	if     (0x00A9==ucs){sjis=0x00FD;}
	else if(0x2122==ucs){sjis=0x00FE;}
	else if(0x2026==ucs){sjis=0x00FF;}

	else if(0x2460<=ucs&&ucs<=0x2473){sjis=ucs+0x60E0;}
	else if(0x2474<=ucs&&ucs<=0x2487){sjis=ucs+0x60EA;}
	else if(0x2776<=ucs&&ucs<=0x2778){sjis=ucs+0x5E06;}
	else if(0x2779<=ucs&&ucs<=0x277E){sjis=ucs+0x5E07;}
	else if(0x2488<=ucs&&ucs<=0x2490){sjis=ucs+0x610A;}
	else if(0x2160<=ucs&&ucs<=0x216B){sjis=ucs+0x643F;}
	else if(0x2170<=ucs&&ucs<=0x217B){sjis=ucs+0x6442;}
	else if(0x249C<=ucs&&ucs<=0x24B5){sjis=ucs+0x613F;}

	else if(0x222E==ucs){sjis=0x8840;}
	else if(0x221F==ucs){sjis=0x8841;}
	else if(0x22BF==ucs){sjis=0x8842;}
	else if(0x301D==ucs){sjis=0x8854;}
	else if(0x301F==ucs){sjis=0x8855;}
	else if(0x3094==ucs){sjis=0x8868;}
	else if(0x30F7==ucs){sjis=0x886A;}
	else if(0x30F8==ucs){sjis=0x886B;}
	else if(0x30F9==ucs){sjis=0x886C;}
	else if(0x30FA==ucs){sjis=0x886D;}

	else if(0xFE33==ucs){sjis=0xEB51;}
	else if(0xFE31==ucs){sjis=0xEB5C;}
	else if(0xFE30==ucs){sjis=0xEB64;}
	else if(0xFE35==ucs){sjis=0xEB69;}
	else if(0xFE36==ucs){sjis=0xEB6A;}
	else if(0xFE39==ucs){sjis=0xEB6B;}
	else if(0xFE3A==ucs){sjis=0xEB6C;}
	else if(0xFE37==ucs){sjis=0xEB6F;}
	else if(0xFE38==ucs){sjis=0xEB70;}
	else if(0xFE3F==ucs){sjis=0xEB71;}
	else if(0xFE40==ucs){sjis=0xEB72;}
	else if(0xFE3D==ucs){sjis=0xEB73;}
	else if(0xFE3E==ucs){sjis=0xEB74;}
	else if(0xFE41==ucs){sjis=0xEB75;}
	else if(0xFE42==ucs){sjis=0xEB76;}
	else if(0xFE43==ucs){sjis=0xEB77;}
	else if(0xFE44==ucs){sjis=0xEB78;}
	else if(0xFE3B==ucs){sjis=0xEB79;}
	else if(0xFE3C==ucs){sjis=0xEB7A;}

	for(i=0;i<=0x96;i++){
		if(iTblSJIStoUCS_APPLE1[i] == ucs){
			sjis=0x8640+i; break;
	}	}
	for(i=0;i<=0xBC;i++){
		if(iTblSJIStoUCS_APPLE2[i] == ucs){
			sjis=0x8740+i; break;
	}	}

	if(sjis != -1){
		pkc->kcode = sjis;
		pkc->codetype = 8;
		re = 1;
	}
	else{re = 0;}
	return re;
}

int fImd_SJISrecover_NEC(int i){
	int		re = 0;

	if((i==0x8790)||(i==0x8791)||(i==0x8792)||
	   (i==0x8795)||(i==0x8796)||(i==0x8797)||
	   (i==0x879A)||(i==0x879B)||(i==0x879C)){
		re = 1;
	}else if(i==0xEEF9){
		re = 1;
	}else if((0xFA40<=i)&&(i<=0xFA7E)||(0xFA80<=i)&&(i<=0xFAFC)||
		 (0xFB40<=i)&&(i<=0xFB7E)||(0xFB80<=i)&&(i<=0xFBFC)||
		 (0xFC40<=i)&&(i<=0xFC4B)){
		re = 1;
	}
	return re;
}

int fImd_SJISrecover_APPLE(int i){
	int		re = 0;

	if     (i == 0x00FF){re = 1;}
	else if((i == 0x8645)||(i == 0x864B)||(i == 0x8650))
		{re = 1;}
	return re;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_SJISNtoUCS クラス                           ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_SJISNtoUCS::convert(ImdKcvCode *pkc){
	ImdKcvCode	*pre = NULL;
	int		iJis,iSjis,iUcs;

	if(pkc->codetype < 0){return pkc;}
	iSjis = pkc->kcode;

	if     (fImd_SJIStoUCS_NEC(pkc) != 0){pre = pkc;}
	else if(fImd_SJIStoUCS_NECIBM(pkc) != 0){pre = pkc;}
	else if(fImd_SJIStoUCS_IBMIBM(pkc) != 0){pre = pkc;}
	else if(fImd_JISX0201toUCS(pkc) != 0){pre = pkc;}
	  // US-ASCII と 半角カナ の範囲
	else if(fImd_SJIStoJIS(iSjis,&iJis) != 0){
		// JISX0208範囲
		fImd_JISX0208toUCS(iJis,&iUcs);
		pkc->kcode = iUcs; pkc->codetype = 4;
		pre = pkc;
	}
	else{// 変換できなかった場合
		pkc->codetype = -2; pre = pkc;
	}

	return pre;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_UCStoSJISN クラス                           ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_UCStoSJISN::convert(ImdKcvCode *pkc){
	ImdKcvCode	*pre = NULL;
	int		iJis,iSjis,iUcs,i;

	i = flags & 0xF;
	if(i==1 && pkc->housetsu != 0){
		if((i = fImd_SJISrecover_NEC(pkc->housetsu)) != 0){
			pkc->kcode = pkc->housetsu;
			if(pkc->housetsu < 0x87FD){pkc->codetype = 5;}
			else if(pkc->housetsu < 0xEFFD){pkc->codetype = 6;}
			else{pkc->codetype = 7;}
			return pkc;
	}	}

	if(pkc->codetype < 0){return pkc;}
	iUcs = pkc->kcode;

	if     (fImd_UCStoJISX0201(pkc) != 0){pre = pkc;}
	  // US-ASCII と 半角カナ の範囲
	else if(fImd_UCStoJISX0208(iUcs,&iJis) != 0){
		// JISX0208範囲
		fImd_JIStoSJIS(iJis,&iSjis);
		pkc->kcode = iSjis; pkc->codetype = 4;
		pre = pkc;
	}
	else if(fImd_UCStoSJIS_NEC(pkc) != 0){pre = pkc;}
	else if(fImd_UCStoSJIS_NECIBM(pkc) != 0){pre = pkc;}
	else if(fImd_UCStoSJIS_IBMIBM(pkc) != 0){pre = pkc;}
	else{// 変換できなかった場合
		pkc->kcode = 0x8145;
		pkc->codetype = -2; pre = pkc;
	}

	return pre;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_SJISAtoUCS クラス                           ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_SJISAtoUCS::convert(ImdKcvCode *pkc){
	ImdKcvCode	*pre = NULL;
	int		iJis,iSjis,iUcs,re;

	if(pkc->codetype < 0){return pkc;}
	iSjis = pkc->kcode;

	if     ((re = fImd_SJIStoUCS_APPLE(pkc)) != 0){
		if(re == 2){ // 対応する UCS が無い場合
			pkc->codetype = -3;
		}
		pre = pkc;
	}
	else if(fImd_JISX0201toUCS(pkc) != 0){pre = pkc;}
	  // US-ASCII と 半角カナ の範囲
	else if(fImd_SJIStoJIS(iSjis,&iJis) != 0){
		// JISX0208範囲
		fImd_JISX0208toUCS(iJis,&iUcs);
		pkc->kcode = iUcs; pkc->codetype = 4;
		pre = pkc;
	}
	else{// 変換できなかった場合
		pkc->codetype = -2; pre = pkc;
	}

	return pre;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_UCStoSJISA クラス                           ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_UCStoSJISA::convert(ImdKcvCode *pkc){
	ImdKcvCode	*pre = NULL;
	int		iJis,iSjis,iUcs,i;

	i = flags & 0xF;
	if(i==1 && pkc->housetsu != 0){
		if((i = fImd_SJISrecover_APPLE(pkc->housetsu)) != 0){
			pkc->kcode = pkc->housetsu;
			pkc->codetype = 8; return pkc;
	}	}

	if(pkc->codetype == -3){ // 非 UCS 文字リカバリ
		pkc->codetype = 8;
		pre = pkc->copy();
		if(fImd_SJIStoUCS_APPLE(pre) != 2){pkc->codetype = -2;}
		delete pre; return pkc;
	}
	else if(pkc->codetype < 0){return pkc;}
	iUcs = pkc->kcode;

	if     (fImd_UCStoJISX0201(pkc) != 0){pre = pkc;}
	  // US-ASCII と 半角カナ の範囲
	else if(fImd_UCStoJISX0208(iUcs,&iJis) != 0){
		// JISX0208範囲
		fImd_JIStoSJIS(iJis,&iSjis);
		pkc->kcode = iSjis; pkc->codetype = 4;
		pre = pkc;
	}
	else if(fImd_UCStoSJIS_APPLE(pkc) != 0){pre = pkc;}
	  // 機種依存文字
	else{// 変換できなかった場合
		pkc->codetype = -2; pre = pkc;
	}

	return pre;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_SJISN_filter クラス                         ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// NEC特殊文字・NEC選定IBM拡張文字・IBM選定IBM拡張文字
// のどれかだったら codetype を -2(変換失敗) に変換するフィルタ
ImdKcvCode* Imd_SJISN_filter::convert(ImdKcvCode *pkc){
	int		i,j;
	ImdKcvCode	kc2;

	if(pkc->codetype < 0){return pkc;}


	// 1. NEC 拡張文字
	// kc2 に引数のコピーを作成して変換しようとして見る
	kc2.kcode = j = pkc->kcode;
	kc2.codetype = pkc->codetype;
	i = fImd_SJIStoUCS_NEC(&kc2);
	if(i == 1){
		// 包摂回避を更に回避 (＝包摂回避をしない)
		if      (j == 0x8790){
			pkc->kcode = 0x81E0; pkc->codetype = 4;
		}else if(j == 0x8791){
			pkc->kcode = 0x81DF; pkc->codetype = 4;
		}else if(j == 0x8792){
			pkc->kcode = 0x81E7; pkc->codetype = 4;
		}else if(j == 0x8795){
			pkc->kcode = 0x81E3; pkc->codetype = 4;
		}else if(j == 0x8796){
			pkc->kcode = 0x81DB; pkc->codetype = 4;
		}else if(j == 0x8797){
			pkc->kcode = 0x81DA; pkc->codetype = 4;
		}else if(j == 0x879A){
			pkc->kcode = 0x81E6; pkc->codetype = 4;
		}else if(j == 0x879B){
			pkc->kcode = 0x81BF; pkc->codetype = 4;
		}else if(j == 0x879C){
			pkc->kcode = 0x81BE; pkc->codetype = 4;
		}else{pkc->codetype = -2;}
	}
	else{
	// 2. NEC選定IBM拡張文字
		kc2.kcode = pkc->kcode;
		kc2.codetype = pkc->codetype;
		i = fImd_SJIStoUCS_NECIBM(&kc2);
		if(i == 1){
			// 包摂回避を更に回避
			if(j == 0xEEF9){
				pkc->kcode = 0x81CA;
				pkc->codetype = 4;
			}else{pkc->codetype = -2;}
		}else{
	// 3. IBM選定IBM拡張文字
			kc2.kcode = pkc->kcode;
			kc2.codetype = pkc->codetype;
			i = fImd_SJIStoUCS_IBMIBM(&kc2);
			if(i == 1){
				// 包摂回避を更に回避
				if      (j == 0xFA54){
					pkc->kcode = 0x81CA;
					pkc->codetype = 4;
				}else if(j == 0xFA5B){
					pkc->kcode = 0x81E6;
					pkc->codetype = 4;
				}else{pkc->codetype = -2;}
	}	}	}
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_SJISA_filter クラス                         ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// Apple拡張文字だったら codetype を -2(変換失敗) に変換する
// フィルタ
ImdKcvCode* Imd_SJISA_filter::convert(ImdKcvCode *pkc){
	int		i,j;
	ImdKcvCode	kc2;

	if(pkc->codetype < 0){return pkc;}
	if(0xFD <= pkc->kcode && pkc->kcode <= 0xFF){
		// 包摂回避を更に回避
		if(pkc->kcode == 0xFF){
			pkc->kcode = 0x8163;
			pkc->codetype = 4;
		}else{pkc->codetype = -2;}
	}
	else{
		kc2.kcode = j = pkc->kcode;
		kc2.codetype = pkc->codetype;
		i = fImd_SJIStoUCS_APPLE(&kc2);
		if(i != 0){
			// 包摂回避を更に回避
			if     (j == 0x8645){
				pkc->kcode = 0x828D;
				pkc->codetype = 4;
			}else if(j == 0x864B){
				pkc->kcode = 0x8287;
				pkc->codetype = 4;
			}else if(j == 0x8650){
				pkc->kcode = 0x828C;
				pkc->codetype = 4;
			}else{pkc->codetype = -2;}
	}	}
	return pkc;
}
