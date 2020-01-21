// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcvlib-code.cxx (C++ Source File)                 CcCcC
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
//  ■■■  補助関数群                                      ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

static char ISO8859F[] = " ABCDLGFHMV";

static int fImd_ChangeEndian(int i,int endian,int byte){
	int		j,re = 0;

	  // 現在登録されているモード
	  // endian = 0,2 : 変換無し
	  // endian = 1,3 : ビッグ<->リトル変換
	if(endian == 0 || endian == 2){re = i;}
	else if(endian == 1 || endian == 3){
		for(j=0;j<byte;j++){
			re = (re << 8) & 0xFFFFFF00 | i & 0xFF;
			i = i >> 8;
		}
	}
	 // endian = 4以降 はその他のエンディアン用(リザーブ)

	return re;
}

static int fImd_BASE64_Decode(int i){
	int		re;

	if   (64 < i && i <  91){re = i - 65;}
	else if(96 < i && i < 123){re = i - 71;}
	else if(47 < i && i <  58){re = i +  4;}
	else if(i == 43){re = 62;}
	else if(i == 47){re = 63;}
	else{re = -1;}

	return re;
}

static int fImd_BASE64_Encode(int i){
	int		re;

	if   ( 0 <= i && i <  26){re = i + 65;}
	else if(25 <  i && i <  52){re = i + 71;}
	else if(51 <  i && i <  62){re = i -  4;}
	else if(i == 62){re = 43;}
	else if(i == 63){re = 47;}
	else{re = -1;}

	return re;
}

static ImdKcvCode* fImd_JIS_Sub01(int es,int eslen){
	int		k;
	ImdKcvCode	*pre = NULL;

	if(32 <= eslen){
		k = (es >> 24) & 0xFF;
		new ImdKcvCode(&pre,k,k,-2);
	}
	if(24 <= eslen){
		k = (es >> 16) & 0xFF;
		new ImdKcvCode(&pre,k,k,-2);
	}
	if(16 <= eslen){
		k = (es >> 8) & 0xFF;
		new ImdKcvCode(&pre,k,k,-2);
	}
	if( 8 <= eslen){
		k = es & 0xFF;
		new ImdKcvCode(&pre,k,k,-2);
	}
	return pre;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvDecoder_UCS クラス                        ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

ImdKcvCode *ImdKcvDecoder_UCS::decode(int i){
	ImdKcvCode	*pre = NULL, *pkc;
	int		j,k;

	if(mode == 0){ // UCS-4
		if(i != -1){
			isvlen += 8;
			isv = ((isv << 8) & 0xFFFFFF00) | i & 0xFF;
			if(isvlen == 32){
				isvlen = 0;
				if(ft == 1){ // 最初の文字は 0xFEFF か？
					ft = 0;
					if(isv == 0xFEFF){
						if(endian == 0 || endian == 1){
							endian = 0;
							return NULL;
						}
						else if(endian == 2){return NULL;}
						else if(endian == 3){}//処理無
					}
					if(isv == 0xFFFE0000){
						if(endian == 0 || endian == 1){
							endian = 1;
							return NULL;
						}
						else if(endian == 2){}//処理無
						else if(endian == 3){return NULL;}
					}
				}
				isv = fImd_ChangeEndian(isv,endian,4);
				new ImdKcvCode(&pre,isv,isv,0);
				isv = 0;
		}	}
		else{
			if(0 < isvlen){
			  // 今田の例外 : EOF の場合で入力が途中の
			  // 場合 0 をパッドして無理やり出力
				for(;isvlen<32;isvlen+=8){
					isv = (isv << 8) & 0xFFFFFF00;
				}
				isv = fImd_ChangeEndian(isv,endian,4);
				pkc = new ImdKcvCode(&pre,isv,isv,-2);
				  // ただし無効文字として出力
				isv = isvlen = 0; ft = 1;
			}
			new ImdKcvCode(&pre,-1,-1,-1);
	}	}

	else if(mode == 7){ // UTF-7
		if(i == -1){
			if((surrogate & 0xFC00) == 0xD800){
			  // 今田の例外 : サロゲートコードの途中の場合
				j = (surrogate << 10) & 0xFFC00;
				new ImdKcvCode(&pre,j,j,-2);
				surrogate = 0;
			}
			new ImdKcvCode(&pre,-1,-1,-1);
		}
		else if(b64m == 0){
			if(i != -1){
				if(i == '+'){b64m = 1;}
				else{new ImdKcvCode(&pre,i,i,0);}
		}	}
		else if(b64m == 1){
			if(i == -1){
				  // 今田の例外 : EOF で + を無効文字として出力
				new ImdKcvCode(&pre,0x2B,0x2B,-2);
				b64m = 0;
			}
			else if(i == 0x2D){ // +- で + とみなす 
				new ImdKcvCode(&pre,0x2B,0x2B,1);
				b64m = 0;
			}
			else{isvlen = 0; b64m = 2;}
		}
		if(b64m == 2){ // not "else if" buf "if"
			j = fImd_BASE64_Decode(i);
			if(j != -1){
				isv = (isv << 6) & 0xFFFFFFC0 | j;
				isvlen += 6;
				if(16 <= isvlen){ // 16ビット以上貯めたら出力
					isvlen -= 16;
					k = (isv >> isvlen) & 0xFFFF;
					if((surrogate & 0xFC00) == 0xD800){
						if((k & 0xFC00) == 0xDC00){
							k = (((surrogate & 0x3FF) << 10) |
							    k & 0x3FF) + 0x10000;
							new ImdKcvCode(&pre,k,k,0);
							surrogate = 0;
						}
						else{
							  // 正しくないときは無効文字で出力
							new ImdKcvCode(&pre,surrogate,surrogate,-2);
							if((k & 0xFC00) == 0xD800){
								surrogate = k;
							}
							else{
								new ImdKcvCode(&pre,k,k,0);
								surrogate = 0;
					}	}	}
					else if((k & 0xFC00) == 0xD800){surrogate = k;}
					else{new ImdKcvCode(&pre,k,k,0);}
			}	}
			else{
				b64m = 0;
				if(i != '-' && i != -1){
					new ImdKcvCode(&pre,i,i,1);
					  // BASE64 文字以外なら BASE64
					  // モードから出てその文字を出力
	}	}	}	}

	else if(mode == 8){ // UTF-8
		if(i == -1){
			if(isvlen != 0){
				 // 今田の例外 : 多バイト文字の途中で EOF なら０
				 // を補充して無効文字として出力
				isv = isv << (-isvlen);
				new ImdKcvCode(&pre,isv,isv,-2);
				isv = isvlen = 0;
			}
			new ImdKcvCode(&pre,-1,-1,-1);
		}
		else if(isvlen == 0){
			if     (i == -1       ){} // 処理無
			else if((i&0x80)==   0){new ImdKcvCode(&pre,i,i,1);}
			else if((i&0xE0)==0xC0){isv = i & 0x1F; isvlen =  -6;}
			else if((i&0xF0)==0xE0){isv = i & 0x0F; isvlen = -12;}
			else if((i&0xF8)==0xF0){isv = i & 0x07; isvlen = -18;}
			else if((i&0xFC)==0xF8){isv = i & 0x03; isvlen = -24;}
			else if((i&0xFE)==0xFC){isv = i & 0x01; isvlen = -30;}
			else{} // 処理無 今田の例外
		}
		else if(isvlen != 0){
			if((i&0xC0)==0x80){
				isv = (isv << 6) & 0xFFFFFFC0 | i & 0x3F;
				isvlen = isvlen + 6;
				if(isvlen == 0){
					new ImdKcvCode(&pre,isv,isv,0);
				}
			}
			else{
				isv = isv << -isvlen; isvlen = 0;
				new ImdKcvCode(&pre,isv,isv,-2);
				  // 今田の例外 : UTF-8 のコードで無い場合
				  // 0 を補充した無効文字
				if(i != -1){
					pkc = decode(i);
					if(pkc != NULL){pkc->cat(&pre);}
	}	}	}	}

	else if(mode == 16){ // UTF-16
		if(i == -1){
			ft = 1;
			if((surrogate & 0xFC00) == 0xD800){
			  // 今田の例外 : サロゲートコードの途中の場合
				j = (surrogate << 10) & 0xFFC00;
				new ImdKcvCode(&pre,j,j,-2);
				surrogate = 0;
			}
			if(isvlen != 0){
				isv = isv << 8; isvlen = 0;
				new ImdKcvCode(&pre,isv,isv,-2);
				  // 文字の途中でEOFなら0を補充した無効文字
			}
			if((surrogate & 0xFC00) == 0xD800){
			  // 今田の例外 : サロゲートコードの途中の場合
				new ImdKcvCode(&pre,surrogate,surrogate,-2);
				surrogate = 0;
			}
			new ImdKcvCode(&pre,-1,-1,-1);
		}
		else if(isvlen == 0){isv = i; isvlen = 8;}
		else if(isvlen == 8){
			isv = (isv << 8) | i & 0xFF; isvlen = 0;
			if(ft == 1){ // 最初の文字は 0xFEFF か？
				ft = 0;
				if     (isv == 0xFEFF){
					if(endian == 0 || endian == 1){
						endian = 0; return NULL;
					}
					else if(endian == 2){return NULL;}
					else if(endian == 3){}//処理無
				}
				else if(isv == 0xFFFE){
					if(endian == 0 || endian == 1){
						endian = 1; return NULL;
					}
					else if(endian == 2){}//処理無
					else if(endian == 3){return NULL;}
			}	}
			isv = fImd_ChangeEndian(isv,endian,2);

			if((surrogate & 0xFC00) == 0xD800){
				if((isv & 0xFC00) == 0xDC00){
					isv = (((surrogate & 0x3FF) << 5) |
						isv & 0x3FF) + 0x10000;
					new ImdKcvCode(&pre,isv,isv,0);
					surrogate = 0;
				}
				else{
					new ImdKcvCode(&pre,surrogate,surrogate,-2);
					if((isv & 0xFC00) == 0xD800){
						surrogate = isv;
					}
					else{
						new ImdKcvCode(&pre,isv,isv,0);
						surrogate = 0;
			}	}	}
			else if((isv & 0xFC00) == 0xD800){surrogate = isv;}
			else{new ImdKcvCode(&pre,isv,isv,0);}
	}	}

	return pre;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvDecoder_JIS クラス                        ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

ImdKcvCode *ImdKcvDecoder_JIS::decode(int i){
	ImdKcvCode	*pre = NULL,*pkc;
	int		j,k;

	if(i == -1){
		if(eslen != 0){
			new ImdKcvCode(&pre,es,es,-2);
			eslen = 0;
		}
		if(isvlen != 0){
			new ImdKcvCode(&pre,isv,isv,-2);
			isv = isvlen = 0;
		}

		new ImdKcvCode(&pre,-1,-1,-1);
	}
	else if(isvlen != 0){
		if(0x20 <= i && i <= 0x7E){ // GL
			if(0x20 <= isv && isv <= 0x7E){
				k = (isv << 8) & 0xFF00 | i & 0xFF;
				new ImdKcvCode(&pre,k,k,gl);
				isv = isvlen = 0;
			}
			else{
				new ImdKcvCode(&pre,isv,isv,-2);
				isv = isvlen = 0; pkc = decode(i);
				if(pkc != NULL){pkc->cat(&pre);}
		}	}
		else if(0xA0 <= i && i <= 0xFF){ //GR
			if(0xA0 <= i && i <= 0xFF){
				k = (isv << 8) & 0x7F00 | i & 0x7F;
				new ImdKcvCode(&pre,k,k,gr);
				isv = isvlen = 0;
			}
			else{
				new ImdKcvCode(&pre,isv,isv,-2);
				isv = isvlen = 0; pkc = decode(i);
				if(pkc != NULL){pkc->cat(&pre);}
		}	}
		else{ // C0,C1
			new ImdKcvCode(&pre,isv,isv,-2);
			isv = isvlen = 0; pkc = decode(i);
			if(pkc != NULL){pkc->cat(&pre);}
		}

		if(ssl != -1){gl = ssl; ssl = -1;}
		if(ssr != -1){gr = ssr; ssr = -1;}
	}
	else if(i == 0x1B){ // ESC
		if(ssl != -1){gl = ssl; ssl = -1;}
		if(ssr != -1){gr = ssr; ssr = -1;}
		if(es != 0){
			pkc = fImd_JIS_Sub01(es,eslen);
			if(pkc != NULL){pkc->cat(&pre);}
		}
		  // エスケープシーケンスの処理途中でさらに
		  // エスケープを見つけた場合は一バイトずつ
		  // 全て無効文字で出力
		es = 0x1B; eslen = 8;
	}
	else if(eslen != 0){ // エスケープシーケンス続き
		es = (es << 8) & 0xFFFFFF00 | i & 0xFF;
		eslen = eslen + 8;

		if(eslen == 16){
			if     (es == 0x1B6E){gl = g[2]; es = eslen = 0;} // ESC n
			else if(es == 0x1B6F){gl = g[3]; es = eslen  = 0;} // ESC o
			else if(es == 0x1B4E){
				ssl = gl; gl = g[2]; es = eslen  = 0;
			} // ESC N (SS2)
			else if(es == 0x1B4F){
				ssl = gl; gl = g[3]; es = eslen  = 0;
			} // ESC O (SS3)
			else if(es == 0x1B22){gr = g[1]; es = eslen  = 0;} // ESC "
			else if(es == 0x1B7D){gr = g[2]; es = eslen  = 0;} // ESC }
			else if(es == 0x1B7C){gr = g[3]; es = eslen  = 0;} // ESC |
			else if(!((i == 0x24) || 0x28 <= i && i <= 0x2B || 
					0x2D <= i && i <= 0x2F)){
				  // ESC [$()*+-./] 以外
				new ImdKcvCode(&pre,0x1B,0x1B,-2);
				eslen = 0;
				pkc = decode(i);
				if(pkc != NULL){pkc->cat(&pre);}
		}	}
		else if(eslen == 24){
			if     (es == 0x1B2442){
				g[0] = 4; es = eslen = 0;
				if(glld != 0){gl = g[0];}
			}
			  // ESC $ B  JISX0208-1983,1999省略形
			else if(es == 0x1B2440){
				g[0] = 4; es = eslen = 0;
				if(glld != 0){gl = g[0];}
			}
			  // ESC $ @  JISX0208-1978省略形
			else if((es & 0xFFFCFF) == 0x1B2842){
				  // ESC [()*+] B  US-ASCII
				j = (es >> 8) & 3; g[j] = 1; es = eslen = 0;
				if(glld != 0 && j == 0){gl = g[0];}
			}
			else if((es & 0xFFFCFF) == 0x1B2849){
				  // ESC [()*+] I  JISX0201-Kana
				j = (es >> 8) & 3; g[j] = 3; es = eslen = 0;
				if(glld != 0 && j == 0){gl = g[0];}
			}
			else if((es & 0xFFFCFF) == 0x1B284A){
				  // ESC [()*+] J  JISX0201-Latin
				j = (es >> 8) & 3; g[j] = 2; es = eslen = 0;
				if(glld != 0 && j == 0){gl = g[0];}
			}
			else if((es & 0xFFFC00) == 0x1B2800){
				  // ESC [()*+] [F] 未定義
				new ImdKcvCode(&pre,es,es,-2);
				es = eslen = 0;
			}
			else if((es & 0xFFFFFC) == 0x1B2428){}
			else{ // ESC [-./] [F]
				k = 0;
				for(j=1;j<=10;j++){
					if(i == ISO8859F[j]){k=j;break;}
				}
				if(k != 0){g[(es>>8)&3] = 885900 + k;}
				else{  // [F] 未定義
					g[(es>>8)&3] = -2;
				}
				es = eslen = 0;
			}
		}
		else{
			if     ((es & 0xFFFFFCFF) == 0x1B242840){
				  // ESC $ [()*+] @  JISX0208-1978
				j = (es >> 8) & 3; g[j] = 4; es = eslen = 0;
				if(glld != 0 && j == 0){gl = g[0];}
			}
			else if((es & 0xFFFFFCFF) == 0x1B242842){
				  // ESC $ [()*+] B  JISX0208-1983,1999
				j = (es >> 8) & 3; g[j] = 4; es = eslen = 0;
				if(glld != 0 && j == 0){gl = g[0];}
			}
			else if((es & 0xFFFFFCFF) == 0x1B242844){
				  // ESC $ [()*+] D  JISX0212
				j = (es >> 8) & 3; g[j] = 9; es = eslen = 0;
				if(glld != 0 && j == 0){gl = g[0];}
			}
			else { // ESC [()*+] [F] 未定義
				j = (es >> 8) & 3; g[j] = -2; es = eslen = 0;
				if(glld != 0 && j == 0){gl = g[0];}
	}	}	}
	else if(i == 0x0E){gl = g[1];}		 // SO (Shift Out)
	else if(i == 0x0F){gl = g[0];}		 // SI (Shift In)
	else if(i == 0x8E){
		if(ssl != -1){gl = ssl; ssl = -1;}
		ssr = gr; gr = g[2];
	} // SS2
	else if(i == 0x8F){
		if(ssl != -1){gl = ssl; ssl = -1;}
		ssr = gr; gr = g[3];
	} // SS3
	else if(0x20 <= i && i <= 0x7E){ // GL
		if(ssr != -1){gr = ssr; ssr = -1;}
		if(4<=gl && gl<=9){isv = i; isvlen = 8;} //複数バイト文字
		else{ // １バイト文字
			if(gl == 3){i = i | 0x80; j = gl;}
			else{j = gl;}
			new ImdKcvCode(&pre,i,i,j);
			if(ssl != -1){gl = ssl; ssl = -1;}
	}	}
	else if(0xA0 <= i && i <= 0xFF){ // GR
		if(ssl != -1){gl = ssl; ssl = -1;}
		if(4<=gr && gr<=9){isv = i; isvlen = 8;} //複数バイト文字
		else{ // １バイト文字
			if(gr == 3){j = gr;}
			else{i = i & 0x7F; j = gr;}
			new ImdKcvCode(&pre,i,i,j);
			if(ssr != -1){gr = ssr; ssr = -1;}
	}	}
	else{ // ESC,SI,SO,SS2,SS3 以外の C0,C1
		  // 定義上 GL に JISX0208 がロードされている状態でも
		  // 改行 (0x0A) など C0, C1 を出力してよいため GL,GR
		  // に関係無く処理をしてます。
		  // C0,C1 を出力する際に US-ASCII か JISX0201-Latin
		  // を GL にロードするのは「ならわし」であって定義
		  // ではありません。
		  // デコーダには全く関係無い話ですが
		if(ssl != -1){gl = ssl; ssl = -1;}
		if(ssr != -1){gr = ssr; ssr = -1;}
		new ImdKcvCode(&pre,i,i,gl);
	}

	return	pre;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvDecoder_EUC クラス                        ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

ImdKcvCode *ImdKcvDecoder_EUC::decode(int i){
	ImdKcvCode	*pre = NULL;

	if(i == -1){
		if(isvlen != 0){
			new ImdKcvCode(&pre,isv,isv,-2);
			isvlen = 0;
		}
		new ImdKcvCode(&pre,-1,-1,-1);
	}
	else if(isvlen == 8){
		if(isv == 0x8E){
			isvlen = 0;
			if(0xA1<=i && i<=0xDF){ // 半角カナ
				new ImdKcvCode(&pre,i,i,3);
			}
			else{ // 今田の例外 : 
				i = (isv << 8) & 0xFF00 | i & 0xFF;
				new ImdKcvCode(&pre,i,i,-2);
			}
		}
		else if(isv == 0x8F){
			isvlen = 16;
			isv = (isv << 8) & 0xFF00 | i & 0xFF;
		}
		else{
			isvlen = 0;
			i = (isv << 8) & 0xFF00 | i & 0xFF;
			new ImdKcvCode(&pre,i,i,0);
			  // JISX0208外字領域が判断できないので
	}	}
	else if(isvlen == 16){// JISX0212
		isvlen = 0;
		i = (isv << 8) & 0xFF00 | i & 0xFF;
		new ImdKcvCode(&pre,i,i,9);
	}
	else if((i & 0x80) == 0){new ImdKcvCode(&pre,i,i,1);}
	else{isv = i; isvlen = 8;}

	return pre;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvDecoder_SJIS クラス                       ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

ImdKcvCode *ImdKcvDecoder_SJIS::decode(int i){
	ImdKcvCode	*pre = NULL;

	if(i == -1){
		if(isvlen == 8){
			new ImdKcvCode(&pre,isv,isv,-2);
			isvlen = 0;
		}
		new ImdKcvCode(&pre,-1,-1,-1);
	}
	else if(isvlen == 8){
		i = (isv << 8) & 0xFF00 | i & 0xFF;
		isvlen = 0;
		new ImdKcvCode(&pre,i,i,0);
	}
	else if((i & 0x80) == 0){new ImdKcvCode(&pre,i,i,1);}
	else if(0xA1 <= i && i <= 0xDF){new ImdKcvCode(&pre,i,i,3);}
	else if(0xFD <= i && i <= 0xFF){new ImdKcvCode(&pre,i,i,1);}
	else{isv = i; isvlen = 8;}

	return pre;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvEncoder_UCS クラス                        ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int ImdKcvEncoder_UCS::encode(ImdKcvCode *pkc,char *s,int *piOffset){
	int	re = 0,kc,cd,i;

	kc = pkc->kcode; cd = pkc->codetype;

	if(mode == 0){ // UCS-4
		if(cd < 0 || kc < 0){return 0;}
		if(_feff == 0){
			_feff = 1; i = 0xFEFF;
			i = fImd_ChangeEndian(i,endian,4);
			s[(*piOffset)++] = (i >> 24) & 0xFF;
			s[(*piOffset)++] = (i >> 16) & 0xFF;
			s[(*piOffset)++] = (i >>  8) & 0xFF;
			s[(*piOffset)++] =  i & 0xFF;
			re = re + 4;
		}
		kc = fImd_ChangeEndian(kc,endian,4);
		s[(*piOffset)++] = (kc >> 24) & 0xFF;
		s[(*piOffset)++] = (kc >> 16) & 0xFF;
		s[(*piOffset)++] = (kc >>  8) & 0xFF;
		s[(*piOffset)++] =  kc & 0xFF;
		re = re + 4;
	}

	else if(mode ==  7){ // UTF-7
		if(cd == -1){
			if(b64m == 1){
				if(isvlen != 0){
					i = (isv << (6 - isvlen)) & 0x3F;
					i = fImd_BASE64_Encode(i);
					s[(*piOffset)++] = (char)i;
					re++; isvlen = 0;
				}
				s[(*piOffset)++] = 0x2D; // '-'
				re++; b64m = 0;
		}	}
		else if(cd < -1 || kc < 0){return 0;}
		else if((kc==0x20)||(kc==0x08)||
		   (0x41<=kc&&kc<=0x5A)||(0x61<=kc&&kc<=0x7A)||
		   (0x30<=kc&&kc<=0x3A)||(0x27<=kc&&kc<=0x29)||
		   (0x2C<=kc&&kc<=0x2F)||(kc==0x3F)||
		   (kc==0x0D)||(kc==0x0A)){
			if(b64m == 1){
				if(isvlen != 0){
					i = (isv << (6 - isvlen)) & 0x3F;
					i = fImd_BASE64_Encode(i);
					s[(*piOffset)++] = (char)i;
					re++; isvlen = 0;
				}
				s[(*piOffset)++] = 0x2D; // '-'
				re++; b64m = 0;
			}
			s[(*piOffset)++] = kc; re++;
		}
		else if(kc == 0x2B){ // '+'
			if(b64m == 1){
				if(isvlen != 0){
					i = (isv << (6 - isvlen)) & 0x3F;
					i = fImd_BASE64_Encode(i);
					s[(*piOffset)++] = (char)i;
					re++; isvlen = 0;
				}
				s[(*piOffset)++] = 0x2D; // '-'
				re++; b64m = 0;
			}
			s[(*piOffset)++] = 0x2B;
			s[(*piOffset)++] = 0x2D; re = re + 2;
		}
		else if(kc < 0x110000){
			if(b64m == 0){
				isv = isvlen = 0; b64m = 1;
				s[(*piOffset)++] = 0x2B; re++;
			}
			if(0x10000 <= kc){
				i = (kc >> 10) & 0x3FF | 0xD800;
				isv = (isv << 16) & 0xFFFF0000 |
					i & 0xFFFF;
				isvlen = isvlen + 16;
				while(6 <= isvlen){
					i = (isv >> (isvlen - 6)) & 0x3F;
					i = fImd_BASE64_Encode(i);
					s[(*piOffset)++] = (char)i;
					re++; isvlen = isvlen - 6;
				}
				isv = isv & 0x1F;
			}
			isvlen = isvlen + 16;	
			isv = (isv << 16) & 0xFFFF0000 | kc & 0xFFFF;
			while(6 <= isvlen){
				i = (isv >> (isvlen - 6)) & 0x3F;
				i = fImd_BASE64_Encode(i);
				s[(*piOffset)++] = (char)i;
				re++; isvlen = isvlen - 6;
			}
			isv = isv & 0x1F;
	}	}

	else if(mode ==  8){ // UTF-8
		if(cd < 0 || kc < 0){return 0;}
		else if(kc <  0x00000080){s[(*piOffset)++] = kc; re++;}
		else if(kc <  0x00000800){
			s[(*piOffset)++] = ((kc >> 6) & 0x1F) | 0xC0;
			s[(*piOffset)++] = (kc & 0x3F) | 0x80;
			re = re + 2;
		}
		else if(kc <  0x00010000){
			s[(*piOffset)++] = ((kc >> 12) & 0x0F) | 0xE0;
			s[(*piOffset)++] = ((kc >>  6) & 0x3F) | 0x80;
			s[(*piOffset)++] = (kc & 0x3F) | 0x80;
			re = re + 3;
		}
		else if(kc <  0x00200000){
			s[(*piOffset)++] = ((kc >> 18) & 0x07) | 0xF0;
			s[(*piOffset)++] = ((kc >> 12) & 0x3F) | 0x80;
			s[(*piOffset)++] = ((kc >>  6) & 0x3F) | 0x80;
			s[(*piOffset)++] = (kc & 0x3F) | 0x80;
			re = re + 4;
		}
		else if(kc <  0x04000000){
			s[(*piOffset)++] = ((kc >> 24) & 0x03) | 0xF8;
			s[(*piOffset)++] = ((kc >> 18) & 0x3F) | 0x80;
			s[(*piOffset)++] = ((kc >> 12) & 0x3F) | 0x80;
			s[(*piOffset)++] = ((kc >>  6) & 0x3F) | 0x80;
			s[(*piOffset)++] = (kc & 0x3F) | 0x80;
			re = re + 5;
		}
		else if(kc <= 0x7FFFFFFF){
			s[(*piOffset)++] = ((kc >> 30) & 0x01) | 0xFC;
			s[(*piOffset)++] = ((kc >> 24) & 0x3F) | 0x80;
			s[(*piOffset)++] = ((kc >> 18) & 0x3F) | 0x80;
			s[(*piOffset)++] = ((kc >> 12) & 0x3F) | 0x80;
			s[(*piOffset)++] = ((kc >>  6) & 0x3F) | 0x80;
			s[(*piOffset)++] = (kc & 0x3F) | 0x80;
			re = re + 6;
	}	}

	else if(mode == 16){ // UTF-16
		if(cd < 0 || kc < 0){return 0;}
		if(_feff == 0){
			_feff = 1; i = 0xFEFF;
			i = fImd_ChangeEndian(i,endian,2);
			s[(*piOffset)++] = (i >>  8) & 0xFF;
			s[(*piOffset)++] =  i & 0xFF;
			re = re + 2;
		}
		if(0x10000 < kc && kc <= 0x10FFFF){
			i = kc - 0x10000;
			kc = i & 0x3FF; i = (i >> 10) & 0x3FF;
			i = fImd_ChangeEndian(i,endian,2);
			s[(*piOffset)++] = (i >>  8) & 0xFF;
			s[(*piOffset)++] =  i & 0xFF;
			re = re + 2;
		}
		kc = fImd_ChangeEndian(kc,endian,2);
		s[(*piOffset)++] = (kc >>  8) & 0xFF;
		s[(*piOffset)++] =  kc & 0xFF;
		re = re + 2;
	}

	return re;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvEncoder_JIS クラス                        ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// ISO-646(US-ASCII), JISX0201ラテン, JISX0201カナ, JISX0208,
// JISX0212, ISO-8859-01～10 が出力可能
int ImdKcvEncoder_JIS::encode(ImdKcvCode *pkc,char *s,int *piOffset){
	int	re = 0,i;

	if(pkc->codetype == -1){
		if(sft == 1){
			sft = 0; gl = g[0];
			s[(*piOffset)++] = 0x0F; re++;
		}
		if(4<=gl && gl<=9 && bASCII_JISX0201Latin != 1){
			s[(*piOffset)++] = 0x1B;
			s[(*piOffset)++] = 0x28;
			s[(*piOffset)++] = 0x42;
			re = re + 3;
		}
	}
	else if(pkc->codetype < 0){} // 処理無
	else if(0 <= pkc->kcode && pkc->kcode <= 0x1F ||
			pkc->kcode == 0x7F){
		if(bc == 0){
			if(gl == 2 && (bASCII_JISX0201Latin != 0) ||
			   (gl != 1 && gl != 2)){
				s[(*piOffset)++] = 0x1B;
				s[(*piOffset)++] = 0x28; re = re + 2;
				if(bASCII_JISX0201Latin == 1){
					s[(*piOffset)++] = 0x4A; re++;
					gl = g[0] = 2; // ESC ( J
				}
				else{
					s[(*piOffset)++] = 0x42; re++;
					gl = g[0] = 1; // ESC ( B
		}	}	}
		s[(*piOffset)++] = pkc->kcode; re++;
	}
	else if(pkc->codetype == 1){
		if(sft == 1){
			sft = 0; gl = g[0];
			s[(*piOffset)++] = 0x0F; re++;
		}
		if(gl == 2 && (bASCII_JISX0201Latin != 0) ||
		   (gl != 1 && gl != 2)){
			s[(*piOffset)++] = 0x1B;
			s[(*piOffset)++] = 0x28; re = re + 2;
			if(bASCII_JISX0201Latin == 1){
				s[(*piOffset)++] = 0x4A; re++;
				gl = g[0] = 2; // ESC ( J
			}
			else{
				s[(*piOffset)++] = 0x42; re++;
				gl = g[0] = 1; // ESC ( B
		}	}
		s[(*piOffset)++] = (char)pkc->kcode; re++;
	}
	else if(pkc->codetype == 2){
		if(sft == 1){
			sft = 0; gl = g[0];
			s[(*piOffset)++] = 0x0F; re++;
		}
		if(gl == 1 && (bASCII_JISX0201Latin != 1) ||
		   (gl != 1 && gl != 2)){
			s[(*piOffset)++] = 0x1B;
			s[(*piOffset)++] = 0x28; re = re + 2;
			if(bASCII_JISX0201Latin == 0){
				s[(*piOffset)++] = 0x42; re++;
				gl = g[0] = 1; // ESC ( B
			}
			else{
				s[(*piOffset)++] = 0x4A; re++;
				gl = g[0] = 2; // ESC ( J
		}	}
		s[(*piOffset)++] = (char)pkc->kcode; re++;
	}
	else if(pkc->codetype == 3){
		if(gl != 3){
			if(escJISX0201KANA == 1){
				sft = 1;
				s[(*piOffset)++] = 0x0E; re++;
			}
			else{
				s[(*piOffset)++] = 0x1B;
				s[(*piOffset)++] = 0x28;
				s[(*piOffset)++] = 0x49;
				re = re + 3; g[0] = 3; // ESC ( I
			}
			gl = 3;
		}
		s[(*piOffset)++] = (char)(pkc->kcode & 0x7F); re++;
	}
	else if(4 <= pkc->codetype && pkc->codetype <= 8){
		if(sft == 1){
			sft = 0; gl = g[0];
			s[(*piOffset)++] = 0x0F; re++;
		}
		if(gl < 4 || 8 < gl){
			s[(*piOffset)++] = 0x1B;
			s[(*piOffset)++] = 0x24; re = re + 2;
			if(escJISX0208==1 || escJISX0208==3){
				s[(*piOffset)++] = 0x28; re++;
			}
			if(escJISX0208==0 || escJISX0208==1){i=0x42;}
			else{i=0x40;}
			s[(*piOffset)++] = i; re++;

			gl = g[0] = 4;
		}
		s[(*piOffset)++] = (char)((pkc->kcode >> 8) & 0xFF);
		s[(*piOffset)++] = (char)(pkc->kcode & 0xFF);
		re = re + 2;
	}
	else if(pkc->codetype == 9){
		if(sft == 1){
			sft = 0; gl = g[0];
			s[(*piOffset)++] = 0x0F; re++;
		}
		if(gl != 9){
			s[(*piOffset)++] = 0x1B;
			s[(*piOffset)++] = 0x24;
			s[(*piOffset)++] = 0x28;
			s[(*piOffset)++] = 0x44;
			re = re + 4; gl = g[0] = 9;
		}
		s[(*piOffset)++] = (char)((pkc->kcode >> 8) & 0xFF);
		s[(*piOffset)++] = (char)(pkc->kcode & 0xFF);
		re = re + 2;
	}
	else if(885901 <= pkc->codetype && pkc->codetype <= 885910){
		if(sft == 1){
			sft = 0; gl = g[0];
			s[(*piOffset)++] = 0x0F; re++;
		}
		if(g[2] != pkc->codetype){
			s[(*piOffset)++] = 0x1B; // ESC . [F]
			s[(*piOffset)++] = 0x2E;
			s[(*piOffset)++] = ISO8859F[pkc->codetype - 885900];
			re = re + 3; g[2] = pkc->codetype;
		}
		s[(*piOffset)++] = 0x1B;
		s[(*piOffset)++] = 0x4E; // SS2 (ESC N)
		s[(*piOffset)++] = (char)(pkc->kcode & 0x7F);
		re = re += 3;
	}

	return re;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvEncoder_EUC クラス                        ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int ImdKcvEncoder_EUC::encode(ImdKcvCode *pkc,char *s,int *piOffset){
	int		cd = pkc->codetype, re = 0;

	if(cd < 0 || cd == 99){}
	else if(4 <= cd && cd <= 8){
		s[(*piOffset)++] = (pkc->kcode >> 8) & 0xFF; re++;
		s[(*piOffset)++] = pkc->kcode & 0xFF; re++;
	}
	else if(cd == 3){ // 半角カナ
		s[(*piOffset)++] = (char)0x8E; re++; // SS2
		s[(*piOffset)++] = pkc->kcode & 0x7F | 0x80; re++;
	}
	else if(cd == 9){ // JISX0212
		s[(*piOffset)++] = (char)0x8F; re++; // SS3
		s[(*piOffset)++] = (pkc->kcode >> 8) & 0xFF; re++;
		s[(*piOffset)++] = pkc->kcode & 0xFF; re++;
	}
	else{
		s[(*piOffset)++] = pkc->kcode & 0xFF; re++;
	}

	return re;
}

//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//  ■■■                                                  ■■■
//  ■■■  ImdKcvEncoder_SJIS クラス                       ■■■
//  ■■■                                                  ■■■
//  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int ImdKcvEncoder_SJIS::encode(ImdKcvCode *pkc,char *s,int *piOffset){
	int		cd = pkc->codetype, re = 0;

	if(cd < 0 || cd == 9 || cd == 99 || 885900 < cd){}
	else if(cd == 8 && pkc->kcode < 0x100){
		s[(*piOffset)++] = pkc->kcode & 0xFF; re++;
	}
	else if(4 <= cd && cd <= 8){ // JISX0208 + 機種依存文字
		s[(*piOffset)++] = (pkc->kcode >> 8) & 0xFF; re++;
		s[(*piOffset)++] = pkc->kcode & 0xFF; re++;
	}
	else{ // US-ASCII (+JISX0201Latin) + JISX0201Kana
		s[(*piOffset)++] = pkc->kcode & 0xFF; re++;
	}

	return re;
}
