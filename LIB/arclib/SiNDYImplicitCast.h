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

#ifndef __SINDY_IMPLICIT_CAST_H__
#define __SINDY_IMPLICIT_CAST_H__

#include "SiNDYTraits.h"

namespace sindy {

/// インタフェースポインタから、引数なしでデータ取得
template <typename TData, typename TInterface>
inline TData castToData(TInterface* pInterface)
{
	if(! pInterface)
		return 0;

	TData aData;
	traits<TInterface>::accessor<TData>::get(pInterface, &aData);
	return aData;
};

/// スマートポインタから、引数なしでデータ取得
template <typename TData, typename TIIID>
inline TData castToData(const _com_ptr_t<TIIID>& pInterface)
{
	if(pInterface == 0)
		return 0;

	TData aData;
	traits<TIIID::Interface>::accessor<TData>::get(pInterface, &aData);
	return aData;
};

/**
 * @brief スマートポインタからポインタを取り出して派生元クラスへ渡すだけのクラス
 */
template <typename TBase>
class InterfacePtr_ : public TBase
{
public:
// 構築
	InterfacePtr_()
	{
	}

	/// ポインタはそのまま
	template <typename TInterface>
	InterfacePtr_(TInterface* ipInterface) : TBase(ipInterface)
	{
	}

	/// スマートポインタはただのポインタに
	template <typename TIIID>
	InterfacePtr_(const _com_ptr_t<TIIID>& rInterface) : TBase(static_cast<TIIID::Interface*>(rInterface))
	{
	}

	/// スマートポインタはただのポインタに
	template <typename TInterface>
	InterfacePtr_(const CComPtr<TInterface>& rInterface) : TBase(static_cast<TInterface*>(rInterface))
	{
	}
};

/**
 * @brief 暗黙の型変換クラス、コンストラクタ定義開始マクロ
 *
 * このマクロは以下のコンストラクタを定義する。
 * - デフォルトコンストラクタ。
 * - 正当な継承関係にあるインタフェースを引数とするコンストラクタ。
 * - ICloneを引数とするコンストラクタ。
 */
#define BEGIN_CONSTRUCTOR(c, b) \
	typedef c class_; \
	typedef b base_; \
	class_() {} \
	class_(b::Interface* pInterface) : base_(pInterface) {} \
	class_(IClone* ipClone) : base_(ipClone) {}

/**
 * @brief 引数をそのままスマートポインタへ渡すマクロ
 */
#define THROUGH(i) class_(i pInterface) : base_(pInterface) {}

/**
 * @brief 引数を型変換してスマートポインタへ渡すマクロ（AddRefあり）
 */
#define CAST_THROUGH(i, c) class_(i pInterface) : base_(c(pInterface)) {}

/**
 * @brief 引数を型変換してスマートポインタへ渡すマクロ（AddRefなし）
 */
#define CAST_ATTACH(i, c) class_(i pInterface) : base_(pInterface ? c(pInterface) : 0, false) {}

/**
 * @brief メンバ変数をスマートポインタへ渡すマクロ（AddRefなし）
 */
#define DATA_ATTACH(i) CAST_ATTACH(i, getData<base_::Interface*>)

#define CAST_DATA_ATTACH(i, c) class_(i pInterface) : base_(castToData<base_::Interface*>(c(pInterface)), false) {}

// 定義開始
/////////////////////////////////////////////////////////////////////////////

/// IFeatureLayerPtrへの型変換を補佐するクラス
class FeatureLayerPtr_ : public IFeatureLayerPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(FeatureLayerPtr_, IFeatureLayerPtr)
	THROUGH(ILayer*)
};

typedef InterfacePtr_<FeatureLayerPtr_> FeatureLayerPtr;

/// ITablePtrへの型変換を補佐するクラス
class TablePtr_ : public ITablePtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(TablePtr_, ITablePtr)
	THROUGH(IFeatureClass*)
	THROUGH(IDataset*)
	CAST_THROUGH(_IRow*, getTable);
	CAST_THROUGH(IFeature*, getTable);
	CAST_THROUGH(IStandaloneTable*, getTable);

private:
	template <typename T>
	ITable* getTable(T* pT)
	{
		if(! pT)
			return 0;

		ITable* ipTable;
		pT->get_Table(&ipTable);

		return ipTable;
	}
};

typedef InterfacePtr_<TablePtr_> TablePtr;

/// IFeatureClassPtrへの型変換を補佐するクラス
class FeatureClassPtr_ : public IFeatureClassPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(FeatureClassPtr_, IFeatureClassPtr)
	THROUGH(ITable*)
	THROUGH(IDataset*)
	DATA_ATTACH(IFeatureLayer*)
	CAST_DATA_ATTACH(ILayer*, FeatureLayerPtr_)
};

typedef InterfacePtr_<FeatureClassPtr_> FeatureClassPtr;

/// IDatasetPtrへの型変換を補佐するクラス
class DatasetPtr_ : public IDatasetPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(DatasetPtr_, IDatasetPtr)
	THROUGH(ITable*)
	THROUGH(IFeatureClass*)
	THROUGH(ILayer*)
	CAST_THROUGH(_IRow*, TablePtr_)
	CAST_THROUGH(IFeature*, TablePtr_)
};

typedef InterfacePtr_<DatasetPtr_> DatasetPtr;

/// IWorkspacePtrへの型変換を補佐するクラス
class WorkspacePtr_ : public IWorkspacePtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(WorkspacePtr_, IWorkspacePtr)
	THROUGH(IFeatureWorkspace*)
	DATA_ATTACH(IDataset*)
	CAST_DATA_ATTACH(ITable*, DatasetPtr_)
	CAST_DATA_ATTACH(IFeatureClass*, DatasetPtr_)
};

typedef InterfacePtr_<WorkspacePtr_> WorkspacePtr;

/// IFeatureWorkspacePtrへの型変換を補佐するクラス
class FeatureWorkspacePtr_ : public IFeatureWorkspacePtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(FeatureWorkspacePtr_, IFeatureWorkspacePtr)
	THROUGH(IWorkspace*)
	CAST_THROUGH(ITable*, WorkspacePtr);
	CAST_THROUGH(IFeatureClass*, WorkspacePtr);
	CAST_THROUGH(IDataset*, WorkspacePtr);
};

typedef InterfacePtr_<FeatureWorkspacePtr_> FeatureWorkspacePtr;

/// IWorkspaceDomainsPtrへの型変換を補佐するクラス
class WorkspaceDomainsPtr_ : public IWorkspaceDomainsPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(WorkspaceDomainsPtr_, IWorkspaceDomainsPtr)
	THROUGH(IWorkspace*)
	THROUGH(IFeatureWorkspace*)
	CAST_THROUGH(ITable*, WorkspacePtr);
	CAST_THROUGH(IFeatureClass*, WorkspacePtr);
	CAST_THROUGH(IDataset*, WorkspacePtr);
};

typedef InterfacePtr_<WorkspaceDomainsPtr_> WorkspaceDomainsPtr;

/// IFieldsPtrへの型変換を補佐するクラス
class FieldsPtr_ : public IFieldsPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(FieldsPtr_, IFieldsPtr)
	THROUGH(IFieldsEdit*)
	CAST_ATTACH(ITable*, getFields);
	CAST_ATTACH(IFeatureClass*, getFields);
	CAST_ATTACH(_IRow*, getFields);
	CAST_ATTACH(IFeature*, getFields);
	CAST_ATTACH(IFeatureBuffer*, getFields);

private:
	template <typename T>
	IFields* getFields(T* pT)
	{
		IFields* ipFields = 0;
		pT->get_Fields(&ipFields);
		return ipFields;
	}
};

typedef InterfacePtr_<FieldsPtr_> FieldsPtr;

/// IDomainPtrへの型変換を補佐するクラス
class DomainPtr_ : public IDomainPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(DomainPtr_, IDomainPtr)
	THROUGH(ICodedValueDomain*)
	DATA_ATTACH(IField*)
};

typedef InterfacePtr_<DomainPtr_> DomainPtr;

/// ICodedValueDomainPtrへの型変換を補佐するクラス
class CodedValueDomainPtr_ : public ICodedValueDomainPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(CodedValueDomainPtr_, ICodedValueDomainPtr)
	THROUGH(IDomain*)
	CAST_THROUGH(IField*, DomainPtr)
};

typedef InterfacePtr_<CodedValueDomainPtr_> CodedValueDomainPtr;

/// _IGeometryPtrへの型変換を補佐するクラス
class GeometryPtr_ : public IGeometryPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(GeometryPtr_, IGeometryPtr)
	THROUGH(IPointCollection*)
	THROUGH(IRelationalOperator*)
	DATA_ATTACH(IFeature*)
};

typedef InterfacePtr_<GeometryPtr_> GeometryPtr;

/// IEnvelopePtrへの型変換を補佐するクラス
class EnvelopePtr_ : public IEnvelopePtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(EnvelopePtr_, IEnvelopePtr)
	CAST_ATTACH(IGeometry*, getEnvelope)
	CAST_ATTACH(IPoint*, getEnvelope)
	CAST_ATTACH(IPath*, getEnvelope)
	CAST_ATTACH(IPolyline*, getEnvelope)
	CAST_ATTACH(IPolygon*, getEnvelope)
	CAST_DATA_ATTACH(IRelationalOperator*, GeometryPtr_)

private:
	template <typename T>
	IEnvelope* getEnvelope(T* pT)
	{
		IEnvelope* ipEnv = 0;
		pT->get_Envelope(&ipEnv);
		return ipEnv;
	}
};

typedef InterfacePtr_<EnvelopePtr_> EnvelopePtr;

/// IGeometryDefPtrへの型変換を補佐するクラス
class GeometryDefPtr_ : public IGeometryDefPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(GeometryDefPtr_, IGeometryDefPtr)
	DATA_ATTACH(IField*)
};

typedef InterfacePtr_<GeometryDefPtr_> GeometryDefPtr;

/// IGeometryDefEditPtrへの型変換を補佐するクラス
class GeometryDefEditPtr_ : public IGeometryDefEditPtr
{
public:
// 構築
	BEGIN_CONSTRUCTOR(GeometryDefEditPtr_, IGeometryDefEditPtr)
	THROUGH(IGeometryDef*)
	CAST_THROUGH(IField*, GeometryDefPtr)
};

typedef InterfacePtr_<GeometryDefEditPtr_> GeometryDefEditPtr;

/////////////////////////////////////////////////////////////////////////////
// 定義終了

#undef BEGIN_CONSTRUCTOR
#undef THROUGH
#undef CAST_THROUGH
#undef CAST_ATTACH
#undef DATA_ATTACH

} // namespace sindy

#endif // __SINDY_IMPLICIT_CAST_H__
