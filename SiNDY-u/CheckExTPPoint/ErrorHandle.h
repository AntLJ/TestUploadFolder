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

// ErrorHandle.h: CErrorHandle �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
#define AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinLib/VersionInfo.h>
#include <io.h>

/**
 * �G���[�啪�ޒ�`
 */
#define E_MISMATCH_ADDRCODE11_NOTSEIBI	100	// �s���E����������11���Z���R�[�h�قȂ�
#define E_MISMATCH_ADDRCODE11_SEIBI		200	// �s���E�������ς�11���Z���R�[�h�قȂ�
#define E_MISMATCH_GAIKU				300	// �s���E�������ςŊX�悪�قȂ�
#define E_MATCH_GAIKU					400	// �s���E�������ςŊX�悪��v

/**
 * �G���[�R�[�h��`
 */
#define	N_GOU_CSPOINT_NOTSEIBI			1	// CS�|�C���g�Ŏ擾(������)
#define W_GOU_ADDRCODE_SEIBI			2	// 11���R�[�h�s��v(������)
#define E_GOU_GAIKU_SEIBI				3	// �X�敄�����قȂ�(������)
#define W_GOU_OAZACODE_SEIBI			25	// 8���R�[�h�s��v�iGOU�����j ... Ver 1.1.0.3 ���ǉ�
#define E_EXGOU_NOTSEIBI				4	// GOU�g�������Ȃ̂ɖ�����(������)
#define W_EXGOU_ADDRCODE_SEIBI			5	// 11���R�[�h�s��v(������)
#define E_EXGOU_GAIKU_SEIBI				24	// �X�敄���i�e�n�ԁj��v�i�����ς݁j ... Ver 1.0.2.2 ���ǉ�
#define W_EXGOU_OAZACODE_SEIBI			26	// 8���R�[�h�s��v�iGOU�g�������j ... Ver 1.1.0.3 ���ǉ�
#define	E_TPG							6	// �������ʃR�[�h�I�����(TPG����)
#define	N_REF_ADDRCODE_NOTSEIBI			7	// 11���R�[�h�s��v(������)
#define	W_REF_ADDRCODE_SEIBI			8	// 11���R�[�h�s��v(������)
#define	N_REF_GAIKU_NOTSEIBI			9	// �X�敄�����قȂ�(������)
#define	N_NUM_ADDRCODE_NOTSEIBI			10	// 11���R�[�h�s��v(������)
#define	W_NUM_ADDRCODE_SEIBI			11	// 11���R�[�h�s��v(������)
#define	N_NUM_GAIKU_NOTSEIBI			12	// �X�敄�����قȂ�(������)
#define	W_ADMIN_ADDRCODE_NOTSEIBI		13	// �s���E�s��v�Ȃ̂Ƀf�[�^�ƈ�v(������)
#define	W_ADMIN_ADDRCODE_SEIBI			14	// �s���E�s��v�Ȃ̂Ƀf�[�^�ƈ�v(������)
#define	E_NOSURVEY_OPERATOR				15	// �������ʃR�[�h�����t�^�R��
#define W_MISMATCH_CITYCODE				16	// 5���R�[�h�s��v
#define E_ON_EXGAIKUFUGO				17	// �X�敄���g���̏�Ƀ|�C���g
#define E_UNKNOWN_POINTSTAT				18	// �s���̒����R�[�h
#define E_DUP_OR_NOTON_ADMIN			19	// �s���E�̋��E��ɂ���܂��͍s���E��ɂȂ�
#define E_ON_NGCITYSITE					20	// �ʒu�s��(�s�s�n�}�w�i)
#define E_ON_NGBASESITE					21	// �ʒu�s��(���k�w�i)
#define E_DUP_OR_NOTON_CITYSITE			22	// �s�s�n�}�w�i�̋��E��ɂ���܂��͍s���E��ɂȂ�
#define E_DUP_OR_NOTON_BASESITE			23	// ���k�w�i�̋��E��ɂ���܂��͍s���E��ɂȂ�

#define W_ALPS_CITYCODE					27	// 5���R�[�h�s��v[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define W_ALPS_OAZACODE					28	// 8���R�[�h�s��v[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define N_ALPS_ADDRCODE_SEIBI			29	// 11���R�[�h�s��v�i�����ρj[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define N_ALPS_ADDRCODE_NOTSEIBI		30	// 11���R�[�h�s��v�i�������j[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define N_ALPS_GAIKU_SEIBI				31	// �X�敄�����قȂ�i�����ρj[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define E_ALPS_ON_NGCITYSITE			32	// �ʒu�s���i�s�s�n�}�w�i�j[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define E_ALPS_ON_NGBASESITE			33	// �ʒu�s���i���k�w�i�j[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define E_ALPS_DUP_OR_NOTON_CITYSITE	34	// �s�s�n�}�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���[�A���v�X] ... Ver 1.1.0.3 ���ǉ�
#define E_ALPS_DUP_OR_NOTON_BASESITE	35	// ���k�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���[�A���v�X] ... Ver 1.1.0.3 ���ǉ�

#define E_SAME_ADDRCODE					36	// ����20�������L�� ... Ver 2.1.2.4 ���ǉ��iBug9296�Œǉ��j

/**
 * @class	CErrorHandle
 */
class CErrorHandle  
{
public:

	/**
	 * @brief	�f�t�H���g�R���X�g���N�^
	 */
	CErrorHandle() : m_bIsAddMode(false){}
	virtual ~CErrorHandle(){};

	/**
	 * @brief	�t�B�[�`���N���X���Z�b�g
	 * @param	lpszFCName	[in]	�t�B�[�`���N���X��
	 */
	void setFCName( LPCTSTR lpszFCName ){ m_strFCName = lpszFCName; }

	/**
	 * @brief	�t�@�C���I�[�v��
	 * @param	lpcszFile	[in]	�t�@�C����
	 * @return	bool
	 */
	bool open( LPCTSTR lpcszFile )
	{
		// ��Ƀt�@�C�����݃`�F�b�N
		m_bIsAddMode = (_access(lpcszFile, 0) != -1)? true : false;
		int nOpenMode = m_bIsAddMode? std::ios::out|std::ios::app : std::ios::out;
		m_fout.open( static_cast<CT2CW>(lpcszFile), nOpenMode );
		return ( m_fout? true : false );
	}

	/**
	 * @brief	�w�b�_�[�o��
	 */
	void printHeader()
	{
		// �ǋL���[�h�łȂ���΃w�b�_�[�o��
		if( !m_bIsAddMode )
		{
			CVersion cVer;
			CString strMsg;
			strMsg.Format( _T("#SINDYSTDLOG\n")
						   _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
						   _T("#�Ή���\t���C����\t�I�u�W�F�N�gID\tX\tY\t�G���[���x��\t�G���[�R�[�h\t�G���[���b�Z�[�W\t")
						   _T("�Z����\t�Z���R�[�h\t��������\t�f�ږ���\t��������\t���\�[�X\t�Z�������|�C���g���t���O\t����20���ێ�OID"),
						   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
			m_fout << strMsg<< std::endl;
		}
	}

	/**
	 * @brief	���O�o��
	 * @param	cExTP			[in]	ExTP�|�C���g
	 * @param	nErrorCode		[in]	�G���[�R�[�h
	 * @param	strBGDomain	[in]	�w�i��ʃh���C��
	 */
	void printLog( const CExTP& ob, int nErrorCode, const std::string& strBGDomain = "" );

	/**
	 * @brief	���O�o��
	 * @param	cExTP			[in]	ExTP�|�C���g
	 * @param	nErrorCode		[in]	�G���[�R�[�h
	 * @param	lOID			[in]	����20���R�[�h�ێ�����̃I�u�W�F�N�gID
	 */
	void printLog( const CExTP& ob, int nErrorCode, const long lOID );		// ����20���R�[�h�ێ�����̃I�u�W�F�N�gID�ǉ��ŁiBug 9296�Ή��j

private:

	/**
	 * @brief	�G���[�啪�ނ���A�G���[�R�[�h���擾
	 * @param	nErrorState	[in]	�G���[�啪��
	 * @return	�G���[�R�[�h
	 */
	int getErrorCode( int nErrorState, POINTSTAT_C ePointStat );

	/**
	 * @brief	�G���[�R�[�h����G���[���b�Z�[�W�擾
	 * @param	nErrorCode	[in]	�G���[�R�[�h
	 * @param	strBGDomain	[in]	�w�i��ʃh���C��
	 * @return	�G���[���b�Z�[�W
	 */
	const std::string getErrorMsg( int nErrorCode, const std::string& strBGDomain = "" ) const;

private:

	bool			m_bIsAddMode;			//!< �ǋL���[�h�H

	std::ofstream	m_fout;					//!< ���O�t�@�C���o�̓X�g���[��

	CString			m_strFCName;			//!< �t�B�[�`���N���X��

	static const LPCTSTR	m_lpszErrMsg[];	//!< �G���[���b�Z�[�W�ꗗ

};

#endif // !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
