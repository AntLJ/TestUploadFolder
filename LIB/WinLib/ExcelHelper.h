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
 * @brief	Excelファイルヘルパークラス
 */
class CExcelHelper
{
public:
	/**
	 * @brief	コンストラクタ
	 */
	CExcelHelper(void){
		init();
	};

	/**
	 * @brief	デストラクタ
	 */
	~CExcelHelper(void){
		m_ipApp->Quit();
	}

	/**
	 * @brief	ファイルオープン（読み取り専用）
	 * @param	lpcszFile [in] ファイル名
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool Open( const CString& strFile ){
		if( !init() ){
			std::cerr << "#ERROR インスタンスの生成に失敗" << std::endl;
			return false;
		}

		if( 1 == m_ipApp->Workbooks->Count ){
			std::cerr << "#ERROR 既にWorkbookがオープンされています" << std::endl;
			return false;
		}

		try{
			m_ipWorkBook = m_ipApp->Workbooks->Open( _bstr_t(strFile) );
			if( NULL == m_ipWorkBook )
			{
				std::cerr << "#ERROR ファイルオープンに失敗 : " << CT2CA(strFile) << std::endl;
				return false;
			}
		}
		catch( ... ){
			std::cerr << "#ERROR ファイルオープンに失敗 : " << CT2CA(strFile) << std::endl;
			return false;
		}

		return true;
	};

	/**
	 * @brief	ファイルオープン（読み取り専用）
	 * @param	lpcszFile [in] ファイル名
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool OpenReadOnly( const CString& strFile )
	{
		if( !init() ){
			std::cerr << "#ERROR インスタンスの生成に失敗" << std::endl;
			return false;
		}

		if( 1 == m_ipApp->Workbooks->Count ){
			std::cerr << "#ERROR 既にWorkbookがオープンされています" << std::endl;
			return false;
		}

		try{
			// UpdateLinks(0), ReadOnly(true)として、Excelファイルオープン
			m_ipWorkBook = m_ipApp->Workbooks->Open( _bstr_t(strFile), 0, true );
			if( NULL == m_ipWorkBook )
			{
				std::cerr << "#ERROR ファイルオープンに失敗 : " << CT2CA(strFile) << std::endl;
				return false;
			}
		}
		catch( ... ){
			std::cerr << "#ERROR ファイルオープンに失敗 : " << CT2CA(strFile) << std::endl;
			return false;
		}

		return true;
	};

	/**
	 * @brief	シート取得
	 * @param	SheetNum [in] シート番号
	 * @retval	_Worksheetポインタ
	 */
	Excel::_WorksheetPtr GetSheet( const long sheetNum ){
		return m_ipWorkBook? m_ipWorkBook->Worksheets->Item[CComVariant(sheetNum)] : NULL;
	}

	/**
	 * @brief	シート数取得
	 * @retval	long シート数
	 */
	long GetSheetCnt(){
		return m_ipApp->Sheets->Count;
	}

	/**
	 * @brief	ファイル名取得
	 * @retval	CString ファイル名
	 */
	CString GetFileName(){
		return m_ipWorkBook? (LPCTSTR)(m_ipWorkBook->Name) : _T("");
	}

	/**
	 * @brief	閉じる
	 * @param	bSaved [in] 保存するか？
	 */
	void Close( bool bSaved = false )
	{
		if( m_ipWorkBook )
			m_ipWorkBook->Close(bSaved);
	}

private:

	/**
	 * @brief	初期化(インスタンス生成)
	 * @retval	true  成功	
	 * @retval	false 失敗
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

	Excel::_ApplicationPtr m_ipApp;      //!< アプリケーションポインタ
	Excel::_WorkbookPtr    m_ipWorkBook; //!< ワークブック
};
