// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  imdkcvlib-conv.h (C++ Header File)                   HhHhH
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
// ������  ImdConvModule �h���^            ������
// ����������������������������������������������

// ----------------------------------------------------------
// �����Ő錾����Ă���N���X
// ----------------------------------------------------------
// Imd_SJISNtoUCS		SJIS(Win�n) -> UCS
// Imd_UCStoSJISN  		UCS         -> SJIS(Win�n)
// Imd_SJISAtoUCS  		SJIS(Mac�n) -> UCS
// Imd_UCStoSJISA  		UCS         -> SJIS(Mac�n)
// Imd_SJISN_filter		Win�n�@��ˑ���������(SJIS�p)
// Imd_SJISA_filter		Mac�n�@��ˑ���������(SJIS�p)
//
// Imd_UCStoJIS			UCS         -> JIS
// Imd_JIStoUCS			JIS         -> UCS
// Imd_JISX0208Gaiji_filter	JISX0208�O������ (JIS�p)
// Imd_JISX0212_filter		JISX0212���� (JIS�p)
// Imd_ISO8859_filter		ISO-8859���� (ISO-2022-JP   ��)
// Imd_ISO8859_filter2		ISO-8859���� (ISO-2022-JP-2 ��)
//
// Imd_UCStoEUC			UCS         -> EUC
// Imd_EUCtoUCS			EUC         -> UCS
//
// Imd_X0208andASCII_passfilter	JISX0208+ASCII�̂ݒʂ�(UCS�p)
// Imd_Replacement		���������� "�E" ���ɕϊ�
// Imd_Remove			�������������ł�����
// Imd_NLandLF_converter	���s�ϊ�
// Imd_JISX0201KANAtoJISX0208	���p�J�i��S�p��(UCS�p)
// Imd_JISX0201KANA_filter	���p�J�i�𖳌�������(UCS�p)
// ----------------------------------------------------------


#ifndef _IMDKCVLIB_CONV_H
#define _IMDKCVLIB_CONV_H 1

#ifndef _IMDKCVLIB_H
#include "imdkcvlib.h"
#endif

// ����������������������������������������������
// ������ SJIS �֘A                        ������
// ����������������������������������������������

// SJISN : SJIS + NEC���ꕶ�� + NEC�I��IBM�g������ +
//	   IBM�I��IBM�g������ (= Windows �Ŏg���镶��)
// SJISA : SJIS + Apple�g������ (= Mac �Ŏg���镶��)
// �@��ˑ������Ȃ��� SJIS ���g�������ꍇ�͊e��t�B���^��
// ���g����������

class Imd_SJISNtoUCS : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // SJISN -> UCS �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�
};

class Imd_UCStoSJISN : public ImdConvModule{
  public:
	int flags;
	  // 0x***0 ��ۃ��J�o�����Ȃ�
	  // 0x***1 ��ۃ��J�o������
	  // ���͂� Windows �n�@��ˑ��������܂� JIS,SJIS,EUC
	  // �łȂ��ꍇ�͕K�� 0 �ɂ��ĉ������B
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // UCS -> SJISN �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�
		 Imd_UCStoSJISN(void){flags = 0;}
};

class Imd_SJISAtoUCS : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // SJISA -> UCS �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�
};

class Imd_UCStoSJISA : public ImdConvModule{
  public:
	int	flags;
	  // 0x***0 ��ۃ��J�o�����Ȃ�
	  // 0x***1 ��ۃ��J�o������
	  // ���͂� Apple�g���������܂� JIS,SJIS,EUC �łȂ��ꍇ
	  // �͕K�� 0 �ɂ��ĉ������B
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // UCS -> SJISA �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�
		 Imd_UCStoSJISA(void){flags = 0;}
};

// NEC���ꕶ���ENEC�I��IBM�g�������EIBM�I��IBM�g������
// �̂ǂꂩ�������� codetype �� -2(�ϊ����s) �ɕϊ�����t�B���^
class Imd_SJISN_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// Apple�g�������������� codetype �� -2(�ϊ����s) �ɕϊ�����
// �t�B���^
class Imd_SJISA_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};






// ����������������������������������������������
// ������ JIS �֘A                         ������
// ����������������������������������������������

class Imd_UCStoJIS : public ImdConvModule{
  public:
	int	f208mode;
	  // NEC���ꕶ����NEC�I��IBM���ꕶ���AApple�g�������ɂ�
	  // �ϊ���₪JISX0208�̊O���̈�ɂ�JISX0212�ɂ����镶��
	  // ���ꕔ���݂���B
	  // ���̕ϊ����ǂ��������� f208mode �ϐ��Ŏw�肷��B
	  // �f�t�H���g�l�� 0x1022
	  //
	  // 0x1***: Windows�n�̕ϊ����s��
	  // 0x2***: Macintosh�n�̕ϊ����s��
	  // �ǂ���ł��Ȃ���΋@��ˑ������̏����͖���
	  //
	  // �� Windows�n
	  // �EJISX0208 13��(NEC���ꕶ��) �̕ϊ��|���V
	  //  0x1**0: X0212 �ւ̕ϊ��̂�
	  //          (�ϊ��ł��Ȃ����̂� codetype = -2 �ɂ���)
	  //  0x1**1: X0212 �D�� ( X0212,13��ǂ���ւ��ϊ��ł���
	  //          ���̂� X0212 �ɕϊ����A13�悾���ɕϊ��ł���
	  //          ���̂� 13��ɕϊ�
	  //  0x1**2: 13��ւ̕ϊ��̂�
	  //   ( 13��D�� == 13��ւ̕ϊ��̂� )
	  //   �ǂ�ɂ����Ă͂܂�Ȃ��ꍇ�� 2 �Ƃ݂Ȃ�
	  //
	  // �EJISX0212 89-92��(NEC�I��IBM�g������) �̕ϊ��|���V
	  //  0x1*0*: X0212 �ւ̕ϊ��̂�
	  //  0x1*1*: X0212 �D��
	  //  0x1*2*: 89-92��ւ̕ϊ��̂�
	  //   ( 89-92��D�� == 89-92��ւ̕ϊ��̂� )
	  //   �ǂ�ɂ����Ă͂܂�Ȃ��ꍇ�� 1 �Ƃ݂Ȃ�
	  //
	  // �EUCS �ɕϊ������ۂɕ�ۂ��ꂽ�����̃��J�o�����s����
	  //  0x10**: ���J�o���͍s��Ȃ�(�f�t�H���g)
	  //  0x11**: ���J�o�����s��
	  //   ���̑��̏ꍇ�� f208mode & 0x100 �̌��ʂɂ��
	  //
	  // ��Macintosh�n
	  // (Apple�g������) �̕ϊ��|���V
	  //  0x2**0: X0212 �ւ̕ϊ��̂�
	  //  0x2**1: X0212 �D��
	  //  0x2**2: JISX0208�O���̈�ւ̕ϊ��̂�
	  //   ( JISX0208�O���̈�D�� == JISX0208�O���̈�ւ̕ϊ��̂� )
	  //   �ǂ�ɂ����Ă͂܂�Ȃ��ꍇ�� 2 �Ƃ݂Ȃ�
	  // 
	  // �EUCS �ɕϊ������ۂɕ�ۂ��ꂽ�����̃��J�o�����s����
	  //  0x20**: ���J�o���͍s��Ȃ�
	  //  0x21**: ���J�o�����s��
	  //   �ǂ�ɂ����Ă͂Ȃ�Ȃ��ꍇ�� 1 �Ƃ݂Ȃ�

	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // UCS -> JIS �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�

		 Imd_UCStoJIS(void){f208mode = 0x1022;}
};

class Imd_JIStoUCS : public ImdConvModule{
  public:
	int	f208mode;
	  // 1: JISX0208�O���̈�� Windows�n �̂��̂Ƃ��Ĕ��f
	  // 2: �� Macintosh�n �̂��̂Ƃ��Ĕ��f
	  // ���̑��͑S�����������ɕϊ�
	  // �����l 1

	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // JIS -> UCS �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�

		 Imd_JIStoUCS(void){f208mode = 1;}
};

// JISX0208 �̊O���������� codetype = -2 �ɂ���t�B���^
class Imd_JISX0208Gaiji_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// JISX0212 �������� codetype = -2 �ɂ���t�B���^
class Imd_JISX0212_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// ISO-8859-XX ������
class Imd_ISO8859_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// ISO-8859-XX ������ (1,7������)
class Imd_ISO8859_filter2 : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};





// ����������������������������������������������
// ������ EUC �֘A                         ������
// ����������������������������������������������

class Imd_EUCtoJIS : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // EUC ->JIS �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�
};

class Imd_JIStoEUC : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
	  // JIS -> EUC �ϊ�
	  // �ϊ��ł��Ȃ����������� codetype = -2 �ɂȂ�
};






// ����������������������������������������������
// ������ �e�����t�B���^                 ������
// ����������������������������������������������

// �EWindows �ŕ\���ł��镶�������ʂ� (���̑��� codetype = -2 ��)
//   �t�B���^ -> Imd_UCStoSJISN �� Imd_SJISNtoUCS �ŉ\
//
// �EMac �ŕ\���ł��镶�������ʂ� (���̑��� codetype = -2 ��)
//   �t�B���^ -> Imd_UCStoSJISA �� Imd_SJISAtoUCS �ŉ\
//
// �E��ʓI�� UNIX �ŕ\���ł��镶��(JISX0208 + ASCII) ����
//   �ʂ�  (���̑��� codetype = -2 ��) �t�B���^
//   -> Imd_UCStoJIS {fNecMode = 0} + Imd_JISX0212_filter    or
//      Imd_UCStoSJISN + Imd_SJISN_filter    or
//      Imd_UCStoSJISA + Imd_SJISA_filter    or
//      Imd_X0208andASCII_passfilter
//  (�ŏI�I�� EUC,JIS,SJIS�ɕϊ�����ꍇ��
//   Imd_X0208andASCII_passfileter ���A��L�̃Z�b�g���g������
//   �������ǂ��ł�)

// 
class Imd_X0208andASCII_passfilter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};


// �ϊ��Ɏ��s����������ʂ̕����ɕϊ�����t�B���^
// ( �Ⴆ�� "�E" �ɕϊ����� )
// �C���X�^���X������Ƀ����o�ϐ� kcRep ��������Ηǂ�
// �����ł� UCS �� "�E" �������l�Ƃ��Ē�`���Ă���B
// kcRep.kcode �̃R�[�h�̒l����������Γ��̓R�[�h
// ( UCS, SJIS, JIS, EUC ,...) �́A�ǂ�ł����܂�Ȃ�
class Imd_Replacement : public ImdConvModule{
  public:
	ImdKcvCode	kcRep,kcRep0;
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc){
		if(-1000000 <= pkc->codetype && pkc->codetype <= -2){
			if(pkc->org_kcode < 0x100){
				pkc->kcode = kcRep0.kcode;
				pkc->codetype = kcRep0.codetype;
				  // org_kcode �͔j�󂵂Ȃ�
			}
			else{
				pkc->kcode = kcRep.kcode;
				pkc->codetype = kcRep.codetype;
				  // org_kcode �͔j�󂵂Ȃ�
			}
		}
		return pkc;
	}

		 Imd_Replacement(void){
		kcRep.kcode = 0x30FB; kcRep.codetype = 4;
		kcRep0.kcode = 0x20; kcRep0.codetype = 1;
		// "�E" UCS:0x30FB  SJIS:0x8145 JIS:0x2126
	}
};

class Imd_Remove : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc){
		if(pkc->codetype == -2){delete pkc;pkc=NULL;}
		return pkc;
	}
};


// ���p�J�i(JISX0201�J�i) �t�B���^
// ���p�J�i�Ȃ� codetype = -2 �ɕϊ�����
// ���́E�o�͂Ƃ� UCS �ł��B
class Imd_JISX0201KANA_filter : public ImdConvModule{
  public:
	int MaxOutputSize(void){return 1;}
	ImdKcvCode* convert(ImdKcvCode *pkc);
};

// ���p�J�i(JISX0201�J�i) -> �S�p�J�i �ϊ����W���[��
// (Apple Macintosh �g��, �J�^�J�i�^�Ђ炪�ȏo�� ���Ή�)
// ���́E�o�͂Ƃ� UCS �ł��B
class Imd_JISX0201KANAtoJISX0208 : public ImdConvModule{
  protected:
	int	isv;
  public:
	int MaxOutputSize(void){return 2;}
	ImdKcvCode* convert(ImdKcvCode *pkc);

	int	bApple,bHiragana;
	  // bApple:  �u"��"�ɑ��_�v�u"��"�ɑ��_�v�u"��"�ɑ��_�v
	  //	    ��L���ɂ���ꍇ (SJIS-mac,UCS �o�͌���)
	  // bHiragana:  �J�^�J�i�łȂ��Ђ炪�Ȃɕϊ�

		 Imd_JISX0201KANAtoJISX0208(void){isv=bApple=bHiragana=0;}
};

// ���s�R�[�h�ϊ����W���[��
class Imd_NLandLF_converter : public ImdConvModule{
  protected:
	int		isv;
	ImdKcvCode*	_onls(int imode);
  public:
	int MaxOutputSize(void){return 2;}
	ImdKcvCode* convert(ImdKcvCode *pkc);

	int	mode;
	  // �o�� ���s��S�ď��� : mode = 0
	  //           0x0A      : mode = 1
	  //      0x0D           : mode = 2
	  //      0x0D 0x0A      : mode = 3
	  // �ϊ��������Ȃ��ꍇ�͂��̃��W���[�����g��Ȃ���Ηǂ�
	  // ���͂� 0x0D 0x0A �̏ꍇ�A�P�̉��s�Ƃ݂Ȃ�

		 Imd_NLandLF_converter(void){isv=0;mode=1;}
};

#endif
