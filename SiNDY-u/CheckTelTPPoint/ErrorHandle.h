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

#if !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
#define AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
 * �G���[�啪�ޒ�`
 */
//�Ȃ�

/**
 * �G���[�R�[�h��`
 */
#define W_GAITOUSISETU_ADDRCODE_NOTSEIBI	1	// 11���R�[�h�s��v(������)
#define W_GAITOUSISETU_ADDRCODE_SEIBI		2	// 11���R�[�h�s��v(������)
#define W_GAITOUSISETU_OAZACODE				3	// 8���R�[�h�s��v
#define W_GAITOUSISETU_CITYCODE				4	// 5���R�[�h�s��v
#define N_HORYU_POINTSTAT					5	// �ۗ��ł���
#define	E_NOSURVEY_OPERATOR					6	// �������ʃR�[�h�����t�^�R��
#define E_UNKNOWN_POINTSTAT					7	// �s���̒����R�[�h
#define E_DUP_OR_NOTON_ADMIN				8	// �s���E�̋��E��ɂ���܂��͍s���E��ɂȂ�

#define W_TELNUM_FREECALL					9	// �d�b�ԍ����t���[�_�C����

#define E_ON_NGCITYSITE						10	// �ʒu�s��(�s�s�n�}�w�i)
#define E_ON_NGBASESITE						11	// �ʒu�s��(���k�w�i)
#define E_DUP_OR_NOTON_CITYSITE				12	// �s�s�n�}�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���
#define E_DUP_OR_NOTON_BASESITE				13	// ���k�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���

#define E_PLURAL_OPERATOR					14	// �|�C���g�S�Œ������ʃR�[�h���Ⴄ�|�C���g������
#define E_POI_TABOO_IPCODE					15  // �g�p�s��POI�t�߂ɐ���
#define E_SAME_SEQUENCE						16	// ����V�[�P���X�����L��(Bug9295�Œǉ��j

/**
 * @class	CErrorHandle
 */
class CErrorHandle  
{
public:

	/**
	 * @brief	�f�t�H���g�R���X�g���N�^
	 */
	CErrorHandle()
	{
	}

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
		m_fout.open( static_cast<CT2CW>(lpcszFile), std::ios::out );
		return ( m_fout? true : false );
	}

	/**
	 * @brief	�w�b�_�[�o��
	 * @param	lpszToolVer	[in]	�c�[���o�[�W����
	 */
	void printHeader( LPCTSTR lpszVer )
	{
		m_fout << "#" << lpszVer << std::endl;
		m_fout << "#SINDYSTDLOG" << std::endl;
		m_fout << "#�Ή���\t���C����\t�I�u�W�F�N�gID\tX\tY\t�G���[���x��\t�G���[�R�[�h\t�G���[���b�Z�[�W\t"
			   << "�Z����\t�Z���R�[�h\t��������\t�f�ږ���\t��������\t���\�[�X\t�d�b�ԍ�\t���W�t�^���\t�V�[�P���X\tTelTP��POI�Ƃ̋���(m)" << std::endl;
	}

	/**
	 * @brief	���O�o��
	 * @param	cTelTP			[in]	TelTP�|�C���g
	 * @param	nErrorCode		[in]	�G���[�R�[�h
	 * @param	lpstrBGDomain	[in]	�w�i��ʃh���C��
	 */
	void printLog( const CTelTP& ob, int nErrorCode, const std::string* lpstrBGDomain = NULL );

	/**
	 * @brief	���O�o��
	 * @param	cTelTP			[in]	TelTP�|�C���g
	 * @param	nErrorCode		[in]	�G���[�R�[�h
	 * @param	dist			[in]	TelTP��POI�Ƃ̋���
	 */
	void printLog( const CTelTP& ob, int nErrorCode, const double dist);	// �����ǉ��� (Bug 5701�Ή� 07/11/30�ǉ�)

private:

	/**
	 * @brief	�G���[�R�[�h����G���[���b�Z�[�W�擾
	 * @param	nErrorCode	[in]	�G���[�R�[�h
	 * @param	lpstrBGDomain	[in]	�w�i��ʃh���C��
	 * @return	�G���[���b�Z�[�W
	 */
	const std::string getErrorMsg( int nErrorCode, const std::string* lpstrBGDomain = NULL ) const;

private:

	std::ofstream	m_fout;					/// ���O�t�@�C���o�̓X�g���[��

	CString			m_strFCName;			/// �t�B�[�`���N���X��

	static const LPCTSTR	m_lpszErrMsg[];	/// �G���[���b�Z�[�W�ꗗ

};

#endif // !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
