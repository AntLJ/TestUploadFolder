// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcv-sub.cxx (C++ Source File)                     CcCcC
// CcCcC                                                       CcCcC
// CcCcC        Copyright (C)  1999-2000  Yosuke IMADA         CcCcC
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

#include "imdkcv.h"

int Imd_StringList::SetString(char *vs){
	int		len;

	if(s != NULL){delete s; s = NULL;}
	if(vs == NULL){return 2;}
	len = strlen(vs) + 1;
	s = new char[len];
	if(s != NULL){
		strcpy(s,vs);
		return 0;
	}
	return 1;
}

void Imd_StringList::ListCat(Imd_StringList **pphead){
	while(*pphead != NULL){
		pphead = &((*pphead)->next);
	}
	*pphead = this;
}

void Imd_StringList::mkext(Imd_StringList *pisl,Imd_StringList *pext){
	int	len,exlen,i,j;

	if(s != NULL){delete s; s = NULL;}
	len   = strlen(pisl->s);
	exlen = strlen(pext->s);
	s = new char[len + exlen + 2];
	if(s != NULL){
		strcpy(s,pisl->s);
		j = len;
		// for(i=len-1;0<=i;i--){
		//	if(s[i] == '.'){j = i;break;}
		//	if(s[i] == '/' || s[i] == '\\'){break;}
		// }
		if(pext->s[0] == '.'){i = 1;} else{i = 0;}
		s[j++] = '.';
		for(;pext->s[i] != '\0';i++){s[j++] = pext->s[i];}
	    s[j++] = '\0';
	}
}

char* ImdKcvArgs::add(char *s,int *pi){
	char	*t;

	*pi = 0;
	if(inm != 0){
		inm = 0;
		outst.SetString(s);
	}
	else if(s[0] == '-'){
		t = &(s[1]);
		while(t[0] != (char)NULL){
			if(strncmp(t,"help",4) == 0){
				*pi = 2; return NULL;
			}
			else if(strncmp(t,"-help",5) == 0){
				*pi = 2; return NULL;
			}
			else if(strncmp(t,"ov",2) == 0){
				if(outmode != 0 && outmode != 1){
					*pi = 1; return exout;
				}
				else if(outmode == 1){
					*pi = 1; return dupout;
				}
				outmode = 1; t = &(t[2]);
			}
			else if(strncmp(t,"xo",2) == 0){
				if(outmode != 0 && outmode != 3){
					*pi = 1; return exout;
				}
				else if(outmode == 3){
					*pi = 1; return dupout;
				}
				outmode = inm = 3;t = &(t[2]);
			}
			else if(strncmp(t,"xi",2) == 0){
				if(outmode != 0 && outmode != 4){
					*pi = 1; return exout;
				}
				else if(outmode == 4){
					*pi = 1; return dupout;
				}
				outmode = inm = 4; t = &(t[2]);
			}
			else if(strncmp(t,"o",1) == 0){
				if(outmode != 0 && outmode != 2){
					*pi = 1; return exout;
				}
				else if(outmode == 2){
					*pi = 1; return dupout;
				}
				outmode = inm = 2; t = &(t[1]);
			}

			else if(strncmp(t,"hkf",3) == 0){
				filter |= 0x0001; t = &(t[3]);
			}
			else if(strncmp(t,"hka",3) == 0){
				filter |= 0x0003; t = &(t[3]);
			}
			else if(strncmp(t,"hk2f",4) == 0){
				filter |= 0x0004; t = &(t[4]);
			}
			else if(strncmp(t,"necf",4) == 0){
				filter |= 0x0008; t = &(t[4]);
			}
			else if(strncmp(t,"applef",6) == 0){
				filter |= 0x0010; t = &(t[6]);
			}
			else if(strncmp(t,"208gf",5) == 0){
				filter |= 0x0020; t = &(t[5]);
			}
			else if(strncmp(t,"212f",4) == 0){
				filter |= 0x0040; t = &(t[4]);
			}
			else if(strncmp(t,"208pf",5) == 0){
				filter |= 0x0080; t = &(t[5]);
			}
			else if(strncmp(t,"appleji",7) == 0){
				filter |= 0x0100; t = &(t[7]);
			}
			else if(strncmp(t,"applejo",7) == 0){
				filter |= 0x0200; t = &(t[7]);
			}
			else if(strncmp(t,"212p",4) == 0){
				filter |= 0x0400; t = &(t[4]);
			}
			else if(strncmp(t,"bgendi",6) == 0){
				filter |= 0x0800; t = &(t[6]);
			}
			else if(strncmp(t,"ltendi",6) == 0){
				filter |= 0x1000; t = &(t[6]);
			}
			else if(strncmp(t,"bgendo",6) == 0){
				filter |= 0x2000; t = &(t[6]);
			}
			else if(strncmp(t,"ltendo",6) == 0){
				filter |= 0x4000; t = &(t[6]);
			}
			else if(strncmp(t,"8859a",5) == 0){
				filter |= 0x08000; t = &(t[5]);
			}
			else if(strncmp(t,"8859b",5) == 0){
				filter |= 0x10000; t = &(t[5]);
			}

			else if(strncmp(t,"A",1) == 0){
				if(incode != 0 && incode != 1){
					*pi = 1; return dupincode;
				}
				incode = 1; t = &(t[1]);
			}
			else if(strncmp(t,"R",1) == 0){
				if(incode != 0 && incode != 2){
					*pi = 1; return dupincode;
				}
				incode = 2; t = &(t[1]);
			}
			else if(strncmp(t,"J",1) == 0){
				if(incode != 0 && incode != 11){
					*pi = 1; return dupincode;
				}
				incode = 11; t = &(t[1]);
			}
			else if(strncmp(t,"E",1) == 0){
				if(incode != 0 && incode != 12){
					*pi = 1; return dupincode;
				}
				incode = 12; t = &(t[1]);
			}
			else if(strncmp(t,"Sn",2) == 0){
				if(incode != 0 && incode != 21){
					*pi = 1; return dupincode;
				}
				incode = 21; t = &(t[2]);
			}
			else if(strncmp(t,"Sa",2) == 0){
				if(incode != 0 && incode != 22){
					*pi = 1; return dupincode;
				}
				incode = 22; t = &(t[2]);
			}
			else if(strncmp(t,"U4",2) == 0){
				if(incode != 0 && incode != 31){
					*pi = 1; return dupincode;
				}
				incode = 31; t = &(t[2]);
			}
			else if(strncmp(t,"U7",2) == 0){
				if(incode != 0 && incode != 32){
					*pi = 1; return dupincode;
				}
				incode = 32; t = &(t[2]);
			}
			else if(strncmp(t,"U8",2) == 0){
				if(incode != 0 && incode != 33){
					*pi = 1; return dupincode;
				}
				incode = 33; t = &(t[2]);
			}
			else if(strncmp(t,"U16",3) == 0){
				if(incode != 0 && incode != 34){
					*pi = 1; return dupincode;
				}
				incode = 34; t = &(t[3]);
			}

			else if(strncmp(t,"j",1) == 0){
				if(outcode != 0 && outcode != 11){
					*pi = 1; return dupoutcode;
				}
				outcode = 11; t = &(t[1]);
			}
			else if(strncmp(t,"e",1) == 0){
				if(outcode != 0 && outcode != 12){
					*pi = 1; return dupoutcode;
				}
				outcode = 12; t = &(t[1]);
			}
			else if(strncmp(t,"sn",2) == 0){
				if(outcode != 0 && outcode != 21){
					*pi = 1; return dupoutcode;
				}
				outcode = 21; t = &(t[2]);
			}
			else if(strncmp(t,"sa",2) == 0){
				if(outcode != 0 && outcode != 22){
					*pi = 1; return dupoutcode;
				}
				outcode = 22; t = &(t[2]);
			}
			else if(strncmp(t,"u4",2) == 0){
				if(outcode != 0 && outcode != 31){
					*pi = 1; return dupoutcode;
				}
				outcode = 31; t = &(t[2]);
			}
			else if(strncmp(t,"u7",2) == 0){
				if(outcode != 0 && outcode != 32){
					*pi = 1; return dupoutcode;
				}
				outcode = 32; t = &(t[2]);
			}
			else if(strncmp(t,"u8",2) == 0){
				if(outcode != 0 && outcode != 33){
					*pi = 1; return dupoutcode;
				}
				outcode = 33; t = &(t[2]);
			}
			else if(strncmp(t,"u16",3) == 0){
				if(outcode != 0 && outcode != 34){
					return dupoutcode;
				}
				outcode = 34; t = &(t[3]);
			}

			else if(strncmp(t,"d",1) == 0){
				if(nl != 0 && nl != 2){
					*pi = 1; return dupnlcode;
				}
				nl = 2; t = &(t[1]);
			}
			else if(strncmp(t,"a",1) == 0){
				if(nl != 0 && nl != 2){
					*pi = 1; return dupnlcode;
				}
				nl = 1; t = &(t[1]);
			}
			else if(strncmp(t,"w",1) == 0){
				if(nl != 0 && nl != 3){
					*pi = 1; return dupnlcode;
				}
				nl = 3; t = &(t[1]);
			}
			else{
				sprintf(szBuf,"Unknown Option %s.",t);
				*pi = 1; return szBuf;
	}	}	}
	else{
		new Imd_StringList(&phfn,s);
	}
	return NULL;
}

char* ImdKcvArgs::endadd(int *pi){
	char	*re = NULL;
	*pi = 0;
	// 戻り値 NULL:エラー無し
	//        その他: *pi=0 ワーニング  *pi=1 エラー

	if(incode == 0){incode = 1;}

	if(inm != 0){*pi = 1; return waitst;}
	if(outcode == 0 && incode != 2){*pi = 1; return nooutcode;}
	if(phfn == NULL && (outmode == 1 ||
		  outmode == 3 || outmode == 4)){
		return outexc;
	}
	if(phfn != NULL){
		if(phfn->next != NULL && outmode == 2){
			*pi = 1; return multiinerr;
		}
	}
	if((filter & 5) == 5){return exhk;}
	if((outcode != 21) && ((filter & 0x008)!=0)){
		re = sznecf;
	}
	if((filter & 0x18000) == 0x18000){*pi = 1; return ex8859;}
	if(outcode != 22 && (filter & 0x010) != 0){
		re = szapplef;
	}
	if(outcode != 11 && outcode != 12){
		if     (filter & 0x00020 != 0){re = sz208gf;}
		else if(filter & 0x00040 != 0){re = sz212f;}
		else if(filter & 0x00200 != 0){re = szapplejo;}
		else if(filter & 0x00400 != 0){re = sz212p;}
		else if(filter & 0x08000 != 0){re = sz8859a;}
		else if(filter & 0x10000 != 0){re = sz8859b;}
	}
	if(incode != 11 && incode != 12 &&
		 incode != 1 && incode != 2 && (filter & 0x100) != 0){
		re = szappleji;
	}
	if((filter & 0x1800) == 0x1800){*pi = 1; return dupiendian;}
	if((filter & 0x6000) == 0x6000){*pi = 1; return dupoendian;}
	if(!(31 <= incode && incode <= 34 ||
		 incode == 1 || incode == 2)){
		if     ((filter & 0x0800) != 0){re = szltendi;}
		else if((filter & 0x1000) != 0){re = szbgendi;}
	}
	if(!(31 <= outcode && outcode <= 34)){
		if     ((filter & 0x0080) != 0){re = sz208pf;}
		else if((filter & 0x2000) != 0){re = szltendo;}
		else if((filter & 0x4000) != 0){re = szbgendo;}
	}

	return re;
}
