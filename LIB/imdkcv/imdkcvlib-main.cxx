// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcvlib-main.cxx (C++ Source File)                 CcCcC
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

//  ��������������������������������������������������������������
//  ������                                                  ������
//  ������  ImdKcvCode �����o�֐�                           ������
//  ������                                                  ������
//  ��������������������������������������������������������������

void ImdKcvCode::cat(ImdKcvCode **pphead){
	while(*pphead != NULL){
		pphead = &((*pphead)->next);
	}
	*pphead = this;
}

ImdKcvCode* ImdKcvCode::copy(void){
	ImdKcvCode	*pi,*pre;

	if(next != NULL){pi = next->copy();}
	else{pi = NULL;}

	pre = new ImdKcvCode(kcode,org_kcode,codetype);
	pre->housetsu = housetsu;
	if(pre == NULL){delete pi; return NULL;}
	  // new �Ɏ��s�����ꍇ�͂���܂ŃR�s�[���������폜��
	  // NULL ��Ԃ�
	pre->next = pi;

	return pre;
}

ImdKcvCode::ImdKcvCode(void){
	next=NULL;
	kcode = org_kcode = codetype = housetsu = 0;
}

ImdKcvCode::ImdKcvCode(int vkcode,int vorg_kcode,
		int vcodetype){
	next = NULL;
	kcode = vkcode; org_kcode = vorg_kcode;
	codetype = vcodetype; housetsu = 0;
}

ImdKcvCode::ImdKcvCode(ImdKcvCode **pphead,int vkcode,int vorg_kcode,
		int vcodetype){
	next = NULL;
	kcode = vkcode; org_kcode = vorg_kcode;
	codetype = vcodetype; housetsu = 0;
	cat(pphead);
}


//  ��������������������������������������������������������������
//  ������                                                  ������
//  ������  ImdConvModuleList �����o�֐�                    ������
//  ������                                                  ������
//  ��������������������������������������������������������������
ImdKcvCode* ImdConvModuleList::execute(ImdKcvCode *pkc){
	int		cdtype;
	ImdKcvCode	*pkci,*pkcj,*pkck,*pkcre;

	pkcre = NULL;
	cdtype = pkc->codetype;
	  // convert �֐����ĂԂ� pkc �̒l�������ɂȂ邽��

	pkci =	pcm->convert(pkc); // ���i�K

	if(cdtype == -1){
		for(pkcj = pkci; pkcj != NULL; pkcj = pkcj->next){
			if(pkcj->codetype == -1){cdtype = 0;}
		}
		if(cdtype == -1){
			  // EOF ����͂����ɂ�������炸�A
			  // ���i�K�� EOF ���o�͂��Ȃ������ꍇ
			new ImdKcvCode(&pkci,-1,-1,-1);
			  // ���i�K�̌��ʂ̍Ō���ɐڑ�
	}	}

	if(next == NULL){pkcre = pkci;}
	else{
		pkcj = pkci;
		while(pkcj != NULL){
			pkck = pkcj; pkcj = pkck->next;
			pkck->next = NULL;
			  // ���i�K�̌��ʂ����؂�o��
			pkci = next->execute(pkck); // ���i�K
			  // ���̎��_�� pkck �̒��g�͖���
			  // delete ����Ă��邩�Apkci �ɗ��p
			  // ����Ă��邩�̂ǂ��炩�B
			  // �Ō�ɑ��i�K�̌��ʂ������o�͂��邪
			  // ���i�K�̌��ʂ� delete ����K�v�͖���
			pkci->cat(&pkcre);
			  // pkcre �̌��� pkci ���q��
	}	}

	return pkcre;
}

int ImdConvModuleList::concat(ImdConvModuleList **pphead){
	// ���X�g�̍Ō���ɂ��̃��X�g��ڑ�

	if(pphead == NULL){return -1;}
	while(*pphead != NULL){pphead = &((*pphead)->next);}
	*pphead = this; next = NULL;
	return 0;
}

void ImdConvModuleList::kill_all(void){
	if(next != NULL){delete next;}
	if(pcm  != NULL){delete pcm ;}
}

int ImdConvModuleList::MaxOutputSize(void){
	int		re = 1;

	if(pcm  != NULL){re *=  pcm->MaxOutputSize();}
	if(next != NULL){re *= next->MaxOutputSize();}
	return re;
}


//  ��������������������������������������������������������������
//  ������                                                  ������
//  ������  ImdKanjiCodeConverter �����o�֐�                ������
//  ������                                                  ������
//  ��������������������������������������������������������������
// �߂�l:�O�̃f�R�[�_ or NULL
ImdKcvDecoder* ImdKanjiCodeConverter::SetDecoder(ImdKcvDecoder *vpkd){
	ImdKcvDecoder	*re = pkd;
	pkd = vpkd; return re;
}

// �߂�l:�O�̃f�R�[�_ or NULL
ImdKcvEncoder* ImdKanjiCodeConverter::SetEncoder(ImdKcvEncoder *vpke){
	ImdKcvEncoder	*re = pke;
	pke = vpke; return re;
}

// �߂�l -1:�G���R�[�_�E�f�R�[�_���Z�b�g����Ă��Ȃ�
int ImdKanjiCodeConverter::execute(int i,char *s,int *piOffset){
	ImdKcvCode		*pkci,*pkcj,*pkck,*pkcm;
	int			re = 0;

	if(pkd == NULL || pke == NULL){
		  // �f�R�[�_�E�G���R�[�_���Z�b�g����Ă��Ȃ�
		return -1;
	}

	pkci = pkd->decode(i);
	  // �f�R�[�h

	if(phead != NULL){
		pkck = NULL;
		while(pkci != NULL){
			pkcm = pkci;pkci = pkci->next;
			pkcm->next = NULL;
			pkcj = phead->execute(pkcm);
			if(pkcj != NULL){pkcj->cat(&pkck);}
		}
		  // �R���o�[�^�Ɉꕶ���ꕶ���n���Ă���
		  // ( �G���R�[�_�����������Ԃ��\�������邩�� )
		pkci = pkck;
	}

	for(pkcj = pkci; pkcj != NULL; pkcj = pkcj->next){
		re = re + pke->encode(pkcj,s,piOffset);
	}

	delete pkci;

	  // �G���R�[�_
	  // �R���o�[�^�̌��� or �f�R�[�_�̌���(�R���o�[�^������
	  // �ꍇ) �����

	return re;
}

int ImdKanjiCodeConverter::MaxOutputSize(void){
	int		re = 1;

	if(pkd == NULL){re = 0;}
	else{re *= pkd->MaxOutputSize();}
	  // �f�R�[�_

	if(phead != NULL){
		re *= phead->MaxOutputSize();
	}
	  // �R���o�[�^

	if(pke == NULL){re = 0;}
	else{re *= pke->MaxOutputSize();}
	  // �G���R�[�_

	return re;
}

int ImdKanjiCodeConverter::ConcatinateModule(ImdConvModule *pcm){
	ImdConvModuleList	*pkcvlist;

	if(pcm == NULL){return -1;}
	pkcvlist = new ImdConvModuleList(&phead,pcm);
	if(pkcvlist == NULL){return -2;}
	return 0;
}

void ImdKanjiCodeConverter::DeleteModules(void){
	if(phead != NULL){delete phead; phead = NULL;}
}

ImdKanjiCodeConverter::ImdKanjiCodeConverter(void){
	pkd = NULL; pke = NULL; phead = NULL;
}

ImdKanjiCodeConverter::~ImdKanjiCodeConverter(){
	if(pkd != NULL){delete pkd;}
	if(phead != NULL){delete phead;}
	if(pke != NULL){delete pke;}
}
