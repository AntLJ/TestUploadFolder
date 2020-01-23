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

#include "util_class.h"
#include <string>
#include <map>

#ifndef	_CHK_ATTR_VALUE_TBL_H_
#define	_CHK_ATTR_VALUE_TBL_H_

using	namespace	std;

#define	DISP_TOP	0x8
#define	DISP_MID	0x4
#define	DISP_BASE	0x2
#define	DISP_CITY	0x1

// �\������		= 0x00
// �����񂠂�	= 0x01
// �L������		= 0x02
// �w���_������	= 0x04
#define	NO_DISP		0x00
#define	EX_STR		0x01
#define	EX_MARK		0x02
#define	EX_POINT	0x04

struct	attr_record
{
	int		e_iMeshDigit;	///< �Y�����郁�b�V���̌���
	string	e_sLayerName;	///< ���C����(���ۂ�
	string	e_sFieldName;	///< �t�B�[���h��
	string	e_sDomainType;	///< �h���C���^�C�v
	string	e_sDomainName;	///< ��������
	string	e_sAttrName;	///< �������́i�h���C���^�C�v��range�̏ꍇ�͎����Ȃ��j
	long	e_lValue;		///< �����l
	long	e_lMin;			///< �ŏ��l
	long	e_lMax;			///< �ő�l


	attr_record() {
		e_iMeshDigit= 0;
		e_lValue	= 0;
		e_lMin		= 0;
		e_lMax		= 0;
	}

	/**
		��r���Z�q��`
		@return	bool
	*/
	bool	operator<(const attr_record c_aDat ) const {
		bool	a_bRet	= false;
		if( e_iMeshDigit != c_aDat.e_iMeshDigit ) {
			if( e_iMeshDigit < c_aDat.e_iMeshDigit ) {
				a_bRet	= true;
			}			
		}else
		if( e_sLayerName != c_aDat.e_sLayerName ) {	///< ���C����
			if( e_sLayerName < c_aDat.e_sLayerName ) {
				a_bRet	= true;
			}
		}else
		if( e_sFieldName != c_aDat.e_sFieldName ) {	///< �t�B�[���h��
			if( e_sFieldName < c_aDat.e_sFieldName ) {
				a_bRet	= true;
			}
		}else
		if( e_sDomainType != c_aDat.e_sDomainType ) {	///< �h���C���^�C�v
			if( e_sDomainType < c_aDat.e_sDomainType ) {
				a_bRet	= true;
			}
		}else
		if( e_lValue != c_aDat.e_lValue ) {		///< �����l
			if( e_lValue < c_aDat.e_lValue ) {
				a_bRet	= true;
			}
		}
		return ( a_bRet );
	}

};

typedef	map<int, attr_record, less<int> >					m_attr_record;
typedef	map<int, attr_record, less<int> >::iterator			im_attr_record;
typedef	set<attr_record, less<attr_record> >				s_attr_record;
typedef	set<attr_record, less<attr_record> >::iterator		is_attr_record;

/**
	�X�P�[���ʕ\�����[��
*/
struct	scale_disp_rule
{
	long	e_lDispRule[4];
	
	scale_disp_rule() {
		int i = 0;
		for( i = 0; i < 4; i++ )
		{
			e_lDispRule[i]	= 0;
		}
	}
	~scale_disp_rule() {
	}

	void	operator=( scale_disp_rule c_aDat ) {
		int i = 0;
		for(i = 0; i < 4; i++ )
		{
			e_lDispRule[i]	= c_aDat.e_lDispRule[i];
		}
	}

};

/**
	���L�\�����[���̈���
*/
struct	note_disp_rule
{
	long			e_lValue;			///< �����l
	string			e_sName;			///< ������
	int				e_iDispType;		///< �\���^�C�v�i���󓙁j
	int				e_iDispFlag;		///< �\���t���O
	scale_disp_rule	e_lTopDispRule;		///< �g�b�v�}�b�v�\�����[��
	scale_disp_rule	e_lMidDispRule;		///< �~�h���}�b�v�\�����[��
	scale_disp_rule	e_lBaseDispRule;	///< �x�[�X�}�b�v�\���c�[��
	scale_disp_rule	e_lCityDispRule;	///< �s�s�n�}�\�����[��

	note_disp_rule() {
		e_iDispFlag		= 0;
		e_iDispType		= 0;
		e_sName			= "";
		e_lValue		= 0;
	}
	~note_disp_rule() {
	}

	void	operator=( note_disp_rule c_aDat ) {
		e_lValue		= c_aDat.e_lValue;			///< �����l
		e_sName			= c_aDat.e_sName;			///< ������
		e_iDispType		= c_aDat.e_iDispType;		///< �\���^�C�v�i���󓙁j
		e_iDispFlag		= c_aDat.e_iDispFlag;		///< �\���t���O
		e_lTopDispRule	= c_aDat.e_lTopDispRule;	///< �g�b�v�}�b�v�\�����[��
		e_lMidDispRule	= c_aDat.e_lMidDispRule;	///< �~�h���}�b�v�\�����[��
		e_lBaseDispRule	= c_aDat.e_lBaseDispRule;	///< �x�[�X�}�b�v�\���c�[��
		e_lCityDispRule	= c_aDat.e_lCityDispRule;	///< �s�s�n�}�\�����[��		
	}

	/**
		��r���Z�q��`
		@return	bool
	*/
	bool	operator<(const note_disp_rule c_aDat ) const {
		bool	a_bRet	= false;
		if( e_lValue != c_aDat.e_lValue ) {			///< �����l
			if( e_lValue < c_aDat.e_lValue ) {
				a_bRet	= true;
			}			
		}
		return ( a_bRet );
	}
};

typedef	multiset<note_disp_rule, less<note_disp_rule> >				ms_note_disp_rule;
typedef	multiset<note_disp_rule, less<note_disp_rule> >::iterator	ims_note_disp_rule;

class	chk_attr_value_tbl
{

private:
	s_attr_record		e_sAttr_Value_Tbl;	// �����l�̃e�[�u��
	ms_note_disp_rule	e_sNote_Disp_Rule;	// ���L�̕\�����[��
	FILE*		e_fpLog;
	util_class	e_uCu;

public:
	chk_attr_value_tbl () {
		e_fpLog	= stderr;
	}
	~chk_attr_value_tbl () {
	}

	/**
		���O�t�@�C���̐ݒ�
	*/
	bool	Log_Init	(
		FILE*	c_fpLogFp
	);

	/**
		�Ή��e�[�u����ǂݍ���
	*/
	bool	read_table	(
		char*	c_cpRFName
	);

	/**
		�Ή��e�[�u����ǂݍ���
	*/
	bool	read_note_disp_rule	(
		char*	c_cpRFName
	);

	/**
		�f�[�^�̃T�[�`���s��
	*/
	bool	find_data	(
		int				c_iMeshDigit,	///< �f�[�^���Y�����郁�b�V������[IN]
		string&			c_sLayerName,	///< ���C����[IN]
		string&			c_sFieldName,	///< �t�B�[���h����[IN]
		int				c_iValue,		///< �����l
		attr_record*	c_apAttRec		///< �������R�[�h[OUT]
	);

	/**
		�Y�����钍�L�\�����[�������o��
	*/
	bool	find_rel	(
		int				c_iMeshDigit,	///< �f�[�^���Y�����郁�b�V������[IN]
		int				c_iValue,		///< �����l[IN]
		note_disp_rule*	c_apAttRec		///< �������R�[�h[OUT]
	);

};

#endif
