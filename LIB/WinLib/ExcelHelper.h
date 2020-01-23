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

#pragma once

/**
 * @class	CExcelHelper
 * @brief	Excel�t�@�C���w���p�[�N���X
 */
class CExcelHelper
{
public:
	/**
	 * @brief	�R���X�g���N�^
	 */
	CExcelHelper(void){
		init();
	};

	/**
	 * @brief	�f�X�g���N�^
	 */
	~CExcelHelper(void){
		m_ipApp->Quit();
	}

	/**
	 * @brief	�t�@�C���I�[�v���i�ǂݎ���p�j
	 * @param	lpcszFile [in] �t�@�C����
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool Open( const CString& strFile ){
		if( !init() ){
			std::cerr << "#ERROR �C���X�^���X�̐����Ɏ��s" << std::endl;
			return false;
		}

		if( 1 == m_ipApp->Workbooks->Count ){
			std::cerr << "#ERROR ����Workbook���I�[�v������Ă��܂�" << std::endl;
			return false;
		}

		try{
			m_ipWorkBook = m_ipApp->Workbooks->Open( _bstr_t(strFile) );
			if( NULL == m_ipWorkBook )
			{
				std::cerr << "#ERROR �t�@�C���I�[�v���Ɏ��s : " << CT2CA(strFile) << std::endl;
				return false;
			}
		}
		catch( ... ){
			std::cerr << "#ERROR �t�@�C���I�[�v���Ɏ��s : " << CT2CA(strFile) << std::endl;
			return false;
		}

		return true;
	};

	/**
	 * @brief	�t�@�C���I�[�v���i�ǂݎ���p�j
	 * @param	lpcszFile [in] �t�@�C����
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool OpenReadOnly( const CString& strFile )
	{
		if( !init() ){
			std::cerr << "#ERROR �C���X�^���X�̐����Ɏ��s" << std::endl;
			return false;
		}

		if( 1 == m_ipApp->Workbooks->Count ){
			std::cerr << "#ERROR ����Workbook���I�[�v������Ă��܂�" << std::endl;
			return false;
		}

		try{
			// UpdateLinks(0), ReadOnly(true)�Ƃ��āAExcel�t�@�C���I�[�v��
			m_ipWorkBook = m_ipApp->Workbooks->Open( _bstr_t(strFile), 0, true );
			if( NULL == m_ipWorkBook )
			{
				std::cerr << "#ERROR �t�@�C���I�[�v���Ɏ��s : " << CT2CA(strFile) << std::endl;
				return false;
			}
		}
		catch( ... ){
			std::cerr << "#ERROR �t�@�C���I�[�v���Ɏ��s : " << CT2CA(strFile) << std::endl;
			return false;
		}

		return true;
	};

	/**
	 * @brief	�V�[�g�擾
	 * @param	SheetNum [in] �V�[�g�ԍ�
	 * @retval	_Worksheet�|�C���^
	 */
	Excel::_WorksheetPtr GetSheet( const long sheetNum ){
		return m_ipWorkBook? m_ipWorkBook->Worksheets->Item[CComVariant(sheetNum)] : NULL;
	}

	/**
	 * @brief	�V�[�g���擾
	 * @retval	long �V�[�g��
	 */
	long GetSheetCnt(){
		return m_ipApp->Sheets->Count;
	}

	/**
	 * @brief	�t�@�C�����擾
	 * @retval	CString �t�@�C����
	 */
	CString GetFileName(){
		return m_ipWorkBook? (LPCTSTR)(m_ipWorkBook->Name) : _T("");
	}

	/**
	 * @brief	����
	 * @param	bSaved [in] �ۑ����邩�H
	 */
	void Close( bool bSaved = false )
	{
		if( m_ipWorkBook )
			m_ipWorkBook->Close(bSaved);
	}

private:

	/**
	 * @brief	������(�C���X�^���X����)
	 * @retval	true  ����	
	 * @retval	false ���s
	 */
	bool init(){
		if( NULL == m_ipApp ){
			m_ipApp.CreateInstance( L"Excel.Application" );
		}

		if( m_ipApp ){
			m_ipApp->Visible[0] = VARIANT_FALSE;
			return true;
		}
		else{
			return false;
		}
	};

protected:

	Excel::_ApplicationPtr m_ipApp;      //!< �A�v���P�[�V�����|�C���^
	Excel::_WorkbookPtr    m_ipWorkBook; //!< ���[�N�u�b�N
};
