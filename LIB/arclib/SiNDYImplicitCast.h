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

/// �C���^�t�F�[�X�|�C���^����A�����Ȃ��Ńf�[�^�擾
template <typename TData, typename TInterface>
inline TData castToData(TInterface* pInterface)
{
	if(! pInterface)
		return 0;

	TData aData;
	traits<TInterface>::accessor<TData>::get(pInterface, &aData);
	return aData;
};

/// �X�}�[�g�|�C���^����A�����Ȃ��Ńf�[�^�擾
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
 * @brief �X�}�[�g�|�C���^����|�C���^�����o���Ĕh�����N���X�֓n�������̃N���X
 */
template <typename TBase>
class InterfacePtr_ : public TBase
{
public:
// �\�z
	InterfacePtr_()
	{
	}

	/// �|�C���^�͂��̂܂�
	template <typename TInterface>
	InterfacePtr_(TInterface* ipInterface) : TBase(ipInterface)
	{
	}

	/// �X�}�[�g�|�C���^�͂����̃|�C���^��
	template <typename TIIID>
	InterfacePtr_(const _com_ptr_t<TIIID>& rInterface) : TBase(static_cast<TIIID::Interface*>(rInterface))
	{
	}

	/// �X�}�[�g�|�C���^�͂����̃|�C���^��
	template <typename TInterface>
	InterfacePtr_(const CComPtr<TInterface>& rInterface) : TBase(static_cast<TInterface*>(rInterface))
	{
	}
};

/**
 * @brief �Öق̌^�ϊ��N���X�A�R���X�g���N�^��`�J�n�}�N��
 *
 * ���̃}�N���͈ȉ��̃R���X�g���N�^���`����B
 * - �f�t�H���g�R���X�g���N�^�B
 * - �����Ȍp���֌W�ɂ���C���^�t�F�[�X�������Ƃ���R���X�g���N�^�B
 * - IClone�������Ƃ���R���X�g���N�^�B
 */
#define BEGIN_CONSTRUCTOR(c, b) \
	typedef c class_; \
	typedef b base_; \
	class_() {} \
	class_(b::Interface* pInterface) : base_(pInterface) {} \
	class_(IClone* ipClone) : base_(ipClone) {}

/**
 * @brief ���������̂܂܃X�}�[�g�|�C���^�֓n���}�N��
 */
#define THROUGH(i) class_(i pInterface) : base_(pInterface) {}

/**
 * @brief �������^�ϊ����ăX�}�[�g�|�C���^�֓n���}�N���iAddRef����j
 */
#define CAST_THROUGH(i, c) class_(i pInterface) : base_(c(pInterface)) {}

/**
 * @brief �������^�ϊ����ăX�}�[�g�|�C���^�֓n���}�N���iAddRef�Ȃ��j
 */
#define CAST_ATTACH(i, c) class_(i pInterface) : base_(pInterface ? c(pInterface) : 0, false) {}

/**
 * @brief �����o�ϐ����X�}�[�g�|�C���^�֓n���}�N���iAddRef�Ȃ��j
 */
#define DATA_ATTACH(i) CAST_ATTACH(i, getData<base_::Interface*>)

#define CAST_DATA_ATTACH(i, c) class_(i pInterface) : base_(castToData<base_::Interface*>(c(pInterface)), false) {}

// ��`�J�n
/////////////////////////////////////////////////////////////////////////////

/// IFeatureLayerPtr�ւ̌^�ϊ���⍲����N���X
class FeatureLayerPtr_ : public IFeatureLayerPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(FeatureLayerPtr_, IFeatureLayerPtr)
	THROUGH(ILayer*)
};

typedef InterfacePtr_<FeatureLayerPtr_> FeatureLayerPtr;

/// ITablePtr�ւ̌^�ϊ���⍲����N���X
class TablePtr_ : public ITablePtr
{
public:
// �\�z
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

/// IFeatureClassPtr�ւ̌^�ϊ���⍲����N���X
class FeatureClassPtr_ : public IFeatureClassPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(FeatureClassPtr_, IFeatureClassPtr)
	THROUGH(ITable*)
	THROUGH(IDataset*)
	DATA_ATTACH(IFeatureLayer*)
	CAST_DATA_ATTACH(ILayer*, FeatureLayerPtr_)
};

typedef InterfacePtr_<FeatureClassPtr_> FeatureClassPtr;

/// IDatasetPtr�ւ̌^�ϊ���⍲����N���X
class DatasetPtr_ : public IDatasetPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(DatasetPtr_, IDatasetPtr)
	THROUGH(ITable*)
	THROUGH(IFeatureClass*)
	THROUGH(ILayer*)
	CAST_THROUGH(_IRow*, TablePtr_)
	CAST_THROUGH(IFeature*, TablePtr_)
};

typedef InterfacePtr_<DatasetPtr_> DatasetPtr;

/// IWorkspacePtr�ւ̌^�ϊ���⍲����N���X
class WorkspacePtr_ : public IWorkspacePtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(WorkspacePtr_, IWorkspacePtr)
	THROUGH(IFeatureWorkspace*)
	DATA_ATTACH(IDataset*)
	CAST_DATA_ATTACH(ITable*, DatasetPtr_)
	CAST_DATA_ATTACH(IFeatureClass*, DatasetPtr_)
};

typedef InterfacePtr_<WorkspacePtr_> WorkspacePtr;

/// IFeatureWorkspacePtr�ւ̌^�ϊ���⍲����N���X
class FeatureWorkspacePtr_ : public IFeatureWorkspacePtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(FeatureWorkspacePtr_, IFeatureWorkspacePtr)
	THROUGH(IWorkspace*)
	CAST_THROUGH(ITable*, WorkspacePtr);
	CAST_THROUGH(IFeatureClass*, WorkspacePtr);
	CAST_THROUGH(IDataset*, WorkspacePtr);
};

typedef InterfacePtr_<FeatureWorkspacePtr_> FeatureWorkspacePtr;

/// IWorkspaceDomainsPtr�ւ̌^�ϊ���⍲����N���X
class WorkspaceDomainsPtr_ : public IWorkspaceDomainsPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(WorkspaceDomainsPtr_, IWorkspaceDomainsPtr)
	THROUGH(IWorkspace*)
	THROUGH(IFeatureWorkspace*)
	CAST_THROUGH(ITable*, WorkspacePtr);
	CAST_THROUGH(IFeatureClass*, WorkspacePtr);
	CAST_THROUGH(IDataset*, WorkspacePtr);
};

typedef InterfacePtr_<WorkspaceDomainsPtr_> WorkspaceDomainsPtr;

/// IFieldsPtr�ւ̌^�ϊ���⍲����N���X
class FieldsPtr_ : public IFieldsPtr
{
public:
// �\�z
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

/// IDomainPtr�ւ̌^�ϊ���⍲����N���X
class DomainPtr_ : public IDomainPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(DomainPtr_, IDomainPtr)
	THROUGH(ICodedValueDomain*)
	DATA_ATTACH(IField*)
};

typedef InterfacePtr_<DomainPtr_> DomainPtr;

/// ICodedValueDomainPtr�ւ̌^�ϊ���⍲����N���X
class CodedValueDomainPtr_ : public ICodedValueDomainPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(CodedValueDomainPtr_, ICodedValueDomainPtr)
	THROUGH(IDomain*)
	CAST_THROUGH(IField*, DomainPtr)
};

typedef InterfacePtr_<CodedValueDomainPtr_> CodedValueDomainPtr;

/// _IGeometryPtr�ւ̌^�ϊ���⍲����N���X
class GeometryPtr_ : public IGeometryPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(GeometryPtr_, IGeometryPtr)
	THROUGH(IPointCollection*)
	THROUGH(IRelationalOperator*)
	DATA_ATTACH(IFeature*)
};

typedef InterfacePtr_<GeometryPtr_> GeometryPtr;

/// IEnvelopePtr�ւ̌^�ϊ���⍲����N���X
class EnvelopePtr_ : public IEnvelopePtr
{
public:
// �\�z
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

/// IGeometryDefPtr�ւ̌^�ϊ���⍲����N���X
class GeometryDefPtr_ : public IGeometryDefPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(GeometryDefPtr_, IGeometryDefPtr)
	DATA_ATTACH(IField*)
};

typedef InterfacePtr_<GeometryDefPtr_> GeometryDefPtr;

/// IGeometryDefEditPtr�ւ̌^�ϊ���⍲����N���X
class GeometryDefEditPtr_ : public IGeometryDefEditPtr
{
public:
// �\�z
	BEGIN_CONSTRUCTOR(GeometryDefEditPtr_, IGeometryDefEditPtr)
	THROUGH(IGeometryDef*)
	CAST_THROUGH(IField*, GeometryDefPtr)
};

typedef InterfacePtr_<GeometryDefEditPtr_> GeometryDefEditPtr;

/////////////////////////////////////////////////////////////////////////////
// ��`�I��

#undef BEGIN_CONSTRUCTOR
#undef THROUGH
#undef CAST_THROUGH
#undef CAST_ATTACH
#undef DATA_ATTACH

} // namespace sindy

#endif // __SINDY_IMPLICIT_CAST_H__
