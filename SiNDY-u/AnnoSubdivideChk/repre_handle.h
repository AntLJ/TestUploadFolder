/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

//------------------------------------------------------------
//
// ��\�_�������N���X
//
//------------------------------------------------------------
#include <stdio.h>
#include <set>

#ifndef	_REPRE_HANDLE_H_
#define	_REPRE_HANDLE_H_

using namespace stlport;

/**
	�X�V���
		2003.4.16	repre_pt�̑㏞��������W�l���g�p����悤�ɕύX�i�������̂ق����悢�H�j
*/

/**
	��\�_�p�N���X
	�P��11���R�[�h�����s���E�ɑ債�ẮA�P�̑�\�_�����Ȃ��Ƃ������Ƃ�O��
*/
class	repre_pt
{
public:
	char	e_cpCode[12];	///< 11���R�[�h
	char	e_cpName[128];	///< ����
	string	e_sLayerName;	///< ���C������
	int		e_iType;		///< �f�[�^�^�C�v
	int		e_iOid;			///< �I�u�W�F�N�gID
	double	e_dX;			///< ��\�_X���W�i�o�x�H�j
	double	e_dY;			///< ��\�_Y���W�i�ܓx�H�j
	IGeometryPtr	e_ipPt;	///< �|�C���g�W�I���g�� 2003.04.21
public:
	/**
		�R���X�g���N�^
	*/
	repre_pt() {
		memset	( e_cpCode, '\0', 12 );
		memset	( e_cpName, '\0', 128 );
		e_sLayerName	= "";
		e_iType	= 0;
		e_iOid	= 0;
		e_dX	= 0.0;
		e_dY	= 0.0;
		e_ipPt	= NULL;
	}
	/**
		�f�X�g���N�^
	*/
	~repre_pt() {
	}

	/**
		��r���Z�q��`
		@return	�^ = 1
		@return �U = 0
	*/
	int	operator<( const repre_pt& c_rDat ) const {
		int	ret	= 0;
		int	cmpret	= 0;
		cmpret	= strcmp ( e_cpCode, c_rDat.e_cpCode );	// 11���ł̔�r
		if( cmpret != 0 ) {
			if( cmpret < 0 ) {
				ret	= 1;
			}
		}else
		if( e_iType != c_rDat.e_iType ) {		// �f�[�^�^�C�v�ł̔�r
			if( e_iType < c_rDat.e_iType ) {
				ret	= 1;
			}
		}
		return	( ret );
	}

	/**
		���l���Z�q��`
		@return	�^ = 1
		@return �U = 0
	*/
	int	operator==( const repre_pt& c_rDat ) const {
		int	ret	= 0;
		if( strcmp ( e_cpCode, c_rDat.e_cpCode ) == 0 ) {
			if( e_dX == c_rDat.e_dX && e_dY == c_rDat.e_dY ) {
				ret	= 1;
			}
		}
		return	( ret );
	}

};

typedef	multiset<repre_pt, less<repre_pt> >				ms_repre_pt;
typedef	multiset<repre_pt, less<repre_pt> >::iterator	msi_repre_pt;

#endif	//_REPRE_HANDLE_H_
