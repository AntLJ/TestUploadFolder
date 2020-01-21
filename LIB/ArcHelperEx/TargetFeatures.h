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
* @file TargetFeatures.h
*
* @brief �t�B�[�`�������X�g�Ǘ�����N���X�Q�̒�`�t�@�C��
*/
#if	!defined( __TARGETFEATURES_H__)
#define	__TARGETFEATURES_H__	//!< �d���C���N���[�h�h�~

#ifdef DEBUG
#include "AheDraw.h"
#endif // ifdef DEBUG

#include "GlobalFunctions.h"
#include "TargetFeature.h"
#include "AheMeshUtil.h"
#include "ErrObj.h"
#include "AheNetwork.h"
#include "ErrorMsg.h"

class CTargetItem;

/**
* @class CTargetItems
*
* @brief ����Ώۃt�B�[�`���Ǘ��R���e�i
*
* @note ����Ώۂ����X�g�Ǘ����A�����I�ȑ�����
*/
template <class T = CTargetItem>
class CTargetItemsT : public std::list<T>
{
public:
	CTargetItemsT<T>()
	{
	}
	virtual ~CTargetItemsT<T>(){};

	/**
	* @brief �R�s�[���Z�q
	*
	* @param cTargetItems [in] �R�s�[��
	*
	* @return CTargetItems& ���g�ւ̎Q��
	*/
	template <class TT>
	const CTargetItemsT<T>& operator=( const CTargetItemsT<TT>& cTargetItems )
	{
		clear();
		return CTargetItemsT<T>::operator +=( cTargetItems );
	}

	/**
	* @brief �ǉ����Z�q
	*
	* @param cTargetItems [in] �ǉ��t�B�[�`���Q
	*
	* @return CTargetItems& ���g�ւ̎Q��
	*/
	template <class TT>
	inline const CTargetItemsT<T>& operator+=( const CTargetItemsT<TT>& cTargetItems )
	{
		for( CTargetItemsT<TT>::const_iterator it = cTargetItems.begin(); it != cTargetItems.end(); ++it)
		{
			CTargetItemsT<T>::push_back( (*it));
		}

		return *this;
	}

	/**
	* @brief �폜���Z�q
	*
	* @param cTargetItems [in] �폜�t�B�[�`���Q
	*
	* @return CTargetItems& ���g�ւ̎Q��
	*/
	template <class TT>
	inline const CTargetItemsT<T>& operator-=( const CTargetItemsT<TT>& cTargetItems )
	{
		for( CTargetItemsT<TT>::const_iterator it = cTargetItems.begin(); it != cTargetItems.end(); ++it)
		{
			CTargetItemsT<T>::iterator it2 = begin();
			while( it2 != end() )
			{
				if( *it == *it2 )
					it2 = erase( it2 );
				else
					it2++;
			}
		}

		return *this;
	}

	template <class TT>
	inline const CTargetItemsT<T>& operator+( const CTargetItemsT<TT>& cTargetItems )
	{
		return CTargetItemsT<T>::operator +=( cTargetItems );
	}
	template <class TT>
	inline const CTargetItemsT<T>& operator-( const CTargetItemsT<TT>& cTargetItems )
	{
		return CTargetItemsT<T>::operator -=( cTargetItems );
	}
	/**
	* @brief ����ւ̗v�f��ǉ�
	*
	* @note �d���}�����֎~���Ă���B
	*
	* @param cItem			[in]			�ǉ�����v�f
	*/
	inline void push_back( const T& cItem )
	{
		if( cItem.GetFeature() == NULL )	// ��t�B�[�`���h�~
		{
			_ASSERTE( cItem.GetFeature() != NULL );
			return;
		}

		if( IsAlreadyExist( cItem ) ) 
			return;
		
		list<T>::push_back( cItem);
	}

	/**
	* @brief ����ւ̗v�f��ǉ�
	*
	* @note �d���}�����֎~���Ă���B
	*
	* @param ipFeature		[in]			�ǉ�����v�f���\������t�B�[�`��
	*/
	inline void push_back( IFeaturePtr ipFeature )
	{
		if( ipFeature == NULL )	// ��t�B�[�`���h�~
		{
			_ASSERTE( ipFeature != NULL );
			return;
		}

		T cItem( ipFeature );
		CTargetItemsT<T>::push_back( cItem );
	}

	/**
	* @brief �擪�ւ̗v�f��ǉ�
	*
	* @note �d���}�����֎~���Ă���B
	*
	* @param cItem			[in]			�ǉ�����v�f
	*/
	inline void push_front( const T& cItem )
	{
		if( cItem.GetFeature() == NULL )	// ��t�B�[�`���h�~
		{
			_ASSERTE( cItem.GetFeature() != NULL );
			return;
		}

		if( IsAlreadyExist( cItem ) )
			return;

		list<T>::push_front( cItem );
	}
	/**
	* @brief �擪�ւ̗v�f��ǉ�
	*
	* @note �d���}�����֎~���Ă���B
	*
	* @param ipFeature		[in]			�ǉ�����v�f���\������t�B�[�`��
	*/
	void push_front( IFeaturePtr ipFeature )
	{
		if( ipFeature == NULL )	// ��t�B�[�`���h�~
		{
			_ASSERTE( ipFeature != NULL );
			return;
		}

		T cItem( ipFeature );
		CTargetItems::push_front( cItem );
	}
	/**
	* @brief �S�Ǘ��Ώۂ��o�b�t�@�����A���������|���S�����쐬
	*
	* @note �|�C���g���o�b�t�@�������̂́A���̌`����o�b�t�@�������̂ƍ����ł��Ȃ��悤�Ȃ̂ŁA���̊֐��ł̓|�C���g��Ώۂ��珜�O���Ă���B
	*
	* @param dbBufferSize [in] �o�b�t�@������T�C�Y(map unit�l
	* @param ppiPolygon [out] ���������|���S��
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL CreatePolygon( double dbBufferSize, IPolygon** ppiPolygon )
	{
		IPolygonPtr piMaster;
		IPolygonPtr piSlave;

		ITopologicalOperatorPtr piPolyTopo;
		IGeometryPtr piGeomMaster;
		IGeometryPtr piGeomSlave;
		IFeaturePtr piFeature;

		for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++)
		{
			if( esriGeometryPoint == it->GetGeometryType())
			{
				// �|�C���g���o�b�t�@�������̂́A���̌`����o�b�t�@�������̂ƃ��j�I���ł��Ȃ��悤�ł���B
				continue;
			}

			if( (*it).CreatePolygon( dbBufferSize, &piSlave))
			{
				if( NULL != piMaster)
				{
					piGeomMaster = piMaster;
					piPolyTopo = piGeomMaster;
					piGeomSlave = piSlave;
					HRESULT hr = piPolyTopo->Union( piGeomSlave, &piGeomMaster);
					if( SUCCEEDED( hr))
					{
						piMaster = piGeomMaster;
					}
				}
				else
				{
					piMaster = piSlave;
				}
			}
		}

		(*ppiPolygon) = piMaster;
		(*ppiPolygon)->AddRef();

		return ( NULL != piMaster);
	}

	/**
	* @brief �S�Ǘ��Ώۂ��܂����`�̈�̎擾
	*
	* @retval IEnvelopePtr ��܂����`�̈�
	* @retval NULL �s���I��
	*/
	IEnvelopePtr GetEnvelope()
	{
		IGeometryPtr piGeom;
		IEnvelopePtr piEnvMaster;
		IEnvelopePtr piEnvSlave;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			piGeom = it->GetShape();
			if( piGeom != NULL )
			{
				piGeom->get_Envelope( &piEnvSlave);
				if( NULL != piEnvMaster)
				{
					piEnvMaster->Union( piEnvSlave);
				}
				else
				{
					piEnvMaster = piEnvSlave;
				}
			}
		}

		return piEnvMaster;
	}
	/**
	 * @brief �S�Ǘ��Ώۂ��|�����C���ɂ��� Union �����
	 *
	 * @note �|�C���g�͑ΏۂɂȂ�܂���
	 *
	 * @retval IPolylinePtr
	 */
	IPolylinePtr GetUnion()
	{
		using namespace libErrMsg;
		IGeometryPtr ipUnionGeom;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++ )
		{
			IGeometryPtr ipGeom;
			switch( it->GetGeometryType() )
			{
			case esriGeometryPolygon:
				ipGeom = AheConvertPolygon2Polyline( it->GetShape() );
				break;
			case esriGeometryPolyline:
				ipGeom = it->GetShapeCopy();
				break;
			default:
				break;
			}

			if( ipGeom != NULL ) 
			{
				if( ipUnionGeom == NULL )
					ipUnionGeom = ipGeom;
				else {
					// ���j�I������ɂ͋�ԎQ�Ƃ������Ă���K�v������
					ISpatialReferencePtr ipSpRef;
					if( AheHasSameSpatialReference( ipGeom, ipUnionGeom ) == FALSE )
					{
						ISpatialReferencePtr ipSpRef1, ipSpRef2;
						ipGeom->get_SpatialReference( &ipSpRef1 );
						ipUnionGeom->get_SpatialReference( &ipSpRef2 );
						ATLTRACE(_T("CTargetItems::GetUnion() : �W�I���g�����m�̋�ԎQ�Ƃ��قȂ�̂ŕЕ��̋�ԎQ�Ƃ��g�p���܂��B���̏ꍇ�A�Ӑ}�������ʂɂȂ�Ȃ��\��������܂��B\n") );
						CString strMsg;
						strMsg.Format(GETUNION_LAYER_OID, it->GetFeatureClassName(), it->GetOID() );
						AheTraceSpatialReference( ipSpRef1, strMsg );
						AheTraceSpatialReference( ipSpRef2, _T("CTargetItems::GetUnion() : ���j�I��") );
					}
					ipUnionGeom->get_SpatialReference( &ipSpRef );

					IGeometryPtr ipGeomTmp = AheUnion( ipGeom, ipUnionGeom, ipSpRef );
					if( ipGeomTmp == NULL )
					{
						ATLASSERT( ipGeomTmp == NULL );
						ATLTRACE(_T("CTargetItems::GetUnion() : ���j�I���Ɏ��s���܂����B\n") );
						AheTraceGeometry( ipGeom, _T("CTargetItems::GetUnion()") );
						AheTraceGeometry( ipUnionGeom, _T("CTargetItems::GetUnion()") );
					}
					else {
						//ITopologicalOperatorPtr ipTopo( ipGeomTmp );
						//if( ipTopo != NULL ) ipTopo->Simplify();	
						AheForceSimplify( ipGeomTmp ); // ���j�I�������� KnownSimple �� FALSE �ɂȂ�
					}

					ipUnionGeom = ipGeomTmp;
				}
			}
		}

		return ipUnionGeom;
	}

	/**
	 * @brief �l�b�g���[�N�����N�t�B�[�`���N���X���擾����
	 * �Ǘ��ꗗ����l�b�g���[�N���f���𔻒f���A�l�b�g���[�N�����N�t�B�[�`���N���X
	 * ���I�[�v�����ĕԂ��܂��B
	 */
	IFeatureClassPtr GetNetworkLinkFeatureClass() const
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			if( AheIsRelateWithNetwork( it->GetFeatureClassName() ) )
				return AheGetNetworkLinkFeatureClass( it->GetFeature() );
		}
		return NULL;
	}

	/**
	 * @brief �l�b�g���[�N�m�[�h�t�B�[�`���N���X���擾����
	 * @note  �Ǘ��ꗗ����l�b�g���[�N���f���𔻒f��
	 * ���߂Ɍ��������l�b�g���[�N�m�[�h�t�B�[�`���N���X���I�[�v�����ĕԂ��܂��B
	 */
	IFeatureClassPtr GetNetworkNodeFeatureClass() const
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			if( AheIsRelateWithNetwork( it->GetFeatureClassName() ) )
				return AheGetNetworkNodeFeatureClass( it->GetFeature() );
		}
		return NULL;
	}

	/**
	* @brief ���H�t�B�[�`���ꗗ�擾
	*
	* @note �Ǘ��ꗗ���ARoad_Link�e�[�u���̃t�B�[�`���̂ݎ擾
	*
	* @param cTargetFeatures	[out]			���H�ꗗ
	* @param bAdd				[in,optional]	�ǉ����邩�ǂ����i�f�t�H���g FALSE�j
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	template <class TT>
	BOOL GetRoadFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{	
		CString str;

		str.Format(_T("|%s|"), ROAD_TABLE_NAME );
		return GetFeatures( cTargetFeatures, str, bAdd );
	}

	/**
	* @brief �l�b�g���[�N�����N�t�B�[�`���ꗗ�擾
	*
	* @note �Ǘ��ꗗ���A�l�b�g���[�N�����N�e�[�u���̃t�B�[�`���̂ݎ擾
	*
	* @param cTargetFeatures	[out]			�l�b�g���[�N�����N�ꗗ
	* @param bAdd				[in,optional]	�ǉ����邩�ǂ����i�f�t�H���g FALSE�j
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	template <class TT>
	BOOL GetNetworkLinkFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{	
		return GetFeatures( cTargetFeatures, AheGetNetworkLinks(), bAdd );
	}

	/**
	* @brief �m�[�h�t�B�[�`���ꗗ�擾
	*
	* @note �Ǘ��ꗗ���ANode�e�[�u���̃t�B�[�`���̂ݎ擾
	*
	* @param cTargetFeatures	[out]			�m�[�h�ꗗ
	* @param bAdd				[in,optional]	�ǉ����邩�ǂ����i�f�t�H���g FALSE�j
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	template <class TT>
	BOOL GetNodeFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{
		CString str;

		str.Format(_T("|%s|"), NODE_TABLE_NAME );
		return GetFeatures( cTargetFeatures, str, bAdd );
	}

	/**
	* @brief �l�b�g���[�N�m�[�h�t�B�[�`���ꗗ�擾
	*
	* @note �Ǘ��ꗗ���A�l�b�g���[�N�m�[�h�e�[�u���̃t�B�[�`���̂ݎ擾
	*
	* @param cTargetFeatures	[out]			�l�b�g���[�N�m�[�h�ꗗ
	* @param bAdd				[in,optional]	�ǉ����邩�ǂ����i�f�t�H���g FALSE�j
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	template <class TT>
	BOOL GetNetworkNodeFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{	
		return GetFeatures( cTargetFeatures, AheGetNetworkNodes(), bAdd );
	}

	/**
	* @brief �w�背�C���̃t�B�[�`���ꗗ�擾
	*
	* @note �Ǘ��ꗗ���A�w��̃e�[�u�����̂̃t�B�[�`���̂ݎ擾
	*
	* @param cTargetItems		[out]			�w�背�C���̃t�B�[�`���ꗗ
	* @param lpcszLayerNames    [in]			�擾���郌�C�����Q("|ROAD_LINK|ROAD_NODE|"�̂悤�� | ��؂�ŒP�Ƃ��邢�͕����w�肷��)
	* @param bAdd				[in,optional]	�ǉ����邩�ǂ����i�f�t�H���g FALSE�j
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	template <class TT>
	BOOL GetFeatures( CTargetItemsT<TT>& cTargetItems, LPCTSTR lpcszLayerNames, BOOL bAdd = FALSE)
	{
		list<CString> listLayerNames;
		AheSepStr2listString( lpcszLayerNames, _T("|"), listLayerNames );

		if( bAdd == FALSE ) cTargetItems.clear();
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )
		{
			CString strLayerName = it->GetFeatureClassName();
			for( list<CString>::iterator itNames = listLayerNames.begin(); itNames != listLayerNames.end(); itNames++ )
			{
				if( strLayerName.CompareNoCase( *itNames ) == 0 )
					cTargetItems.push_back( *it );
			}
		}
		return TRUE;
	}

	/**
	* @brief �w�背�C���̃t�B�[�`���ꗗ�擾
	*
	* @note �Ǘ��ꗗ���A�w��̃e�[�u�����̂̃t�B�[�`���̂ݎ擾
	*
	* @param cTargetItems		[out]			�w�背�C���̃t�B�[�`���ꗗ
	* @param lpcszLayerName     [in]			�w�背�C�����i | �͓���Ȃ����Ɓj
	* @param bAdd				[in,optional]	�ǉ����邩�ǂ����i�f�t�H���g FALSE�j
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	template <class TT>
	BOOL GetFeature( CTargetItemsT<TT>& cTargetItems, LPCTSTR lpcszLayerName, BOOL bAdd = FALSE)
	{
		CString layerName;
		layerName.Format(_T("|%s|"), lpcszLayerName);
		return GetFeatures( cTargetItems, layerName, bAdd );
	}

	/**
	* @brief �Ǘ��t�B�[�`���ꗗ�ɂQ�����b�V�����E�m�[�h���܂ށH
	*
	* @retval TRUE �܂�
	* @retval FALSE �܂܂Ȃ�
	*/
	BOOL IsIncludedBorderNode( void)
	{
		CTargetItemsT<T>::iterator it = begin();
		while( it != end())
		{
			if( it->IsBorderNode() || it->IsBorderPECNode() ) 
				return TRUE;

			++it;
		}
		return FALSE;
	}

	/**
	* @brief �Ǘ��t�B�[�`���ꗗ�Ƀl�b�g���[�N�֘A�̃t�B�[�`�����܂܂�邩�m�F
	*
	* @note �l�b�g���[�N�֘A�̃t�B�[�`�����ۂ��̔���ɂ�AheIsRelateWithNetwork���g�p
	*
	* @retval TRUE �܂�
	* @retval FALSE �܂܂Ȃ�
	*/
	BOOL IsIncludedNetworkItems( void)
	{
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			if( AheIsRelateWithNetwork( it->GetFeature() ) )
				return TRUE;
		}
		return FALSE;
	}
	
	/**
	* @brief �l�b�g���[�N���f���̃^�C�v���擾����
	*
	* @note �����̃^�C�v�����݂��Ă����ꍇ�� sindyeNotNetwork ��Ԃ�
	*
	* @retrn TRUE �l�b�g���[�N���f���^�C�v
	*/
	sindyeNetworkModel GetNetworkType()
	{
		if( CTargetItems::empty() )
			return sindyeNotNetwork;

		// �����̎�ނ����݂��Ă����� NotNetwork ��Ԃ�
		sindyeNetworkModel type = AheGetNetWorkType( CTargetItems::begin()->GetFeature() );
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			if( type != AheGetNetWorkType( it->GetFeature() ) )
			{
				type = sindyeNotNetwork;
				break;
			}
		}

		return type;
	}

	/**
	* @brief �Ǘ��t�B�[�`���ꗗ�ɑ��݂��邩�ǂ���
	*
	* @param item	[in]	��r�A�C�e��
	*
	* @retval TRUE ���݂���
	* @retval FALSE ���݂��Ȃ�
	*/
	template <class TT>
	BOOL IsAlreadyExist( const TT& item ) const
	{
		for( CTargetItemsT<T>::const_iterator it = begin(); it != end(); ++it )
		{
			if( *it == item ) return TRUE;
		}
		return FALSE;
	}

	/**
	* @brief �Ǘ��t�B�[�`���ꗗ�ɑ��݂��邩�ǂ���
	*
	* @param ipFeature	[in]	IFeaturePtr
	*
	* @retval TRUE ���݂���
	* @retval FALSE ���݂��Ȃ�
	*/
	BOOL IsAlreadyExist( IFeaturePtr ipFeature ) const
	{
		T item( ipFeature );
		return IsAlreadyExist( item );
	}

	/**
	* @brief �X�V���̐ݒ�
	*
	* @note �S�Ǘ��Ώۂ̍X�V�ݒ���Ăяo���āA�X�V����ݒ肷��B
	*
	* @param emUpdateType [in] �X�V�^�C�v
	* @param lpModifyTime [in] �X�V����
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL SetModifyData( sindyUpdateType emUpdateType, LPSYSTEMTIME lpModifyTime = NULL)
	{
		// �ύX�������w�肳��Ă��Ȃ��ꍇ�S�̂𓯈ꎞ�ԂŒ���
		// �}�V�����Ԃ̐������l����ƁA"SYSDATE"���w�肷��̂��őP�Ǝv���邪�ASDE��SYSDATE�𐳂��������Ȃ��悤�ł���B
		SYSTEMTIME stModifyTime;
		if( NULL == lpModifyTime)
		{
			::GetLocalTime( &stModifyTime);
		}
		else
		{
			CopyMemory( &stModifyTime, lpModifyTime, sizeof( SYSTEMTIME));
		}

		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			(*it).SetModifyData( emUpdateType, &stModifyTime);
		}
		return TRUE;
	}

	/**
	* @brief �Ǘ��Ώۈꗗ����A�w��̈ʒu�ɍł��߂�Item���擾
	*
	* @param piPoint �`�F�b�N�Ώۂ̃|�C���g
	* @param dbTolerance 臒l
	* @param pcItem �ł��߂��v�f
	* @param piHitPoint �q�b�g�ʒu
	* @param plVertexIndex 
	*
	* @retval esriGeometryPartNone �w�肵���G���A���Ńq�b�g���Ȃ�
	* @retval esriGeometryPartVertex ���_�Ƀq�b�g����
	* @retval esriGeometryPartBoundary �ӏ�Ƀq�b�g����
	*/
	esriGeometryHitPartType GetNearestItem( IPointPtr piPoint, double dbTolerance, CTargetItem* pcItem, IPoint** piHitPoint, long* plVertexIndex)
	{
		double dbHitDist;
		double dbMinDist = -1;

		// Results
		CTargetItemsT<T>::iterator itResultIt = end();
		esriGeometryHitPartType emResultHitPart = esriGeometryPartNone;
		IPointPtr piResultPoint;
		long lResultVertex;

		IPointPtr piRetPoint;
		long lRetVertex;
		esriGeometryHitPartType enRetHitPart;
		esriGeometryType geomType = esriGeometryNull;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )
		{
			esriGeometryType type = it->GetGeometryType();
			enRetHitPart = (*it).HitTest( piPoint, dbTolerance, &piRetPoint, &dbHitDist, NULL, &lRetVertex);
			if( esriGeometryPartNone != enRetHitPart)
			{
				DOUBLE dDelta = dbMinDist - dbHitDist;
				BOOL bIsNearest = FALSE;
				
				switch( type )
				{
					case esriGeometryPoint:
						switch( geomType )
						{
							// �|�C���g�̏ꍇ�ŁA���ێ�����Ă�����̂��|�C���g�̏ꍇ�͋����Ŕ�r
							case esriGeometryPoint:
								if( dDelta < dbTolerance && dDelta > 0.0 )
									bIsNearest = TRUE;
								break;
							// �ێ�����Ă�����̂��|�C���g�ȊO�̂��̂Ȃ�A�ⓚ���p
							default:
								bIsNearest = TRUE;
								break;
						}
						break;
					case esriGeometryPolyline:
					case esriGeometryPolygon:
						switch( geomType )
						{
							// �܂������ێ�����Ă��Ȃ��ꍇ�͖ⓚ���p
							case esriGeometryNull:
								bIsNearest = TRUE;
								break;
							// �|�����C���E�|���S���̏ꍇ�ŁA���ێ�����Ă�����̂��|�����C���E�|���S���̏ꍇ
							// �q�b�g�ʒu�������Ȃ狗���Ŕ�r
							// �q�b�g�ʒu���������Ȃ�A���Vertex�D��
							case esriGeometryPolyline:
							case esriGeometryPolygon:
								if( ( emResultHitPart == enRetHitPart && dDelta < dbTolerance && dDelta > 0.0 ) ||
									( emResultHitPart > enRetHitPart ) )
									bIsNearest = TRUE;
								break;
						}
						break;
					default:
						break;
				}

				if( bIsNearest == TRUE )
				{
					dbMinDist = dbHitDist;
					geomType = type;
					itResultIt = it;
					piResultPoint = piRetPoint;
					lResultVertex = lRetVertex;
					emResultHitPart = enRetHitPart;
				}
			}
		}

		if( end() != itResultIt)
		{
			if( NULL != pcItem)(*pcItem) = ( *itResultIt);
			if( NULL != piHitPoint)
			{
				(*piHitPoint) = piResultPoint;
				(*piHitPoint)->AddRef();
			}
			if( NULL != plVertexIndex)(*plVertexIndex) = lResultVertex;
		}
		return emResultHitPart;
	}

	/**
	* @brief �Ǘ��Ώۈꗗ����A�w��̈ʒu�ɍł��߂�Item�Q���擾
	*
	* @param piPoint �`�F�b�N�Ώۂ̃|�C���g
	* @param dbTolerance 臒l
	* @param pcItems �ł��߂��v�f�Q
	* @param piHitPoint �q�b�g�ʒu
	* @param plVertexIndex 
	* @param bIgnoreGeometryType [in,optional] �W�I���g���^�C�v�̗D�揇�ʂ𖳎����邩�ǂ����i�f�t�H���g�Ffalse�j
	*
	* @retval esriGeometryPartNone �w�肵���G���A���Ńq�b�g���Ȃ�
	* @retval esriGeometryPartVertex ���_�Ƀq�b�g����
	* @retval esriGeometryPartBoundary �ӏ�Ƀq�b�g����
	*/
	template <class TT>
	esriGeometryHitPartType GetNearestItems( IPointPtr piPoint, double dbTolerance, CTargetItemsT<TT>* pcItems, IPoint** piHitPoint, long* plVertexIndex, bool bIgnoreGeometryType = false )
	{
		if ( ! ( piPoint && pcItems ) )
			return esriGeometryPartNone;

		IPointPtr			 	piResultPoint, piRetPoint;
		double 					dbHitDist = 0.0f, dbMinDist = -1.0f;
		long 					lResultVertex = -1, lRetVertex = -1;
		esriGeometryHitPartType emResultHitPart = esriGeometryPartNone, enRetHitPart = esriGeometryPartNone;
		esriGeometryType 		geomType = esriGeometryNull;

		// ���X�g�N���A
		pcItems->clear();
		for ( iterator it = begin(); it != end(); it++ ) {
			enRetHitPart = (*it).HitTest( piPoint, dbTolerance, &piRetPoint, &dbHitDist, NULL, &lRetVertex);
			ATLTRACE( _T("%s\n"), AheGetFeatureClassName( it->GetFeature() ) );
			if ( esriGeometryPartNone != enRetHitPart) {
				// �����������Ȃ�ǉ�
				if ( dbMinDist == dbHitDist && ( bIgnoreGeometryType || geomType == it->GetGeometryType() ) )
					pcItems->push_back( *it );
				// �������Z������D��i�������A�|�C���g�ƕӂł͏�Ƀ|�C���g���D��j
				else if( ( ( ! bIgnoreGeometryType ) && geomType > it->GetGeometryType() ) || // TODO: ����������A�e�X�g�Ώۂ�2�����Ăǂ�������C�������C���̏ꍇ�A��ɍŏ��̂ق������̗p����Ȃ��̂ł́H�H���̊֐��{���ɑ��v�H�H
						 ( dbMinDist >= dbHitDist || -1 == dbMinDist ) &&
						 ( emResultHitPart == esriGeometryPartNone || enRetHitPart == esriGeometryPartVertex ) ) { // TODO: �Ȃ�ŁA�\���_�݂̂ɍi��K�v������H�H���ꂾ�ƁA���񂾂�����Boundary�ւ̃q�b�g��������Ȃ��B���̊֐��{���ɑ��v�H�H
					geomType = it->GetGeometryType();
					pcItems->clear();
					pcItems->push_back( *it );
					dbMinDist = dbHitDist;
					piResultPoint = piRetPoint;
					lResultVertex = lRetVertex;
					emResultHitPart = enRetHitPart;
				}
			}
		}

		if ( piHitPoint && piResultPoint ) {
			(*piHitPoint) = piResultPoint;
			(*piHitPoint)->AddRef();
		}

		if ( plVertexIndex )
			(*plVertexIndex) = lResultVertex;

		return emResultHitPart;
	}

	/**
	* @brief �Ǘ��Ώۈꗗ����A�ҏW�Ώۈꗗ���쐬
	*
	* @param piPoint �`�F�b�N�Ώۂ̃|�C���g
	* @param dbTolerance 臒l
	* @param ppiHitPont �q�b�g�����ʒu
	*
	* @return �I�����ꂽ�v�f��
	*/
	int SelectByPoint( IPointPtr piPoint, double dbTolerance, IPoint** ppiHitPont)
	{
		IPointPtr piHitPoint;
		if( esriGeometryPartNone != GetNearestItem( piPoint, dbTolerance, NULL, &piHitPoint, NULL))
		{
			if( NULL != ppiHitPont)
			{
				(*ppiHitPont) = piHitPoint;
				(*ppiHitPont)->AddRef();
			}

			CTargetItemsT<T>::iterator it = begin();
			while( it != end())
			{
				IGeometryPtr ipGeom( it->GetShapeCopy() );
				// HitTest�����{����
				// ���܂ł�IRelationalOperator���g�p���Ă������A�덷���傫������B
				// �������A�|�C���g�̏ꍇ��AheIsDBEqual�ɔ�΂���邽�߁A�|�C���g�Ƃ���ȊO��
				// ����Ɍ덷���g�傷��B
				// �����������̃��\�b�h��臒l��^�����Ă���̂ŁA�{��HitTest�Ŋm�F���ׂ�����
				IHitTestPtr ipHitTest( ipGeom );
				IPointPtr ipHit( AheCreateClone(piHitPoint) );
				double dHitDistance = 0.0;
				long lHitVertex = 0, lHitSegment = 0;
				VARIANT_BOOL vbRightSide = VARIANT_FALSE, vbHit = VARIANT_FALSE;
				// �܂��͍\���_�Ŋm�F
				ipHitTest->HitTest( piHitPoint, dbTolerance, esriGeometryPartVertex, ipHit, &dHitDistance, &lHitVertex, &lHitSegment, &vbRightSide, &vbHit );
				if( ! vbHit )
				{
					// �\���_�Ńq�b�g���Ȃ�������Boundary�Ŋm�F
					ipHitTest->HitTest( piHitPoint, dbTolerance, esriGeometryPartBoundary, ipHit, &dHitDistance, &lHitVertex, &lHitSegment, &vbRightSide, &vbHit );
					if( ! vbHit )
					{
						it = erase( it );
						continue;
					}
				}
				++it;
			}
		} // TODO: �ǂ��ɂ��q�b�g���Ȃ��ꍇ��clear����ׂ��ł́H�H���̊֐����{���ɕK�v���A�v�m�F
		return size();
	}

	/**
	* @brief �Ǘ��t�B�[�`���Q�̌`��`�F�b�N
	*
	* @param strErrMsg					[out]			�G���[���b�Z�[�W
	* @param bCheckSegmentOverlapOnly	[in,optional]	�Z�O�����g�d���`�F�b�N�݂̂̏ꍇ�� TRUE
	* @param ipCheckEnv					[in,optional]	�|���S���̏d���`�F�b�N���̌���G���A�i�f�t�H���g�FNULL�j
	* @param ipRule						[in,optional]	���[���I�u�W�F�N�g�i�f�t�H���g�FNULL�j
	*
	* @retval sindyeGeometryCheck
	*/
	sindyeGeometryCheck IsSafeGeometries( CString& strErrMsg, BOOL bCheckSegmentOverlapOnly = FALSE, IEnvelopePtr ipCheckEnv = NULL, ISiNDYRule3Ptr ipRule = NULL )
	{
		sindyeGeometryCheck emRet = sindyeGeometryNoCheck; // �߂�l

		CErrObjContainer errs;
		switch( IsSafeGeometries( errs, bCheckSegmentOverlapOnly, ipCheckEnv, ipRule ) )
		{
			case sindyeLogLevel_INFO: emRet = sindyeGeometryOK; break;
			case sindyeLogLevel_WARN: emRet = sindyeGeometryWarnning; break;
			case sindyeLogLevel_ERR:  emRet = sindyeGeometryError; break;
			default: break;
		}

		strErrMsg = errs.ToString();
		return emRet;
	}
	/**
	* @brief �Ǘ��t�B�[�`���Q�̌`��`�F�b�N
	*
	* @param strErrMsg					[out]			�G���[���b�Z�[�W
	* @param bCheckSegmentOverlapOnly	[in,optional]	�Z�O�����g�d���`�F�b�N�݂̂̏ꍇ�� TRUE
	* @param ipCheckEnv					[in,optional]	�|���S���̏d���`�F�b�N���̌���G���A�i�f�t�H���g�FNULL�j
	* @param ipRule						[in,optional]	���[���I�u�W�F�N�g�i�f�t�H���g�FNULL�j
	*
	* @retval sindyeLogLevel
	*/
	sindyeLogLevel IsSafeGeometries( CErrObjContainer& errs, BOOL bCheckSegmentOverlapOnly = FALSE, IEnvelopePtr ipCheckEnv = NULL, ISiNDYRule3Ptr ipRule = NULL )
	{
		using namespace sindy::schema;
		using namespace libErrMsg;
#ifdef DEBUG
		DWORD DWSTART = GetTickCount();
#endif // ifdef DEBUG

		CTargetItemsT<T>::iterator it, it2;
		CTargetItems cRoadItems;	// �ڑ��p�`�F�b�N����ł���̂ł���ɓ���Ă���
		CTargetItems cNodeItems;	// �ڑ��e�`�F�b�N���Ƀm�[�h�̎�ʂ��݂�̂ŕK�v
		BOOL bIsCheckAngle = ( bCheckSegmentOverlapOnly ) ? FALSE : TRUE;

		GetNetworkNodeFeatures( cNodeItems );	// �m�[�h�擾

		// �|���S���̏d���`�F�b�N�͎��Ԃ�������̂ŁA�G���A�w�肪����ꍇ�͕ʃo�[�W�����Ń`�F�b�N
		// �Z�O�����g�I�[�o�[���b�v�`�F�b�N�݂̂̎��̓X���[
		if( ! bCheckSegmentOverlapOnly )
		{
			if( ipCheckEnv )
				CheckPolygonOverlap( ipCheckEnv, errs );
		}
		// ���H�����N�����[�v���Ă��Ȃ����ǂ���
		// �Z�O�����g���I�[�o�[���b�v���Ă��Ȃ����ǂ����m�F�i�Ǘ��t�B�[�`�����̂݁j
		for( it = begin(); it != end(); ++it )
		{
			CComVariant vaFromID, vaToID;
			CString strItFeatureClassName = it->GetFeatureClassName();
			CString strItOwnerDotClassName = it->GetOwnerDotClassName();

			// FROM�ATO�擾�i���H�̂݁j
			if( AheIsNetworkLink( strItFeatureClassName ) )
			{
				cRoadItems.push_back( *it );	// �ێ�
				vaFromID = it->GetAttribute( FROM_FIELD );
				vaToID   = it->GetAttribute( TO_FIELD   );

				if( ( ! bCheckSegmentOverlapOnly ) && ( vaFromID.vt != VT_I4 || vaToID.vt != VT_I4 ) )
					errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_FROM_OR_TO_MISSING );
			}

			// �\���_�����݂ă`�F�b�N����E���Ȃ��𔻒f�i�������x�����߁c�j
			// �Z�O�����g�I�[�o�[���b�v�`�F�b�N�݂̂̎��̓X���[�����
			if ( bIsCheckAngle )
			{
				if( it->GetGeometryType() == esriGeometryPolygon && strItFeatureClassName.Compare( _T("PARKING_SHAPE") ) != 0 ) {
					LONG lPointCount = -1;
					IGeometryPtr ipGeom;
					it->GetFeature()->get_Shape( &ipGeom );
					((IPointCollectionPtr)ipGeom)->get_PointCount( &lPointCount );
					if( lPointCount > 10000 )	// [bug 4288]
					{
						CString strMsg;
						strMsg.Format(CONFIRM_CHECK_POLYGON_WITH_OVER_10000_POINT, strItFeatureClassName, it->GetOID(), lPointCount );
						if( AheMessageBox( ::GetActiveWindow(), strMsg, GEOMETRY_CHECK, MB_YESNO ) == IDNO )
							bIsCheckAngle = FALSE;
					}
				}
			}

			// �d���`�F�b�N�i�G���A�Ȃ��̏ꍇ�B�|���S���̂݁j
			// �Z�O�����g�I�[�o�[���b�v�`�F�b�N�݂̂̎��̓X���[
			if( ! bCheckSegmentOverlapOnly )
			{
				if( ! ipCheckEnv )
				{
					if( it->GetGeometryType() == esriGeometryPolygon &&
						strItFeatureClassName.Compare( _T("PARKING_SHAPE") ) != 0 &&
						!AheIsOverlapableLayer( it->GetFeatureClassName() ) )
					{
						IFeatureClassPtr ipFeatureClass( it->GetFeatureClass() );
						if( ipFeatureClass  )
						{
							CString strSubField;
							CComBSTR bstrOIDField, bstrShapeField;
							IGeometryPtr ipGeom;

							ipFeatureClass->get_OIDFieldName( &bstrOIDField );
							ipFeatureClass->get_ShapeFieldName( &bstrShapeField );
							strSubField.Format(_T("%s,%s"), COLE2CT(bstrOIDField), COLE2CT(bstrShapeField) );
							it->GetFeature()->get_Shape( &ipGeom );

							if( ! bIsCheckAngle )
								continue;

							IFeatureCursorPtr ipFeatCursor( AheSelectByShape( ipGeom, ipFeatureClass, esriSpatialRelIntersects, strSubField, _T(""), TRUE ) );
							if( ipFeatCursor )
							{
								IFeaturePtr ipFeat;
								while( SUCCEEDED( ipFeatCursor->NextFeature( &ipFeat ) ) )
								{
									if( ! ipFeat )
										break;

									CTargetItem item( ipFeat );
									LONG lOIDTmp = item.GetOID();

									if( it->GetOID() != lOIDTmp )
									{
										// �ړ��ΏیQ�Ɋ܂܂�Ă���ꍇ�͂��̌`��Ń`�F�b�N
										BOOL bIsOverlaps = FALSE;
										if( IsAlreadyExist( item ) )
										{
											// IsAlreadyExist�͔p�~���āAfind()��L�����p���ׂ��I�I
											iterator itItem = find( begin(), end(), item );
											_ASSERTE( itItem != end() );
											bIsOverlaps = AheIsOverlap2( ipGeom, itItem->GetShape() );
										}
										else
											bIsOverlaps = AheIsOverlap2( ipGeom, AheGetShape( ipFeat ) );

										if( bIsOverlaps )
											errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetAreaPoint( it->GetShape() ) ), -1, sindyeLogLevel_WARN, ERROR_DUPLICATE_OF_OID, lOIDTmp );
									}
								}
							}
						}
					}
				}
			}
			it2 = it;
			++it2;
			for( ; it2 != end(); it2++ )
			{
				if( strItFeatureClassName.Compare( it2->GetFeatureClassName() ) == 0 )
				{
					if( ( ! bCheckSegmentOverlapOnly ) && AheIsNetworkLink( strItFeatureClassName ) )
					{
						CComVariant vaFromID2 = it2->GetAttribute( FROM_FIELD );
						CComVariant vaToID2   = it2->GetAttribute( TO_FIELD );
						if( vaFromID2.vt != VT_I4 || vaFromID2.lVal < 1 || vaToID2.vt != VT_I4 || vaToID2.lVal < 1 )
						{
							errs.add( it2->GetOwnerDotClassName(), it2->GetOID(), it2->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it2->GetShape() ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_LINK_MUST_HAVE_FROM_OR_TO_ID );
						}

						// ���[�v�`�F�b�N
						if( ( vaFromID == vaFromID2 && vaToID == vaToID2 ) || ( vaFromID == vaToID2 && vaToID == vaFromID2 ) )
						{
							errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, ERROR_LINK_LOOPES, it->GetOID(), it2->GetOID() );
						}
					}
					// �Z�O�����g�I�[�o�[���b�v�`�F�b�N
					if(( AheIsNetworkLink( strItFeatureClassName ) ||	// �l�b�g���[�N�����N
						strItFeatureClassName.CompareNoCase( GRAD_TABLE_NAME ) == 0 ) &&	// �X��
						strItFeatureClassName.CompareNoCase( adam_v2::lane_link::kTableName ) != 0 )	// [ADAM] �Ԑ������N�͏���
					{
						if( AheIsOverlap2( it->GetShape(), it2->GetShape() ) )
						{
							errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, ERROR_SEGMENT_OVERLAP, it->GetOID(), it2->GetOID() );
						}
					}
					else if( strItFeatureClassName.CompareNoCase( SEGMENTATTR_TABLE_NAME ) == 0 )	// �����N������
					{
						// ��ʂ��ꏏ�̏ꍇ�̂݃G���[�Ƃ���
						if( AheIsOverlap2( it->GetShape(), it2->GetShape() ) && 
							it->GetAttribute( _T("LINKATTR_C") ) == it2->GetAttribute( _T("LINKATTR_C") ) )
						{
							errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, ERROR_SAME_TYPE_SEGMENT_OVERLAP, it->GetOID(), it2->GetOID() );
						}
					}
					if( ! bCheckSegmentOverlapOnly )
					{
						// �����^�]�x���}�b�v��2�����b�V���`�F�b�N���Ȃ�
						if( AheIsNetworkLink(strItFeatureClassName) && !AheIsAdamNetworkLink(strItFeatureClassName) )
						{
							// ���H�����N�̐ڑ��p�`�F�b�N/2�����b�V�����ł̐܂�Ԃ��`�F�b�N
							// �Z�O�����g�I�[�o�[���b�v�`�F�b�N�݂̂̎��̓X���[
							IGeometryPtr ipBaseCopy(it->GetShapeCopy()), ipTestCopy(it2->GetShapeCopy());
							IPointPtr ipTouchPoint;
							if( AheIsTouchEndPoint( ipBaseCopy, ipTestCopy, NULL, &ipTouchPoint ) ) 
							{
								// �ړ_�����b�V���ɐڂ��Ă��邩�ǂ���
								INT ix = 0, iy = 0; 
								DOUBLE dX = 0.0, dY = 0.0;
								ipTouchPoint->QueryCoords( &dX, &dY );
								unsigned int iMeshCode = AheLL2MESH( dX, dY, 2, ix, iy, 1 ); // bug 8919
								if( ix == 0 || iy == 0 )
								{
									// [bug 1870] ���b�V�������͂��񂾃����N��135���ȏ�̊p�x�����Ă���K�v������
									DOUBLE dAngle = AheGetAngle( it->GetShape(), it2->GetShape() );
									if( -135.0 <= dAngle && dAngle <= 135.0 )
										errs.add( it->GetOwnerDotClassName(), it->GetOID(), ipBaseCopy, -1, MakeWKSPoint( dX, dY ), -1, sindyeLogLevel_ERR, ERROR_ANGLE_IS_135_DEGREE_OR_LESS, it->GetOID(), it2->GetOID(), (int)dAngle );
									// �܂�Ԃ�
									IPointPtr ipCenterPoint1( CLSID_Point ), ipCenterPoint2( CLSID_Point );
									ipCenterPoint1->putref_SpatialReference( it->GetSpatialReference() ); // ��ԎQ�ƕt�^
									ipCenterPoint2->putref_SpatialReference( it2->GetSpatialReference() ); // ��ԎQ�ƕt�^
									DOUBLE dLen1 = 0, dLen2 = 0;
									DOUBLE dCenterX1 = 0, dCenterY1 = 0, dCenterX2 = 0, dCenterY2 = 0;

									((IPolylinePtr)ipBaseCopy)->get_Length( &dLen1 );
									((IPolylinePtr)ipTestCopy)->get_Length( &dLen2 );
									((IPolylinePtr)ipBaseCopy)->QueryPoint( esriNoExtension, dLen1/2.0, VARIANT_FALSE, ipCenterPoint1 );
									((IPolylinePtr)ipTestCopy)->QueryPoint( esriNoExtension, dLen2/2.0, VARIANT_FALSE, ipCenterPoint2 );
									ipCenterPoint1->QueryCoords( &dCenterX1, &dCenterY1 );
									ipCenterPoint2->QueryCoords( &dCenterX2, &dCenterY2 );

									int ixDummy = 0, iyDummy = 0;
									UINT iMeshCode1 = AheLL2MESH( dCenterX1, dCenterY1, 2, ixDummy, iyDummy );
									UINT iMeshCode2 = AheLL2MESH( dCenterX2, dCenterY2, 2, ixDummy, iyDummy );
									if( iMeshCode1 == iMeshCode2 )
										errs.add( it->GetOwnerDotClassName(), it->GetOID(), ipBaseCopy, -1, MakeWKSPoint( dX, dY ), -1, sindyeLogLevel_ERR, ERROR_ROAD_LINK_IS_FOLDED_MESH_BOUNDARY, it->GetOID(), it2->GetOID() );
								}
							}
							// [CHECK] [ROADCHECK 02320] �\���_���m���ڐG���Ă��邩�ǂ����ibug 8463�j
							IPointPtr ipTouch;
							if( AheIsTouchVertex( (IPointCollectionPtr)ipBaseCopy, (IPointCollectionPtr)ipTestCopy, ipTouch ) )
							{
								errs.add( it->GetOwnerDotClassName(), it->GetOID(), ipBaseCopy, -1, MakeWKSPoint( ipTouch ), -1, sindyeLogLevel_ERR, ERROR_POINTS_ARE_TOUCHING, it->GetOID(), it2->GetOID() );
							}
						}
					}
				}
			}
			// �`��`�F�b�N
			// �Z�O�����g�I�[�o�[���b�v�`�F�b�N�݂̂̎��̓X���[
			if( ! bCheckSegmentOverlapOnly )
				(*it).IsSafeGeometry( errs, FALSE, bIsCheckAngle, ipRule );
		}
		// [bug 1870] ���H�����N�̐ڑ��p�`�F�b�N�i�Q���H�Ŋp�x20���ȉ��j
		// �Z�O�����g�I�[�o�[���b�v�`�F�b�N�݂̂̎��̓X���[
		if( ! bCheckSegmentOverlapOnly )
		{
			// �`��ł͂Ȃ�FROM�ATO�Őڑ��󋵂𒲂ׂ�(bug 5477)
			if( cRoadItems.size() > 1 ) // �����N��2�{�ȏ�Ȃ��ƃ`�F�b�N�ł��Ȃ�
			{
				// TODO �ȉ��A�l�b�g���[�N�����N�ʂɃ`�F�b�N�𕪂���悤�ɂ��Ȃ��ƃ����e�i���X������ɂȂ�̂ŕ����邱��

				/// [ROADCHECK 02322] ����m�[�h�ɐڑ�����������ߐڂ��Ă�����`�F�b�N���邩�ۂ�
				// ���������N�ȊO�Ȃ�`�F�b�N����(���s��Phase2�Ή�)
				bool bCheckCloseNode = (sindyeNetworkWalk!=AheGetNetWorkType(cRoadItems.begin()->GetFeature()));
				/// [�S���S�H��] �S�������N�ł͓񍳘H�̐ڑ��p�x�`�F�b�N�͌x���ɂ���
				// [�����^�]�x���}�b�v]�Ԑ��E�㉺�����f���ł��`�F�b�N�͌x���ɂ���
				bool bWarnAngleChk = (sindyeNetworkRailway==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkLane==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkUpDown==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkCompart==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkBorder==AheGetNetWorkType(cRoadItems.begin()->GetFeature()));
				// �ڑ��󋵂𒲂ׂ�
				std::map<long,CTargetItems> listGraph; // map<�m�[�hID,list<�����NID>>
				for( CTargetItems::iterator itRoads1 = cRoadItems.begin(); itRoads1 != cRoadItems.end(); ++itRoads1 )
				{
					//if ( itRoads1->GetFeatureClassName().CompareNoCase( LANE_LINK_TABLE_NAME ) == 0 )
					if ( (sindyeNetworkLane==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
						|| (sindyeNetworkUpDown==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
						|| (sindyeNetworkCompart==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
						|| (sindyeNetworkBorder==AheGetNetWorkType(cRoadItems.begin()->GetFeature())) )
						continue;

					long lOID = itRoads1->GetOID();
					listGraph[itRoads1->GetAttribute( FROM_FIELD ).lVal].push_back(*itRoads1);
					listGraph[itRoads1->GetAttribute( TO_FIELD ).lVal  ].push_back(*itRoads1);
				}
				for( std::map<long,CTargetItems>::const_iterator itGraph = listGraph.begin(); itGraph != listGraph.end(); ++itGraph )
				{
					// 2���H��������
					if( 2 == itGraph->second.size() )
					{
						// �ڑ����Ă���m�[�h��2���H�m�[�h�ȊO�̏ꍇ�͔���ΏۂƂ��Ȃ�
						CTargetItems::iterator itNode = cNodeItems.begin();
						for( ; itNode != cNodeItems.end(); ++itNode )
						{
							if( itNode->GetOID() == itGraph->first )
								break;		
						}
						if( itNode != cNodeItems.end() )
						{
							CComVariant vaNodeClass = itNode->GetAttribute( NODECLASS_FIELD );
							if( vaNodeClass.vt == VT_I4 && 
								( vaNodeClass.lVal == sindyeNodeIntersection ) || 
								( vaNodeClass.lVal == sindyeNodeConnectPecLink ) ||
								( vaNodeClass.lVal == sindyePECNode ) ||
								( vaNodeClass.lVal == sindyeRelHWY ) )
								ATLTRACE(_T("CTargetItems::IsSafeGeometries() : �����_�m�[�h�y��PEC�EHWY�֌W�m�[�h�ɐڑ����Ă��邽�ߊp�x����͂��܂���F%d-%d\n"), itGraph->second.begin()->GetOID(), (++itGraph->second.begin())->GetOID() );
							else {
								// �ڑ��p
								DOUBLE dAngle = AheGetAngle( itGraph->second.begin()->GetShape(), (++itGraph->second.begin())->GetShape() );
								if( -20.0 <= dAngle && dAngle <= 20.0 )
								{
									errs.add( itGraph->second.begin()->GetOwnerDotClassName(), itGraph->second.begin()->GetOID(), itGraph->second.begin()->GetShapeCopy(), -1, 
									MakeWKSPoint( AheGetConnectPoint( itGraph->second.begin()->GetShape(), (++itGraph->second.begin())->GetShape() ) ), -1,
									((bWarnAngleChk)? sindyeLogLevel_WARN:sindyeLogLevel_ERR), 
									ERROR_ANGLE_IS_20_DEGREE_OR_LESS, itGraph->second.begin()->GetOID(), (++itGraph->second.begin())->GetOID(), (int)dAngle );
								}
							}
						}
					}
					if( bCheckCloseNode && 2 <= itGraph->second.size() )
					{
						// [CHECK] [ROADCHECK 02322] ����m�[�h�ɐڑ�����������ߐڂ��Ă���ibug 8463�j
						for( CTargetItems::const_iterator itConnectRoad = itGraph->second.begin(); itConnectRoad != itGraph->second.end(); ++itConnectRoad )
						{
							CTargetItems::const_iterator itConnectRoadNext = itConnectRoad;
							if( itGraph->second.end() == ++itConnectRoadNext )
								itConnectRoadNext = itGraph->second.begin();
							IPointCollectionPtr ipBaseGeom( itConnectRoad->GetShape() );
							IPointCollectionPtr ipTestGeom( itConnectRoadNext->GetShape() );
							double dLen = min( AheGetConnectSegmentCrack( ipBaseGeom, ipTestGeom ), AheGetConnectSegmentCrack( ipTestGeom, ipBaseGeom ) );
							if( 20.0 >= dLen )
								errs.add( itConnectRoad->GetOwnerDotClassName(), itConnectRoad->GetOID(), itConnectRoad->GetShapeCopy(), -1, 
								MakeWKSPoint( AheGetConnectPoint( itGraph->second.begin()->GetShape(), (++itGraph->second.begin())->GetShape() ) ), -1, sindyeLogLevel_WARN,
								ERROR_SEGMENT_CONNECT_SAME_NODE_IS_CLOSE, itConnectRoad->GetOID(), itConnectRoadNext->GetOID(), dLen );
						}
					}
				}
			}
		}
#ifdef DEBUG
		ATLTRACE(_T("������ CTargetItems::IsSafeGeometries() : %lf ������\n"), (DOUBLE)(GetTickCount() - DWSTART ) / (DOUBLE)1000.0 );
#endif

		return errs.GetMaxLogLevel();
	}

	/**
	* @brief �Ǘ��Ώۈꗗ�̕ҏW����
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL Store();

	/**
	* @brief �n���Q�Ɛݒ�
	*
	* @note �n���Q�Ƃ�ݒ肷��B
	*
	* @param piSpatialReference [in] �ݒ肷��n���Q�Ǝw�肵�Ȃ��ꍇ�́A�Ǘ��Ώۂ̃��C������擾���Đݒ�
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL SnapToSpatialReference( ISpatialReferencePtr piSpatialReference = NULL);

	/**
	 * @brief �G���[���b�Z�[�W�̎擾
	 *
	 * @retval CString �G���[���b�Z�[�W
	 */
	CString GetErrMessage()
	{
		CString strErrMessage;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )
			strErrMessage += it->GetErrMessage();

		return strErrMessage;
	}

	/**
	 * @brief �G���[���b�Z�[�W�̍폜
	 */
	void RemoveErrMessage()
	{
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )	it->RemoveErrMessage();
	}

	/**
	* @brief ISet�ɃA�C�e�����i�[
	*
	* @param ipSet [out] ISet�̃C���X�^���X
	*/
	void SetISetFromItems(_ISet** ipSet);

	/**
	* @brief ISet�ɃA�C�e�����i�[
	*
	* @param ipSet [out] ISet�̃C���X�^���X
	*/
	_ISetPtr GetFeaturesSet();
	
	/**
	* @brief �A�C�e����ISet���i�[
	*
	* @param ipMap [in] IMap�̃C���X�^���X
	* @param ipSet [in] ISet�̃C���X�^���X
	*/
	void SetItemsFromISet(IMapPtr ipMap, _ISetPtr ipSet);

	/**
	* @brief �A�C�e����ISet���i�[(��r)
	*
	* @param ipMap [in] IMap�̃C���X�^���X
	* @param cCompItems [in] ��r�A�C�e��
	* @param ipSet [in] ISet�̃C���X�^���X
	*/
	void SetItemsFromISetFromComp(IMapPtr ipMap, CTargetItemsT cCompItems, _ISetPtr ipSet);

	/**
	* @brief �A�C�e����ISet���i�[
	*
	* @param ipSet	[in]			_ISetPtr
	* @param bAdd	[in,optional]	�ǉ��i�[�̏ꍇ�� TRUE�i�f�t�H���g�FFALSE�j
	*/
	void SetFeaturesSet( _ISetPtr ipSet, BOOL bAdd = FALSE );

	/**
	 * @brief �����I�� Simplify() ������
	 *
	 * @see AheForceSimplify
	 *
	 * @retval TRUE ����
	 * @retval FALSE ���s
	 */
	BOOL Simplify()
	{
		BOOL bRet = TRUE;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it ) 
		{
			if( it->Simplify() == FALSE )
				bRet = FALSE;
		}

		return bRet;
	}
#ifdef _DEBUG
	void Trace( LPCTSTR lpcszHeader )
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			ATLTRACE(_T("%s : TABLE[%s] OID[%d]\n"), lpcszHeader, AheGetFeatureClassName( it->GetFeature() ), it->GetOID() );
			AheTraceGeometry( it->GetShapeCopy(), lpcszHeader );
		}
	}
#else
	void Trace( LPCTSTR/* lpcszHeader*/ ){}
#endif // ifdef _DEBUG
private:
	BOOL CheckPolygonOverlap( IEnvelopePtr ipCheckEnv, CErrObjContainer& errs ) const
	{
#ifdef DEBUG
		DWORD DWSTART = GetTickCount();
#endif // ifdef DEBUG

		using namespace libErrMsg;

		// �t�B�[�`���N���X�ʂɕ�����
		std::map<LONG, CTargetItems> cFeaturesMap;
		for( CTargetItemsT<T>::const_iterator it = begin(); it != end(); ++it )
		{
			if( it->GetGeometryType() == esriGeometryPolygon &&
				it->GetFeatureClassName().Compare( _T("PARKING_SHAPE") ) != 0 &&
				!AheIsOverlapableLayer( it->GetFeatureClassName() ) )
			{
				IFeatureClassPtr ipFeatClass( it->GetFeatureClass() );
				if( ipFeatClass != NULL )
				{
					LONG lObjectClassID = -1;
					ipFeatClass->get_ObjectClassID( &lObjectClassID );
					cFeaturesMap[lObjectClassID].push_back( *it );
				}
			}
		}
		// �t�B�[�`���N���X�ʂɃ`�F�b�N
		for( std::map<LONG, CTargetItems>::iterator itMap = cFeaturesMap.begin(); itMap != cFeaturesMap.end(); ++itMap )
		{
			IFeatureClassPtr ipFeatClass( itMap->second.begin()->GetFeatureClass() );
			CString strItFeatureClassName = AheGetFeatureClassName( ipFeatClass );

			// �܂��͕ҏW�t�B�[�`�����ł̂݃`�F�b�N
			for( CTargetItems::iterator itFeatures = itMap->second.begin(); itFeatures != itMap->second.end(); ++itFeatures )
			{
				CTargetItems::iterator itFeatures2 = itFeatures;
				itFeatures2++;
				for( ; itFeatures2 != itMap->second.end(); ++itFeatures2 )
				{
					if( AheIsOverlap2( itFeatures->GetShape(), itFeatures2->GetShape() ) )  // �|���S���d���͌x���Ɂibug 5573�j
					{
						errs.add( itFeatures->GetOwnerDotClassName(), itFeatures->GetOID(), itFeatures->GetShapeCopy(), -1, MakeWKSPoint( AheGetAreaPoint( itFeatures->GetShape() ) ), -1, sindyeLogLevel_WARN, ERROR_DUPLICATE_OF_OID, itFeatures2->GetOID() );
					}
				}
			}
			// ����
			CString strSubField;
			CComBSTR bstrOIDField, bstrShapeField;

			ipFeatClass->get_OIDFieldName( &bstrOIDField );
			ipFeatClass->get_ShapeFieldName( &bstrShapeField );
			strSubField.Format(_T("%s,%s"), CString(bstrOIDField), CString(bstrShapeField) );
			IFeatureCursorPtr ipFeatCursor = AheSelectByShape( (IGeometryPtr)ipCheckEnv, ipFeatClass, esriSpatialRelIntersects, strSubField, _T(""), TRUE );
			// �`�F�b�N
			if( ipFeatCursor )
			{
				IFeaturePtr ipFeat;
				while( SUCCEEDED( ipFeatCursor->NextFeature( &ipFeat ) ) )
				{
					if( ! ipFeat )
						break;

					CTargetItem cItemTmp( ipFeat );
					LONG lOIDTmp = cItemTmp.GetOID();

					for( CTargetItems::iterator itFeatures = itMap->second.begin(); itFeatures != itMap->second.end(); ++itFeatures )
					{
						if( itFeatures->GetOID() != lOIDTmp )
						{
							// �ړ��ΏیQ�Ɋ܂܂�Ă���ꍇ�͂��̌`��Ń`�F�b�N
							BOOL bIsOverlaps = FALSE;
							if( ! IsAlreadyExist( cItemTmp ) )
							{
								IGeometryPtr ipTestGeom;
								ipFeat->get_Shape( &ipTestGeom );
								bIsOverlaps = AheIsOverlap2( itFeatures->GetShape(), ipTestGeom );
							}

							if( bIsOverlaps )
							{
								errs.add( itFeatures->GetOwnerDotClassName(), itFeatures->GetOID(), itFeatures->GetShapeCopy(), -1, MakeWKSPoint( AheGetAreaPoint( itFeatures->GetShape() ) ), -1, sindyeLogLevel_WARN, ERROR_DUPLICATE_OF_OID, lOIDTmp );
							}
						}
					}
				}
			}
		}
#ifdef DEBUG
		ATLTRACE(_T("������    CTargetItems::CheckPolygonOverlap() : %lf ������\n"), ((DOUBLE)GetTickCount() - DWSTART )/(DOUBLE)1000.0 );
#endif
		return TRUE;
	}
};

typedef CTargetItemsT<CTargetItem>		CTargetItems;

// �Ǘ��Ώۈꗗ�̕ҏW����
template <typename T>
inline BOOL CTargetItemsT<T>::Store()
{
	for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++ ) {
		(*it).Store();
	}
	return TRUE;
}

// �n���Q�Ɛݒ�
template <typename T>
inline BOOL CTargetItemsT<T>::SnapToSpatialReference( ISpatialReferencePtr piSpatialReference)
{
	for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++)
	{
		( *it).SnapToSpatialReference( piSpatialReference);
	}
	return TRUE;
}

template <typename T>
inline void CTargetItemsT<T>::SetISetFromItems(_ISet** ipSet)
{
	_ISetPtr			ipSetTmp(CLSID_Set);

	for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++) {
		ipSetTmp->Add( it->GetFeature() );
	}
	(*ipSet) = ipSetTmp;
	(*ipSet)->AddRef();
	return;
}

template <typename T>
inline _ISetPtr CTargetItemsT<T>::GetFeaturesSet()
{
	_ISetPtr ipSet( CLSID_Set );

	for( iterator it = begin(); it != end(); it++ ) 
	{
		ipSet->Add( it->GetFeature() );
	}

	return ipSet;
}

template <typename T>
inline void CTargetItemsT<T>::SetItemsFromISet(IMapPtr ipMap, _ISetPtr ipSet)
{
	USES_CONVERSION;

	LONG		lCount;

	ipSet->get_Count( &lCount );
	ipSet->Reset();

	clear();
	for ( LONG i = 0; i < lCount; i++ ) {
		ILayerPtr		ipLayer;
		IUnknownPtr		ipUnk;
		CComBSTR		bstrLayerName;

		ipSet->Next( &ipUnk );
		push_back( (IFeaturePtr)ipUnk );
	}
	return;
}

template <typename T>
inline void CTargetItemsT<T>::SetItemsFromISetFromComp(IMapPtr ipMap, CTargetItemsT cCompItems, _ISetPtr ipSet)
{
	LONG		lCount;

	ipSet->get_Count( &lCount );
	ipSet->Reset();

	clear();
	for ( LONG i = 0; i < lCount; i++ ) {
		IUnknownPtr 		ipUnk;
		CString 			strName;
		LONG 				lOID = 0;

		ipSet->Next( &ipUnk );

		IFeaturePtr 		ipFeature( ipUnk );

		if( ipFeature == NULL )
			continue;

		ipFeature->get_OID( &lOID );
		strName = AheGetFeatureClassName( ipFeature );

		for( CTargetItemsT<T>::iterator it = cCompItems.begin(); it != cCompItems.end(); it++ ) 
		{
			IFeaturePtr ipFeature2 = it->GetFeature();
			if ( lOID == (*it).GetOID() && strName.CompareNoCase( AheGetFeatureClassName( ipFeature2 ) ) == 0 )
				push_back( *it );
		}
	}
	return;
}

template <typename T>
inline void CTargetItemsT<T>::SetFeaturesSet( _ISetPtr ipSet, BOOL bAdd/* = FALSE*/ )
{
	if( ipSet == NULL ) return;

	if( bAdd == FALSE ) clear();

	ipSet->Reset();
	IUnknownPtr ipUnk;
	while( SUCCEEDED( ipSet->Next( &ipUnk ) ) )
	{
		if( ipUnk == NULL ) break;

		push_back( (IFeaturePtr)ipUnk );
	}
}

#endif	//__TARGETFEATURES_H__
