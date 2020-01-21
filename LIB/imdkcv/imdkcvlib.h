// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  imdkcvlib.h (C++ Header File)                        HhHhH
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

// ----------------------------------------------------------
// �����Ő錾����Ă���N���X
// ----------------------------------------------------------
// ImdKcvCode			imdkcvlib �����̕����R�[�h
//				(�ꕶ�����E���X�g�`��)
//
// ImdKcvDecoder		�f�R�[�_���ۃN���X
// ImdKcvEncoder		�G���R�[�_���ۃN���X
// ImdConvModule		�R���o�[�^���ۃN���X
// 
// ImdConvModuleList		�R���o�[�^�p���X�g
// ImdKanjiCodeConverter	�R�[�h�R���o�[�^�S��
//				�f�R�[�_�A�G���R�[�_�A�R���o�[�^��
//				�Ǘ�����
//
// Imd_AutoInputCodeDetecter	�R�[�h�̎��(SJIS�Ȃ̂��`�Ȃ�)��
//				�������o����֐�
//
// (���̑����������⏕�֐��̐錾)
// ----------------------------------------------------------

// �f�R�[�_�E�G���R�[�_�̔h���N���X�� imdkcvlib-code.h
// �R���o�[�^�̔h���N���X�� imdkcvlib-conv.h
// �ł��ꂼ��錾����Ă���
// ( ���̃t�@�C���̓r���ł����̃t�@�C����ǂ݂���ł��� )


#ifndef _IMDKCVLIB_H
#define _IMDKCVLIB_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef NULL
#define NULL 0
#endif

class ImdKcvCode{
  public:
	ImdKcvCode	*next;		// ���X�g�p
	int		kcode;		// ���̓R�[�h�̒l
	int		org_kcode;	// ���Ƃ��Ƃ̒l
	int		housetsu;	// ��ۏ��
	int		codetype;
	  // codetype �̒l
	  // -2:�ϊ��Ɏ��s��������
	  // -1:EOF(���̏ꍇ kcode �͖���)
	  // 0:Unknown 1:US-ASCII 2:JISX0201-Latin 3:JISX0201-Kana
	  // 4:JISX0208 5:NEC���ꕶ�� 6:NEC�I��IBM�g������
	  // 7:IBM�I��IBM�g������ 8:Apple�g������
	  // 9:JISX0212
	  // 885901-885999: ISO-8859-XX (���� 1 - 10 �܂Ŏg�p��)
	  // 10 �`1000000 : �V�X�e�����U�[�u
	  // -4 �` -1000000 : �V�X�e�����U�[�u
	  // -3 : Mac �g���l�� ( UCS �ɕϊ��ł��Ȃ��@��ˑ����� )
	  //      kcode �̒l�� SJIS �l

	void	cat(ImdKcvCode **pphead);
	  // *pphead �̌��Ɏ�����ڑ�

	ImdKcvCode* copy(void);
	  // �����Ǝ����̌����R�s�[

	void	kill_all(void)
		{if(next != NULL){delete next; next = NULL;}}
	  // next �ȍ~��S�� delete

		 ImdKcvCode(void);
		 ImdKcvCode(int vkcode,int vorg_kcode,
				int vcodetype);
		 ImdKcvCode(ImdKcvCode **pphead,int vkcode,int vorg_kcode,
					int vcodetype);
		~ImdKcvCode(void)
			{kcode=org_kcode=codetype=housetsu=0;kill_all();}
};

// JIS�̃G�X�P�[�v�V�[�P���X�̏�����UTF-7,8,16��UCS4�ւ̃f�R�[�h
// �Ȃǂ��s���A�ꕶ���ꕶ���؂�o�����߂̃��W���[���̊�{���ۃN���X
// �Q�o�C�g�����̏ꍇ decode���Q�x�Ăяo���ď��߂Ĉꕶ������
// ���̂Ƃ��P�o�C�g�ڂ̏������ʂ� NULL �|�C���^
class ImdKcvDecoder{
  public:
	virtual int MaxOutputSize(void) = 0;
	  // ImdConvModule �̓����̊֐��ƈӖ��͓���
	virtual ImdKcvCode *decode(int i) = 0;
};

// JIS�ɃG�X�P�[�v�V�[�P���X��t��������AUCS4 �� UTF-? �ւ̃G��
// �R�[�h�����s���B
class ImdKcvEncoder{
  public:
	virtual int MaxOutputSize(void) = 0;
	  // ImdConvModule �̓����̊֐��ƈӖ��͓���
	virtual int encode(ImdKcvCode *pkc,char *s,int *piOffset) = 0;
};

// �ϊ����W���[���̒��ۃN���X
class ImdConvModule{
  public:
	virtual int MaxOutputSize(void) = 0;
	  // �ő�o�͕���
	  // �Ⴆ��"��"����͂Ƃ��Ď󂯎�����Ƃ���
	  // "��" �� "��" �� "��" ���o�͂���ꍇ�� return 3;
	virtual ImdKcvCode* convert(ImdKcvCode *pkc) = 0;
	  // �ϊ����W���[��
};

class ImdConvModuleList{
  protected:
	ImdConvModuleList	*next;
	ImdConvModule		*pcm;
  public:
	ImdKcvCode*	execute(ImdKcvCode *pkcvc);

	int	concat(ImdConvModuleList **pphead);
        int	MaxOutputSize(void);
	void	kill_all(void);

		 ImdConvModuleList(void)
			{next = NULL;pcm = NULL;}
		 ImdConvModuleList(ImdConvModule *vpcm)
			{next = NULL; pcm=vpcm;}
		 ImdConvModuleList(ImdConvModuleList **pphead,
			ImdConvModule *vpcm){pcm=vpcm;concat(pphead);}
		~ImdConvModuleList(){kill_all();}
};

class ImdKanjiCodeConverter{
  protected:
	ImdKcvDecoder		*pkd;
	ImdConvModuleList	*phead;
	ImdKcvEncoder		*pke;

  public:
	ImdKcvDecoder* SetDecoder(ImdKcvDecoder *vpkd);
	ImdKcvEncoder* SetEncoder(ImdKcvEncoder *vpke);
	int ConcatinateModule(ImdConvModule *pcm);
	void DeleteModules(void);

	int	execute(int i,char *s,int *piOffset);
	  // s �ɏ������񂾕�������Ԃ��܂�
	int	MaxOutputSize(void);
	  // execute �� s �ɍŒ���K�v�ȋ�o�C�g�� ��Ԃ�

		 ImdKanjiCodeConverter(void);
		~ImdKanjiCodeConverter();
};




// �f�R�[�_�E�G���R�[�_�̔h���N���X

#ifndef _IMDKCVLIB_CODE_H
#include "imdkcvlib-code.h"
#endif



// �R���o�[�^�̔h���N���X

#ifndef _IMDKCVLIB_CONV_H
#include "imdkcvlib-conv.h"
#endif




class Imd_AutoInputCodeDetecter{
  protected:
	ImdKcvDecoder_UCS	ducs4b,ducs4l,dutf7,dutf8,
				dutf16b,dutf16l;
	ImdKcvDecoder_JIS	djis;
	ImdKcvDecoder_EUC	deuc;
	ImdKcvDecoder_SJIS	dsjis;

	Imd_JIStoUCS		jn_u,ja_u,en_u,ea_u;
	Imd_SJISNtoUCS		sn_u;
	Imd_SJISAtoUCS		sa_u;

  public:
	// �e�R�[�h�Ƃ��ēǂ݂��񂾂Ƃ���
	// ���������łȂ��L���ȕ����̐��ƕ�������
	int	 ucs4b_v, ucs4b_all, ucs4b_k,  ucs4l_v, ucs4l_all, ucs4l_k;
	int	  utf7_v,  utf7_all,  utf7_k,   utf8_v,  utf8_all,  utf8_k;
	int	utf16b_v,utf16b_all,utf16b_k, utf16l_v,utf16l_all,utf16l_k;
	int	  jisn_v,  jisn_all,  jisn_k,   jisa_v,  jisa_all,  jisa_k;
	int	  eucn_v,  eucn_all,  eucn_k,   euca_v,  euca_all,  euca_k;
	int	 sjisn_v, sjisn_all, sjisn_k,  sjisa_v, sjisa_all, sjisa_k;

	void read(int i);
	int guess(void);
	  // �\�z�����R�[�h�̃^�C�v��Ԃ�

		 Imd_AutoInputCodeDetecter(void);
};
// �R�[�h�̃^�C�v
//  1: SJISN (SJIS + Windows�n�@��ˑ�����)
//  2: SJISA (SJIS + Macintosh�n�@��ˑ�����)
//  3: JISN  (JIS  + Windows�n�@��ˑ�����(JISX0208�O���̈�))
//  4: JISA  (JIS  + Macintosh�n�@��ˑ�����(JISX0208�O���̈�))
//  5: EUCN  (EUC  + Windows�n�@��ˑ�����(JISX0208�O���̈�))
//  6: EUCA  (EUC  + Macintosh�n�@��ˑ�����(JISX0208�O���̈�))
//  7: UCS-4 Big-endian
//  8: UCS-4 Little-endian
//  9: UTF-7
// 10: UTF-8
// 11: UTF-16 Big-endian
// 12: UTF-16 Little-endian
//
// (3-6�� -appleji �I�v�V������t���邩�t���Ȃ����ɑΉ�)







//  ��������������������������������������������������������������
//  ������                                                  ������
//  ������  �⏕�֐��錾                                    ������
//  ������                                                  ������
//  ��������������������������������������������������������������


//  ������ imdkcvlib-misc.cxx �Œ�`����Ă����Ȋ֐�      ������
// (��UCS) <-> UCS US-ASCII,JISX0201-{���e��,�J�i}
int fImd_JISX0201toUCS(ImdKcvCode *pkc);
int fImd_UCStoJISX0201(ImdKcvCode *pkc);
int fImd_UCSto8859(ImdKcvCode *pkc);
int fImd_8859toUCS(ImdKcvCode *pkc);

//  ������ imdkcvlib-jis2.cxx �Œ�`����Ă����Ȋ֐�      ������
// JIS <-> UCS �ϊ�
int fImd_UCStoJISX0208(int iUcs,int *piJis208);
int fImd_JISX0208toUCS(int iJis208,int *piUcs);
int fImd_UCStoJISX0212(int iUcs,int *piJis212);
int fImd_JISX0212toUCS(int iJis212,int *piUcs);

// NEC���ꕶ���E{NEC|IBM}�I��IBM�g������ �p�֐� (JIS��)
// JISX0208�O���̈�(13,89-91��) <-> UCS �̃}�b�s���O
int fImd_JIStoUCS_NEC(ImdKcvCode *pkc);
int fImd_UCStoJIS_NEC(ImdKcvCode *pkc);
int fImd_JIStoUCS_NECIBM(ImdKcvCode *pkc);
int fImd_UCStoJIS_NECIBM(ImdKcvCode *pkc);

// Apple�g�������p�֐� (JIS��)
int fImd_JIStoUCS_APPLE(ImdKcvCode *pkc);
int fImd_UCStoJIS_APPLE(ImdKcvCode *pkc);


//  ������ imdkcvlib-sjis.cxx �Œ�`����Ă����Ȋ֐�      ������
// JISX0208 �͈�(�@��ˑ�����������)�ɂ��邩�ǂ���
int fImd_IsJISX0208(int jis);   //  JIS�R�[�h��
int fImd_IsSJIS(int sjis);      // SJIS�R�[�h��

// JISX0208 �͈͓��Ɍ����� JIS<->SJIS �ϊ�
// �߂�l 0:JISX0208�͈͊O  1:�ϊ�����
int fImd_JIStoSJIS(int iJis, int *piSjis);
int fImd_SJIStoJIS(int iSjis, int *piJis);
// �O�����܂߂��o�[�W���� -1 ���߂�l�Ȃ�͈͊O
int fImd_JIStoSJIS_X(int iJis);
int fImd_SJIStoJIS_X(int iSjis);

// NEC���ꕶ���E{NEC|IBM}�I��IBM�g������ �p�֐� (SJIS��)
int fImd_SJIStoUCS_NEC(ImdKcvCode *pkc);
int fImd_UCStoSJIS_NEC(ImdKcvCode *pkc);
int fImd_SJIStoUCS_NECIBM(ImdKcvCode *pkc);
int fImd_UCStoSJIS_NECIBM(ImdKcvCode *pkc);
int fImd_SJIStoUCS_IBMIBM(ImdKcvCode *pkc);
int fImd_UCStoSJIS_IBMIBM(ImdKcvCode *pkc);
int fImd_SJISrecover_NEC(int i);

// Apple�g�������p�֐� (SJIS��)
int fImd_SJIStoUCS_APPLE(ImdKcvCode *pkc);
int fImd_UCStoSJIS_APPLE(ImdKcvCode *pkc);
int fImd_SJISrecover_APPLE(int i);

#endif // _IMDKCVLIB_H
