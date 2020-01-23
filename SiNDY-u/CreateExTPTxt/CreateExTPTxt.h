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

// CreateExTPTxt.h: CreateExTPTxt �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEEXTPTXT_H__84BB5780_E09F_4BDC_B954_0A1380233C0B__INCLUDED_)
#define AFX_CREATEEXTPTXT_H__84BB5780_E09F_4BDC_B954_0A1380233C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

/**
 * @struct	CExTPRec
 * @brief	ExTP���R�[�h
 */
struct CExTPRec
{
	long		m_lOID;			//!< OBJECTID
	CString		m_strAddrCode;	//!< �Z���R�[�h
	long		m_lInfoSrc;		//!< �������W�񋟌��R�[�h
	WKSPoint	m_point;		//!< �ʒu���

	CExTPRec() : m_lOID(-1), m_lInfoSrc(-1)
	{
		m_point.X = 0.0; m_point.Y = 0.0;
	}
};

typedef std::map<CString, CExTPRec> EXTPREC_MAP;	// �L�[ : 20���Z���R�[�h �l : ExTP���R�[�h

/**
 * @class	CreateExTPTxt
 * @brief	ExTP�o�̓N���X
 */
class CreateExTPTxt
{
public:
	CreateExTPTxt(){}
	~CreateExTPTxt(){}

	/**
	 * @brief	�g�p�@
	 */
	static void Usage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool Init( int argc, char** argv );

	/**
	 * @brief	���s
	 */
	void Execute();

private:

	/**
	 * @brief	ExTP�ǉ�
	 * @param	cExTP	[in]	ExTP
	 */
	void AddExTP( const CExTPRec& cExTP );

	/**
	 * @brief	���R�[�h�o��
	 */
	void OutputExTP();

private:

	std::ofstream		m_ofs;				//!< �o�̓X�g���[��
	IFeatureClassPtr	m_ipExTPClass;		//!< ExTP�|�C���g�N���X

	EXTPREC_MAP			m_mapExTP;			//!< ExTP���X�g
};

#endif // !defined(AFX_CREATEEXTPTXT_H__84BB5780_E09F_4BDC_B954_0A1380233C0B__INCLUDED_)
