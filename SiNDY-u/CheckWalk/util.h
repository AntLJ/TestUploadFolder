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

#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <TDC/sindylib_core/FieldMap.h>
#include "FieldLists.h"
namespace util
{

inline void GetDomainMap( const CString& fieldName, const sindy::CFieldMap& fieldMap, std::map<long, CString>& classMap )
{
	std::map<CString, long> domainMap = fieldMap.GetDomain( fieldName );
	for( auto& domain : domainMap )
		classMap[domain.second] = domain.first;
};

/**
	* @brief	���ݎ����擾�p
	* @retval	CString  �uYYYY/MM/DD hh:mm:ss�v 
	*/
inline CString GetCurTime(){
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strTime;
	strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return strTime;
	};

/// 3�_�̓��ς����߂�
inline double innerproduct( double x1, double y1, double x2, double y2, double x3, double y3 )
{
	return ( x1 - x2 ) * ( x3 - x2 ) + ( y1 - y2 ) * ( y3 - y2 );
};

/// 2�_�̒��������߂�
inline double length( double x1, double y1, double x2, double y2 )
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	return sqrt( dx*dx + dy*dy );
};

/**
 * @brief 3�_����Ȃ�����̊p�x�����߂�
 *
 * @param x1	[in]	DOUBLE
 * @param y1	[in]	DOUBLE
 * @param x2	[in]	DOUBLE
 * @param y2	[in]	DOUBLE
 * @param x3	[in]	DOUBLE
 * @param y3	[in]	DOUBLE
 *
 * @retval �p�x�i-180�`180�x�j
 */
inline double angle( double x1, double y1, double x2, double y2, double x3, double y3 ){
	DOUBLE dInnerProduct = util::innerproduct( x1, y1, x2, y2, x3, y3 );
	DOUBLE d = util::length( x1, y1, x2, y2 ) * util::length( x2, y2, x3, y3 );

	return ( acos( dInnerProduct / d ) * (double)180.0 ) / (double)M_PI;
};


/**
 * @brief R-Tree�𐶐�����
 * @param geoClassMap [in]	�Ώۃt�B�[�`���N���X
 * @param rTree	[out] ���ʂ�R-Tree(�`����[r]�͎Q�Ƃ̈Ӗ��ł͂Ȃ��ł�)
 */
void createRTree(
	const std::map<long, GeoClass>& geoMap,
	BGRTree& rTree
	);

/**
 * @brief �x�[�X���b�V���`����擾
 * @param meshs [in] �擾����x�[�X���b�V���̃��b�V���R�[�h���X�g
 * @param table [in] �x�[�X���b�V����ێ�����e�[�u��
 * @return �x�[�X���b�V���R�[�h�ƃ��b�V���`��̑Ή��\
 */
std::map<CString, IGeometryPtr> getBaseMesh(const std::set<std::string>& meshCodes, ITablePtr table);

/**
 * @brief	Polyline���\���_�ɕ���
 * @param	ipLineGeometry	[in]	Polyline��Geometry
 * @param	seqPoint	[out]	�\���_�̃��X�g
 * @retval	true	����
 * @retval	false	���s
 */
bool getLine2Points(
	const IGeometryPtr& ipLineGeometry,
	std::vector<IPointPtr>& seqPoint
	);

/**
 * @brief	R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
 * @param	box	[in]	�擾�������͈�(�G���x���[�v)
 * @param	rTree [in]	�擾���Ă���Ώۂ�R-Tree
 * @retval	set<long>	�Ώ۔͈͂�ID�Q
 */
std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

/**
 * @brief �t�B�[�`���}�b�v�������̑����l�����t�B�[�`����ID�Q���擾
 * @param featureMap   [in] �t�B�[�`���}�b�v
 * @param fieldName    [in] ������
 * @param fieldValues  [in] �擾���鑮���l���X�g
 * @param meshGeometry [in] ���b�V���`��
 * @return �t�B�[�`����ID�Q
 */
std::set<long> getTargetIDs(
	const FeaturesPack& feature
	, LPCTSTR fieldName
	, const std::set<long> fileldValues
	, const IGeometryPtr& meshGeometry );

/**
 * @brief �t�B�[�`���}�b�v���烁�b�V���Ɋ܂܂��t�B�[�`����ID�Q���擾
 * @param featureMap   [in] �t�B�[�`���}�b�v
 * @param meshGeometry [in] ���b�V���`��
 * return �t�B�[�`����ID�Q
 */
std::set<long> getTargetIDs(
	const std::map<long, GeoClass>& featureMap
	, const IGeometryPtr& meshGeometry );

/**
 * @brief �t�B�[�`���}�b�v�̓��A�����ɓn����ID�����t�B�[�`���̌`���Union
 * @param featureMap [in] �t�B�[�`���}�b�v
 * @param ids        [in] Union����ID�Q
 * @return Union���ꂽ����
 */
IGeometryPtr getUnionGeometry(
	const std::map<long, GeoClass>& featureMap
	, const std::set<long>& ids );

/**
 * @brief �t�B�[�`���}�b�v�̃t�B�[�`���̌`���Union
 * @param featureMap [in] �t�B�[�`���}�b�v
 * @return Union���ꂽ����
 */
IGeometryPtr getUnionGeometry(const std::map<long, const GeoClass*>& featureMap);

/**
 * @brief �|�C���g�Q��Union
 * @param pointList [in] �|�C���g�Q
 * @return Union���ꂽ����
 */
IGeometryPtr getUnionGeometry(const std::vector<IPointPtr>& pointList);

/**
 * @brief  ��_���擾����
 * @note   ��_�����݂��Ȃ��ꍇ�͋��ԋp
 * @param  baseGeometry  [in]  �����N1
 * @param  testGeometry  [in]  �����N2
 * @return ��_�̍��W�Q
 */
std::vector<IPointPtr> getCrossPoint(IGeometryPtr baseGeometry , IGeometryPtr testGeometry);

/**
 * @brief ID�Q����w��`��Ɋ��S��������̂�����
 * @detail �w��`��̘g����̃t�B�[�`���͊��S�����Ă��Ȃ����̂�
 * ���f����
 * @wanirng featureIds�̗v�f��OBJECTID�ɂ��t�B�[�`����
 * featureMap���ɑ��݂��Ȃ��ꍇ�Aout_of_range��O�����s�����
 * @param featureIds [in] ID�Q
 * @param geometry   [in] �w��`��
 * @param featureMap [in] �t�B�[�`���}�b�v(ids�̌`��Q�Ɨp)
 * @return ID�Q����w��`��Ɋ��S��������̂�������ID�Q
 */
std::set<long> excludeContainLink(
	const std::set<long>& featureIds
	, const IGeometryPtr& geometry
	, const std::map<long, GeoClass>& featureMap);

/**
 * @brief �q�����Ă��郊���N����OBJECTID�Q�𕪂���
 * @detail
 * �E�߂�l��
 * <�q�����Ă��郊���N�̓���ԏ�����OBJECTID,
 * �L�[��OBJECTID���瓞�B�\�ȃ����N��OBJECTID�Q><br>
 * �L�[��OBJECTID���瓞�B�\�ȃ����N��OBJECTID�Q�ɂ́A�L�[��OBJECTID���܂܂��B<br>
 * ��) �����N 1,2,3���A�����N 4,5,6,7���q�����Ă���Ƃ��A
 * <1, {1,2,3}>,<4,{5,6,7}><br>
 * �E�����N���m���q�����Ă��邩�ǂ����́AFROM_NODE_ID/TO_NODE_ID�̒l��
 * ���n���ǂ����Ŕ��肵�Ă���
 * @param linIds  [in] �q�����Ă��邩���肷�郊���N�Q
 * @param linkMap [in] �t�B�[�`���}�b�v(linkIds�̌`��Q�Ɨp)
 * @return <�����NID, �����NID���瓞�B�\�ȃ����NID��set>
 */
std::map<long, std::set<long>> toEveryPartLink(
	const std::set<long>& linkIds
	, const FeaturesPack& link );

/**
 * @brief	���ӂ̕��s�҃m�[�h�ɓ����|�C���g�����m�[�h�����݂��邩�`�F�b�N
 * @param	checkObj	[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
 * @param	checkObjMap	[in]	�`�F�b�N�ΏۃI�u�W�F�N�g��Map
 * @param	walkNodeMap	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�hMap
 * @param	walkNodeRTree	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�hRTree
 * @param	nameField	[in]	���̃t�B�[���h
 * @param	yomiField	[in]	���~�t�B�[���h
 * @param	idField	[in]	ID�t�B�[���h
 * @param   refer_id_threshold [in] �G���[�Ƃ��鏈����臒l
 * @param	checkWalkNodeClass	[in]	�`�F�b�N�Ώۂ̃m�[�h��ʁi�f�t�H���g:-1�j
 * @retval	true	���݂���
 * @retval	false	���݂��Ȃ�
 */
bool existAroundWalkNodeSameName(
	const GeoClass& checkObj,
	const FeaturesPack& checkFeatures,
	const FeaturesPack& walkNode,
	const BGRTree& walkNodeRTree,
	const CString& nameField,
	const CString& yomiField,
	const CString& idField,
	double refer_id_threshold,
	const long checkWalkNodeClass = -1
	);

/**
 * @brief �ŏI�X�V�����E�v���O�����X�V�����̓��A�V�������t��Ԃ�
 * @detail �����NULL�Ȃ�NULL�łȂ�����Ԃ��B
 * ������NULL�Ȃ�VT_NULL��Ԃ�
 * @param modifyDate        [in] �ŏI�X�V����
 * @param programModifyDate [in] �v���O�����X�V����
 * @return DATE�^��CComVariant
 * @retval NULL�^��CComVariant modifyDate,programModifyDate�Ƃ�NULL
 */
CComVariant selectModifyTime(
	const CComVariant& modifyDate
	, const CComVariant& programModifyDate);

/**
 * @brief �w������ȍ~�ɕҏW����Ă��邩�ۂ�
 * @param modifyFilter [in] �ҏW�����t�B���^
 * @param modifyDate   [in] �`�F�b�N�Ώۂ̍ŏI�X�V����
 * @param programModifyDate [in] �`�F�b�N�Ώۂ̃v���O�����X�V����
 * @retval true  �w������ȍ~�ɕҏW����Ă���
 * @retval false �w������ȍ~�ɂ͕ҏW����Ă��Ȃ����A
 * �ŏI�X�V�����E�v���O�����ŏI�X�V�������Ƃ���NULL�ł���
 */
bool checkModifyTime(
	const CComVariant& modifyFilter
	, const CComVariant& modifyDate
	, const CComVariant& programModifyDate);

/**
 * @brief �|�C���g�������N��ɂ��邩�ǂ����𔻒�
 * @note
 * �|�C���g�ƃ����N�̍ŋߖT������DB�̍��W���x��菬�����Ȃ�A�����N��ɂ���Ɣ��肷��B
 * �����[�X�f�[�^�쐬���Ɏg�p����SplitLinkWLN�ŁA
 * �����N��̃m�[�h�̔��肷�鏈��������A������`�F�b�J�[�p�ɏC�������B
 * ��SplitLinkWLN�ł́A�m�[�h�����W���x���傫������Buffer���쐬���A
 *  ���ꂪ�����N�Ɛڂ��Ă��邩�ǂ����𔻒肵�Ă���B
 * ������̕��@�ł��ŋߖT�������ǂ����̔���͂ł��邪�A�{�֐��̎����̕�������)
 * ����𐮔����ŏC�����Ȃ��ƁA��q�̃����[�X�c�[�����ُ�I������̂ŁA�`�F�b�J�[�Ō��o����B
 * range�ɂ́ADB�̍��W���e�l�����邱�Ƃ�z�肵�Ă���B
 * @param [in] link  �����N
 * @param [in] point �|�C���g
 * @param [in] range �����N��Ƀ|�C���g�����݂���Ɣ��肷�鋖�e�l
 * @retval true  �|�C���g�������N��ɑ��݂���
 * @retval false �|�C���g�������N��ɑ��݂��Ȃ�
*/
bool isPointOnLink(IGeometryPtr link, IGeometryPtr point, double range);

/**
 * @brief �`�F�b�N�Ώۂ̃����N�̒[�_���AoverlapLink��ɑ��݂��邩�ۂ��𔻒�
 * @detail �[�_�ƃ����N��range�ȓ��ł���Ȃ�true�Ƃ���
 * @param [in] checkTargetLink �`�F�b�N�Ώۂ̃����N
 * @param [in] overlapLink	   �`�F�b�N�Ώۂ̃����N���܂ރ����N
 * @param [in] range           �����N��Ƀ|�C���g�����݂���Ɣ��肷�鋖�e�l
 * @retval true  �����N�̒[�_�������Ƃ�overlapLink��ɑ��݂���
 * @retval false �ǂ��炩����̃����N�̒[�_��overlapLink��ɑ��݂��Ȃ�
 */
bool isLinkEdgeOnLink(IGeometryPtr checkTargetLink, IGeometryPtr overlapLink, double range);

/**
 * @brief ���C���̍\���_�̓��A���郉�C���ɏd�Ȃ��Ă��镔���Ɋ܂܂��\���_�Q���擾
 * @detail �\���_�̏��Ԃ�checkdLine�̎n�_���I�_�����Ŋi�[
 * @param [in] checkedLine �\���_�Q���擾���郊���N
 * @param [in] overlapLine �\���_�擾�p���C��
 * @param [in] range       �����N��Ƀ|�C���g�����݂���Ɣ��肷�鋖�e�l
 * @return �d�Ȃ��Ă��镔���̍\���_
 */
std::vector<IPointPtr> getOverlapConfigurationPoints(IPolylinePtr checkedLine, IPolylinePtr overlapLine, double range);

/**
 * @brief	�w�肳�ꂽ2��GeoClass(Point)�Ԃ̋��������߂�
 * @param	geo1	[in]	Point��GeoClass1
 * @param	geo2	[in]	Point��GeoClass2
 * @retval	����(m)
 */
double GetDistGeoClasses(
	const GeoClass& geo1,
	const GeoClass& geo2
	);

/**
 * @brief	�w�肳�ꂽ2��Geometry(Point)�Ԃ̋��������߂�
 * @param	geo1	[in]	Point��Geometry1
 * @param	geo2	[in]	Point��Geometry2
 * @retval	����(m)
 */
double GetDistGeometries(
	const IGeometryPtr& geo1,
	const IGeometryPtr& geo2
	);

/**
 * @brief ID�Z�b�g����Ώۂ�OID��������ID�Q�̃I�u�W�F�N�g���X�g���擾����
 * @param oID    [in] �Ώۂ�OID
 * @param oIDs   [in] ID�Z�b�g
 * @param geoMap [in] �I�u�W�F�N�g�̊i�[��
 * @return �I�u�W�F�N�g���X�g
 */
std::list<GeoClass> GetObjects( 
	long oID,
	const std::set<long>& oIDs,
	const std::map<long, GeoClass>& geoMap
	);

/**
 * @brief ID�Z�b�g����Ώۂ�OID��������ID�Q�̃I�u�W�F�N�g�}�b�v���擾����
 * @param oID    [in] �Ώۂ�OID
 * @param oIDs   [in] ID�Z�b�g
 * @param geoMap [in] �I�u�W�F�N�g�̊i�[��
 * @return �I�u�W�F�N�g�}�b�v <OID, �I�u�W�F�N�g�ւ̃|�C���^>
 */
std::map<long, const GeoClass*> GetObjectMap(
	long oID,
	const std::set<long>& oIDs,
	const std::map<long, GeoClass>& geoMap
	);


/**
 * @brief �w��̃����N�Ɋւ��āA�w��̃m�[�hID�ɕR�t���Ȃ����̒[�_�ɕR�t���m�[�hID���擾
 * @note �n�_��OID:1,�I�_��OID:2�̃m�[�h���R�t���Ă���ꍇ�A
 * �m�[�hID�Ƃ���"1"�������ɗ^����ƁA2��Ԃ�
 * @param linkId  [in] �����NID
 * @param linkMap [in] �����N�̑����l���i�[�����}�b�v
 * @param nodeId  [in] �m�[�hID
 * @retrun �w��̃m�[�hID�Ƃ͋t�̒[�_�ɕR�t���m�[�h�̃m�[�hID
 * @retval 0 �����ɉ��L�̂����ꂩ�̃G���[������<br>
 * �ElinkId��linkMap�̒��ɑ��݂��Ȃ�<br>
 * �EnodeId��linkId�Ŏw�肵�������N�ɕR�t���Ă��Ȃ�<br>
 */
long getOtherEdgeNode( long linkId, const FeaturesPack& linkMap, long nodeId );

/**
 * @brief �w��m�[�h�����b�V�����E��m�[�h���ǂ�������
 * @param nodeId  [in] �m�[�hID
 * @param nodeMap [in] �m�[�h�̑����l���i�[�����}�b�v
 */
bool isMeshEdgeNode( long nodeId, const FeaturesPack& nodeMap, bool isWalk );

/**
 * @brief �񎟃��b�V�����E�̌��������̃����N�ɐڑ����郊���N�Q���擾
 * @detail �����ɁA�m�[�hID��2�����b�V�����E�̌��������̃m�[�hID�ɍX�V
 * @param linkId  [in] �����NID
 * @param linkMap [in] �����N�̑����l���i�[�����}�b�v
 * @param nodeId  [in/out] �m�[�hID
 * @param nodeMap [in] �m�[�h�̑����l���i�[�����}�b�v
 * @param nodeLinkIDs [in] �m�[�hID�ɑΉ����郊���NID�̑Ή��\
 */
std::set<long> getMeshOverLink(
	long linkId, const FeaturesPack& link
	, long& nodeId, const FeaturesPack& node
	, const NODEIDMAP& nodeLinkIDs
	, bool isWalk );

/**
 * @brief nodeId��[�_�Ƃ����Ƃ��̗אڂ��郊���N���擾
 * @detail nodeId���}�s��m�[�h�̏ꍇ�A
 * 2�����b�V�����E�̌��������̃����N�ɐڑ����Ă��郊���N�Q���擾
 * �����ɁA�[�_�m�[�hID��2�����b�V�����E�̌��������̃m�[�hID�ɍX�V
 * @param nodeId       [in/out] �[�_�m�[�hID
 * @param adjacentLink [in] �[�_�m�[�h�ɐڑ����郊���N�Q
 * (�אڂ��擾�����������N�̃����NID��������Ă��邱��)
 * @param linkMap      [in] �����N�̑����l���i�[�����}�b�v
 * @param nodeMap      [in] �m�[�h�̑����l���i�[�����}�b�v
 * @param nodeLinkIDs  [in] �m�[�hID�ɑΉ����郊���NID�̑Ή��\
 */
std::set<long> getAdjacentLink(
	long& nodeId
	, const std::map<long, const GeoClass*>& adjacentLink
	, const FeaturesPack& link
	, const FeaturesPack& node
	, const NODEIDMAP& nodeLinkIDs
	, bool isWalk
	);

/**
 * @brief �w��m�[�h�������NID�̎n�_�ɐڑ����Ă��邩�ۂ�
 * @param linkId  [in] �w��m�[�h�Ƃ̐ڑ����m�F���郊���N�̃����NID
 * @param linkMap [in] �����NID�ɑ΂��鑮���E�`��̑Ή��\
 * @param nodeId  [in] �w��m�[�h
 * @retval true  �n�_�ł���
 * @retval false �n�_�ł͂Ȃ�
 */
bool isFromNode( long linkId, const FeaturesPack& linkMap, long nodeId );


/**
 * @brief	�t�B�[�`���[���w�肳�ꂽ���b�V���R�[�h�Q�̂ǂ̃��b�V���Ɋ܂܂�Ă��邩�i�g��܂ށj�擾����B
 * @param	geoClass	[in]	�Ώۂ̃t�B�[�`��
 * @param	cityMeshMap	[in]	���b�V���R�[�hMap
 * @return	�i�Y�����郁�b�V�����������ꍇ�j���b�V���R�[�h
 * @return	�i�Y�����郁�b�V�����Ȃ������ꍇ�j0
 */
long GetExistMeshcode(
	const GeoClass& geoClass,
	const std::map<long, std::pair<IGeometryPtr,IGeometryPtr>>& cityMeshMap
	);

/**
 * @brief	�w��m�[�h�ɕR�t�������N�͎w�肵�������N�݂̂ł��邩�i���s�җp�j
 * @note	�R�t�������N���w�胊���N�݂̂ł��A�w��m�[�h���[�_�m�[�h�̏ꍇ�A���茋�ʂ�false�ɂ���
 * @param	nodeMapIteId	[in] �m�[�h�̑����l���i�[�����}�b�v�̃C�e���[�^
 * @param	linkId			[in] �����NID
 * @param	nodeLinkIDs		[in] �m�[�h�ɕR�t�������N�Q�̏��imap�j
 * @param	nodeClassIndex	[in] �m�[�h�����C���f�b�N�X
 * @retval  true �R�t�������N�͎w�胊���N�̂�
 * @retval	false �w�胊���N�ȊO�̃����N���R�t�����A�����R�t���Ȃ�
 */
bool isOnlyTargetWalkLink(
	const std::map<long, GeoClass>::iterator& nodeMapIt,
	long linkId,
	const NODEIDMAP& nodeLinkIDs,
	long nodeClassIndex );

/**
* @brief	�w��m�[�h�ɕR�t�������N�͎w�肵�������N�݂̂ł��邩�i���H�p�j
* @note	�R�t�������N���w�胊���N�݂̂ł��A�w��m�[�h���[�_�m�[�h�̏ꍇ�A���茋�ʂ�false�ɂ���
* @param	nodeMapIteId	[in] �m�[�h�̑����l���i�[�����}�b�v�̃C�e���[�^
* @param	linkId			[in] �����NID
* @param	nodeLinkIDs		[in] �m�[�h�ɕR�t�������N�Q�̏��imap�j
* @param	nodeClassIndex	[in] �m�[�h�����C���f�b�N�X
* @retval	true �R�t�������N�͎w�胊���N�̂�
* @retval	false �w�胊���N�ȊO�̃����N���R�t�����A�����R�t���Ȃ�
*/
bool isOnlyTargetRoadLink(
	const std::map<long, GeoClass>::iterator& nodeMapIt,
	long linkId,
	const NODEIDMAP& nodeLinkIDs,
	long nodeClassIndex );

/**
* @brief	�w��W�I���g���Ɋ֌W���郊���N�t�B�[�`�����m�[�h���Ƌ��Ɏ擾
* @note		features��m_ipTable��m_FieldMap�͗\�ߐݒ肳��Ă���K�v������
* @param	ipGeometry	[in] ��W�I���g��
* @param	fieldList	[in] �擾����t�B�[���h���X�g
* @param	strWhere	[in] �����I��������
* @param	features	[in/out] �t�B�[�`���Q�N���X
* @param	nodeNodeIDs [out] �m�[�hID�ɑΉ�����m�[�hID�̑Ή��\�ifrom/to to/from�j
* @param	nodeLinkIDs [out] �m�[�hID�ɑΉ����郊���NID�̑Ή��\
* @retval	true ����
* @retval	false ���s
*/
bool GetLinkFeaturesWithNodeIdInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features,
	NODEIDMAP& nodeNodeIDs,
	NODEIDMAP& nodeLinkIDs );

/**
* @brief	�w��W�I���g���Ɋ֌W���郊���N�t�B�[�`�����m�[�h���Ƌ��Ɏ擾
* @note		features��m_ipTable��m_FieldMap�͗\�ߐݒ肳��Ă���K�v������
* @param	ipGeometry	[in] ��W�I���g��
* @param	fieldList	[in] �擾����t�B�[���h���X�g
* @param	strWhere	[in] �����I��������
* @param	features	[in/out] �t�B�[�`���Q�N���X
* @param	nodeLinkIDs [out] �m�[�hID�ɑΉ����郊���NID�̑Ή��\
* @retval	true ����
* @retval	false ���s
*/
bool GetLinkFeaturesInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features,
	NODEIDMAP& nodeLinkIDs );

/**
* @brief	�w��W�I���g���Ɋ֌W����t�B�[�`�����擾
* @note		features��m_ipTable��m_FieldMap�͗\�ߐݒ肳��Ă���K�v������
* @param	ipGeometry	[in] ��W�I���g��
* @param	fieldList	[in] �擾����t�B�[���h���X�g
* @param	strWhere	[in] �����I��������
* @param	features	[in/out] �t�B�[�`���Q�N���X
* @retval	true ����
* @retval	false ���s
*/
bool GetFeaturesInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features );

/**
* @brief �w��ID���L���b�V�����猟�����A�Ȃ���΃e�[�u���S�̂��猟������
* @note �usearchEdgeNode�v�O���ėp����������
* @param features [in] ��������t�B�[�`���Q
* @param id [in] ��������I�u�W�F�N�g��OBJECTID
* @return �������ꂽ�m�[�h�̏����i�[����GeoClass
* @retval GeoClass��m_ObjectID��-1 �����Ɏ��s
*/
GeoClass SearchObj( const FeaturesPack& features, long id );


/**
* @brief ���H�����N�͕��s�҃����N��芮�S��v�Ȃ��̌������擾����
* @param [in] checkedLinkIds ���H�����N�Q
* @param [in] walkLinkIds ���s�҃����N�Q
* @param [in] roadLink ���H�����N�̃t�B�[�`���Q�N���X
* @param [in] walkLink ���s�҃����N�̃t�B�[�`���Q�N���X
* @return ���H�����N�͕��s�҃����N��芮�S��v�Ȃ��̌���
*/
long getDifferenceRoadLinkAndWalkLinkCount( std::set<long>checkedLinkIds, std::vector<long>walkLinkIds, const FeaturesPack&roadLink, const FeaturesPack&walkLink);

/**
* @brief �`�F�b�N�ΏۂɌ����Ȃ�|���S��������
* @param [in] geoObject         �`�F�b�N�Ώۂ̏��
* @param [in] pack              �Q�N���X
* @param [in] rtree             RTree
* @param [in] crossObjects      �����Ȃ�|���S���̃��X�g
* @param [in] range             �L����͈�(�P��:m)
* @return  �����Ȃ�|���S���̐�
*/
long findOverlapGeometry(
	const GeoClass& geoObject,
	const FeaturesPack& pack,
	const BGRTree& rtree,
	std::vector<GeoClass>& crossObjects,
	double range = 0
	);
} // util
