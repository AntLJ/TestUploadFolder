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

/**
 * @file Map.h
 * @brief <b>CMap��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G\n
 * @version $Id$
 */
#ifndef _MAP_H_
#define _MAP_H_

#include <atl2/seal.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AheBugEvasion.h"

/**
 * @brief ���C���̎��
 * LayerIterator��CMapLayers�Ŏg�p���܂�
 */
enum iterator_type
{
	LayerTypeAny,             //!< �S�Ẵ��C��
	LayerTypeFeatureLayer,    //!< �t�B�[�`�����C��
	LayerTypeGraphicsLayer,   //!< �O���t�B�b�N�X�i���X�^�Ȃǁj���C��
	LayerTypeGroupLayer,      //!< �O���[�v���C��
	LayerTypeDataLayer,
};

/**
 * @class LayerIterator
 * @brief ���C���C�e���[�^�N���X
 * CMapLayers�Ŏg�p���܂�
 */
class LayerIterator
{
public:
	LayerIterator() : editableonly_(false), visibleonly_(false)
	{
	}
	LayerIterator( IMap* map__, iterator_type type__ = LayerTypeAny, bool editableonly__ = false, bool visibleonly__ = false )
	{
		if( ! map__ )
			return;

		maplayers_ = map__;
		editableonly_ = editableonly__;
		visibleonly_ = visibleonly__;

		IUIDPtr ipUID( CLSID_UID );
		switch( type__ )
		{
			case LayerTypeAny:
				ipUID->put_Value( CComVariant(L"{34c20002-4d3c-11d0-92d8-00805f7c28b0}") ); // ILayer
				break;
			case LayerTypeFeatureLayer:
				ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
				break;
			case LayerTypeGraphicsLayer:
				ipUID->put_Value( CComVariant(L"{34B2EF81-F4AC-11D1-A245-080009B6F22B}") ); // IGraphicsLayer
				break;
			case LayerTypeGroupLayer:
				ipUID->put_Value( CComVariant(L"{EDAD6644-1810-11D1-86AE-0000F8751720}") ); // IGroupLayer
				break;
			case LayerTypeDataLayer:
				ipUID->put_Value( CComVariant(L"{6CA416B1-E160-11D2-9F4E-00C04F6BC78E}") ); // IDataLayer
			default:
				_ASSERTE( false ); // ���̂��g�p�������ꍇ�͒ǉ����Ă�������
				ipUID->put_Value( CComVariant(L"{34c20002-4d3c-11d0-92d8-00805f7c28b0}") ); // ILayer
				break;
		}
		if( SUCCEEDED( map__->get_Layers( ipUID, VARIANT_TRUE, &layers_ ) ) && layers_ != NULL )
			layers_->Reset();

		operator++();
	}
	virtual ~LayerIterator()
	{
		layers_ = NULL;
	}
	const LayerIterator& operator++() const
	{
		if( ! is_end() )
		{
			layers_->Next( &current_layer_ );

			if( current_layer_ != NULL )
			{
				if( visibleonly_ && ( ! is_visible( maplayers_, current_layer_ ) ) )   // �\������Ă��邩�ǂ����̔���
					operator++();

				if( editableonly_ && ( ! is_editable( current_layer_ ) ) ) // �ҏW�\���ǂ����̔���
					operator++();
			}
		}

		if( current_layer_ == NULL )
			layers_ = NULL;

		return *this;
	}
	ILayerPtr operator->() const { return current_layer_; }
	ILayerPtr operator*() const { return current_layer_; }
	ITablePtr getTable() const
	{
		IFeatureLayerPtr flayer__( current_layer_ );
		if( flayer__ )
		{
			IFeatureClassPtr class__;
			flayer__->get_FeatureClass( &class__ );
			return (ITablePtr)class__;
		}
		return NULL;
	}
	bool operator==( const LayerIterator& iterator__ ) const
	{
		if( (IUnknown*)layers_ == (IUnknown*)iterator__.layers_ &&
			(IUnknown*)current_layer_ == (IUnknown*)iterator__.current_layer_ )
			return true;
		else
			return false;
	}
	bool operator!=( const LayerIterator& iterator__ ) const
	{
		return ! ( *this == iterator__ );
	}
	static bool is_visible( IMapLayers* maplayers__, ILayer* layer__ )
	{
		if( maplayers__ == NULL || ( ! layer__ ) )
			return false;

		VARIANT_BOOL vb = VARIANT_FALSE;
		maplayers__->IsLayerVisible( layer__, &vb );
		return ( vb ) ? true : false;
	}
	static bool is_editable( ILayer* layer__ )
	{
		if( ! layer__ )
			return false;

		IFeatureLayerPtr featlayer__( layer__ );
		if( featlayer__ != NULL )
		{
			IFeatureClassPtr featclass__;
			featlayer__->get_FeatureClass( &featclass__ );
			IDatasetPtr dataset__( featclass__ );
			if( dataset__ != NULL )
			{
				IWorkspacePtr workspace__;
				IDataset_getWorkspace( dataset__, &workspace__ ); // �Ԃт����������b�p�[�Ŏ擾
				IWorkspaceEditPtr editspace__( workspace__ );
				if( editspace__ != NULL )
				{
					VARIANT_BOOL vb = VARIANT_FALSE;
					editspace__->IsBeingEdited( &vb );
					return ( vb ) ? true : false;
				}
			}
		}
		return false;
	}
private:
	bool is_end() const
	{
		if( layers_ == NULL )
			return true;
		else
			return false;
	}
private:
	IMapLayersPtr maplayers_;
	mutable IEnumLayerPtr layers_;
	mutable ILayerPtr current_layer_;
	bool editableonly_;
	bool visibleonly_;
};

/**
 * @class CMap
 * @brief <b>IMap���b�v�N���X</b>\n
 *
 * CMapLayers cLayers( ipMap );
 * for( CMapLayers::iterator it = cLayers.begin(); it != cLayers.end(); ++it )
 * {
 *     ...
 * }
 * �݂����Ɏg���Ă��������B
 * begin()�ň�����^���邱�ƂŁA�Ⴆ�΃t�B�[�`�����C���݂̂Ƃ��A�ҏW�\
 * �݂̂Ƃ��\������Ă�����݂̂̂Ƃ��ɍi�荞��Ŏ擾���邱�Ƃ��\�ł��B
 * �����������w�肵�Ȃ���΁A�S�Ẵ��C�������ԂɎ擾����܂��B
 * ���K�w�O���[�v���C���ɑΉ����Ă��܂��B
 */
 class CMap : public atl2::seal<CComPtr<IMap> >
{
public:
	CMap()
	{
	}

	virtual ~CMap()
	{
	}

	CMap( IMap* lp )
	{
		*this = lp;
	}
	CMap& operator=( IMap* lp )
	{
        if(*this!=lp)
    		AtlComPtrAssign((IUnknown**)&p, lp);

        return *this;
	}

	//@{ @name IMap �C���^�[�t�F�[�X�i�ꕔ�j
	ILayerPtr GetLayer( long index ) const;
	long GetLayerCount() const;
	IEnumLayerPtr GetLayers( IUID* UID, bool recursive ) const;
	double GetMapScale() const;
	//@}
	/**
	 * @brief <b>�t�B�[�`�����C���݂̂̃��C���Z�b�g��Ԃ�</b>\n
	 * ������GetLayers���g�p���܂��B
	 * @param recursive	[in]	�O���[�v���C�������Ώۂɓ����ꍇ��true
	 * @retval NULL	�G���[���̓t�B�[�`�����C�������݂��Ȃ�
	 * @retval NULL�ȊO IEnumLayerPtr
	 */
	IEnumLayerPtr GetFeatureLayers( bool recursive ) const;

	/**
	 * @brief <b>TOC��̃O���[�v���C����S�Ď擾����</b>\n
	 * @note ������GetLayers���g�p���܂��B
	 * @param recursive	[in]	�O���[�v���C�������Ώۂɓ����ꍇ��true
	 * @retval NULL	�G���[���̓O���[�v���C�������݂��Ȃ�
	 * @retval NULL�ȊO IEnumLayerPtr
	 */
	IEnumLayerPtr GetGroupLayers( bool recursive ) const;

	/**
	 * @brief <b>�����ɓn�������C������������O���[�v���C�����擾����</b>\n
	 * @note ������GetGroupLayers���g�p���܂��B
	 * @param ipLayer [in] �O���[�v���C�����擾���郌�C��
	 * @retval NULL	�G���[���̓t�B�[�`�����C�������݂��Ȃ�
	 * @retval NULL�ȊO ICompositeLayerPtr
	 */
	ICompositeLayerPtr GetParentCompositeLayer( ILayerPtr ipLayer ) const;

#if 0
	/**
	 * @brief <b>���C������������</b>\n
	 * �e�[�u�����ȊO��NULL�ɂ���ƁA���̕����͈�v����Ɏg�p���܂���B
	 * lpcszServerName		[in]	�T�[�o��
	 * lpcszVersionName		[in]	�o�[�W������
	 * lpcszOwnerName		[in]	�I�[�i�[��
	 * lpcszTableName		[in]	�e�[�u����
	 * @retval NULL	�G���[���͌�����Ȃ�
	 * @retval NULL�ȊO ILayerPtr
	 */
	ILayerPtr FindLayer( LPCTSTR lpcszServerName, LPCTSTR lpcszVersionName, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>���C������������</b>\n
	 * �e�[�u�����ȊO��NULL�ɂ���ƁA���̕����͈�v����Ɏg�p���܂���B
	 * lpcszServerName		[in]	�T�[�o��
	 * lpcszVersionName		[in]	�o�[�W������
	 * lpcszOwnerTableName	[in]	�I�[�i�[.�T�[�o��
	 * @retval NULL	�G���[���͌�����Ȃ�
	 * @retval NULL�ȊO ILayerPtr
	 */
	ILayerPtr FindLayer( LPCTSTR lpcszServerName, LPCTSTR lpcszVersionName, LPCTSTR lpcszOwnerTableName ) const;
#endif // if 0
	/**
	 * @brief <b>���C������������</b>\n
	 * �e�[�u�����ȊO��NULL�ɂ���ƁA���̕����͈�v����Ɏg�p���܂���B
	 * @param ipWorkspace    [in] ���[�N�X�y�[�X
	 * @param lpcszOwnerName [in] �I�[�i�[��
	 * @param lpcszTableName [in] �e�[�u����
	 * @param first_only     [in,optional] �ŏ���1����������return����i�f�t�H���g�Ffalse�j
	 * @return ��������ILayerPtr�̃��X�g
	 */
	std::list<CAdapt<ILayerPtr>> FindLayers( const IWorkspacePtr& ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool first_only = false ) const;

	/**
	 * @brief <b>���C������������</b>\n
	 * �e�[�u�����ȊO��NULL�ɂ���ƁA���̕����͈�v����Ɏg�p���܂���B
	 * �������݂���ꍇ�͍ŏ��Ɍ��������C����Ԃ��܂�
	 * @param ipWorkspace			[in]	���[�N�X�y�[�X
	 * @param lpcszOwnerName		[in]	�I�[�i�[��
	 * @param lpcszTableName		[in]	�e�[�u����
	 * @retval NULL	�G���[���͌�����Ȃ�
	 * @retval NULL�ȊO ILayerPtr
	 */
	ILayerPtr FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>���C������������</b>\n
	 * �e�[�u�����ȊO��NULL�ɂ���ƁA���̕����͈�v����Ɏg�p���܂���B
	 * �������݂���ꍇ�͍ŏ��Ɍ��������C����Ԃ��܂�
	 * @param ipWorkspace			[in]	���[�N�X�y�[�X
	 * @param lpcszOwnerTableName	[in]	�I�[�i�[.�e�[�u����
	 * @retval NULL	�G���[���͌�����Ȃ�
	 * @retval NULL�ȊO ILayerPtr
	 */
	ILayerPtr FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerTableName ) const;

	/**
	 * @brief <b>�t�B�[�`���N���X�ɑΉ����郌�C������������</b>\n
	 * @note �t�B�[�`���N���X�ɑΉ����郌�C�����������݂���ꍇ��
	 * ��Ԏn�߂Ɍ����������̂�Ԃ��B
	 * @param ipFeatureClass [in] IFeatureClass�C���X�^���X
	 * @retval NULL	�G���[���͌�����Ȃ�
	 * @retval NULL�ȊO �w�肵���t�B�[�`���N���X�ɑΉ����郌�C��
	 */
	ILayerPtr FindLayer( const IFeatureClassPtr ipFeatureClass ) const;

	/**
	 * @brief <b>�w�背�C���Ɠ����O���[�v�ɏ������郌�C����S�Č�������</b>\n
	 * @detail �����O���[�v���Ȃ����̓��m�͓����O���[�v�ɏ���������̂Ƃ��Ĉ���
	 * @param ipUnknown [in] IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
	 * @param lpcszOTableName [in] ���C���̃e�[�u����
	 * @return ��������ILayerPtr�̃��X�g
	 */
	std::list<CAdapt<ILayerPtr>> FindSameGroupLayers( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>�w�背�C���Ɠ����O���[�v�ɏ������郌�C������������</b>\n
	 * @detail �����O���[�v���Ȃ����̓��m�͓����O���[�v�ɏ���������̂Ƃ��Ĉ����B<br>
	 * �w��̃e�[�u�����������C������������ꍇ�A��ԏ��߂Ɍ����������C����Ԃ��B
	 * @param ipUnknown [in] IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
	 * @param lpcszOTableName [in] ���C���̃e�[�u����
	 * @retval NULL	�G���[���͌�����Ȃ�
	 * @retval NULL�ȊO ILayerPtr
	 */
	ILayerPtr FindSameGroupLayer( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>�O���[�v���C�����烌�C����S�Č�������</b>\n
	 * @param ipTargetComposite  [in] �����Ώۂ̃O���[�v���C��
	 * @param ipWorkspace        [in] ���[�N�X�y�[�X
	 * @param lpcszOwnerName     [in] �I�[�i�[��
	 * @param lpcszOTableName    [in] ��������e�[�u����
	 * @param bOnlyDirectlyUnder [in,optional] �O���[�v���C���̒����̂݌������邩�ǂ����i�f�t�H���g�Ffalse�j
	 * @return ��������ILayerPtr�̃��X�g
	 */
	static std::list<CAdapt<ILayerPtr>> FindLayersFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder = false );

	/**
	 * @brief <b>�O���[�v���C�����烌�C������������</b>\n
	 * @note �����O���[�v�ɕ����̓����t�B�[�`���N���X���������ꍇ��
	 * ��ԍŏ��Ɍ����������̂�Ԃ�
	 * @param ipTargetComposite  [in] �����Ώۂ̃O���[�v���C��
	 * @param ipWorkspace        [in] ���[�N�X�y�[�X
	 * @param lpcszOwnerName     [in] �I�[�i�[��
	 * @param lpcszOTableName    [in] ��������e�[�u����
	 * @param bOnlyDirectlyUnder [in,optional] �O���[�v���C���̒����̂݌������邩�ǂ����i�f�t�H���g�Ffalse�j
	 * @retval NULL	�G���[���͌�����Ȃ�
	 * @retval NULL�ȊO ILayerPtr
	 */
	static ILayerPtr FindLayerFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder = false);

public:
	//@{ @name �C�e���[�^
	typedef LayerIterator iterator;
	typedef const LayerIterator const_iterator;
	const_iterator begin( iterator_type type = LayerTypeAny, bool editableonly = false, bool visibleonly = false ) const { return LayerIterator( p, type, editableonly, visibleonly ); }
	iterator begin( iterator_type type = LayerTypeAny, bool editableonly = false, bool visibleonly = false ) { return LayerIterator( p, type, editableonly, visibleonly ); }
	const_iterator end() const { return LayerIterator(); }
	iterator end() { return LayerIterator(); }
	//@}
};

#endif // ifndef _MAP_H_
