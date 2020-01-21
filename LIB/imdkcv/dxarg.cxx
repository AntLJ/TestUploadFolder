// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  dxarg.cxx (C++ Source File)                          CcCcC
// CcCcC                       Version  3. 0. 0                CcCcC
// CcCcC                                                       CcCcC
// CcCcC        Copyright (C)  1997-2000  Yosuke IMADA         CcCcC
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include "dxarg.h"

// ���� _IMD_DXARG_DOUBLEAST �� 0 �ȊO�ɂ���� \**\ ��
// �O�ȏ�̔C�ӂ̃f�B���N�g���Ƃ��Ă݂Ȃ��D
// ���Ȃ݂ɂ��̏ꍇ \*\**\ �Ə����΂P�ȏ�̔C�ӂ̃f�B���N�g���ɂȂ�
#define _IMD_DXARG_DOUBLEAST 0

void ImdCargl::kill(void){
	if(df != 0){
		delete s; df = 0;
	}
	s = NULL;
}

int  ImdCargl::set(char *t){
	kill();
	s = new char[strlen(t)+1];
	if(s == NULL){return 0;}
	df = 1; strcpy(s,t);
	return 1;
}

void ImdCarg::kill(void){
	ImdCargl	*b;

	while(head != NULL){
		b = head; head = head->next;
		delete b;
	}
	size = 0; pp = &head;
}

void ImdCarg::add(char *t){
	// t ���R�s�[���ă��X�g���쐬����
	*pp = new ImdCargl(t);
	pp = &((*pp)->next); size++;
}

void ImdCarg::add2(char *t){
	// ���X�g�� t ���̂������D(�R�s�[���Ȃ�)
	// t �� delete ����K�v���Ȃ��̂� pp->df == 0 �Ń��X�g�쐬
	*pp = new ImdCargl();
	(*pp)->s = t; pp = &((*pp)->next); size++;
}

char *ImdCarg::get(int i){
	ImdCargl	*c = head;

	if(i < 0 || size <= i) return NULL;
	while(i){c = c->next; i--;}
	return c->s;
}

void ImdDxArg::kill(void){
	if(nwargv    != NULL){free(nwargv);    nwargv = NULL;}
	if(nwnomatch != NULL){free(nwnomatch); nwnomatch = NULL;}
}

#if _IMD_DXARG_DOUBLEAST
  // \**\ ���O�ȏ�̔C�ӂ̃f�B���N�g���Ƃ݂Ȃ��o�[�W����
  void ImdDxArg::searchA(char *fn){
	int			i,ifn,bmark,prevbs;
	char			s[1024];
	struct _finddata_t	fd;
	long			lh;

	ifn = 0;	// fn �p�C���W�P�[�^
	bmark = 0;	// * �� ? ���������� 1 �ɂȂ�
	prevbs = 0;	// �Ō�� '\\' �̈ʒu

	for(;fn[ifn] != '\0';ifn++){
		if     ((fn[ifn] == '\\') && (bmark == 0)){
			// '\\' �̈ʒu�̃o�b�N�A�b�v
			prevbs = ifn;
		}else if(fn[ifn] == '\\'){
			// �f�B���N�g�������� * �� ? ���������ꍇ
			for(i=0;i <= prevbs;i++){s[i] = fn[i];}

			if((fn[ifn-3] == '\\') && (fn[ifn-2] == '*') &&
			   (fn[ifn-1] == '*')){
				// \**\ �̏ꍇ
				// �Ⴆ�� fn = c:\home\data\**\*.txt �̏ꍇ
				// s = c:\home\data\ fn = c:\home\data\* �ɂ���
				s[prevbs + 1] = '\0'; fn[ifn - 1] = '\0';

				// fn �Ō���
				// \**\ ���P�ȏ�̃f�B���N�g���Ƃ��Ă݂Ȃ��ꍇ
				lh = _findfirst(fn,&fd); i = (int)lh;
				for(;i != -1; i = _findnext(lh,&fd)){
					// �f�B���N�g������Ȃ���� continue
					if(!(fd.attrib & _A_SUBDIR)){continue;}
					if(strcmp(fd.name,".") && strcmp(fd.name,"..")){
						// . �ł� .. �ł��Ȃ���� s �ɋ�̓I��
						// ���O�����čċN�Ăяo��
						// �� s = c:\home\data\xyz\**\*.txt
						s[prevbs+1] = '\0';
						strcat(s,fd.name);
						strcat(s,"\\**\\");
						strcat(s,&(fn[ifn+1]));
						searchA(s);
				}	}
				_findclose(lh);

				// \**\ ���O�̃f�B���N�g���Ƃ��Ă݂Ȃ��ꍇ
				// s = c:\home\data\*.txt
				strcat(s,&(fn[ifn+1])); searchA(s);
				fn[ifn - 1] = '*';  // fn �����ɖ߂�
				return ;
			}else{
				// �Ⴆ�� fn = c:\home\data\x?z\*.txt �̏ꍇ
				// s = c:\home\data\ fn = c:\home\data\x?z �ɂ���
				fn[ifn] = '\0';

				// fn �Ō���
				lh = _findfirst(fn,&fd); i = (int)lh;
				for(;i != -1; i = _findnext(lh,&fd)){
					// �f�B���N�g������Ȃ���� continue
					if(!(fd.attrib & _A_SUBDIR)){continue;}
					if(strcmp(fd.name,".") && strcmp(fd.name,"..")){
						// . �ł� .. �ł��Ȃ���� s �ɋ�̓I��
						// ���O�����čċN�Ăяo��
						// �� s = c:\home\data\xyz\*.txt
						s[prevbs+1] = '\0';
						strcat(s,fd.name);
						strcat(s,"\\");
						strcat(s,&(fn[ifn+1]));
						searchA(s);
				}	}
				_findclose(lh);
				fn[ifn] = '\\';  // fn �����ɖ߂�
				return ;
			}
		}
		else if(fn[ifn] == '*' || fn[ifn] == '?'){bmark = 1;}
	}

	// �t�@�C������ *,? ������ꍇ (�� c:\data\*.txt )
	// �t�@�C�����ȊO�� s �Ɋi�[ (s = c:\data\ )
	for(i = 0; i <= prevbs; i++){s[i] = fn[i];}

	lh = _findfirst(fn,&fd); i = (int)lh;
	for(;i != -1; i = _findnext(lh,&fd)){
		// �t�@�C������Ȃ��ꍇ�� continue
		if(fd.attrib & _A_SUBDIR){continue;}

		// s �ɋ�̓I�Ȗ��O������
		s[prevbs + 1] = '\0';	// (s == c:\data\ )
		strcat(s,fd.name);	// (fd.name == a.txt )
		ca.add(s);
	}
	_findclose(lh);
  }

#else // _IMD_DXARG_DOUBLEAST
  // \**\ ���O�ȏ�̔C�ӂ̃f�B���N�g���Ƃ݂Ȃ��Ȃ��o�[�W����

  void ImdDxArg::searchA(char *fn){
	int			i,ifn,bmark,prevbs;
	char			s[1024];
	struct _finddata_t	fd;
	long			lh;

	ifn = 0;	// fn �p�C���W�P�[�^
	bmark = 0;	// * �� ? ���������� 1 �ɂȂ�
	prevbs = 0;	// �Ō�� '\\' �̈ʒu

	for(;fn[ifn] != '\0';ifn++){
		if     ((fn[ifn] == '\\') && (bmark == 0)){
			// '\\' �̈ʒu�̃o�b�N�A�b�v
			prevbs = ifn;
		}else if(fn[ifn] == '\\'){
			// �f�B���N�g�������� * �� ? ���������ꍇ

			// �Ⴆ�� fn = c:\home\data\x?z\*.txt �̏ꍇ
			// s = c:\home\data\, fn = c:\home\data\x?z �ɂ���D
			for(i=0;i <= prevbs;i++){s[i] = fn[i];}
			fn[ifn] = '\0';

			// fn �Ō���
			lh = _findfirst(fn,&fd); i = (int)lh;
			for(;i != -1; i = _findnext(lh,&fd)){
				// �f�B���N�g������Ȃ���� continue
				if(!(fd.attrib & _A_SUBDIR)){continue;}
				if(strcmp(fd.name,".") && strcmp(fd.name,"..")){
					// . �ł� .. �ł��Ȃ���� s �ɋ�̓I��
					// ���O�����čċN�Ăяo��
					// �� s = c:\home\data\xyz\*.txt
					s[prevbs+1] = '\0';
					strcat(s,fd.name);
					strcat(s,"\\");
					strcat(s,&(fn[ifn+1]));
					searchA(s);
			}	}
			_findclose(lh);
			fn[ifn] = '\\';  // fn �����ɖ߂�
			return ;
		}
		else if(fn[ifn] == '*' || fn[ifn] == '?'){bmark = 1;}
	}

	// �t�@�C������ *,? ������ꍇ (�� c:\data\*.txt )
	// �t�@�C�����ȊO�� s �Ɋi�[ (s = c:\data\ )
	for(i = 0; i <= prevbs; i++){s[i] = fn[i];}

	lh = _findfirst(fn,&fd); i = (int)lh;
	for(;i != -1; i = _findnext(lh,&fd)){
		// �t�@�C������Ȃ��ꍇ�� continue
		if(fd.attrib & _A_SUBDIR){continue;}

		// s �ɋ�̓I�Ȗ��O������
		s[prevbs+1] = '\0';	// ( s == c:\data\ )
		strcat(s,fd.name);	// ( fd.name == a.txt )
		ca.add(s);
	}
	_findclose(lh);
  }

#endif // _IMD_DXARG_DOUBLEAST


void ImdDxArg::search(char *t){
	char		c,c_prev;
	int		i,j,slen,slenN,nDot,tlen,bAbsPath;
	char		s[1024];

	// ���̊֐��ł� * ? �͂܂��W�J���Ȃ�
	// �܂��t�@�C�����𑊑΃p�X����h���C�u���t��΃p�X�ɕϊ�����
	// (c:data.txt -> c:\tmp\data.txt �̕ϊ��Ȃ�)

	tlen = strlen(t);
	bAbsPath = 0;	// s �ɐ�΃p�X���i�[������ 1 ����

	if(t[0] == '\\'){
		// [1] ��΃p�X�Ńh���C�u���Ȃ�
		// �h���C�u���� (�Ⴆ�� C: �Ȃ�) ��t������
		s[0] = (char)(_getdrive() + 'A' - 1);
		s[1] = ':';
		strcpy(&(s[2]),t);
		// ����� s �͐�΃p�X�ɂȂ���
		bAbsPath = 1;
	}
	else if(2 < tlen){
		if(t[1] == ':'){
			if(t[2] == '\\'){
				// [2] ��΃p�X�Ńh���C�u������
				// ���ɏ����Ȃ�
				// �h���C�u���͑啶���ł���K�v������
				strcpy(s,t); s[0] = (char)toupper(s[0]);
			}
			else{
				// [3] ���΃p�X�Ńh���C�u������
				// ���̃h���C�u�̃J�����g�f�B���N�g���𓾂�
				_getdcwd(toupper(t[0]) - 'A' + 1,s,1024);
				slen = strlen(s);
				// �����J�����g�f�B���N�g���̍Ōオ '\\' ����
				// �Ȃ������� �����ǉ�
				if(s[slen - 1] != '\\'){
					s[slen] = '\\'; s[slen+1] = '\0';
				}
				// t �� s �ɐڑ�����ΐ�΃p�X�̏o���オ��
				strcat(s,&(t[2]));
			}
			bAbsPath = 1;
	}	}

	if(bAbsPath == 0){
		// [4] ���΃p�X�Ńh���C�u���Ȃ�
		_getcwd(s,1024); j = strlen(s);
		if(s[j-1] != '\\'){s[j] = '\\'; s[j+1] = '\0';}
		strcat(s,t);
	}

	// ���̎��_�� s �ɂ̓h���C�u���t��΃p�X���i�[����Ă���
	// ���C c:\tmp\..\text\\data.txt �݂����Ȍ`�ɂȂ��Ă��邩������Ȃ�
	// �̂ŁC . �� .. �̕ϊ��C\\ �̕ϊ����s���D
	// �Ȃ��C���[�g�f�B���N�g���̐e�̓��[�g�f�B���N�g���Ƃ���

	// s[] ������������ s[] �ɏ������ށD
	// �Z���Ȃ邾���Ȃ̂œ����o�b�t�@�ɏ������ނ��Ƃ��\�D
	// �ǂݍ��ݗp�̃C���W�P�[�^�� slen �ŏ������ݗp�� slenN �ɂȂ�D

	slen = slenN = 0; nDot = 0;

	// �ꕶ���O
	c_prev = '\0';

	while((c = s[slen]) != '\0'){
		if     ((c == '\\') && (c_prev == '\\')){
			// \ ���Q�����ꍇ�͂P�Ƀ}�[�W
			// slenN �̕����X�V���Ȃ����ƂłP���������
			slen++; continue;
		}else if((c == '.') && (c_prev == '\\')){
			// \. 
			nDot = 1;
		}else if((c == '.') && (nDot != 0)){
			// \.. �� \... �Ȃǂ̎�
			nDot++;
		}else if((c == '\\') && (nDot != 0)){
			// \.. �̃h�b�g�����m�肵���ꍇ (\..\)
			// �߂�ʒu��T��
			// �Ⴆ�� magic\data\imada\home\...\a.txt
			// �Ȃ� data �̌��� \ �̈ʒu�� slenN �Ɋi�[
			i = slenN - 1; slenN = -1;
			while(0 <= i){
				if(s[i] == '\\'){
					nDot--;
					if(nDot == 0){slenN = i; break;}
				}
				i--;
			}
			if(slenN == -1){
				// ���[�g�f�B���N�g���̐e�̓��[�g�f�B���N�g��
				// �h���C�u�����K�����Ă���̂�
				// �擪�� C:\ �̂悤�Ȍ`�ɂȂ��Ă���
				slenN = 2;
			}
			// magic\data\imada\home\...\a.txt ��
			// magic\data\a.txt �ŁC s[slen] �� a.txt �̑O�� \
			// s[j] �� data �̌��� \ �ɂȂ�D

			// s[slenN] �� s[slen] �� '\\' ������
			slen++; slenN++; c_prev = '\\';
			continue;
		}else if(nDot != 0){
			// \.emacs �̂悤�ȏꍇ�������Ƃ�
			nDot = 0;
		}

		// �ꕶ���O�̍X�V
		s[slenN] = s[slen]; slenN++; slen++;
		c_prev = c;
	}
	s[slenN] = '\0';

	// ���̎��_�� s �͐��K�����ꂽ�h���C�u���t��΃p�X

	searchA(s);
}

void ImdDxArg::init(int argc,char *argv[],int *xargc,char ***xargv,
			int *nNoMatch,char ***sNoMatch){
	int		i,j,bmark,iSizeBack;
	ImdCarg		caNoMatch;


	// �m�肵�����̂� ca �Ɋi�[���Ă���
	// argv[0] �͊m��
	ca.add2(argv[0]);

	for(i = 1; i < argc; i++){
		j = bmark = 0;
		// *,? ���܂܂��� bmark �� 0 �ȊO�ɂ���
		while(argv[i][j] != '\0'){
			if(argv[i][j] == '*' || argv[i][j] == '?'){bmark = 1;}
			j++;
		}
		// ������ * �� ? ���܂܂�Ă��Ȃ���� ���̂܂� ca �Ɋi�[
		if(bmark == 0){ca.add2(argv[i]);}
		// �܂܂�Ă���ΓW�J����
		else{
			// ���݊m�肵�Ă��� argc ���o�b�N�A�b�v
			iSizeBack = ca.getsize();
			search(argv[i]);
			if(iSizeBack == ca.getsize()){
				// search() �̌��� argc �������Ȃ������ꍇ
				// �܂� argv[i] �Ƀ}�b�`������̂�1��
				// �Ȃ��ꍇ
				caNoMatch.add2(argv[i]);
	}	}	}

	// �V���� argc �̒l���Z�o
	*xargc = j = ca.getsize();

	// �V���� argv ���쐬����  ( �T�C�Y j �� (char *) �z�� )
	nwargv = *xargv = (char **)malloc(sizeof(char *) * j);
	for(i=0;i < j;i++){
		(*xargv)[i] = ca.get(i);
	}

	if(sNoMatch != NULL && nNoMatch != NULL){
		// �}�b�`���Ȃ���������������ꂼ��i�[����
		*nNoMatch = j = caNoMatch.getsize();
		nwnomatch = *sNoMatch = (char **)malloc(sizeof(char *) * j);
		for(i=0;i < j;i++){
			(*sNoMatch)[i] = caNoMatch.get(i);
		}
	}else{
		nwnomatch = NULL;
	}
}

