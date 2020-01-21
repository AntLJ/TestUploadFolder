// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  imdkcv.h (C++ Header File)                           HhHhH
// HhHhH                                                       HhHhH
// HhHhH        Copyright (C)  1999-2000  Yosuke IMADA         HhHhH
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

#ifndef _IMDKCV_H
#define _IMDKCV_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imdkcvlib.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

extern char	szBuf[512];

// #define _WIN32 1
// Visual C++'s compiler predefine "_WIN32"
#ifdef _WIN32
  #define DIRSEP '\\'
#else
  #define DIRSEP '/'
#endif
class Imd_StringList{
  public:
	Imd_StringList		*next;
	char			*s;

	int SetString(char *vs);

	void ListCat(Imd_StringList **pphead);

	void mkext(Imd_StringList *pisl,Imd_StringList *pext);

		 Imd_StringList(void){s = NULL; next = NULL;}
		 Imd_StringList(char *vs){
			s = NULL; next = NULL;
			SetString(vs);
		 }
		 Imd_StringList(Imd_StringList **pphead,char *vs = NULL){
			s = NULL; next = NULL;
			SetString(vs); ListCat(pphead);
		 }
		~Imd_StringList(){
			if(s    != NULL){delete    s; s    = NULL;}
			if(next != NULL){delete next; next = NULL;}
		 }
};

// -A        : ���̓R�[�h��������
// -R		 : ���̓R�[�h�񍐂̂�
// -J   -j   : JIS�R�[�h    �Ǐ�
// -Sn  -sn  : SJIS (win�n) �Ǐ�
// -Sa  -sa  : SJIS (mac�n) �Ǐ�
// -E   -e   : EUC-jp       �Ǐ�
// -U4  -u4  : UCS-4        �Ǐ�
// -U7  -u7  : UTF-7        �Ǐ�
// -U8  -u8  : UTF-8        �Ǐ�
// -U16 -u16 : UTF-16       �Ǐ�

//             ( �ȉ��R���w�肵���ꍇ NLandLF filter ��L���� )
//      -d   : ���s�R�[�h�� 0x0D      �œ���
//      -a   : ���s�R�[�h��      0x0A �œ���
//      -w   : ���s�R�[�h�� 0x0D-0x0A �œ���

// -hkf      : Imd_JISX0201KANAtoJISX0208 �L����
// -hka      : Imd_JISX0201KANAtoJISX0208 �ŃA�b�v���g�������L���
// -hk2f     : Imd_JISX0201KANA_filter �L����
// -necf     : Imd_SJISN_filter ��L����
// -applef   : Imd_SJISA_filter ��L����
// -208gf    : Imd_JISX0208Gaiji_filter ��L����
// -212f     : Imd_JISX0212_filter ��L����
// -208pf    : Imd_X0208andASCII_passfilter ��L����
// -8859a    : Imd_ISO-8859_filter ��L����
// -8859b    : Imd_ISO-8859_filter2 ��L����

// -appleji  : Imd_JIStoUCS �� JISX0208�O���̈�� Apple �g������
//             �Ƃ��Ĉ��� ( �w�肵�Ȃ��ꍇ Windows�n�@��ˑ����� )
// -applejo  : Imd_UCStoJIS �� JISX0208�O���̈�ւ̃}�b�s���O��
//             Apple �g������ �Ɏg�� ( �w�肵�Ȃ��ꍇ Windows�n
//             �@��ˑ������Ɏg�� )
// -212p     : Imd_UCStoJIS �ŋ@��ˑ������̕ϊ��� JISX0212 �D���

// -ltendi   : UCS-4, UTF-16 ���͂ŋ����I�Ƀ��g���G���f�B�A����
// -bgendi   : UCS-4, UTF-16 ���͂ŋ����I�Ƀr�b�O�G���f�B�A����
// -ltendo   : UCS-4, UTF-16 �o�͂Ń��g���G���f�B�A����
// -bgendo   : UCS-4, UTF-16 �o�͂Ńr�b�O�G���f�B�A����

// -ov       : �I�[�o�[���C�g
// -o  [fn]  : [fn] �ɏo��
// -xo [ext] : �o�̓t�@�C���̊g���q�� [ext] ��
// -xi [ext] : ���̓t�@�C���̊g���q�� [ext] ��

class ImdKcvArgs{
  protected:
	int	inm;
	  // 0:���͑҂�����
	  // 2-4: outmode �̓��͑҂�

  public:
	int	incode,outcode;
	  // 0:�w�薳(�f�t�H���g)  1:�����F�� 2:�����E��
	  // 11:JIS  12:EUC-jp
	  // 21:SJIS-N  22:SJIS-A
	  // 31:UCS-4  32:UTF-7  33:UTF-8  34:UTF-16
	int	nl;
	  // 0:�ϊ���(�f�t�H���g)
	  // 1:     0x0A
	  // 2:0x0D
	  // 3:0x0D-0x0A
	Imd_StringList	*phfn;
	  // NULL:      �W�����͂������(�f�t�H���g)
	  // otherwise: ���̓t�@�C�����̃��X�g
	int	outmode;
	  // 0:�W���o�͂֏o��(�f�t�H���g)
	  // 1:�e�t�@�C�����I�[�o�[���C�g
	  // 2:outst�Ŏw�肳�ꂽ�t�@�C���ɏo��
	  // 3:outst�Ŏw�肳�ꂽ�g���q���o�̓t�@�C���ɕt��
	  // 4:outst�Ŏw�肳�ꂽ�g���q����̓t�@�C���ɕt��
	Imd_StringList	outst;
	int	filter;
	  // �e�t�B���^���g�����ǂ����Ȃ�
	  // 0x000001 : Imd_JISX0201KANAtoJISX0208 �L����
	  // 0x000002 : Imd_JISX0201KANAtoJISX0208::bApple = 1
	  // 0x000004 : Imd_JISX0201KANA_filter �L����
	  // 0x000008 : Imd_SJISN_filter ��L����
	  // 0x000010 : Imd_SJISA_filter ��L����
	  // 0x000020 : Imd_JISX0208Gaiji_filter ��L����
	  // 0x000040 : Imd_JISX0212_filter ��L����
	  // 0x000080 : Imd_X0208andASCII_passfilter ��L����
	  // 0x000100 : Imd_JIStoUCS::f208mode = 0x0002
	  // 0x000200 : Imd_UCStoJIS::f208mode = 0x2***
	  // 0x000400 : Imd_UCStoJIS::f208mode = 0x**22
	  // 0x000800 : ImdKcvDecoder::endian = 2 (Big)
	  // 0x001000 : ImdKcvDecoder::endian = 3 (Little)
	  // 0x002000 : ImdKcvEncoder::endian = 0 (Big)
	  // 0x004000 : ImdKcvEncoder::endian = 1 (Little)
	  // 0x008000 : Imd_ISO8859_filter ��L����
	  // 0x010000 : Imd_ISO8859_filter2 ��L����

	char* add(char *s,int *pi);
	  // ���������
	  // �o�͂̓G���[�R�[�h(���[�j���O�͖���)

	char* endadd(int *pi);
	  // �S�������͏I������s
	  // �o�͂̓G���[�R�[�h(���[�j���O�͖���)

		 ImdKcvArgs(void){
			incode = outcode = outmode = nl = 0;
			phfn = NULL; filter = 0; inm = 0;
		 }
};

// Error strings and usage. 
extern char RB[],WB[],FS[],cannotopen[],cannottmp[],pressany[],
	dupincode[],dupoutcode[],dupnlcode[],exhk[],exout[],
	dupout[],outexc[],waitst[],nooutcode[],multiinerr[],
	sznecf[],szapplef[],sz208gf[];

extern char sz212f[],sz208pf[],sz212p[],szappleji[],szapplejo[],
	dupiendian[],dupoendian[],szltendi[],szbgendi[],
	szltendo[],szbgendo[],ex8859[],sz8859a[],sz8859b[];

extern char usage0[],usage1[],usage2[],usage3[],
	usage4[],usage5[];

// end of "ifndef _IMDKCV_H"
#endif
