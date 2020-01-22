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

//--------------------------------------------
// �Z��DB��舵���p�N���X�E�E
// eleven.cpp�����x�[�X�ɍ쐬
//--------------------------------------------
#include <io.h>
#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <Oracl.h>	// �I���N���p

#ifndef	_ADDR_DB_HANDLE_H_
#define	_ADDR_DB_HANDLE_H_

//using namespace stlport;
using namespace std;

// ���[�h�ݒ�p��`
#define	READ_ODB	0x01	///< �I���N��DB�ڑ�
#define	READ_TDB	0x02	///< �e�L�X�gDB�ڑ�

typedef map<string, string, less<string> >	m_string;
typedef map<string, int, less<string> >		m_string2;
typedef set<string, less<string> >			l_string;

struct	addr_dat
{
	string	e_sCode;	///< �Z���R�[�h
//	string	e_sName;	///< �Z����

	string	e_sKen_Kanji;
	string	e_sShi_Kanji;
	string	e_sOAza_Kanji;
	string	e_sAza_Kanji;

	addr_dat() {
	}
	~addr_dat() {
	}
	bool	operator<(const addr_dat c_aDat) const {
		bool	a_bRet	= false;
		if( e_sCode != c_aDat.e_sCode ) {
			if( e_sCode < c_aDat.e_sCode ) {
				a_bRet	= true;
			}
		}
		return	( a_bRet );
	}
};

typedef set<string, less<string> >							s_string;
typedef set<string, less<string> >::iterator				is_string;

typedef	set<addr_dat, less<addr_dat> >						s_addr_dat;
typedef	set<addr_dat, less<addr_dat> >::iterator			is_addr_dat;
typedef	map<string, addr_dat, less<string> >				m_addr_dat;
typedef	map<string, addr_dat, less<string> >::iterator		im_addr_dat;

/**
	�Z��DB�̎�舵���p�N���X
*/
class	addr_db_handle
{
	string		m_sDBPath;		///< �e�L�X�gDB�̃p�X
    ODatabase	m_oAdm_DB;		///< �Z��DB�ւ̃A�N�Z�X
	// TEXT_DB�Ŏg�p
	m_string	m_Map;			///< �ǂݍ��񂾃e�L�X�gDB�̊i�[�̈�
	m_string2	m_City;			///< �ǂݍ��񂾃e�L�X�gDB�̊i�[�̈�(5���p)
    l_string	m_Ready;		///< �ǂݍ��ݍς�5�����̂̃��X�g
	// DB�Ŏg�p
	s_addr_dat	m_sDatas;		///< �Z���f�[�^�̊i�[�̈�
	s_addr_dat	m_sOldDatas;	///< �V�R�[�h�Ƌ����̂̃f�[�^�Z�b�g
	is_addr_dat	m_iOldIndx;		///< ���f�[�^�̃C���f�b�N�X
	int			m_iMode;		///< DB�ǂݍ��݃��[�h
	bool		m_bExec_OK;		///< ���s�\���ǂ��� 2004.09.06

	m_addr_dat	m_sRevData;		///< �n����5���p�̏��

	FILE*		m_fpErr_Fp;		///< �G���[�o�͗p�t�@�C���|�C���^ 2009.01.26

	/**
		�e�L�X�gDB�̓ǂݍ���
	*/
	bool		LoadAddress_TextDB(
					const char*	c_cpCode
				);
	
	/**
		�I���N���Z��DB�̓ǂݍ���
	*/
	bool		LoadAddress_DB(
					const char*	c_cpCode
				);
	/**
		�I���N���Z��DB�̓ǂݍ���
	*/
	bool		LoadOldAddress_DB(
					const char*	c_cpCode
				);

	/**
		��ɑΉ�����s��5���R�[�h�̏����擾����
	*/
	bool		GetShiKuInfo(
					ODatabase&	c_oAdm_DB,	///< �Z��DB[IN]
					string&		c_sKen,		///< �s���{����[IN]
					string&		c_sShi,		///< �s�撬����[IN]
					addr_dat&	c_aDat		///< �n���f�[�^[OUT]
				);
					

public:
	/**
		�e�L�X�g�Z��DB�̃��[�h
	*/
    bool		LoadAddress	(
					const char*	c_cpCode	///<	�Z���R�[�h
				);
	/**
	*/
    void		FormatStr	(
					char*		Addr,
					const char*	Src
				);

	/**
		���s�\���ǂ�����Ԃ�
		@return	���s�\	true
		@return	���s�s�\	false
	*/
	bool		Exec_OK	( void ) {
					return	( m_bExec_OK );
				};

public:
	enum ret_value	{
		RET_OK		= 1,	///	�������R�[�h
		RET_HAISHI,			///	�p�~�ɂȂ����R�[�h
		RET_000				///	000����
	};
	// �R���X�g���N�^
	addr_db_handle	( ) {
		m_Map.clear		();
		m_City.clear	();
		m_Ready.clear	();
		m_iMode			= 0;	///< �ǂ̃��[�h�ł��Ȃ�
		m_bExec_OK		= false;
		m_fpErr_Fp		= stderr;
	}
	// �f�X�g���N�^
	~addr_db_handle	( ) {
		m_Map.clear		();
		m_City.clear	();
		m_Ready.clear	();
	}

	// ������(DB)
	bool	Init_DB		(
				char*	c_cpConnectStr,				///< DB������������
				FILE*	c_fpErr_Fp=NULL
			);

	// ������(TEXTDB)
	bool	Init_TextDB (
				char*	c_cpDBPath,					///< �e�L�X�gDB�p�X��
				FILE*	c_fpErr_Fp=NULL
			);

	/**
		�s���R�[�h�̐������`�F�b�N
		@return RET_OK		�������R�[�h
		@return RET_HAISHI	�p�~�ɂȂ����R�[�h
		@return RET_000		000����
	*/
    int		IsValidCode11(
				const char	*Code,					///< 11���R�[�h
				char		*AreaName
			);

	/**
		�s���R�[�h�̐������`�F�b�N�i5���p�j
		@return RET_OK		�������R�[�h
		@return RET_HAISHI	�p�~�ɂȂ����R�[�h
		@return RET_000		000����
	*/
	int		IsValidCode5 (
				const char	*Code			///< 5���R�[�h
			);

	/**
		OracleDB���璼�Ŏ���Ă�����������擾
	*/
	bool 	GetOraDBStr	(
				const char	*Code,		///< 11���R�[�h[IN]
				addr_dat&	c_aDat		///< �n���f�[�^[OUT]
			);

	/**
		�於����s��5�����̏����擾����
	*/
	bool 	GetCitybyWard(
				addr_dat&	c_aInputDat,///< ���͏��[IN]
				addr_dat&	c_aRetDat	///< �ԋp���[IN]
			);

	/**
		�f�[�^����
	*/
	addr_dat	Find_Addr_Data	(
					char*	c_cpKey		///< �T�[�`�L�[�i5���R�[�h�j
				);

	/**
		�������X�g�̍ŏ���Ԃ�
	*/
	bool		Find_Old_Name_First	( 
					char*		c_cpKey,	///<�T�[�`�L�[�i5���R�[�h�j
					addr_dat&	c_aDat		///< �����f�[�^[OUT]
				);

	/**
		�������X�g�̎���Ԃ�
	*/
	bool		Find_Old_Name_Next	(
					char*		c_cpKey,	///<�T�[�`�L�[�i5���R�[�h�j
					addr_dat&	c_aDat		///<�����f�[�^[OUT]
				);
};

#endif	// _ADDR_DB_HANDLE_H_
