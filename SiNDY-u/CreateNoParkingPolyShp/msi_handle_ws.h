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

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
#include "feature_info.h"

#ifndef	_MSI_HANDLE_WS_H_
#define	_MSI_HANDLE_WS_H_

#define	RET_COUNT	0x01
#define	RET_FCURSOR	0x02

/**
	���[�N�X�y�[�X���̃n���h���p�̃N���X
*/
class	msi_handle_ws
{
protected:
	IFeatureWorkspacePtr	e_ipFeWSpace;		///< ���[�N�X�y�[�X
	IFeatureClassPtr		e_ipFeCls;			///< ��r���t�B�[�`���N���X
	IFeatureCursorPtr		e_ipFeCsr;			///< ��r���t�B�[�`���J�[�\��
	char*					e_cpLayer;			///< ���C������
	long					e_lData_Count;		///< ���݃T�[�`���������̃f�[�^��
public:
	/**
		�R���X�g���N�^
	*/
	msi_handle_ws	() {
		e_ipFeWSpace	= NULL;
		e_ipFeCls		= NULL;
		e_ipFeCsr		= NULL;
		e_cpLayer		= NULL;
		e_lData_Count	= 0;
	}

	/**
		�f�X�g���N�^
	*/
	~msi_handle_ws	() {
		if( e_cpLayer != NULL ) {
			delete [] e_cpLayer;
		}
	}
	
	/**
		�����������o
	*/
	bool	Init	(
		IWorkspacePtr	c_ipWSpace,		///< �I�[�v���������[�N�X�y�[�X
		char*			c_cpLayer		///< ���C������
	);
	
	/**
		�w��t�B�[�`���ɑ΂���esriSpatialRelEnum�Ŏw�肵���֌W�����f�[�^��
		�Ƃ��Ă���
		�W�I���g����NULL�̏ꍇ��Where�傾���̌������s��
	*/
	IFeatureCursorPtr	Search_Feature	(
		IGeometryPtr		c_ipGeo,			///< �w��W�I���g��
		esriSpatialRelEnum	c_ipSpatialRel,		///< �֌W�w��
		std::string			c_sWhere,			///< Where�������
		int					c_iRetType			///< ����Ԃ���
	);

	/**
		���C�����̂𓾂�
	*/
	char*	Get_Layer_Name	( void ) {
		return ( e_cpLayer );		
	}

	/**
		�����f�[�^�������l������

	*/
	long	Get_Data_Count	( void ) {
		return	( e_lData_Count );		
	}

	/**
		�G���[�o��
	*/
	void	Print_Err	(
				char*	c_cpAddMsg,		///< ��{�G���[���ɒǉ����������b�Z�[�W
				char*	c_cpFuncName 	///< �֐���
			);


};

#endif

