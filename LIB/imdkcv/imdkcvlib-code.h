// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  imdkcvlib-code.h (C++ Header File)                   HhHhH
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

// ����������������������������������������������
// ������  ImdKcvDecoder/Encoder �h���^    ������
// ����������������������������������������������

// ---------------------------------------------
// �����Ő錾����Ă���N���X
// ---------------------------------------------
// ImdKcvDecoder_UCS, ..._JIS, ..._EUC, ..._SJIS
// ImdKcvEncoder_UCS, ..._JIS, ..._EUC, ..._SJIS
// ---------------------------------------------

#ifndef _IMDKCVLIB_CODE_H
#define _IMDKCVLIB_CODE_H 1

#ifndef _IMDKCVLIB_H
#include "imdkcvlib.h"
#endif

class ImdKcvDecoder_UCS : public ImdKcvDecoder{
  protected:
	int	isv,isvlen,b64m,surrogate,ft;
  public:
        int mode;
          //  0: UCS-4
          //  7: UTF-7
          //  8: UTF-8
          // 16: UTF-16 (Unicode)
        int endian;
          //  0: Big-Endian
          //  1: Little-Endian ( Intel )
	  //  2: Big-Endian    (Force)
	  //  3: Little-Endian (Force)

	virtual int MaxOutputSize(void){return 1;}
	virtual ImdKcvCode *decode(int i);

		 ImdKcvDecoder_UCS(void){
			isv=isvlen=b64m=surrogate=0;
			mode=16; endian=0; ft = 1;
		}
};

class ImdKcvDecoder_JIS : public ImdKcvDecoder{
  protected:
	int		isv,isvlen,es,eslen,gl,gr,g[4],glld,ssl,ssr;
	 // es : ���ݕۑ����̃G�X�P�[�v�V�[�P���X
	 // gl : GL �Ƀ��[�h����Ă���R�[�h�̎��(���L�Q�� �����l1)
	 // gr : GR �Ƀ��[�h����Ă���R�[�h�̎��(���L�Q�� �����l4)
	 // g[]: G0-G3 �Ƀ��[�h����Ă���R�[�h�̎��
	 //      (���L�Q�� �����l{1,3,3,9})
	 //       1:US-ASCII 2:JISX0201-Latin 3:JISX0201-Kana
	 //       4:JISX0208(+�@��ˑ�����(�P�ɊO���`�F�b�N�����Ȃ�����))
	 //       9:JISX0212
	 // glld : G0 �ɃR�[�h���ǂ݂��܂ꂽ�玩���I�� GL �Ƀ��[�h
	 //        ���邩�ǂ����̃t���O(�����l1)
	 // ssl,ssr : SS2, SS3 ���L�����ǂ�����
	 //     GL,GR�̃o�b�N�A�b�v or -1(����)
  public:
	virtual int MaxOutputSize(void){return 1;}
	virtual ImdKcvCode *decode(int i);

		 ImdKcvDecoder_JIS(void){
			isv = isvlen = es = eslen = 0; gl = 1;
			gr = 885901; glld = 1; ssl = ssr = -1;
			g[0] = 1; g[1] = 4; g[2] = 885901; g[3] = 9;
		}
};

class ImdKcvDecoder_EUC : public ImdKcvDecoder{
  protected:
	int		isv,isvlen;
  public:
	virtual int MaxOutputSize(void){return 1;}
	virtual ImdKcvCode *decode(int i);

		 ImdKcvDecoder_EUC(void){isv=isvlen=0;}
};

class ImdKcvDecoder_SJIS : public ImdKcvDecoder{
  protected:
	int		isv,isvlen;
  public:
	virtual int MaxOutputSize(void){return 1;}
	virtual ImdKcvCode *decode(int i);

		 ImdKcvDecoder_SJIS(void){isv=isvlen=0;}
};

class ImdKcvEncoder_UCS : public ImdKcvEncoder{
  protected:
	int isv,isvlen,b64m;
	  // UTF-7 �� BASE64���[�h���ǂ����̃t���O
  public:
        int mode;
          //  0: UCS-4
          //  7: UTF-7
          //  8: UTF-8
          // 16: UTF-16 (Unicode) �f�t�H���g
        int endian;
          //  0: Big-Endian (�f�t�H���g)
          //  1: Little-Endian ( Intel )
	int _feff;
	  //  0: BOM(0xFEFF) ���o�͂���(�f�t�H���g)
	  //  1: �o�͂��Ȃ� (�����ł͏o�͂����� 1 ��)
	  // UCS-4, UTF-16�̂�

	virtual int MaxOutputSize(void){return 8;}
	virtual int encode(ImdKcvCode *pkc,char *s,int *piOffset);

		 ImdKcvEncoder_UCS(void){
			mode=16; endian=_feff=b64m=0;
		}
};

class ImdKcvEncoder_JIS : public ImdKcvEncoder{
  // G0,GL �̂ݎg�����G���R�[�f�B���O (SO/SI�͗�O)
  // ������B (ISO-2022-JP-2 + ���p�J�i�n)
  // ISO-2022-JP �Ƃ��ďo�͂������ꍇ�� JISX0212 �t�B���^��
  // ���p�J�i�t�B���^���R���o�[�^�ŗp����΂悢
  protected:
	int	g[4],gl,gr,sft;
	  // ImdKcvDecoder_JIS �� g,gl �Ɠ���
	  // sft : Shift Out ���Ă���Ȃ� 1  In �Ȃ� 0
  public:
	int bASCII_JISX0201Latin;
	  // 0 : ��� US-ASCII �Ƃ��ďo�� (�f�t�H���g)
	  // 1 : ��� JISX0201-Latin �Ƃ��ďo��
	  // 2 : ���������؂�ւ���
	int bc;
	  // 0 : C0,C1 �� US-ASCII (or JISX0201Latin) ��
	  //     �o�� (�f�t�H���g)
	  // 1 : �؂�ւ����ɂ��̂܂܏o��
        int escJISX0201KANA;
	  // 0 : ESC ( I �Ő؂�ւ� (�f�t�H���g)
	  // 1 : SO/SI �Ő؂�ւ�
	int escJISX0208;
	  // 0 : ESC $ B (1983,1990) �f�t�H���g
	  // 1 : ESC $ ( B (1983,1990)
	  // 2 : ESC $ @ (1978)
	  // 3 : ESC $ ( @ (1978)

	virtual int MaxOutputSize(void){return 6;}
	virtual int encode(ImdKcvCode *pkc,char *s,int *piOffset);

		 ImdKcvEncoder_JIS(void){
			bASCII_JISX0201Latin = escJISX0201KANA =
			escJISX0208 = bc = 0; gl = 1; sft = 0;
			gr = 0; g[0] = 1;
			g[1] = g[2] = g[3] = 0;
		}
};

class ImdKcvEncoder_EUC : public ImdKcvEncoder{
  public:
	virtual int MaxOutputSize(void){return 3;}
	virtual int encode(ImdKcvCode *pkc,char *s,int *piOffset);
};

class ImdKcvEncoder_SJIS : public ImdKcvEncoder{
  public:
	virtual int MaxOutputSize(void){return 2;}
	virtual int encode(ImdKcvCode *pkc,char *s,int *piOffset);
};

#endif
