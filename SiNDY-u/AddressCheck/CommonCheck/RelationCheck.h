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
class LayerCheckBase;

/**
 * @class	RelationCheck
 * @brief	�֌W�`�F�b�N�N���X
 */
class RelationCheck
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	RelationCheck(void){}

	 /**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~RelationCheck(void){}

	/**
	* @brief	�����`�F�b�N
	* @param	[in]ipMeshGeometry	�@�@���b�V���W�I���g��
	* @param	[in]ipGeometryCollection citypolygon�W�I���g���R���N�V����
	* @param	[in]ipSpatialReference�@��ԎQ��
	* @param	[in]errorInfo           ���ꂽ�G���[���
	* @param	[in]layerCheck          ���C���`�G�b�N
	* @param	[in]errorInfoNoPolygonInMesh �G���[���(���b�V�����ɍs���E�|���S�����Ȃ�)
	* @param	[in]meshName            ���b�V����
	* @param	[in]meshId              ���b�V���I�u�W�F�N�gID
	* @retval	true:����
	* @retval	false:���s
	*/
	bool checkOutArea( const IGeometryPtr& ipMeshGeometry, IGeometryCollectionPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference, const ErrorIdInfo&errorInfo, LayerCheckBase* layerCheck, const ErrorIdInfo& errorInfoNoPolygonInMesh, const CString& meshName, long meshId )const;
};

