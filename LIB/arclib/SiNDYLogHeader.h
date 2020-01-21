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

// SiNDYLogHeader.h: SiNDYLogHeader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLOGHEADER_H__D9B2099B_780B_475A_B97E_2F6631DAF6C4__INCLUDED_)
#define AFX_SINDYLOGHEADER_H__D9B2099B_780B_475A_B97E_2F6631DAF6C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <map>
#include <vector>
#include <list>
#include "SiNDYTableName.h"
#include "SiNDYTableFinder.h"

#pragma warning(disable : 4290) // 例外構文警告解除

class Executor;

/**
 * @brief SiNDY自由形式ログ機能ヘッダクラス
 *
 * @note 要CStringクラス。
 */
class SiNDYLogHeader  
{
public:
// 定義
	typedef std::vector<CString> LogRow;
	typedef std::set<int> FlagColSet;
	typedef std::set<int> EditableColSet;
	typedef SiNDYTableName TableName;

	class FieldValues : public std::list<CComVariant>
	{
	public:
	// 構築/消滅
		FieldValues(IFieldPtr ipField = 0) : m_ipField(ipField) {}

	// 設定/取得
		CString getValuesString() const;

	// プロパティ
		IFieldPtr m_ipField;
	};

	/// オブジェクト一覧
	class RowMap : public std::map<long ,_IRowPtr>
	{
	public:
		RowMap(LPCTSTR lpszTableName);
		RowMap(const TableName& rTableName);

		bool insert(const RowMap& rRowMap);

		TableName m_cTableName;
	};

	class LogRowInfo
	{
	public:
	// 構築/消滅
		LogRowInfo(LogRow& rLogRow, const SiNDYLogHeader& rHeader);

	// 設定/取得
		void clearInfo();
		void initInfo(const SiNDYLogHeader& rHeader);

		void updateCellsString(const std::set<int>& rColSet, LPCTSTR lpszNew, std::set<int>* pUpdatedColSet);
		bool updateCellString(int nCol, LPCTSTR lpszNew, std::set<int>* pUpdatedColSet);

		LogRow::size_type size() const { return m_rLogRow.size(); }
		LogRow::const_reference operator [] (LogRow::size_type  n) const;

	// プロパティ
		TableName m_cTableName; ///< テーブル名
		long m_nObjectId; ///< オブジェクトID
		CString m_strWhereClause; ///< Where句
		IGeometryPtr m_ipGeometry; ///< 形状

	private:
	// 静的関数
		static bool updateCellString(CString& rCell, LPCTSTR lpszNew);

	// プロパティ
		LogRow& m_rLogRow;
	};

	/**
	 * @brief 機能関数
	 */
	class Function
	{
	public:
	// 定義

		typedef std::map<CString, Function*> FunctionMap;
		typedef std::set<int> ColumnSet;
		typedef std::auto_ptr<Executor> ExecutorPtr;

	// 構築/消滅
		virtual ~Function();

	// 設定/取得
		virtual bool addNextFunctions(int nCol, LPCTSTR lpszFunctionList);
		virtual Function* addNextFunction(Function* pNextFunction);

		const FunctionMap& nextFunctionMap() const { return m_cNextFunctionMap; }
		const ColumnSet& columnSet() const { return m_cColSet; }

		void getUpdateColumnSet(ColumnSet& rColSet) const;

		virtual bool empty() const { return m_cNextFunctionMap.empty(); }

		virtual LPCTSTR getClassName() const = 0;
		virtual CString getKey() const = 0;
		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error) = 0;

	// 処理
		void fillAllCells(LogRowInfo& rLogRow, LPCTSTR lpszNew, ColumnSet* pUpdatedColSet) const;

	private:
	// プロパティ
		FunctionMap m_cNextFunctionMap;
		ColumnSet m_cColSet;
	};

	/**
	 * @brief 属性値取得関数クラス
	 */
	class FieldFunction : public Function
	{
	public:
	// 構築/消滅
		FieldFunction(LPCTSTR lpszFieldName);

	// 設定/取得
		const CString& fieldName() const { return m_strFieldName; }

	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("FIELD"); }

	protected:
	// プロパティ
		CString m_strFieldName; ///< フィールド名
	};

	class SubFieldFunction : public FieldFunction
	{
	public:
	// 構築/消滅
		SubFieldFunction(LPCTSTR lpszFieldName) : FieldFunction(lpszFieldName)
		{
		}

	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("SUBFIELD"); }
	};

	class PointFunction : public Function
	{
	public:
	// 構築/消滅
		PointFunction(LPCTSTR lpszNo);

	// 設定/取得
		IPointPtr getPoint(IPointCollection* ipPointCol) const;

	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("POINT"); }

	protected:
	// プロパティ
		int m_nNo; ///< 点番号
		bool m_bLast; ///< 最後の点？
	};

	class BufferFunction : public Function
	{
	public:
	// コンストラクタとデストラクタ
		BufferFunction(LPCTSTR lpszDistance);

	// 処理
		IGeometryPtr buffer(IGeometry* ipGeometry) const;

	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("BUFFER"); }

	private:
		double m_dDistance;
	};

	class LengthFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("LENGTH"); }
		static LPCTSTR shortClassName() { return _T("LEN"); }
	};

	/**
	 * @brief 経度取得関数クラス
	 */
	class LongitudeFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("LONGITUDE"); }
		static LPCTSTR shortClassName() { return _T("LON"); }
	};

	/**
	 * @brief 緯度取得関数クラス
	 */
	class LatitudeFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("LATITUDE"); }
		static LPCTSTR shortClassName() { return _T("LAT"); }
	};

	class BaseMeshFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("BASEMESH"); }
		static LPCTSTR shortClassName() { return _T("BASE"); }
	};

	class MeshcodeFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("MESHCODE"); }
		static LPCTSTR shortClassName() { return _T("MESH"); }
	};

	class MeshXFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("X"); }
	};

	class MeshYFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("Y"); }
	};

	/**
	 * @brief ユニーク・ソート関数クラス
	 */
	class DistinctFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("DISTINCT"); }
	};

	class AvgFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("AVG"); }
	};

	class MaxFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("MAX"); }
	};

	class MinFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("MIN"); }
	};

	class SumFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("SUM"); }
	};

	/**
	 * @brief カウント関数クラス
	 */
	class CountFunction : public Function
	{
	public:
	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("COUNT"); }
	};

	class SelectFunction : public Function
	{
	public:
	// 構築/消滅
		SelectFunction();
		SelectFunction(LPCTSTR lpszLayerName, LPCTSTR lpszWhereClause);

	// 設定/取得
		void setTableName(const CString& rTableName) { m_cTableName = rTableName; }
		void setTableName(const TableName& rTableName) { m_cTableName = rTableName; }

		const TableName& tableName() const { return m_cTableName; }
		const CString& whereClause() const { return m_strWhereClause; }

	private:
	// プロパティ
		TableName m_cTableName; ///< 選択を行うレイヤ名
		CString m_strWhereClause; ///< 再選択を行う際に使用するWhere句 (option)
	};

	/**
	 * @brief 関連オブジェクト再選択クラス
	 */
	class BelongFunction : public SelectFunction
	{
	public:
	// 構築/消滅
		BelongFunction(LPCTSTR lpszLayerName, LPCTSTR lpszWhereClause);

	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 静的関数
		static LPCTSTR className() { return _T("BELONG"); }

	};

	class RootFunction : public SelectFunction
	{
	public:
	// 設定/取得
		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// 仮想関数のオーバーライド
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

	// 静的関数
		static LPCTSTR className() { return _T("ROOT"); }
	};

	typedef std::map<int, CString> FieldColMap;
	typedef std::map<int, CString> ComboMap;
	typedef std::map<int, CString> OpenColMap;
	typedef std::map<int, CString> AliasMap;

// 構築/消滅
	SiNDYLogHeader();
	virtual ~SiNDYLogHeader();

// 設定/取得
	void clear();

	bool setHeader(const CString& strHeader, LPCTSTR sep = 0);
	bool setHeader(const LogRow& rLogRow);
	bool setColumnHeader(const CString& rColumnHeader, int nCol);

	bool isEditableColumn(int nCol) const { return m_cEditableColSet.find(nCol) != m_cEditableColSet.end(); }
	bool isFlagColumn(int nCol) const { return m_cFlagColSet.find(nCol) != m_cFlagColSet.end(); }
	bool canGetLayerName() const { return m_nLayerCol >= 0 || ! m_strLayerName.IsEmpty(); }
	bool canIdentifyObject() const { return m_nObjectIdCol >= 0 && canGetLayerName(); }
	bool canIdentifyVdbPrimitive() const { return m_nVdbMeshcodeCol >= 0 && m_nVdbPrimitiveIdCol >= 0 && canGetLayerName(); }
	bool canSelectObjects() const;
	bool canGetLonLat() const { return m_nLongitudeCol >= 0 && m_nLatitudeCol >= 0; }
	bool canGetMeshXY() const { return m_nMeshcodeCol >= 0 && m_nXCol >= 0 && m_nYCol >= 0; }
	bool haveScaleColumn() const { return m_nScaleCol >= 0; }
	bool haveUpdateColumn() const { return ! (m_cRootFunction.empty()); }

	CString getColumnHeaderAlias(int nCol);

	bool getLayerName(const LogRow& rRow, CString& rLayerName) const;
	bool getObjectIdentifier(const LogRow& rRow, CString& rLayerName, long& rObjectId) const;
	bool getVdbMeshcode(const LogRow& rRow, long& rVdbMeshcode) const;
	bool getObjectsSelector(const LogRow& rRow, CString& rLayerName, CString& rWhereClause) const;
	bool getLonLat(const LogRow& rRow, double& rLongitude, double& rLatitude) const;
	bool getMeshXY(const LogRow& rRow, unsigned int& rMeshcode, int& rX, int& rY) const;
	bool getScale(const LogRow& rRow, double& rScale) const;

	_IRowPtr getIdentifiedObject(const LogRow& rRow, SiNDYTableFinder& rTableCarrier) const;
	IFeaturePtr getIdentifiedVdbPrimitive(const LogRow& rRow, SiNDYTableFinder& rTableCarrier) const;

	virtual LPCTSTR getId() const { return 0; }
	virtual LPCTSTR getDescription() const { return 0; }
	virtual LPCTSTR getSeps() const { return _T("\t"); }

// プロパティ
	int m_nLayerCol; ///< レイヤ名列番号
	int m_nObjectIdCol; ///< オブジェクトID列番号
	int m_nVdbMeshcodeCol; ///< VDBメッシュコード列番号
	int m_nVdbPrimitiveIdCol; ///< VDBプリミティブID列番号
	int m_nWhereClauseCol; ///< Where句列番号
	int m_nScaleCol; ///< スケール列番号
	int m_nLongitudeCol; ///< 経度列番号
	int m_nLatitudeCol; ///< 緯度列番号
	int m_nMeshcodeCol; ///< メッシュコード列番号
	int m_nXCol; ///< X座標列番号
	int m_nYCol; ///< Y座標列番号
	FlagColSet m_cFlagColSet; ///< フラグ列番号
	EditableColSet m_cEditableColSet; ///< コメント列番号
	ComboMap m_cComboMap; ///< コメント列に許された要素一覧
	OpenColMap m_cOpenColMap; ///< OPEN列と前置詞一覧
	FieldColMap m_cFieldColMap; ///< フィールド列番号とフィールド名

	RootFunction m_cRootFunction;

	CString m_strLayerName; ///< 固定レイヤ名

private:
	bool setFunctionColumnHeader(const CString& strColumnHeader, int nCol);

	CString getObjectsSelectorByObjectId(const LogRow& rRow) const;
	CString getObjectsSelectorByVdbPrimitiveId(const LogRow& rRow) const;
	CString getObjectsSelectorByField(const LogRow& rRow) const;
	CString getObjectsSelectorByWhereClause(const LogRow& rRow) const;

	CString m_strFunctionHeader;
};

#endif // !defined(AFX_SINDYLOGHEADER_H__D9B2099B_780B_475A_B97E_2F6631DAF6C4__INCLUDED_)
