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
 * @brief フィールドタイプ
 */
namespace FIELDTYPE {
	enum ECode {
		UNKNOWN_TYPE = 0,	//!< 未設定
		SHAPE,				//!< 形状
		INTEGER,			//!< 整数
		DOUBLE,				//!< 実数
		STRING				//!< 文字列
	};
}

/**
 * @brief 条件範囲用クラス
 * @note 対応しているのは数値型のみ<br>
 *       表現イメージは下記<br>
 *           symbol1 : >  <br>
 *           value1  : 10 <br>
 *         上記を数式にすると→[判定したい変数] > 10 <br>
 *           symbol2 : <=  <br>
 *           value2  : 31 <br>
 *         上記を数式にすると→[判定したい変数] <= 31  <br>
 *         上記2つを合わせると
 *           10 < [判定したい変数] <= 31 <br>
 *         となる。 <br>
 *         範囲とならない組み合わせ（ [判定したい変数] < 10, [判定したい変数] >= 31 ）<br>
 *         となるケースは認めない。
 */
class CRange {
public:
	CRange(){}
	~CRange(){}
	
public:

	bool operator<(const CRange& cRange) const {
		return _ttof(this->m_strValue1) != _ttof(cRange.m_strValue1) ? _ttof(this->m_strValue1) < _ttof(cRange.m_strValue1) : 
			_ttof(this->m_strValue2) != _ttof(cRange.m_strValue2) ? _ttof(this->m_strValue2) < _ttof(cRange.m_strValue2) : 
			this->m_strSymbol1 != cRange.m_strSymbol1 ? this->m_strSymbol1 < cRange.m_strSymbol1 : this->m_strSymbol2 < cRange.m_strSymbol2;
	}
	
	bool operator>(const CRange& cRange) const {
		return _ttof(this->m_strValue1) != _ttof(cRange.m_strValue1) ? _ttof(this->m_strValue1) > _ttof(cRange.m_strValue1) : 
			_ttof(this->m_strValue2) != _ttof(cRange.m_strValue2) ? _ttof(this->m_strValue2) > _ttof(cRange.m_strValue2) : 
			this->m_strSymbol1 != cRange.m_strSymbol1 ? this->m_strSymbol1 > cRange.m_strSymbol1 : this->m_strSymbol2 > cRange.m_strSymbol2;
	}

public:
	bool init(const CString& groupName, const CString& column1, const CString& type, const CString& strSymbol1, const CString& strValue1, const CString& strSymbol2, const CString& strValue2);

public:
	CString m_strSymbol1;
	CString m_strValue1;
	CString m_strSymbol2;
	CString m_strValue2;

};

/**
 * @brief 変換フィールド対応表クラス
 */
class CFieldDefInfo{
public:
	CFieldDefInfo() : m_srcFieldIndex(-1), m_canNull(false), m_Length(-1), m_Number(-1), m_Decimal(-1), m_bNullValue(false), m_bFixedValue(false) {}
	~CFieldDefInfo(){}

public:
	/**
	 * @brief 初期化
	 * @param groupName [in] グループ名*
	 * @param line [in] 変換フィールド対応表の1レコード
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool init(const CString& groupName, const CString& line);

private:
	/**
	 * @brief 変換元情報格納
	 * @param groupName [in] グループ名
	 * @param column1 [in] 文字列（1列目）
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setSrcInfo(const CString& groupName, const CString& column1);

	/**
	 * @brief 変換先情報格納
	 * @param groupName [in] グループ名
	 * @param column2 [in] 文字列（2列目）
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setDstInfo(const CString& groupName, const CString& column2);

	/**
	 * @brief フィールド情報格納
	 * @param groupName [in] グループ名
	 * @param column1 [in] フィールド対応表のカラム文字列（1列目）（エラー出力文字用）
	 * @param column3 [in] フィールド対応表のカラム文字列（3列目）
	 * @param column4 [in] フィールド対応表のカラム文字列（4列目）
	 * @param column5 [in] フィールド対応表のカラム文字列（5列目）
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setFieldInfo(const CString& groupName, const CString& column1, const CString& column3, const CString& column4, const CString& column5);

	/**
	 * @brief 丸め込み値取得
	 * @param groupName [in] グループ名（エラー出力文字用）
	 * @param column1 [in] フィールド対応表のカラム文字列（1列目）（エラー出力文字用）
	 * @param cToken [in] 変換フィールド対応表のタブ分割トークン
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setRoundValue(const CString& groupName, const CString& column1, CStringTokenizer& cToken);

	/**
	 * @brief 条件値取得
	 * @param groupName [in] グループ名（エラー出力文字用）
	 * @param column1 [in] フィールド対応表のカラム文字列（1列目）（エラー出力文字用）
	 * @param type [in] エラー値か有効値か（エラー出力文字用）
	 * @param cToken [in] 変換フィールド対応表のタブ分割トークン
	 * @param setCheckValue [out] 条件値群
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setConditionalValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, std::set<CString>& setCheckValue);

	/**
	 * @brief 条件範囲取得
	 * @param groupName [in] グループ名（エラー出力文字用）
	 * @param column1 [in] フィールド対応表のカラム文字列（1列目）（エラー出力文字用）
	 * @param type [in] エラー値か有効値か（エラー出力文字用）
	 * @param cToken [in] 変換フィールド対応表のタブ分割トークン
	 * @param setConditinalRange [out] 条件範囲群
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setConditionalRange(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, std::set<CRange>& setConditinalRange);

	/**
	 * @brief Null値取得
	 * @param groupName [in] グループ名（エラー出力文字用）
	 * @param column1 [in] フィールド対応表のカラム文字列（1列目）（エラー出力文字用）
	 * @param type [in] エラー値か有効値か（エラー出力文字用）
	 * @param cToken [in] 変換フィールド対応表のタブ分割トークン
	 * @param strNullValue [out] Null値
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setNullValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, CString& strNullValue);

	/**
	 * @brief 固定値値取得
	 * @param groupName [in] グループ名（エラー出力文字用）
	 * @param column1 [in] フィールド対応表のカラム文字列（1列目）（エラー出力文字用）
	 * @param type [in] エラー値か有効値か（エラー出力文字用）
	 * @param cToken [in] 変換フィールド対応表のタブ分割トークン
	 * @param strFixedValue [out] 固定値
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool setFixedValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, CString& strFixedValue);

	/**
	 * @brief 条件範囲チェック
	 * @param groupName [in] グループ名（エラー出力文字用）
	 * @param column1 [in] フィールド対応表のカラム文字列（1列目）（エラー出力文字用）
	 * @param type [in] エラー値か有効値か（エラー出力文字用）
	 * @param setConditionalRange [out] 条件範囲値群
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool checkConditionalRange(const CString& groupName, const CString& column1, const CString& type, const std::set<CRange>& setConditionalRange);

public:
	CString m_srcTableName;	//!< 変換元テーブル名
	CString m_srcFieldName;	//!< 変換元フィールド名
	long m_srcFieldIndex;	//!< 変換元フィールドのフィールドインデックス
	CString m_dstFieldName;	//!< 変換先フィールド名
	FIELDTYPE::ECode m_FieldType;	//!< フィールドタイプ
	bool m_canNull;			//!< NULL許可
	long m_Length;			//!< STRINGのみ
	long m_Number;			//!< INTEGER,DOUBLE（整数部分の桁数）
	long m_Decimal;			//!< DOUBLEのみ（小数部分の桁数）

	bool m_bNullValue;		//!< NULL値設定されているかどうか
	CString m_NullValue;	//!< NULL値
	bool m_bFixedValue;		//!< 固定値設定されているかどうか
	CString m_FixedValue;	//!< 固定値

	std::map<CString,CString> m_mapRoundValue;	//!< 丸め込み用
	std::set<CString> m_setErrValue;			//!< エラー値用
	std::set<CRange> m_setErrRange;				//!< エラー範囲用
	std::set<CString> m_setValidValue;			//!< 有効値用
	std::set<CRange> m_setValidRange;			//!< 有効範囲用
};
