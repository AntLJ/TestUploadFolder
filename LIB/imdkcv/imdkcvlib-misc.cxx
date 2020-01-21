// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcvlib-misc.cxx (C++ Source File)                 CcCcC
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

//JISX0201
// int fImd_JISX0201toUCS(ImdKcvCode *pkc);
// int fImd_UCStoJISX0201(ImdKcvCode *pkc);
//ISO-8859-1～10,13,14,15
// int fImd_UCSto8859(ImdKcvCode *pkc);
// int fImd_8859toUCS(ImdKcvCode *pkc);

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  補助関数・定数群                                ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// 変換成功した場合 0 以外を返す
int fImd_JISX0201toUCS(ImdKcvCode *pkc){
	int		re = 0,iJis = pkc->kcode;

	if(pkc->codetype != 0 && pkc->codetype != 1 &&
		pkc->codetype != 2 && pkc->codetype != 3){return re;}
	if(iJis <= 0x7E){
		re = 1;
		if(pkc->codetype == 2){
			if(iJis == 0x5C){
				  // JISX0201 では 0x5C は 半角￥記号
				  // (半角バックスラッシュではない)
				pkc->kcode = 0x00A5;
				pkc->codetype = 1;
			}
			else if(iJis == 0x7E){
				  // JISX0201 では 0x7E は 半角￣記号
				  // (半角チルダではない)
				pkc->kcode = 0x203E;
				pkc->codetype = 4;
			}
			else{pkc->codetype = 1;}
		}
		else{pkc->codetype = 1;}
	}
	else if(0xA1 <= iJis && iJis <= 0xDF){
		  // 半角カナの処理
		pkc->codetype = 3; re = 1;
		pkc->kcode = iJis + 0xFEC0;
	}
	else if(0x80 <= iJis && iJis <= 0xFF){
		pkc->kcode = 1; re = 1;
	}

	return re;
}

int fImd_UCStoJISX0201(ImdKcvCode *pkc){
	int		re = 0, iUcs = pkc->kcode;

	if(iUcs == 0x00A5){
		// JISX0201 では 0x5C は 半角￥記号
		// (半角バックスラッシュではない)
		pkc->kcode = 0x5C;
		pkc->codetype = 2; re = 1;
	}
	else if(iUcs == 0x203E){
		// JISX0201 では 0x7E は 半角￣記号
		// (半角チルダではない)
		pkc->kcode = 0x7E;
		pkc->codetype = 2; re = 1;
	}
	else if(0xFF61 <= iUcs && iUcs <= 0xFF9F){
		// 半角カナ
		pkc->codetype = 3; re = 1;
		pkc->kcode = iUcs - 0xFEC0;
	}
	else if(0 <= iUcs && iUcs <= 0x9F){
		// C0,US-ASCII,C1
		pkc->codetype = 1; re = 1;
	}

	return re;
}

int iTblISO8859_2[]={
	0x00A0,0x0104,0x02D8,0x0141,0x00A4,0x013D,0x015A,0x00A7,
	0x00A8,0x0160,0x015E,0x0164,0x0179,0x00AD,0x017D,0x017B,
	0x00B0,0x0105,0x02DB,0x0142,0x00B4,0x013E,0x015B,0x02C7,
	0x00B8,0x0161,0x015F,0x0165,0x017A,0x02DD,0x017E,0x017C,
	0x0154,0x00C1,0x00C2,0x0102,0x00C4,0x0139,0x0106,0x00C7,
	0x010C,0x00C9,0x0118,0x00CB,0x011A,0x00CD,0x00CE,0x010E,
	0x0110,0x0143,0x0147,0x00D3,0x00D4,0x0150,0x00D6,0x00D7,
	0x0158,0x016E,0x00DA,0x0170,0x00DC,0x00DD,0x0162,0x00DF,
	0x0155,0x00E1,0x00E2,0x0103,0x00E4,0x013A,0x0107,0x00E7,
	0x010D,0x00E9,0x0119,0x00EB,0x011B,0x00ED,0x00EE,0x010F,
	0x0111,0x0144,0x0148,0x00F3,0x00F4,0x0151,0x00F6,0x00F7,
	0x0159,0x016F,0x00FA,0x0171,0x00FC,0x00FD,0x0163,0x02D9};

int iTblISO8859_3[]={
	0x00A0,0x0126,0x02D8,0x00A3,0x00A4,0x013D,0x0124,0x00A7,
	0x00A8,0x0130,0x015E,0x011E,0x0134,0x00AD,0x017D,0x017B,
	0x00B0,0x0127,0x00B2,0x00B3,0x00B4,0x00B5,0x0125,0x00B7,
	0x00B8,0x0131,0x015F,0x011F,0x0135,0x00BD,0x017E,0x017C,
	0x00C0,0x00C1,0x00C2,0x0102,0x00C4,0x010A,0x0108,0x00C7,
	0x00C8,0x00C9,0x00CA,0x00CB,0x00CC,0x00CD,0x00CE,0x00CF,
	0x0110,0x00D1,0x00D2,0x00D3,0x00D4,0x0120,0x00D6,0x00D7,
	0x011C,0x00D9,0x00DA,0x00DB,0x00DC,0x016C,0x015C,0x00DF,
	0x00E0,0x00E1,0x00E2,0x0103,0x00E4,0x010B,0x0109,0x00E7,
	0x00E8,0x00E9,0x00EA,0x00EB,0x00EC,0x00ED,0x00EE,0x00EF,
	0x0111,0x00F1,0x00F2,0x00F3,0x00F4,0x0121,0x00F6,0x00F7,
	0x011D,0x00F9,0x00FA,0x00FB,0x00FC,0x016D,0x015D,0x02D9};

int iTblISO8859_4[]={
	0x00A0,0x0104,0x0138,0x0156,0x00A4,0x0128,0x013B,0x00A7,
	0x00A8,0x0160,0x0112,0x0122,0x0166,0x00AD,0x017D,0x00AF,
	0x00B0,0x0105,0x02DB,0x0157,0x00B4,0x0129,0x013C,0x02C7,
	0x00B8,0x0161,0x0113,0x0123,0x0167,0x014A,0x017E,0x014B,
	0x0100,0x00C1,0x00C2,0x00C3,0x00C4,0x00C5,0x00C6,0x012E,
	0x010C,0x00C9,0x0118,0x00CB,0x0116,0x00CD,0x00CE,0x012A,
	0x0110,0x0145,0x014C,0x0136,0x00D4,0x00D5,0x00D6,0x00D7,
	0x00D8,0x0172,0x00DA,0x00DB,0x00DC,0x0168,0x016A,0x00DF,
	0x0101,0x00E1,0x00E2,0x00E3,0x00E4,0x00E5,0x00E6,0x012F,
	0x010D,0x00E9,0x0119,0x00EB,0x0117,0x00ED,0x00EE,0x012B,
	0x0111,0x0146,0x014D,0x0137,0x00F4,0x00F5,0x00F6,0x00F7,
	0x00F8,0x0173,0x00FA,0x00FB,0x00FC,0x0169,0x016B,0x02D9};

int iTblISO8859_5[]={
	0x00A0,0x0401,0x0402,0x0403,0x0404,0x0405,0x0406,0x0407,
	0x0408,0x0409,0x040A,0x040B,0x040C,0x00AD,0x040E,0x040F,
	0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,
	0x0418,0x0419,0x041A,0x041B,0x041C,0x041D,0x041E,0x041F,
	0x0420,0x0421,0x0422,0x0423,0x0424,0x0425,0x0426,0x0427,
	0x0428,0x0429,0x042A,0x042B,0x042C,0x042D,0x042E,0x042F,
	0x0430,0x0431,0x0432,0x0433,0x0434,0x0435,0x0436,0x0437,
	0x0438,0x0439,0x043A,0x043B,0x043C,0x043D,0x043E,0x043F,
	0x0440,0x0441,0x0442,0x0443,0x0444,0x0445,0x0446,0x0447,
	0x0448,0x0449,0x044A,0x044B,0x044C,0x044D,0x044E,0x044F,
	0x2116,0x0451,0x0452,0x0453,0x0454,0x0455,0x0456,0x0457,
	0x0458,0x0459,0x045A,0x045B,0x045C,0x00A7,0x045E,0x045F};

int iTblISO8859_6[]={
	0x00A0,0x0401,0x0402,0x0403,0x00A4,0x0405,0x0406,0x0407,
	0x0408,0x0409,0x040A,0x040B,0x060C,0x00AD,0x040E,0x040F,
	0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,
	0x0418,0x0419,0x041A,0x061B,0x041C,0x041D,0x041E,0x061F,
	0x0420,0x0621,0x0622,0x0623,0x0624,0x0625,0x0626,0x0627,
	0x0628,0x0629,0x062A,0x062B,0x062C,0x062D,0x062E,0x062F,
	0x0630,0x0631,0x0632,0x0633,0x0634,0x0635,0x0636,0x0637,
	0x0638,0x0639,0x063A,0x043B,0x043C,0x043D,0x043E,0x043F,
	0x0640,0x0641,0x0642,0x0643,0x0644,0x0645,0x0646,0x0647,
	0x0648,0x0649,0x064A,0x064B,0x064C,0x064D,0x064E,0x064F,
	0x0650,0x0651,0x0652,0x0453,0x0454,0x0455,0x0456,0x0457,
	0x0458,0x0459,0x045A,0x045B,0x045C,0x00A7,0x045E,0x045F};

int iTblISO8859_7[]={
	0x00A0,0x2018,0x2019,0x00A3,0x00A4,0x0405,0x00A6,0x00A7,
	0x00A8,0x00A9,0x040A,0x00AB,0x00AC,0x00AD,0x040E,0x2015,
	0x00B0,0x00B1,0x00B2,0x00B3,0x0384,0x0385,0x0386,0x00B7,
	0x0388,0x0389,0x038A,0x00BB,0x038C,0x00BD,0x038E,0x038F,
	0x0390,0x0391,0x0392,0x0393,0x0394,0x0395,0x0396,0x0397,
	0x0398,0x0399,0x039A,0x039B,0x039C,0x039D,0x039E,0x039F,
	0x03A0,0x03A1,0x0632,0x03A3,0x03A4,0x03A5,0x03A6,0x03A7,
	0x03A8,0x03A9,0x03AA,0x03AB,0x03AC,0x03AD,0x03AE,0x03AF,
	0x03B0,0x03B1,0x03B2,0x03B3,0x03B4,0x03B5,0x03B6,0x03B7,
	0x03B8,0x03B9,0x03BA,0x03BB,0x03BC,0x03BD,0x03BE,0x03BF,
	0x03C0,0x03C1,0x03C2,0x03C3,0x03C4,0x03C5,0x03C6,0x03C7,
	0x03C8,0x03C9,0x03CA,0x03CB,0x03CC,0x03CD,0x03CE,0x045F};

int iTblISO8859_8[]={
	0x00A0,0x2018,0x00A2,0x00A3,0x00A4,0x00A5,0x00A6,0x00A7,
	0x00A8,0x00A9,0x00D7,0x00AB,0x00AC,0x00AD,0x00AE,0x203E,
	0x00B0,0x00B1,0x00B2,0x00B3,0x00B4,0x00B5,0x00B6,0x00B7,
	0x00B8,0x00B9,0x00F7,0x00BB,0x00BC,0x00BD,0x00BE,0x038F,
	0x0390,0x0391,0x0392,0x0393,0x0394,0x0395,0x0396,0x0397,
	0x0398,0x0399,0x039A,0x039B,0x039C,0x039D,0x039E,0x039F,
	0x03A0,0x03A1,0x0632,0x03A3,0x03A4,0x03A5,0x03A6,0x03A7,
	0x03A8,0x03A9,0x03AA,0x03AB,0x03AC,0x03AD,0x03AE,0x2017,
	0x05D0,0x05D1,0x05D2,0x05D3,0x05D4,0x05D5,0x05D6,0x05D7,
	0x05D8,0x05D9,0x05DA,0x05DB,0x05DC,0x05DD,0x05DE,0x05DF,
	0x05E0,0x05E1,0x05E2,0x05E3,0x05E4,0x05E5,0x05E6,0x05E7,
	0x05E8,0x05E9,0x05EA,0x03CB,0x03CC,0x03CD,0x03CE,0x045F};

int iTblISO8859_9[]={
	0x00A0,0x00A1,0x00A2,0x00A3,0x00A4,0x00A5,0x00A6,0x00A7,
	0x00A8,0x00A9,0x00AA,0x00AB,0x00AC,0x00AD,0x00AE,0x00AF,
	0x00B0,0x00B1,0x00B2,0x00B3,0x00B4,0x00B5,0x00B6,0x00B7,
	0x00B8,0x00B9,0x00BA,0x00BB,0x00BC,0x00BD,0x00BE,0x00BF,
	0x00C0,0x00C1,0x00C2,0x00C3,0x00C4,0x00C5,0x00C6,0x00C7,
	0x00C8,0x00C9,0x00CA,0x00CB,0x00CC,0x00CD,0x00CE,0x00CF,
	0x011E,0x00D1,0x00D2,0x00D3,0x00D4,0x00D5,0x00D6,0x00D7,
	0x00D8,0x00D9,0x00DA,0x00DB,0x00DC,0x0130,0x015E,0x00DF,
	0x00E0,0x00E1,0x00E2,0x00E3,0x00E4,0x00E5,0x00E6,0x00E7,
	0x00E8,0x00E9,0x00EA,0x00EB,0x00EC,0x00ED,0x00EE,0x00EF,
	0x011F,0x00F1,0x00F2,0x00F3,0x00F4,0x00F5,0x00F6,0x00F7,
	0x00F8,0x00F9,0x00FA,0x00FB,0x00FC,0x0131,0x015F,0x00FF};

int iTblISO8859_10[]={
	0x00A0,0x0104,0x0112,0x0122,0x0124,0x0128,0x0136,0x00A7,
	0x013B,0x0110,0x0160,0x0166,0x017D,0x00AD,0x016A,0x014A,
	0x00B0,0x0105,0x0113,0x0123,0x012B,0x0129,0x0137,0x00B7,
	0x013C,0x0111,0x0161,0x0167,0x017E,0x2015,0x016B,0x014B,
	0x0100,0x00C1,0x00C2,0x00C3,0x00C4,0x00C5,0x00C6,0x012E,
	0x010C,0x00C9,0x0118,0x00CB,0x0116,0x00CD,0x00CE,0x00CF,
	0x00D0,0x0145,0x014C,0x00D3,0x00D4,0x00D5,0x00D6,0x0168,
	0x00D8,0x0172,0x00DA,0x00DB,0x00DC,0x00DD,0x00DE,0x00DF,
	0x0101,0x00E1,0x00E2,0x00E3,0x00E4,0x00E5,0x00E6,0x012F,
	0x010D,0x00E9,0x0119,0x00EB,0x0117,0x00ED,0x00EE,0x00EF,
	0x00F0,0x0146,0x014D,0x00F3,0x00F4,0x00F5,0x00F6,0x0169,
	0x00F8,0x0173,0x00FA,0x00FB,0x00FC,0x00FD,0x00FE,0x0138};

int iTblISO8859_13[]={
	0x00A0,0x201D,0x00A2,0x00A3,0x00A4,0x201E,0x00A6,0x00A7,
	0x00D8,0x00A9,0x0156,0x00AB,0x00AC,0x00AD,0x00AE,0x00C6,
	0x00B0,0x00B1,0x00B2,0x00B3,0x201C,0x00B5,0x00B6,0x00B7,
	0x00F8,0x00B9,0x0157,0x00BB,0x00BC,0x00BD,0x00BE,0x00E6,
	0x0104,0x012E,0x0100,0x0106,0x00C4,0x00C5,0x0118,0x0112,
	0x010C,0x00C9,0x0179,0x0116,0x0122,0x0136,0x012A,0x013B,
	0x0160,0x0143,0x0145,0x00D3,0x014C,0x00D5,0x00D6,0x00D7,
	0x0172,0x0141,0x015A,0x016A,0x00DC,0x017B,0x017D,0x00DF,
	0x0105,0x012F,0x0101,0x0107,0x00E4,0x00E5,0x0119,0x0113,
	0x010D,0x00E9,0x017A,0x0117,0x0123,0x0137,0x012B,0x013C,
	0x0161,0x0144,0x0146,0x00F3,0x014D,0x00F5,0x00F6,0x00F7,
	0x0173,0x0142,0x015B,0x016B,0x00FC,0x017C,0x017E,0x2019};

int iTblISO8859_14[]={
	0x00A0,0x1E02,0x1E03,0x00A3,0x010A,0x010B,0x1E0A,0x00A7,
	0x1E80,0x00A9,0x1E82,0x1E0B,0x1EF2,0x00AD,0x00AE,0x0178,
	0x1E1E,0x1E1F,0x0120,0x0121,0x1E40,0x1E41,0x00B6,0x1E56,
	0x1E81,0x1E57,0x1E83,0x1E60,0x1EF3,0x1E84,0x1E85,0x1E61,
	0x00C0,0x00C1,0x00C2,0x00C3,0x00C4,0x00C5,0x00C6,0x00C7,
	0x00C8,0x00C9,0x00CA,0x00CB,0x00CC,0x00CD,0x00CE,0x00CF,
	0x0174,0x00D1,0x00D2,0x00D3,0x00D4,0x00D5,0x00D6,0x1E6A,
	0x00D8,0x00D9,0x00DA,0x00DB,0x00DC,0x00DD,0x0176,0x00DF,
	0x00E0,0x00E1,0x00E2,0x00E3,0x00E4,0x00E5,0x00E6,0x00E7,
	0x00E8,0x00E9,0x00EA,0x00EB,0x00EC,0x00ED,0x00EE,0x00EF,
	0x0175,0x00F1,0x00F2,0x00F3,0x00F4,0x00F5,0x00F6,0x1E6B,
	0x00F8,0x00F9,0x00FA,0x00FB,0x00FC,0x00FD,0x0177,0x00FF};

int iTblISO8859_15[]={
	0x00A0,0x00A1,0x00A2,0x00A3,0x20AC,0x00A5,0x0160,0x00A7,
	0x0161,0x00A9,0x00AA,0x00AB,0x00AC,0x00AD,0x00AE,0x00AF,
	0x00B0,0x00B1,0x00B2,0x00B3,0x017D,0x00B5,0x00B6,0x00B7,
	0x017E,0x00B9,0x00BA,0x00BB,0x0152,0x0153,0x0178,0x00BF,
	0x00C0,0x00C1,0x00C2,0x00C3,0x00C4,0x00C5,0x00C6,0x00C7,
	0x00C8,0x00C9,0x00CA,0x00CB,0x00CC,0x00CD,0x00CE,0x00CF,
	0x00D0,0x00D1,0x00D2,0x00D3,0x00D4,0x00D5,0x00D6,0x00D7,
	0x00D8,0x00D9,0x00DA,0x00DB,0x00DC,0x00DD,0x00DE,0x00DF,
	0x00E0,0x00E1,0x00E2,0x00E3,0x00E4,0x00E5,0x00E6,0x00E7,
	0x00E8,0x00E9,0x00EA,0x00EB,0x00EC,0x00ED,0x00EE,0x00EF,
	0x00F0,0x00F1,0x00F2,0x00F3,0x00F4,0x00F5,0x00F6,0x00F7,
	0x00F8,0x00F9,0x00FA,0x00FB,0x00FC,0x00FD,0x00FE,0x00FF};

int *iTblISO8859_A[]={
	iTblISO8859_2, iTblISO8859_3, iTblISO8859_4, iTblISO8859_5,
	iTblISO8859_6, iTblISO8859_7, iTblISO8859_8, iTblISO8859_9,
	iTblISO8859_10};

int *iTblISO8859_B[]={
	iTblISO8859_13,iTblISO8859_14,iTblISO8859_15};

int fImd_8859toUCS(ImdKcvCode *pkc){
	int	i,j;

	if(885901 <= pkc->codetype && pkc->codetype <= 885910){
		if(!(0x20 <= pkc->kcode && pkc->kcode <= 0x7F)){
			pkc->codetype = -2;
			return 1;
		}
		if(pkc->codetype == 885901){
			pkc->kcode = pkc->kcode | 0x80;
		}
		else{
			i = pkc->codetype - 885902;
			j = pkc->kcode - 0x20;
			pkc->kcode = iTblISO8859_A[i][j];
		}
		return 1;
	}
	if(885913 <= pkc->codetype && pkc->codetype <= 885915){
		if(!(0x20 <= pkc->kcode && pkc->kcode <= 0x7F)){
			pkc->codetype = -2;
			return 1;
		}
		i = pkc->codetype - 885913;
		j = pkc->kcode - 0x20;
		pkc->kcode = iTblISO8859_B[i][j];
		return 1;
	}
	return 0;
}

int fImd_UCSto8859(ImdKcvCode *pkc){
	int	i,j;

	if(885902 <= pkc->codetype && pkc->codetype <= 885910){
		i = pkc->codetype - 885902;

		for(j=0;j<0x60;j++){
			if(iTblISO8859_B[i][j] == pkc->kcode){
				pkc->kcode = 0x20 + j;
				return 1;
	}	}	}
	if(885913 <= pkc->codetype && pkc->codetype <= 885915){
		i = pkc->codetype - 885913;

		for(j=0;j<0x60;j++){
			if(iTblISO8859_B[i][j] == pkc->kcode){
				pkc->kcode = 0x20 + j;
				return 1;
	}	}	}

	return 0;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_EUCtoJIS クラス                             ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_EUCtoJIS::convert(ImdKcvCode *pkc){

	if(pkc->codetype < 0){} // 作業無し
	else if((pkc->kcode & 0xFF00) != 0){
		pkc->kcode &= 0x7F7F;
	}
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_JIStoEUC クラス                             ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_JIStoEUC::convert(ImdKcvCode *pkc){

	if(pkc->codetype < 0){} // 作業無し
	else if((pkc->kcode & 0xFF00) != 0){
		pkc->kcode |= 0x8080;
	}
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_NLandLF_converter クラス                    ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_NLandLF_converter::_onls(int imode){
	ImdKcvCode 	*pre = NULL,*p;
	int			i;

	if(mode & 2){
		if(imode == 1){i = 0x0A;}else{i = 0x0D;}
		p = new ImdKcvCode(&pre,0x0D,i,/*codetype*/0);
	}
	if(mode & 1){
		if(imode == 2){i = 0x0D;}else{i = 0x0A;}
		p = new ImdKcvCode(&pre,0x0A,i,/*codetype*/0);
	}
	return pre;
}

// 改行コード変換モジュール
ImdKcvCode* Imd_NLandLF_converter::convert(ImdKcvCode *pkc){
	ImdKcvCode	*pre = NULL;
	int		ex = 0;

	if(pkc->codetype < 0){
		if(isv == 0x0D){
			pre = _onls(2); isv = 0;
			pkc->cat(&pre);
		}
		else{pre = pkc;}
	}
	else if(isv == 0x0D){
		isv = 0;
		if(pkc->kcode != 0x0A){
			pre = _onls(2);
			if(pkc->kcode != 0x0D){pkc->cat(&pre);}
			else{isv = 0x0D; delete pkc;}
		}
		else{pre = _onls(3);}
	}
	else if(pkc->kcode == 0x0D){isv = 0x0D; delete pkc;}
	else if(pkc->kcode == 0x0A){
		pre = _onls(1); delete pkc;
	}
	else{pre = pkc;}

	return pre;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_JISX0201KANAtoJISX0208 クラス               ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// 半角カタカナ -> 全角カタカナ(UCS)
static int iTblKANA_ZK[] = {
	0x0000,0x3002,0x300C,0x300D, 0x3001,0x30FB,0x30F2,0x30A1,
	0x30A3,0x30A5,0x30A7,0x30A9, 0x30E3,0x30E5,0x30E7,0x30C3,
	0x30FC,0x30A2,0x30A4,0x30A6, 0x30A8,0x30AA,0x30AB,0x30AD,
	0x30AF,0x30B1,0x30B3,0x30B5, 0x30B7,0x30B9,0x30BB,0x30BD,
	0x30BF,0x30C1,0x30C4,0x30C6, 0x30C8,0x30CA,0x30CB,0x30CC,
	0x30CD,0x30CE,0x30CF,0x30D2, 0x30D5,0x30D8,0x30DB,0x30DE,
	0x30DF,0x30E0,0x30E1,0x30E2, 0x30E4,0x30E6,0x30E8,0x30E9,
	0x30EA,0x30EB,0x30EC,0x30ED, 0x30EF,0x30F3,0x309B,0x309C};

// 半角カタカナ -> 全角ひらがな(UCS)
static int iTblKANA_ZH[] = {
	0x0000,0x3002,0x300C,0x300D, 0x3001,0x30FB,0x3092,0x3041,
	0x3043,0x3045,0x3047,0x3049, 0x3083,0x3085,0x3087,0x3063,
	0x30FC,0x3042,0x3044,0x3046, 0x3048,0x304A,0x304B,0x304D,
	0x304F,0x3051,0x3053,0x3055, 0x3057,0x3059,0x305B,0x305D,
	0x305F,0x3061,0x3064,0x3066, 0x3068,0x306A,0x306B,0x306C,
	0x306D,0x306E,0x3067,0x3072, 0x3075,0x3078,0x307B,0x307E,
	0x307F,0x3080,0x3081,0x3082, 0x3084,0x3086,0x3088,0x3089,
	0x308A,0x308B,0x308C,0x308D, 0x308F,0x3093,0x309B,0x309C};

static char cTblDaku[] = /*
 0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF */
"      K            B  YYYYYYYYYYYYYYY     YYYYY             K   ";
static char cTblHandaku[] = /*
 0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF */
"                                          YYYYY                 ";
// 半角カナ(JISX0201カナ) -> 全角カナ 変換モジュール
// (Apple Macintosh 拡張, カタカナ／ひらがな出力 両対応)
// 入力・出力とも UCS です。
ImdKcvCode* Imd_JISX0201KANAtoJISX0208::convert(ImdKcvCode *pkc){
	int		ucode,i;
	ImdKcvCode	*pre = NULL,*pkci;

	ucode = pkc->kcode;
	if(ucode < 0xFF61 || 0xFF9F < ucode || pkc->codetype < 0){
		  // 半角カタカナで無い場合は引数をそのまま返す
		if(isv != 0){ // isv に貯めこんだものも出力
			if(bHiragana==0){ucode=iTblKANA_ZK[isv];}
			else{ucode=iTblKANA_ZH[isv];}
			pre = new ImdKcvCode(ucode,isv+0xFF60,4);
			isv = 0;
		}
		pkc->cat(&pre);
	}
	else{
		ucode = ucode - 0xFF60;
		if(isv != 0 && ucode == 0x3E){
			i = 0; // 処理済かどうか -1:処理済
			if(isv == 0x13){ // "う/ウ"に濁点
				if(bHiragana!=0 && bApple!=0){
					pkc->kcode = 0x3094;
					pkc->org_kcode = 0xFF73;
					pkc->codetype = 8;
					pre = pkc; i = -1;
				}
				else if(bHiragana==0){
					pkc->kcode = 0x30F4;
					pkc->org_kcode = 0xFF73;
					pkc->codetype = 4;
					pre = pkc; i = -1;
				}
			}
			if(bApple!=0 && bHiragana==0){
				if(isv == 0x3C){ // "ワ"に濁点
					pkc->kcode = 0x30F7;
					pkc->org_kcode = 0xFF9C;
					pkc->codetype = 8;
					pre = pkc; i = -1;
				}
				else if(isv == 6){ // "ヲ"に濁点
					pkc->kcode = 0x30FA;
					pkc->org_kcode = 0xFF66;
					pkc->codetype = 8;
					pre = pkc; i = -1;
				}
			}
			if(i != -1){
				if(cTblDaku[isv] == 'Y'){
					if(bHiragana != 0){
						pkc->kcode=iTblKANA_ZH[isv]+1;
					}
					else{pkc->kcode=iTblKANA_ZK[isv]+1;}
					pkc->codetype = 4;
					pre = pkc; i = -1;
				}
				else{
					// "あ"+"゛"のような出力の場合
					if(bHiragana != 0){
						pkc->kcode=iTblKANA_ZH[isv];
					}
					else{pkc->kcode=iTblKANA_ZK[isv];}
					pkc->org_kcode = isv + 0xFF60;
					pkc->codetype = 4;
					pre = pkc;
					pkci = new ImdKcvCode(
						&pre,0x309B,0xFF9E,4);
					i = -1;
			}	}
			isv = 0;
		}
		else if(isv != 0 && 0x3F == ucode){
			if(cTblHandaku[isv] == 'Y'){
				if(bHiragana != 0){
					pkc->kcode=iTblKANA_ZH[isv]+2;
				}
				else{pkc->kcode=iTblKANA_ZK[isv]+2;}
				pkc->org_kcode = isv + 0xFF60;
				pkc->codetype = 4;
				pre = pkc;
			}
			else{
				// "あ"+"゜"のような出力の場合
				if(bHiragana != 0){
					pkc->kcode=iTblKANA_ZH[isv];
				}
				else{pkc->kcode=iTblKANA_ZK[isv];}
				pkc->org_kcode = isv + 0xFF60;
				pkc->codetype = 4;
				pre = pkc;
				pkci = new ImdKcvCode(&pre,0x309C,0xFF9F,4);
			}
			isv = 0;
		}
		else{
			if(isv != 0){
				  // "カ"を貯めこんだ状態で"キ"が入力された場合は
				  // "カ"に濁点がつかないことが決定したので
				  // "カ"を出力する
				if(bHiragana==0){ucode=iTblKANA_ZK[isv];}
				else{ucode=iTblKANA_ZH[isv];}
				pre = new ImdKcvCode(ucode,isv+0xFF60,4);
				isv = 0; ucode = pkc->kcode - 0xFF60;
			}

			if(cTblDaku[ucode] != ' '){isv = ucode; delete pkc;}
			else{
				if(bHiragana==0){pkc->kcode=iTblKANA_ZK[ucode];}
				else{pkc->kcode=iTblKANA_ZH[ucode];}
				pkc->codetype = 4;
				pkc->cat(&pre);
	}	}	}

	return pre;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_JISX0201KANA_filter クラス                  ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

// 半角カナ(JISX0201カナ) フィルタ
// 半角カナなら codetype = -2 に変換する
// 入力・出力とも UCS です。
ImdKcvCode* Imd_JISX0201KANA_filter::convert(ImdKcvCode *pkc){
	if(0 <= pkc->kcode){
		if(0xFF61 <= pkc->kcode && pkc->kcode <= 0xFF9F){
			pkc->codetype = -2;
		}
	}
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_X0208andASCII_passfilter クラス             ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_X0208andASCII_passfilter::convert(ImdKcvCode *pkc){
	int		iJis,iUcs = pkc->kcode;

	if    (fImd_UCStoJISX0208(iUcs,&iJis) != 0){return pkc;}
	else if(iUcs < 0x7F || iUcs == 0xA5){return pkc;}

	pkc->codetype = -2;
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_ISO8859_filter クラス                       ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_ISO8859_filter::convert(ImdKcvCode *pkc){
	if(885901 <= pkc->codetype && pkc->codetype <= 885999){
		pkc->codetype = -2;
	}
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_ISO8859_filter2 クラス                      ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
ImdKcvCode* Imd_ISO8859_filter2::convert(ImdKcvCode *pkc){
	if(885901 <= pkc->codetype && pkc->codetype <= 885999){
		if(pkc->codetype == 885901){}
		else if(pkc->codetype == 885907){}
		else{pkc->codetype = -2;}
	}
	return pkc;
}


//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  Imd_AutoInputCodeDetecter クラス                ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

int fImd_IsNihongo(ImdKcvCode *pkc);

void Imd_AutoInputCodeDetecter::read(int i){
	ImdKcvCode	*pkci,*pkcj,*pkck;
	int		kn;

	pkci = pkci = ducs4b.decode(i);
	while(pkci != NULL){
		if(pkci->codetype == -1){delete pkci; break;}
		if((kn = fImd_IsNihongo(pkci)) != 0){
			ucs4b_v++;
			if(kn == 2){ucs4b_k++;}
		}
		ucs4b_all++;
		pkcj = pkci->next; pkci->next = NULL;
		delete pkci; pkci = pkcj;
	}

	pkci = ducs4l.decode(i);
	while(pkci != NULL){
		if(pkci->codetype == -1){delete pkci; break;}
		if((kn = fImd_IsNihongo(pkci)) != 0){
			ucs4l_v++;
			if(kn == 2){ucs4l_k++;}
		}
		ucs4l_all++;
		pkcj = pkci->next; pkci->next = NULL;
		delete pkci; pkci = pkcj;
	}

	pkci = dutf7.decode(i);
	while(pkci != NULL){
		if(pkci->codetype == -1){delete pkci; break;}
		if((kn = fImd_IsNihongo(pkci)) != 0){
			utf7_v++;
			if(kn == 2){utf7_k++;}
		}
		utf7_all++;
		pkcj = pkci->next; pkci->next = NULL;
		delete pkci; pkci = pkcj;
	}

	pkci = dutf8.decode(i);
	while(pkci != NULL){
		if(pkci->codetype == -1){delete pkci; break;}
		if((kn = fImd_IsNihongo(pkci)) != 0){
			utf8_v++;
			if(kn == 2){utf8_k++;}
		}
		utf8_all++;
		pkcj = pkci->next; pkci->next = NULL;
		delete pkci; pkci = pkcj;
	}

	pkci = dutf16b.decode(i);
	while(pkci != NULL){
		if(pkci->codetype == -1){delete pkci; break;}
		if((kn = fImd_IsNihongo(pkci)) != 0){
			utf16b_v++;
			if(kn == 2){utf16b_k++;}
		}
		utf16b_all++;
		pkcj = pkci->next; pkci->next = NULL;
		delete pkci; pkci = pkcj;
	}

	pkci = dutf16l.decode(i);
	while(pkci != NULL){
		if(pkci->codetype == -1){delete pkci; break;}
		if((kn = fImd_IsNihongo(pkci)) != 0){
			utf16l_v++;
			if(kn == 2){utf16l_k++;}
		}
		utf16l_all++;
		pkcj = pkci->next; pkci->next = NULL;
		delete pkci; pkci = pkcj;
	}

	if((pkci = djis.decode(i)) != NULL){
		pkck = pkci->copy();
		pkci = jn_u.convert(pkci);
		for(pkcj=pkci;pkcj!=NULL;pkcj=pkcj->next){
			if(pkci->codetype == -1){break;}
			else if(0 <= pkcj->codetype){
				jisn_v++;
				if((pkcj->org_kcode & 0xFF00) != 0){
					jisn_k++;
			}	}
			jisn_all++;
		}
		if(pkci != NULL){delete pkci;}
		pkci = pkck;
		pkck = pkci->copy();
		if(pkck == NULL){kn = 1;} else{kn = 0;}
		if(kn == 0){
			if(fImd_JIStoUCS_APPLE(pkck) == 2){
				jisa_v++; jisa_k++; jisa_all++;
			}
			else{kn = 1;}
			delete pkck;
		}
		if(kn == 1){
			pkci = ja_u.convert(pkci);
			for(pkcj=pkci;pkcj!=NULL;pkcj=pkcj->next){
				if(pkci->codetype == -1){break;}
				else if(0 <= pkcj->codetype){
					jisa_v++;
					if((pkcj->org_kcode & 0xFF00) != 0){
						jisa_k++;
				}	}
				else if(fImd_SJIStoUCS_APPLE(pkcj) == 2){
					jisa_v++; jisa_k++;
				}
				jisa_all++;
		}	}
		if(pkci != NULL){delete pkci;}
	}

	if((pkci = deuc.decode(i)) != NULL){
		for(pkcj=pkci;pkcj!=NULL;pkcj=pkcj->next){
			  // JIS 化
			if((pkcj->kcode & 0xFF00) != 0){
				pkcj->kcode = pkcj->kcode & 0x7F7F;
		}	}
		pkck = pkci->copy();
		pkci = en_u.convert(pkci);
		for(pkcj=pkci;pkcj!=NULL;pkcj=pkcj->next){
			if(pkci->codetype == -1){break;}
			else if(0 <= pkcj->codetype){
				eucn_v++;
				if((pkcj->org_kcode & 0xFF00) != 0){
					eucn_k++;
			}	}
			eucn_all++;
		}
		if(pkci != NULL){delete pkci;}
		pkci = pkck; pkck = pkci->copy();
		if(pkck == NULL){kn = 1;} else{kn = 0;}
		if(kn == 0){
			if(fImd_JIStoUCS_APPLE(pkck) == 2){
				euca_v++; euca_k++; euca_all++;
			}
			else{kn = 1;}
			delete pkck;
		}
		if(kn == 1){
			pkci = ea_u.convert(pkci);
			for(pkcj=pkci;pkcj!=NULL;pkcj=pkcj->next){
				if(pkci->codetype == -1){break;}
				else if(0 <= pkcj->codetype){
					euca_v++;
					if((pkcj->org_kcode & 0xFF00) != 0){
						euca_k++;
				}	}
				else if(fImd_SJIStoUCS_APPLE(pkcj) == 2){
					euca_v++; euca_k++;
				}
				euca_all++;
		}	}
		if(pkci != NULL){delete pkci;}
	}

	if((pkci = dsjis.decode(i)) != NULL){
		pkck = pkci->copy();
		pkci = sn_u.convert(pkci);
		for(pkcj=pkci;pkcj!=NULL;pkcj=pkcj->next){
			if(pkci->codetype == -1){break;}
			else if(0 <= pkcj->codetype){
				sjisn_v++;
				if((pkcj->org_kcode & 0xFF00) != 0){
					sjisn_k++;
			}	}
			sjisn_all++;
		}
		if(pkci != NULL){delete pkci;}
		pkci = pkck; pkck = pkci->copy();
		if(pkck == NULL){kn = 0;}else{kn = 1;}
		if(kn == 1){
			if(fImd_SJIStoUCS_APPLE(pkck) == 2){
				sjisa_v++; sjisa_k++; sjisa_all++;
			}
			else{kn = 0;}
			delete pkck;
		}
		if(kn == 0){
			pkci = sa_u.convert(pkci);
			for(pkcj=pkci;pkcj!=NULL;pkcj=pkcj->next){
				if(pkci->codetype == -1){break;}
				else if(0 <= pkcj->codetype){
					sjisa_v++;
					if((pkcj->org_kcode & 0xFF00) != 0){
						sjisa_k++;
				}	}
				sjisa_all++;
		}	}
		if(pkci != NULL){delete pkci;}
	}
}

static void _cgv(int ia,int ik,int iv,int *pima,int *pimk, int *pimv,
		int *pre,int ano){
	double	dv,dk,dmv,dmk;

	if(ia == 0){dv = dk = 0.0;}
	else{
		dv = ((double)iv) / ((double)ia);
		if(iv == 0){dk = 0.0;}
		else{dk = ((double)ik) / ((double)iv);}
	}
	if(*pima == 0){dmv = dmk = 0.0;}
	else{
		dmv = ((double)(*pimv)) / ((double)(*pima));
		if((*pimv) == 0){dmk = 0.0;}
		else{dmk = ((double)(*pimk)) / ((double)(*pimv));}
	}

	if((dmv < dv) || ((dmv == dv) && (dmk < dk))){
		*pre = ano; *pima = ia; *pimk = ik; *pimv = iv;
	}
}

int Imd_AutoInputCodeDetecter::guess(void){
	int		re = 1,ima,imv,imk;

	ima = imv = imk = 0;
	_cgv( sjisn_all, sjisn_k, sjisn_v,&ima,&imv,&imk,&re, 1);
	_cgv( sjisa_all, sjisa_k, sjisa_v,&ima,&imv,&imk,&re, 2);
	_cgv(  eucn_all,  eucn_k,  eucn_v,&ima,&imv,&imk,&re, 5);
	_cgv(  euca_all,  euca_k,  euca_v,&ima,&imv,&imk,&re, 6);
	_cgv(  jisn_all,  jisn_k,  jisn_v,&ima,&imv,&imk,&re, 3);
	_cgv(  jisa_all,  jisa_k,  jisa_v,&ima,&imv,&imk,&re, 4);
	_cgv( ucs4b_all, ucs4b_k, ucs4b_v,&ima,&imv,&imk,&re, 7);
	_cgv( ucs4l_all, ucs4l_k, ucs4l_v,&ima,&imv,&imk,&re, 8);
	_cgv(  utf7_all,  utf7_k,  utf7_v,&ima,&imv,&imk,&re, 9);
	_cgv(  utf8_all,  utf8_k,  utf8_v,&ima,&imv,&imk,&re,10);
	_cgv(utf16b_all,utf16b_k,utf16b_v,&ima,&imv,&imk,&re,11);
	_cgv(utf16l_all,utf16l_k,utf16l_v,&ima,&imv,&imk,&re,12);

	return re;
}

Imd_AutoInputCodeDetecter::Imd_AutoInputCodeDetecter(void){
	 sjisn_v =  sjisn_all =  sjisn_k = 0; 
	 sjisa_v =  sjisa_all =  sjisa_k = 0;
	  jisn_v =   jisn_all =   jisn_k = 0;
	  jisa_v =   jisa_all =   jisa_k = 0;
	  eucn_v =   eucn_all =   eucn_k = 0;
	  euca_v =   euca_all =   euca_k = 0;
	 ucs4b_v =  ucs4b_all =  ucs4b_k = 0;
	 ucs4l_v =  ucs4l_all =  ucs4l_k = 0;
	  utf7_v =   utf7_all =   utf7_k = 0;
	  utf8_v =   utf8_all =   utf8_k = 0;
	utf16b_v = utf16b_all = utf16b_k = 0;
	utf16l_v = utf16l_all = utf16l_k = 0;

	// デコーダの初期化
	ducs4b.mode  =  0; ducs4b.endian = 2;
	ducs4l.mode  =  0; ducs4l.endian = 3;
	dutf7.mode   =  7;
	dutf8.mode   =  8;
	dutf16b.mode = 16; dutf16b.endian = 2;
	dutf16l.mode = 16; dutf16l.endian = 3;

	// 各種コンバータ
	jn_u.f208mode = en_u.f208mode = 1;
	ja_u.f208mode = ea_u.f208mode = 2;

}

// UCS で日本語かどうかをチェックする
// 引数の内容が破壊される
// 全角なら2を返す。
// US-ASCII は 日本語扱だが ISO-8859 は非日本語扱
int fImd_IsNihongo(ImdKcvCode *pkc){
	int	re = 0,iUcs,iJis;

	iUcs = pkc->kcode;
	if     (fImd_UCStoJISX0201(pkc) != 0){re = 1;}
	else if(fImd_UCStoJISX0208(iUcs,&iJis) != 0){
		re = 2;
	}

	else if(fImd_UCStoSJIS_NEC(pkc) != 0){re = 2;}
	else if(fImd_UCStoSJIS_NECIBM(pkc) != 0){re = 2;}
	else if(fImd_UCStoSJIS_IBMIBM(pkc) != 0){re = 2;}
	else if(fImd_UCStoSJIS_APPLE(pkc) != 0){re = 2;}

	else if(fImd_UCStoJISX0212(iUcs,&iJis) != 0){
		re = 2;
	}

	return re;
}
