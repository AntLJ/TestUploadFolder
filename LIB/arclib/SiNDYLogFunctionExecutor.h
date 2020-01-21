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

// SiNDYLogFunctionExecutor.h: SiNDYLogFunctionExecutor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef __SINDYLOG_FUNCTION_EXECUTOR_H__
#define __SINDYLOG_FUNCTION_EXECUTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SiNDYTableFinder.h"
#include "SiNDYLogHeader.h"

#define EXECUTOR_CONSTRUCTOR(own, base) \
own(const Function& rFunction, SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) : \
base(rFunction, rTableFinder, rLogRow, pUpdatedColSet) \
{ \
}

namespace LogFE{

class MeshXY
{
public:
	MeshXY(unsigned int nMeshcode, int nX, int nY) :
	m_nMeshcode(nMeshcode),
	m_nX(nX),
	m_nY(nY)
	{
	}

	unsigned int m_nMeshcode;
	int m_nX;
	int m_nY;
};
}//end LogFE

class MeshXYList : public std::vector<LogFE::MeshXY>
{
public:
	CString getString() const;
};

class Executor
{
public:
	typedef SiNDYLogHeader::LogRowInfo LogRowInfo;
	typedef SiNDYLogHeader::FieldValues FieldValues;
	typedef SiNDYLogHeader::RowMap RowMap;
	typedef SiNDYLogHeader::Function::ColumnSet ColumnSet;
	typedef SiNDYLogHeader::Function::FunctionMap FunctionMap;
	typedef SiNDYTableName TableName;

	class RowMapWithFieldMap : public RowMap
	{
	public:
		RowMapWithFieldMap(LPCTSTR lpszTableName) :
		RowMap(lpszTableName)
		{
		}

		RowMapWithFieldMap(const TableName& rTableName) :
		RowMap(rTableName)
		{
		}

		std::map<CString, IFieldPtr> m_FieldMap;
	};

// 例外
	class Exception
	{
	public:
		void setReason(const CString& rReason) { m_strReason = rReason; }

		LPCTSTR what() const { return m_strReason; }

	private:
		CString m_strReason;
	};

	// 未実装例外
	class NotImplementException : public Exception
	{
	public:
		NotImplementException(LPCTSTR lpszKey, LPCTSTR lpszInput);
	};

	// 値許容値外
	class OverflowException : public Exception
	{
	public:
		OverflowException(const TableName& rTableName, int nCount);
	};

	// レイヤ名例外
	class LayerNotFoundException : public Exception
	{
	public:
		LayerNotFoundException(const TableName& rTableName);
	};

	// 属性フィールド名例外
	class FieldNotFoundException : public Exception
	{
	public:
		FieldNotFoundException(const CString& rFieldName);
	};

	// 計算不可例外
	class CantCalcException : public Exception
	{
	public:
		CantCalcException(LPCTSTR lpszFieldName = 0);
	};

	class SQLException : public Exception
	{
	public:
		SQLException();
	};

// 構築/消滅
	Executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet);

// 設定/取得
	SiNDYTableFinder& tableFinder() { return m_rTableFinder; }
	LogRowInfo& logRow() { return m_rLogRow; }
	ColumnSet* updatedColSet() { return m_pUpdatedColSet; }
	virtual const CString getKey() const = 0;
	virtual const FunctionMap& nextFunctionMap() const = 0;

	ITablePtr getTable(const CString& rTableName) { return getTable(TableName(rTableName)); }
	ITablePtr getTable(const TableName& rTableName);

// 処理
	virtual bool execute() { throw NotImplementException(getKey(), _T("void")); return false; }
	virtual bool execute(_IRow*) { throw NotImplementException(getKey(), _T("ユニーク・オブジェクト集合")); return false; ; }
	virtual bool execute(const RowMap&) { throw NotImplementException(getKey(), _T("オブジェクト集合")); return false; ; }
	virtual bool execute(const RowMapWithFieldMap&) { throw NotImplementException(getKey(), _T("オブジェクト集合2")); return false; ; }
	virtual bool execute(const FieldValues&) { throw NotImplementException(getKey(), _T("属性値集合")); return false; ; }
	virtual bool execute(const MeshXYList&) { throw NotImplementException(getKey(), _T("メッシュXY座標値集合")); return false; ; }

protected:
// 設定/取得
	virtual const SiNDYLogHeader::Function& _function() const = 0;

// 処理
	template <typename T>
	void chain(const T& t)
	{
		const FunctionMap& rFunctionMap = nextFunctionMap();
		for(FunctionMap::const_iterator it = rFunctionMap.begin(); it != rFunctionMap.end(); ++it) {
			const SiNDYLogHeader::Function* pFunction = it->second;
			if(onChain(*pFunction)) {
				try {
					pFunction->executor(m_rTableFinder, m_rLogRow, m_pUpdatedColSet)->execute(t);
				}
				catch(const Exception& e) {
					pFunction->fillAllCells(logRow(), e.what(), updatedColSet());
				}
			}
		}
		update(t);
	}
	void chain(IFieldPtr ipField, const CComVariant& va);

	template <typename T>
	void update(const T&)
	{
	}

	void update(const MeshXYList& rMeshXYList);
	void update(const FieldValues& rFieldValues);
	void update(LPCTSTR lpszValue);

	virtual bool onChain(const SiNDYLogHeader::Function&) const { return true; }

private:
// プロパティ
	SiNDYTableFinder& m_rTableFinder;
	LogRowInfo& m_rLogRow;
	ColumnSet* m_pUpdatedColSet;
};

template <typename T, typename TBase = Executor>
class ExecutorT : public TBase
{
public:
	typedef T Function;

	ExecutorT(
		const T& rFunction, SiNDYTableFinder& rTableFinder,
		SiNDYLogHeader::LogRowInfo& rLogRow,
		SiNDYLogHeader::Function::ColumnSet* pUpdatedColSet)
			: TBase(rTableFinder, rLogRow, pUpdatedColSet)
			, m_rFunction(rFunction)
	{
	}


// 設定/取得
	const T& function() const { return m_rFunction; }

	virtual const CString getKey() const { return function().getKey(); }
	virtual const SiNDYLogHeader::Function::FunctionMap& nextFunctionMap() const { return function().nextFunctionMap(); }

protected:
	virtual const SiNDYLogHeader::Function& _function() const { return function(); }

private:
// プロパティ
	const T& m_rFunction;
};

class _FieldExecutor : public ExecutorT<SiNDYLogHeader::FieldFunction>
{
protected:
	EXECUTOR_CONSTRUCTOR(_FieldExecutor, ExecutorT<SiNDYLogHeader::FieldFunction>)

// 処理
	static void getFieldValues(_IRow* ipRow, long nFieldIndex, FieldValues& rFieldValues);
	static void getFieldValues(const RowMap& rRowMap, long nFieldIndex, FieldValues& rFieldValues);
};

class FieldExecutor : public _FieldExecutor
{
public:
// 構築/消滅
	EXECUTOR_CONSTRUCTOR(FieldExecutor, _FieldExecutor)

// 処理
	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
};

class SubFieldExecutor : public _FieldExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(SubFieldExecutor, _FieldExecutor)

// 処理
	virtual bool execute(const RowMapWithFieldMap& rRowMap);
};

typedef ExecutorT<SiNDYLogHeader::PointFunction> _PointExecutor;
class PointExecutor : public _PointExecutor
{
public:
// 構築/消滅
	EXECUTOR_CONSTRUCTOR(PointExecutor, _PointExecutor)

// 処理
	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
	virtual bool execute(const FieldValues& rFieldValues);

private:
// 処理
	IPointPtr getPoint(IFeature* ipFeature);
	IPointPtr getPoint(IPointCollection* ipPointCol);
};

class BufferExecutor : public ExecutorT<SiNDYLogHeader::BufferFunction>
{
public:
// 構築/消滅
	EXECUTOR_CONSTRUCTOR(BufferExecutor, ExecutorT<SiNDYLogHeader::BufferFunction>)

// 処理
	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
	virtual bool execute(const FieldValues& rFieldValues);

private:
// 処理
	IGeometryPtr buffer(IFeature* ipFeature);
	IGeometryPtr buffer(IGeometry* ipGeometry);
};

typedef ExecutorT<SiNDYLogHeader::LengthFunction, Executor> _LengthExecutor;
class LengthExecutor : public _LengthExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(LengthExecutor, _LengthExecutor)

	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
	virtual bool execute(const FieldValues& rFieldValues);

protected:
	double getLength(IFeature* ipFeature);
	double getLength(IPointCollection* ipPointCol);
	double getLength(double dX1, double dY1, double dX2, double dY2);
};

class LonLatExecutor : public Executor
{
public:
// 構築/消滅
	LonLatExecutor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) :
	Executor(rTableFinder, rLogRow, pUpdatedColSet)
	{
	}

	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
	virtual bool execute(const FieldValues& rFieldValues);

protected:
	CComVariant getValue(IFeature* ipFeature);
	CComVariant getValue(const VARIANT& rVariant);
	virtual CComVariant getValue(IPoint* ipPoint) = 0;
};

typedef ExecutorT<SiNDYLogHeader::LongitudeFunction, LonLatExecutor> _LongitudeExecutor;
class LongitudeExecutor : public _LongitudeExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(LongitudeExecutor, _LongitudeExecutor)

protected:
	virtual CComVariant getValue(IPoint* ipPoint);
};

typedef ExecutorT<SiNDYLogHeader::LatitudeFunction, LonLatExecutor> _LatitudeExecutor;
class LatitudeExecutor : public _LatitudeExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(LatitudeExecutor, _LatitudeExecutor)

protected:
	virtual CComVariant getValue(IPoint* ipPoint);
};

class MeshExecutor : public Executor
{
public:
// 構築/消滅
	MeshExecutor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) :
	Executor(rTableFinder, rLogRow, pUpdatedColSet)
	{
	}

	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
	virtual bool execute(const FieldValues& rFieldValues);

protected:
	LogFE::MeshXY getMeshXY(IFeature* ipFeature);
	LogFE::MeshXY getMeshXY(const VARIANT& rVariant);
	LogFE::MeshXY getMeshXY(IPoint* ipPoint);
	virtual LogFE::MeshXY getMeshXY(double dLongitude, double dLatitude) = 0;
};

typedef ExecutorT<SiNDYLogHeader::BaseMeshFunction, MeshExecutor> _BaseMeshExecutor;
class BaseMeshExecutor : public _BaseMeshExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(BaseMeshExecutor, _BaseMeshExecutor)

protected:
// 処理
	LogFE::MeshXY getMeshXY(double dLongitude, double dLatitude);
};

class MeshXYExecutor : public Executor
{
public:
// 構築/消滅
	MeshXYExecutor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) :
	Executor(rTableFinder, rLogRow, pUpdatedColSet)
	{
	}

	virtual bool execute(const MeshXYList& rMeshXYList);

protected:
	virtual CComVariant getValue(const LogFE::MeshXY& rMeshXY) = 0;
};

typedef ExecutorT<SiNDYLogHeader::MeshcodeFunction, MeshXYExecutor> _MeshcodeExecutor;
class MeshcodeExecutor : public _MeshcodeExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(MeshcodeExecutor, _MeshcodeExecutor)

protected:
	virtual CComVariant getValue(const LogFE::MeshXY& rMeshXY) { return CComVariant(rMeshXY.m_nMeshcode, VT_UI4); }
};

typedef ExecutorT<SiNDYLogHeader::MeshXFunction, MeshXYExecutor> _MeshXExecutor;
class MeshXExecutor : public _MeshXExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(MeshXExecutor, _MeshXExecutor)

protected:
	virtual CComVariant getValue(const LogFE::MeshXY& rMeshXY) { return CComVariant(rMeshXY.m_nX, VT_I4); }
};

typedef ExecutorT<SiNDYLogHeader::MeshYFunction, MeshXYExecutor> _MeshYExecutor;
class MeshYExecutor : public _MeshYExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(MeshYExecutor, _MeshYExecutor)

protected:
	virtual CComVariant getValue(const LogFE::MeshXY& rMeshXY) { return CComVariant(rMeshXY.m_nY, VT_I4); }
};

typedef ExecutorT<SiNDYLogHeader::DistinctFunction> _DistinctExecutor;
class DistinctExecutor : public _DistinctExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(DistinctExecutor, _DistinctExecutor)

	virtual bool execute(const FieldValues& rFieldValues);
};

typedef ExecutorT<SiNDYLogHeader::AvgFunction> _AvgExecutor;
class AvgExecutor : public _AvgExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(AvgExecutor, _AvgExecutor)
	virtual bool execute(const FieldValues& rFieldValues);
};

typedef ExecutorT<SiNDYLogHeader::MaxFunction> _MaxExecutor;
class MaxExecutor : public _MaxExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(MaxExecutor, _MaxExecutor)
	virtual bool execute(const FieldValues& rFieldValues);
};

typedef ExecutorT<SiNDYLogHeader::MinFunction> _MinExecutor;
class MinExecutor : public _MinExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(MinExecutor, _MinExecutor)
	virtual bool execute(const FieldValues& rFieldValues);
};

typedef ExecutorT<SiNDYLogHeader::SumFunction> _SumExecutor;
class SumExecutor : public _SumExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(SumExecutor, _SumExecutor)
	virtual bool execute(const FieldValues& rFieldValues);
};

typedef ExecutorT<SiNDYLogHeader::CountFunction> _CountExecutor;
class CountExecutor : public ExecutorT<SiNDYLogHeader::CountFunction>
{
public:
	EXECUTOR_CONSTRUCTOR(CountExecutor, _CountExecutor)

	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
	virtual bool execute(const FieldValues& rFieldValues);
};

class SelectExecutor : public Executor
{
public:
	typedef SiNDYLogHeader::SelectFunction SelectFunction;

	SelectExecutor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet);

	virtual const SelectFunction& selectFunction() const = 0;

protected:
	IFeatureCursorPtr getFeatureCursor(IFeatureClass* ipFeatureClass, IQueryFilter* ipQueryFilter) throw (Exception);
	void getRowMap(ITable* ipTable, BSTR bstrFieldName, const std::set<long> rValueSet, RowMap& rRowMap, BSTR bstrSortFieldName = 0) throw (Exception);
	void getRowMap(ITable* ipTable, BSTR bstrWhereClause, RowMap& rRowMap, BSTR bstrFieldName = 0) throw (Exception);
	void getRowMap(ITable* ipTable, IQueryFilter* ipQueryFilter, RowMap& rRowMap, BSTR bstrFieldName = 0) throw (Exception);
	_ICursorPtr getCursor(ITable* ipTable, IQueryFilter* ipQueryFilter) throw (Exception);

	void getRowMap(BSTR bstrTables, BSTR bstrWhereClause, const std::set<CString>& rFieldNameSet, RowMapWithFieldMap& rRowMap);
	_ICursorPtr getCursor(BSTR bstrSubFields, BSTR bstrTables, BSTR bstrWhereClause);
};

typedef ExecutorT<SiNDYLogHeader::BelongFunction, SelectExecutor> _BelongExecutor;
class BelongExecutor : public _BelongExecutor
{
public:
	EXECUTOR_CONSTRUCTOR(BelongExecutor, _BelongExecutor)

// 設定/取得
	virtual const SelectFunction& selectFunction() const { return function(); }

// 処理
	virtual bool execute(_IRow* ipRow);
	virtual bool execute(const RowMap& rRowMap);
	virtual bool execute(const FieldValues& rFieldValues);

private:
// 処理
	void belong(_IRow* ipRow, RowMap& rResultRowMap);
	void belong(const RowMap& rRowMap, RowMap& rResultRowMap);
	void belong(const FieldValues& rFieldValues, RowMap& rResultRowMap);

	void belongByInf(const TableName& rInfTableName, _IRow* ipInfRow, RowMap& rResultRowMap);
	void belongByLq(const TableName& rLqTableName, _IRow* ipRow, RowMap& rResultRowMap);
	void belongByRoadLink(const TableName& rLinkTableName, IFeature* ipFeature, RowMap& rResultRowMap);
	void belongByFeature(IFeature* ipFeature, RowMap& rResultRowMap);
	void belongByGeometry(IGeometry* ipGeometry1, RowMap& rResultRowMap);

	void belongByInf(const RowMap& rRowMap, RowMap& rResultRowMap);
	void belongByLq(const RowMap& rRowMap, RowMap& rResultRowMap);
	void belongByRoadLink(const RowMap& rRowMap, RowMap& rResultRowMap);
	void belongByFeature(const RowMap& rRowMap, RowMap& rResultRowMap);

	void getLqByInf(_IRow* ipRow, RowMap& rResultRowMap);
	void getInfByLq(_IRow* ipRow, RowMap& rResultRowMap);
	void getRoadLinkByLq(_IRow* ipRow, RowMap& rResultRowMap);
	void getLqByRoadLink(IFeature* ipFeature, RowMap& rResultRowMap);
	void getRoadLinkByLinkIdSequence(const std::map<long, long>& cLinkIdSequence, RowMap& rResultRowMap);

	void getLqByInf(const RowMap& rRowMap, RowMap& rResultRowMap);
	void getRoadLinkByLq(const RowMap& rRowMap, RowMap& rResultRowMap);
	void getLqByRoadLink(const RowMap& rRowMap, RowMap& rResultRowMap);
	void getInfByLq(const RowMap& rRowMap, RowMap& rResultRowMap);
};

typedef ExecutorT<SiNDYLogHeader::RootFunction, SelectExecutor> _RootExecutor;
class RootExecutor : public ExecutorT<SiNDYLogHeader::RootFunction, SelectExecutor>
{
public:
	EXECUTOR_CONSTRUCTOR(RootExecutor, _RootExecutor)

// 設定/取得
	virtual const SelectFunction& selectFunction() const { return function(); }

// 処理
	bool execute();

protected:
	bool onChain(const SiNDYLogHeader::Function& rFunction) const;

private:
	bool m_bChainSubField;
};

#endif // __SINDYLOG_FUNCTION_EXECUTOR_H__
