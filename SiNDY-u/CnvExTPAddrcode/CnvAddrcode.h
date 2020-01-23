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

#ifndef __CNVADDRCODE_H__
#define __CNVADDRCODE_H__

#include "TempCodeTable.h"
#include "Oracl.h"

// ��`�l
#define PROGNAME _T("CnvExTPAddrcode.exe")

// �G���[�R�[�h
#define O_UPDATE						1
#define O_UPDATE_TMP					2
#define E_UPDATE_PROGNAME				3
#define E_UPDATE_MODIFYDATE				4
#define E_UPDATE_ADDRCODE				5
#define E_STORE							6
#define O_UPDATE_DATA					7
#define	C_NOCHANGE_KARICODE				8
#define O_UPDATE_HAISHI_ADDRCODE		9
#define O_UPDATE_DATA_HAISHI_ADDRCODE	10
#define E_NOUPDATE_HAISHI_ADDRCODE		11
#define E_NOUPDATE_NG_ADDRCODE			12


// �Z���R�[�h�̏��
#define SIKOU_CODE	1	// �{�s���
#define HAISHI_CODE	2	// �p�~���
#define NG_CODE		3	// �����R�[�h

typedef std::map<CString, CString> CODEMAP;

/** 
 * @class	CCnvAddrcode
 */
class CCnvAddrcode
{
public:
	CCnvAddrcode(void);
	virtual ~CCnvAddrcode(void);

	/**
	 * @brief	�g�p�@
	 */
	static void printUsage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�R�}���h���C�������̐�
	 * @param	argv	[in]	�R�}���h���C������
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	���s
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	���ϐ�����K�v���̐ݒ�
	 * @return	bool
	 */
	bool setInfoFromEnv();

	/**
	 * @brief	11���Z���R�[�h���L�[�ŁA�ŐV11���Z���R�[�h���l��map�R���e�i���쐬
	 * @return	bool
	 */
	bool makeAddrcodeMap();

	/**
	 * @brief	���R�[�h���L�[�ŁA���R�[�h���l��map�R���e�i�쐬
	 * @return	bool
	 */
	bool makeTmpcodeMap();

	/**
	 * @brief	�ҏW�p�q�o�[�W�����쐬
	 * @return	bool
	 */
	bool makeChildVersion();

	/**
	 * @brief	�ҏW�J�n
	 * @return	bool
	 */
	bool editStart();

	/**
	 * @brief	�ҏW�I��
	 * @return	bool
	 */
	bool editStop();

	/**
	 * @brief	�g���d�b�ԍ������|�C���g�ҏW
	 * @param	ipRow	[in]	�d�b�ԍ������|�C���g
	 */
	void editExTPPoint( _IRow* ipRow );

	/**
	 * @brief	�ŐV11���Z���R�[�h�擾�֐�
	 * @param	strCode		[in]	11���Z���R�[�h
	 * @param	strNewCode	[out]	�ŐV11���Z���R�[�h(���݂���Ȃ�)
	 * @retval	0 : �Z���ύX�Ȃ�
	 * @retval	1 : �Z���ύX�L��
	 * @retval	2 : �\�����ʃG���[
	 */
	int getNewAddrcode( const CString& strCode, CString& strNewCode )
	{
		CODEMAP::iterator itr = m_mapAddrcode.find( strCode );
		if( itr != m_mapAddrcode.end() )
		{
			strNewCode = itr->second;
			return ( strCode == itr->second? 0 : 1 );
		}
		else
			return 2;
	}

	/**
	 * @brief	���R�[�h���`�F�b�N���A���R�[�h�Ȃ�O����̃R�[�h���擾
	 * @param	strCode		[in]	11���Z���R�[�h
	 * @param	strOldCode	[out]	���R�[�h�̑O����̏Z���R�[�h(���݂���Ȃ�)
	 * @retval	true : ���R�[�h
	 * @retval	false : ���R�[�h�łȂ�
	 */
	bool isTmpCode( const CString& strCode, CString& strOldCode )
	{
		CODEMAP::iterator itr = m_mapTmpcode.find( strCode );
		if( itr != m_mapTmpcode.end() )
		{
			strOldCode = itr->second;
			return true;
		}
		else
			return false;
	}

	/**
	 * @brief	11���Z���R�[�h�̌��݂̏�Ԏ擾
	 * @param	strCode	[in]	�Z���R�[�h
	 * @retval	SIKOU_CODE	�{�s����Ă���R�[�h
	 * @retval	HAISHI_CODE	�p�~����Ă���R�[�h
	 * @retval	NG_CODE		�����ȃR�[�h
	 */
	int	checkAddrCodeState( const CString& strCode );

	/**
	 * @brief	���O�w�b�_�[�o��
	 */
	void printHeader()
	{
		std::cout << "#SINDYSTDLOG\n"
				  << "#\t\t\t\t\t\t\t\t�X�V�O�R�[�h\t�X�V��R�[�h\t�Z����\t�������W�񋟌��R�[�h\t�������ʃR�[�h" << std::endl;
	}

	/**
	 * @brief	���O�o��
	 * @param	ipRow		[in]	Row
	 * @param	nErrorCode	[in]	�G���[�R�[�h
	 * @param	strOldCode	[in]	�X�V�O�R�[�h
	 * @param	strNewCode	[in]	�X�V��R�[�h
	 */
	void printLog( _IRow* ipRow, int nErrorCode, const CString& strOldCode = "", const CString& strNewCode = "" );

private:

	bool		m_bEditMode;		//!< �ҏW���[�h?

	IWorkspacePtr		m_ipWorkspace;		//!< ���[�N�X�y�[�X
	IWorkspaceEditPtr	m_ipEditWorkspace;	//!< �ҏW�p���[�N�X�y�[�X

	CString		m_strExTPFName;		//!< �g���d�b�ԍ������|�C���g�t�B�[�`���N���X��
	ITablePtr	m_ipExTPTable;		//!< �g���d�b�ԍ������|�C���g�e�[�u����

	ODatabase	m_cJDBNew;			//!< �Z��DB�\�[�X(�V)
	ODatabase	m_cJDBOld;			//!< �Z��DB�\�[�X(��)
	
	LONG m_lAddrcode_FID;			//!< �Z���R�[�h�t�B�[���h�h�c
	LONG m_lAddrName_FID;			//!< �Z�����t�B�[���h�h�c
	LONG m_lPointStatC_FID;			//!< �������ʃR�[�h�t�B�[���h�h�c
	LONG m_lInfoSrcC_FID;			//!< �������W�񋟌��R�[�h�t�B�[���h�h�c
	LONG m_lProgModifyDate_FID;		//!< �ŏI�v���O�����X�V�����t�B�[���h�h�c
	LONG m_lModifyProgName_FID;		//!< �ŏI�X�V�v���O�������t�B�[���h�h�c
	
	CODEMAP		m_mapAddrcode;		//!< 11���Z���R�[�h���L�[�ŁA�ŐV11���Z���R�[�h���l
	CODEMAP		m_mapTmpcode;		//!< ���R�[�h���L�[�ŁA���R�[�h���l

	CTempCodeTable	m_cTmpTable;	//!< ���R�[�h�Ή��\

};


#endif // __CNVADDRCODE_H__
