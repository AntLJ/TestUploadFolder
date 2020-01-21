// HhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhHhH
// HhHhH                                                       HhHhH
// HhHhH  dxarg.h (C++ Header File)                            HhHhH
// HhHhH                       Version  3. 0. 0                HhHhH
// HhHhH                                                       HhHhH
// HhHhH        Copyright (C)  1997-2000  Yosuke IMADA         HhHhH
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

#ifndef _IMD_DXARG_H
#define _IMD_DXARG_H 1

#ifndef NULL
  // NULL ����`����Ă��Ȃ��Ȃ� stdio.h ���C���N���[�h����D
  // NULL ����`����Ă��Ȃ��ƃR���p�C���ł��Ȃ�����ł��D
  #include <stdio.h>
#endif

class ImdCargl{
  // ������̃��X�g�̃N���X
  public:
	ImdCargl	*next;	// ���̃��X�g
	int		df;	// s �� delete ����K�v�����邩�ǂ����̃t���O
	char		*s;	// ������

	void  kill(void);	// �폜
	int   set(char *t);	// s �𓮓I�m�ۂ��ĕ�������R�s�[���Ċi�[
				// ���I�m�ێ��s������ 0 ��Ԃ�

	      ImdCargl(void){s = NULL; next = NULL; df = 0;}
	      ImdCargl(char *t){s = NULL; next = NULL; df = 0; set(t);}
	     ~ImdCargl(){kill();}
};

class ImdCarg{
  public:
	int		size;	// �����Ă��� Cargl �̐�
	ImdCargl	*head,	// ImdCargl �̐擪�ւ̃A�h���X
			**pp;	// ���X�g���q���Ƃ��ɏ���������ꏊ

	void  kill(void);	// �S�Ẵ��X�g��j��
	void  add(char *t);	// t ���R�s�[���ă��X�g���쐬����
	void  add2(char *t);	// t ���w�����X�g���쐬����
	char *get(int i);	// i �Ԗڂ̕�����𓾂� (i: 0 �ȏ� size ����)
	int   getsize(void){return size;}

	      ImdCarg(void){size = 0; head = NULL; pp = &head;}
	     ~ImdCarg(){kill();}
};

// Windows �ł̓V�F���� * �� ? ��W�J�����ɓn���D
// ����� UNIX �̂悤�ɓW�J������̂����̃N���X�� init �֐��̖ړI�ł���D
// �P�Ȃ�֐��ł��ǂ����C���I�Ɋm�ۂ����������������I�ɊJ�����邽�߂�����
// �N���X�̌`���Ƃ��Ă���D
//   �g����:
//    1. dxarg.h ���C���N���[�h����
//	#include "dxarg.h"
//    2. main �֐��� main_dx �֐��Ƀ��l�[������D
//	int main_dx(int argc,char **argv);
//
//    3. ���̂悤�� main �֐����`����
//	int main(int dargc,char *dargv[]){
//		int		i,argc,nc;
//		char		**argv,**nv;
//		ImdDxArg	_imdda(dargc,dargv,&argc,&argv,&nc,&nv);
//
//		// no match 
//		for(i = 0; i < nc; i++){printf("no match %s\n",nv[i]);}
//		return main_dx(argc,argv);
//	}
//
class ImdDxArg{
  protected:
	char		**nwargv,	// �V���� argv �̎��������p�|�C���^
			**nwnomatch;	// �}�b�`���Ȃ������t�@�C��
	ImdCarg		ca;		// �V���� argv �̕����񃊃X�g��

	void kill(void);
	void searchA(char *fn);		// *,? ��W�J����
	void search(char *t);		// ��΃p�X�ɂ��� searchA �ɓn��
	void init(int argc,char *argv[],int *xargc,char ***xargv,
			int *nNoMatch,char ***sNoMatch);
		// *,? �������K�v�ȃt�@�C����T���� search �ɓn��

  public:
	     ImdDxArg(int argc,char *argv[],int *xargc,char ***xargv,
			int *nNoMatch = NULL,char ***sNoMatch = NULL)
			{init(argc,argv,xargc,xargv,nNoMatch,sNoMatch);}
	    ~ImdDxArg(){kill();}
};

#endif // _IMD_DXARG_H

