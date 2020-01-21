// CcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcCcC
// CcCcC                                                       CcCcC
// CcCcC  imdkcvlib-jis1.cxx (C++ Source File)                 CcCcC
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
//  ������  Imd_UCStoJIS �N���X                             ������
//  ������                                                  ������
//  ��������������������������������������������������������������
ImdKcvCode* Imd_UCStoJIS::convert(ImdKcvCode *pkc){
	int		iUcs = pkc->kcode,iJis,inec,inecibm,
			iapple,iwa,i;
	ImdKcvCode	*pkci;

	inec    = f208mode & 0xF;
	iapple  = inec;
	inecibm = (f208mode >> 4 ) & 0xF;
	iwa     = (f208mode >> 12) & 0xF;

	i = f208mode & 0x100;
	if(i != 0 && pkc->housetsu != 0){ // ��ۃ��J�o��
		if(iwa == 1){
			i = fImd_SJISrecover_NEC(pkc->housetsu);
			if(i != 0){
				i = fImd_SJIStoJIS_X(pkc->housetsu);
				if(i != -1){
					pkc->kcode = i;
					if(pkc->housetsu < 0x87FD){pkc->codetype = 5;}
					else{pkc->codetype = 7;}
					return pkc;
		}	}	}
		else if(iwa==2 && (0xFF < pkc->housetsu)){
			i = fImd_SJISrecover_APPLE(pkc->housetsu);
			if(i != 0){
				pkc->kcode = fImd_SJIStoJIS_X(pkc->housetsu);
				pkc->codetype = 8;
				return pkc;
	}	}	}

	if(pkc->codetype == -3){ // �� UCS �������J�o��
		if(iwa == 2 && 1 <= iapple){ // JISX0212 �͂Ȃ�
			pkc->codetype = 8; pkci = pkc->copy();
			if(fImd_SJIStoUCS_APPLE(pkci) == 2){
				pkc->kcode = fImd_SJIStoJIS_X(pkc->kcode);
				delete pkci; return pkc;
		}	}
		pkc->codetype = -2; return pkc;
	}

	if(pkc->codetype < 0){return pkc;}
	if(fImd_UCStoJISX0201(pkc) != 0){return pkc;}
	if(fImd_UCStoJISX0208(iUcs,&iJis) != 0){
		pkc->kcode = iJis;
		pkc->codetype = 4;
		return pkc;
	}

	if(iwa == 1){
		if(2 <= inec){ // X0208�O���̈�ɕϊ�����ꍇ(NEC����)
			if(fImd_UCStoJIS_NEC(pkc) != 0){return pkc;}
		}
		if(2 <= inecibm){ // X0208�O���̈�ɕϊ�����ꍇ(NEC�I��IBM)
			if(fImd_UCStoJIS_NECIBM(pkc) != 0){return pkc;}
	}	}
	else if(iwa == 2){
		if(2 <= iapple){ // X0208�O���̈�ɕϊ�����ꍇ(Apple�g��)
			if(fImd_UCStoJIS_APPLE(pkc) != 0){return pkc;}
	}	}

	if(fImd_UCStoJISX0212(iUcs,&iJis) != 0){
		pkc->kcode = iJis;
		pkc->codetype = 9;
		return pkc;
	}

	if(iwa == 1){
		if(1 == inec){ // X0212��D�悳����ꍇ(NEC����)
			if(fImd_UCStoJIS_NEC(pkc) != 0){return pkc;}
		}
		if(1 == inecibm){ // X0212��D�悳����ꍇ(NEC�I��IBM)
			if(fImd_UCStoJIS_NECIBM(pkc) != 0){return pkc;}
	}	}
	else if(iwa == 2){
		if(1 == iapple){ // X0212��D�悳����ꍇ(Apple�g��)
			if(fImd_UCStoJIS_APPLE(pkc) != 0){return pkc;}
	}	}

	if(fImd_UCSto8859(pkc) != 0){return pkc;}

	pkc->codetype = -2; return pkc;
}


//  ��������������������������������������������������������������
//  ������                                                  ������
//  ������  Imd_JIStoUCS �N���X                             ������
//  ������                                                  ������
//  ��������������������������������������������������������������
ImdKcvCode* Imd_JIStoUCS::convert(ImdKcvCode *pkc){
	int		iUcs,iJis = pkc->kcode;

	if(pkc->codetype < 0){return pkc;}
	else if(pkc->codetype == 9){
	  // �R�[�h�ԍ������ł� 208 �� 212 �̋�ʂ��s�\
		if(fImd_JISX0212toUCS(iJis,&iUcs) != 0){
			pkc->kcode = iUcs;
		}
		else{pkc->codetype = -2;}
		return pkc;
	}
	else if(fImd_JISX0201toUCS(pkc) != 0){return pkc;}
	else if(fImd_8859toUCS(pkc) != 0){return pkc;}
	else if(fImd_JISX0208toUCS(iJis,&iUcs) != 0){
		pkc->kcode = iUcs;
		pkc->codetype = 4;
		return pkc;
	}

	// JISX0208�O���̈�
	if(f208mode == 1){
		if     (fImd_JIStoUCS_NEC(pkc) != 0){return pkc;}
		  // 13��(NEC���ꕶ��)�̏ꍇ
		else if(fImd_JIStoUCS_NECIBM(pkc) != 0){return pkc;}
		  // 89-92��(NEC�I��IBM�g������)�̏ꍇ
		  // 
		  // IBM�I��IBM�g�������� JISX0208�O���̈�ւ�
		  // �}�b�s���O�͖���
		else{pkc->codetype = -2; return pkc;}
	}
	else if(f208mode == 2){
		if     (fImd_JIStoUCS_APPLE(pkc) != 0){return pkc;}
		  // Apple�g�������̏ꍇ
		else{pkc->codetype = -2; return pkc;}
	}

	else{pkc->codetype = -2;}

	return pkc;
}


//  ��������������������������������������������������������������
//  ������                                                  ������
//  ������  Imd_JISX0208Gaiji_filter �N���X                 ������
//  ������                                                  ������
//  ��������������������������������������������������������������
ImdKcvCode* Imd_JISX0208Gaiji_filter::convert(ImdKcvCode *pkc){
	int	i,ku,ten;

	// codetype == 0 �������������������AJISX0208��JISX0212��
	// ���ʂł��Ȃ����� codetype == 0 �̓t�B���^���Ȃ�
	// ( ���������I���W�i���̃f�R�[�_�E�R���o�[�^��JIS�R�[�h
	//   �ɕϊ�����ۂ� codetype �� 0 �ɂȂ�Ȃ� )

	if(pkc->codetype < 0){}
	else if(4 <= pkc->codetype && pkc->codetype <= 8){
		i = pkc->kcode - 0x2020;
		ku = (i >> 8) & 0xFFFF;
		ten = i & 0xFF;
		if(1<=ku && ku<=94 && 1<=ten && ten<=94){
			  // 1�`94 ��� 1�`94 �_�̏ꍇ
			if     (ku == 2 && (15<=ten && ten<=25 ||
			 34<=ten && ten<=41 || 49<=ten && ten<=59 ||
			 75<=ten && ten<=81 || 90<=ten && ten<=93))
				{pkc->codetype = -2;}
			else if(ku == 3 && (1<=ten && ten<=15 ||
			 26<=ten && ten<=32 || 59<=ten && ten<=64 ||
			 91<=ten)){pkc->codetype = -2;}
			else if(ku == 4 && 84<=ten)
				{pkc->codetype = -2;}
			else if(ku == 5 && 87<=ten)
				{pkc->codetype = -2;}
			else if(ku == 6 && (25<=ten && ten<=32 ||
			 57<=ten)){pkc->codetype = -2;}
			else if(ku == 7 && (34<=ten && ten<=48 ||
			 82<=ten)){pkc->codetype = -2;}
			else if(ku == 8 && 33<=ten)
				{pkc->codetype = -2;}
			else if(9<=ku && ku <=15 ||
			 85<=ku && ku<=94)
				{pkc->codetype = -2;}
			else if(ku == 47 && 52<=ten)
				{pkc->codetype = -2;}
			else if(ku == 84 && 6<=ten)
				{pkc->codetype = -2;}

		}
		else{pkc->codetype = -2;}
	}
	return pkc;
}


//  ��������������������������������������������������������������
//  ������                                                  ������
//  ������  Imd_JISX0212_filter �N���X                      ������
//  ������                                                  ������
//  ��������������������������������������������������������������
ImdKcvCode* Imd_JISX0212_filter::convert(ImdKcvCode *pkc){
	if(pkc->codetype == 9){
		pkc->codetype = -2;
	}

	return pkc;
}


