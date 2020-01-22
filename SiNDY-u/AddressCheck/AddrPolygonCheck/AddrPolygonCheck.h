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

#include "ErrorObject.h"	
#include "..\TextAddressMaster.h"

/**
* @brief	�t�B�[���h
*/
struct Fields
{
	CString addrCode1;              //!< �Z���R�[�h1
	IGeometryPtr ipGeometry;        //!< �W�I���g��
};


/**
* @brief	�t�B�[���h�O���[�v�L�[
*/
struct FieldGroupKey
{
	CString addrCode1Str;              //!< �Z���R�[�h1
	CString addrCode2Str;              //!< �Z���R�[�h2
	CString cityCodeStr;               //!< �s�撬���R�[�h	
	long gaikufugoCode;                //!< �X�敄��

	FieldGroupKey():gaikufugoCode(INVALID_VALUE){};
	FieldGroupKey(const CString& addrCode1, const CString& addrCode2, const CString& cityCode, long gaikufugo ):addrCode1Str(addrCode1),addrCode2Str(addrCode2),cityCodeStr(cityCode),gaikufugoCode(gaikufugo)
	{};

	/**
	* @brief	���Z�q<�̃I�[�o���[�h
	* @param	[in] fieldGroupKey	     �t�B�[���h�O���[�v�L�[
	* @retval	true:������
	* @retval	false:���܂��͓�����
	*/
	bool operator <(const FieldGroupKey& fieldGroupKey) const
	{
		if( gaikufugoCode < fieldGroupKey.gaikufugoCode )	return true;
		if( gaikufugoCode > fieldGroupKey.gaikufugoCode )	return false;
		if( addrCode1Str.Compare( fieldGroupKey.addrCode1Str ) < 0) return true;
		if( addrCode1Str.Compare( fieldGroupKey.addrCode1Str ) > 0) return false;
		if( addrCode2Str.Compare( fieldGroupKey.addrCode2Str ) < 0) return true;
		if( addrCode2Str.Compare( fieldGroupKey.addrCode2Str ) > 0) return false;
		if( cityCodeStr.Compare( fieldGroupKey.cityCodeStr ) < 0) return true;
		return false;
	}

	/**
	* @brief	���Z�q=�̃I�[�o���[�h
	* @param	[in] fieldGroupKey	     �t�B�[���h�O���[�v�L�[
	* @return	�t�B�[���h�O���[�v�L�[
	*/
	FieldGroupKey &operator =(const FieldGroupKey& fieldGroupKey)
	{
		addrCode1Str = fieldGroupKey.addrCode1Str;
		addrCode2Str = fieldGroupKey.addrCode2Str;
		cityCodeStr = fieldGroupKey.cityCodeStr;
		gaikufugoCode = fieldGroupKey.gaikufugoCode;
	};
};

/**
* @brief	�t�B�[���h�O���[�v�l
*/
struct FieldGroupValue
{
	IGeometryCollectionPtr ipGeometryCollection;   //!< �s�撬���R�[�h�A�Z���R�[�h�A�Z���R�[�h2�A�X�敄���̑g�ݍ��킹���������s���E�|���S�����}�[�W��������
		
	std::map<CString, CString> addrTypeMap;        //!< �Z���������,�I�u�W�F�N�gID
	std::map<long, IGeometryPtr> geoMap;           //!< �`�}�b�v
};

/**
* @brief	�t�B�[���h�O���[�v�L�[
*/
struct FieldGroupKey2
{
	long addrTypeCode;               //!< �Z��������ʃR�[�h
	CString extName1;                //!< �g�����́i���́j
	CString extName2;                //!< �g�����́i�ʏ́j
	FieldGroupKey fieldKey;          //!< ���t�B�[���h
	/**
	 * @brief  �R���X�g���N�^
	 * @param	[in] addrType    //!< �Z��������ʃR�[�h
	 * @param	[in] extName1Str //!< �g�����́i���́j
	 * @param	[in] extName2Str //!< �g�����́i�ʏ́j
	 * @param	[in] fieldGroup  //!< ���t�B�[���h
	 */
	FieldGroupKey2(long addrType, const CString & extName1Str, const CString & extName2Str, const FieldGroupKey& fieldGroup):addrTypeCode(addrType),extName1(extName1Str),extName2(extName2Str),fieldKey(fieldGroup)
	{}

	/**
	* @brief	���Z�q<�̃I�[�o���[�h
	* @param	[in] fieldGroupKey	     �t�B�[���h�O���[�v�L
	* @retval	true:������
	* @retval	false:���܂��͓�����
	*/
	bool operator < (const FieldGroupKey2& fieldGroupKey)const
	{
		if(addrTypeCode < fieldGroupKey.addrTypeCode)return true;
		if(addrTypeCode > fieldGroupKey.addrTypeCode)return false;
		if(extName1.Compare( fieldGroupKey.extName1 ) < 0) return true;
		if(extName1.Compare( fieldGroupKey.extName1 ) > 0) return false;
		if(extName2.Compare( fieldGroupKey.extName2 ) < 0) return true;
		if(extName2.Compare( fieldGroupKey.extName2 ) > 0) return false;
		return fieldKey < fieldGroupKey.fieldKey;
	}
};



/**
 * @class	AddrPolygonCheck
 * @brief	�Z���n�`�F�b�N
 */
class AddrPolygonCheck : public LayerCheckBase
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	AddrPolygonCheck(void){}
	
	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~AddrPolygonCheck(void){}

private:
	/**
	 * @brief	���C���`�F�b�N���擾
	 * @return	���C���`�F�b�N��
	 */
	LPCTSTR getName(){ return addr_polygon::kTableName; }

	/**
	 * @brief	������
	 * @param	[in] settingFile		�ݒ�t�@�C��
	 * @param	[in] tableFinder		�e�[�u���Ǘ�
	 * @param	[in] errorLog           �G���[�t�@�C���Ǘ�
	 * @param	[in] textAddressMaster  �Z���}�X�^�f�[�^�p�[�X
	 * @retval	true:����
	 * @retval	false:���s
	 */
	bool initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster );

	/**
	 * @brief	�`�F�b�N
	 * @param	[in] ulMeshCode		    ���b�V���R�[�h
	 * @param	[in] ipMeshGeometry	    ���b�V���W�I���g��
	 * @param	[in] ulMeshID	    �@�@���b�V���I�u�W�F�N�gID
	 */
	void check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID );

	/**
	* @brief	�d�Ȃ�`�F�b�N
	* @param	[in] ipGeometry	     �W�I���g��
	* @param	[in] ipFeature	     �t�B�[�`��
	* @param	[in] query	         ��������
	* @param	[in] info	         ���
	* @param	[in] oID             �I�u�W�F�N�gID
	*/
	void checkOverlap( const IGeometryPtr& ipGeometry, const IFeaturePtr& ipFeature, const CString& query, const CString& info, long oID );

	/**
	* @brief	���������̌`���W�߂�
	* @param	[in]  oID             �I�u�W�F�N�gID
	* @param	[in]  ipAddressGeometry   �W�I���g��
	* @param	[in]  fieldGroup      �t�B�[���h�O���[�v
	* @param	[in]  addrTypeCode�@�@�Z��������ʃR�[�h	
	* @param	[in]  extName1Str     �g�����́i���́j
	* @param	[in]  extName2Str     �g�����́i�ʏ́j
	* @param	[in]  touchGeoMap     �R���e�i
	*/
	void collectSameAttrGeometry( long oID, const IGeometryPtr& ipAddressGeometry, const FieldGroupKey& fieldGroup, long addrTypeCode, const CString& extName1Str, const CString& extName2Str, map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap );

	/**
	* @brief	�}�X�^����ʂ��Ẵ`�G�b�N
	* @param	[in]  oID                 �I�u�W�F�N�gID
	* @param	[in]  ipAddressGeometry   �W�I���g��
	* @param	[in]  cityCodeStr         �s�撬���R�[�h
	* @param	[in]  addrCodeStr         �Z���R�[�h(����)
	* @param	[in]  extName1Str         �g�����́i���́j
	*/
	void checkWithMasterInfo(long oID, const IGeometryPtr& ipAddressGeometry, const CString& cityCodeStr, const CString& addrCodeStr, const CString& extName1Str);

	/**
	* @brief	�n����\�_�̃`�G�b�N
	* @param	[in]  oID                 �I�u�W�F�N�gID
	* @param	[in]  ipAddressGeometry   �W�I���g��
	* @param	[in]  cityCodeStr         �s�撬���R�[�h
	* @param	[in]  addrCodeStr         �Z���R�[�h(����)
	*/
	void checkRepresentPoint(long oID, const IGeometryPtr& ipAddressGeometry, const CString& cityCodeStr, const CString& addrCodeStr );

	/**
	* @brief	��ђn�̃`�G�b�N
	* @param	[in] ipSpatialReference   ��ԎQ��
	* @param	[in] differentAddrCodeMap �R���e�i
	*/
	void checkEnclave(const ISpatialReferencePtr& ipSpatialReference,  map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap);

	/**
	* @brief	�t�B�[���h���������Z��������ʂ̈قȂ�̃`�G�b�N
	* @param	[in]  oID                �I�u�W�F�N�gID
	* @param	[in]  ipAddressGeometry  �W�I���g��
	* @param	[in]  ipSpatialReference ��ԎQ��
	* @param	[in]  addrTypeStr        �Z��������ʃR�[�h	
	* @param	[in]  addrCodeStr        �Z���R�[�h(����)
	* @param	[in]  addrCode2Str       �Z���R�[�h(�ʏ�)
	* @param	[in]  cityCodeStr        �s�撬���R�[�h
	* @param	[in]  gaikufugo          �X�敄��
	* @param	[out] differentAddrCodeMap        �R���e�i
	*/
	void checkSameAttrAddrTypeC(long oID, IGeometryPtr& ipAddressGeometry, const ISpatialReferencePtr& ipSpatialReference, const CString& addrTypeStr, const CString& addrCodeStr, const CString& addrCode2Str, const CString& cityCodeStr, long gaikufugoCode, map<FieldGroupKey, FieldGroupValue>& differentAddrCodeMap);

	/**
	* @brief	�t�B�[���h�̃`�G�b�N
	* @param	[in]  oID                �I�u�W�F�N�gID
	* @param	[in]  ipAddressGeometry  �W�I���g��
	* @param	[in]  addrType           �Z��������ʃR�[�h	
	* @param	[in]  cityCode           �s�撬���R�[�h
	* @param	[in]  addrCode1          �Z���R�[�h(����)
	* @param	[in]  addrCode2          �Z���R�[�h(�ʏ�)
	* @param	[in]  gaikufugo          �X�敄��
	* @param	[in]  extName1           �g�����́i���́j
	* @param	[in]  extName2           �g�����́i�ʏ́j
	* @param	[in]  extYomi2           �g���ǂ݁i�ʏ́j
	*/
	void checkField(long oID, const IGeometryPtr& ipAddressGeometry, const CComVariant& addrType, const CComVariant& cityCode, const CComVariant& addrCode1, const CComVariant& addrCode2, const CComVariant& gaikufugo, const CComVariant& extName1, const CComVariant& extName2, const CComVariant& extYomi2);

	/**
	* @brief	�֑������̃`�G�b�N
	* @param	[in]  oID                �I�u�W�F�N�gID
	* @param	[in]  ipAddressGeometry  �W�I���g��
	* @param	[in]  extName1           �g�����́i���́j
	*/
	void checkProhibitChar(long oID, const IGeometryPtr& ipAddressGeometry, const CString& extName1Str);

	/**
	* @brief	�t�B�[���h���������s���E���אڂ��Ă���̃`�G�b�N
	* @param	[in]  touchGeoMap        �R���e�i
	*/
	void checkSameAttrShape(const map<FieldGroupKey2, map<long,IGeometryPtr>>& touchGeoMap);

	/**
	* @brief	��Ԍ���
	* @param	[out] ipCursor           �����J�[�\��
	* @param	[in]  ipGeometry         ���b�V���W�I���g��	
	* @retval	true:����
	* @retval	false:���s
	*/
	bool select( IFeatureCursorPtr& ipCursor, const IGeometryPtr& ipGeometry );


	IFeatureClassPtr   m_ipAddressPolygon;        //!< �Z���|���S���t�B�[�`���N���X
	IFeatureClassPtr   m_ipCityPolygon;           //!< �s�s�|���S���t�B�[�`���N���X
	IFeatureClassPtr   m_ipPlaseNameRepPoint;     //!< �n����\�_�|�C���g�t�B�[�`���N���X
	FIELDSMAP          m_addressFieldsMap;        //!< �Z���|���S���t�B�[�`���N���X�̃t�B�[���h�}�b�v

	TextAddressMaster  m_textAddressMaster;       //!< �e�L�X�g�Z���}�X�^
	CString            m_prohibitStr;             //!< �֑�������

	RelationCheck      m_relationCheck;           //!< ���ʂ̊֌W�`�G�b�N
};
