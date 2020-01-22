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

#include "SchemaWriter.h"

#include "AdamSchema.h"
#include "SurveyCSV.h"

namespace adam_v2
{

/**
 * @brief	ADAM�X�L�[�}�������݃N���X
 */
class ADAMSchemaWriter : public SchemaWriter
{
public:
	ADAMSchemaWriter(void);
	~ADAMSchemaWriter(void);

	/**
	 * @brief	�f�[�^�ǂݍ���
	 * @note	���g�p
	 * @param[in]	fileName	CSV�t�@�C����
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool Read(const uh::tstring& fileName)
	{
		return false;
	}
	
	/**
	 * @brief	�f�[�^��������
	 * @param[in]	tableName	ADAM�e�[�u����
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool Write(const uh::tstring& tableName);

	/**
	 * @brief	�f�[�^�N���A
	 */
	void Clear();

	/**
	 * @brief	�f�[�^�Z�b�g
	 * @param[in]	csv		CSV�f�[�^
	 */
	void SetCSV(const SurveyCSVPtr& csv);

	/**
	 * @brief	SRID�Z�b�g
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool SetSpatialReferenceId();

	/**
	 * @brief	�ҏW�J�n/�I��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool StartEdit();
	bool StopEdit();
	bool AbortEdit();

	/**
	 * @brief	�C���|�[�g�ΏۃX�L�[�}�ƒ�`�X�L�[�}�̃o�[�W�����`�F�b�N���s��
	 * @note	ADAM_VERSION �e�[�u��������΃`�F�b�N���{
	 * @retval		true:����
	 * @retval		false:���s
	 */
	bool CheckAdamVersion();

	/**
	 * @brief	�X�L�[�}�ɐڑ�
	 * @retval		true:����
	 * @retval		false:���s
	 */
	bool ConnectDB();

private:
	/**
	 * @brief	DB�֐ڑ����A�C���|�[�g�ۑ�����
	 * @param[in]	func	�֐��|�C���^
	 * @param[in]	tablename	�e�[�u����
	 * @retval		true:����
	 * @retval		false:���s
	 */
	bool Insert(
		bool (ADAMSchemaWriter::*func)(const uh::tstring&), 
		const uh::tstring& tablename
		);

	/**
	 * @brief	�t�B�[�`�����Ƃ�INSERT����
	 * @note	�m�[�h�A�����N�A�m�[�h��̏��ɏ������邱��
	 * @TODO	�t�B�[�`�����ƂɕʃN���X�ɂ���������...
	 * @param[in]	tableName	�e�[�u����
	 * @param[in]	ptr			ADAS-MAP�f�[�^����
	 * @param[in]	joinTxt		�R�t���f�[�^����
	 * @retval		true:����
	 * @retval		false:���s
	 */
	bool InsertLaneLink					(const uh::tstring& tablename);
	bool InsertLaneNode					(const uh::tstring& tablename);
	bool InsertNQLaneNode				(const uh::tstring& tablename);
	bool InsertRelRoadLinkLaneLink		(const uh::tstring& tablename);
	bool InsertLaneLinkGroup			(const uh::tstring& tablename);
	bool InsertCompartLink				(const uh::tstring& tablename);
	bool InsertCompartNode				(const uh::tstring& tablename);
	bool InsertNQCompartNode			(const uh::tstring& tablename);
	bool InsertRelCompartLinkLaneLink	(const uh::tstring& tablename);
	bool InsertBorderLink				(const uh::tstring& tablename);
	bool InsertBorderNode				(const uh::tstring& tablename);
	bool InsertNQBorderNode				(const uh::tstring& tablename);
	bool InsertRelBorderLinkLaneLink	(const uh::tstring& tablename);
	bool InsertSignalPoint				(const uh::tstring& tablename);
	bool InsertRelLaneNodeSignalPoint	(const uh::tstring& tablename);
	bool InsertCrossWalkLine			(const uh::tstring& tablename);
	bool InsertRelLaneNodeCrossWalkLine	(const uh::tstring& tablename);
	bool InsertRoadSignPoint			(const uh::tstring& tablename);
	bool InsertRelLaneNodeRoadSignPoint	(const uh::tstring& tablename);
	bool InsertVirtualCompartLine		(const uh::tstring& tablename);
	bool InsertCoveringArea				(const uh::tstring& tablename);

	/**
	 * @brief	3�����W�I���g���̍쐬
	 * @param[in]	loc			�ʒu���(���̓f�[�^shape)
	 * @return	ip_polygon		3�����W�I���g��(���s����nullptr)
	 * @note	��ԎQ�Ǝ擾���t�B�[�`���͉��L�Œ�
	 *			�|�C���g	LANE_NODE
	 *			�|�����C��	LANE_LINK
	 * @return	�W�I���g��
	 */
	IPointPtr CreatePointZ(const Location& loc);
	IPointPtr CreatePointZ(const Point& loc);
	IPolylinePtr CreatePolylineZ(const std::vector<Location>& loc);
	IPolylinePtr CreatePolylineZ(const Polyline& loc);
	IPolygonPtr CreatePolygonZ(const std::vector<Location>& loc);
	IPolygonPtr CreatePolygonZ(const Polygon& loc);

	/**
	 * @brief	�ҏW�������̐ݒ�
	 * @param[in]	adamSchema		AdamiPCFeature/TableClass
	 * @param[in]	ipRow			IRowBufferPtr
	 */
	void SetModifyData(const AdamSchema::AdamiPCFeature& adamSchema, IRowBufferPtr ipRow);
	void SetModifyData(const AdamSchema::TableClass& adamSchema, IRowBufferPtr ipRow);


	/**
	 * @brief	���[�h�I�����[���[�h�g�p�v���Z�X
	 * @param[in]	ipT		�e�[�u��
	 */
	void StartProcess(ITablePtr& ipT);
	static void FinishProcess(ITablePtr& ipT);

	/**
	 * @brief	�X�L�[�}�̃��b�N/�A�����b�N����
	 * @note	IFeatureClassLoad::LoadOnlyMode()���g�����́A�X�L�[�}���b�N����K�v������
	 * @param[in]	ipT		�e�[�u��
	 * @param[in]	eStatus	���b�N/�A�����b�N�iesriSharedSchemaLock:���b�N, esriExclusiveSchemaLock:�A�����b�N�j
	 */
	static void ChangeSchemeLock(ITablePtr& ipT, esriSchemaLock eStatus);

	/**
	 * @brief	���[�h�I�����[���[�h��ON/OFF����
	 * @detail	IFeatureClassLoad::LoadOnlyMode()���g�p���A
	 *			�C���|�[�g���ɔ�������u��ԃC���f�b�N�X �O���b�h �T�C�Y������������܂���B�v�G���[���������
	 * @note	uh::scope_guard�Ƒg�ݍ��킹�āA�G���[�n���h�����O�O�Ńc�[���������Ă�OFF�ɂł���悤�ɂ��Ă���
	 *			�e�[�u���̃��R�[�h�ǉ��ɂ��Ή����Ă��邪�ALoadOnlyMode()��SDE�ł̓t�B�[�`���[�N���X�݂̂Ɏg�p�\�Ȃ̂ŁA
	 *			�c�[����SDE�Ŏg���邱�Ƃ��z�肵�āA�e�[�u���̍ۂɂ͎g���Ă��Ȃ�
	 * @param[in]	ipT		�e�[�u��
	 * @param[in]	vbOnOff	ON/OFF�iVARIANT_TRUE:ON, VARIANT_FALSE:OFF�j
	 */
	static void ChangeLoadOnlyMode(ITablePtr& ipT, VARIANT_BOOL vbOnOff);

private:
	AdamSchemaPtr	_schema;				//!< �X�L�[�}
	SurveyCSVPtr	_csv;					//!< ���̓f�[�^
};
typedef std::shared_ptr<ADAMSchemaWriter> ADAMSchemaWriterPtr;

};	//namespace adam_v2