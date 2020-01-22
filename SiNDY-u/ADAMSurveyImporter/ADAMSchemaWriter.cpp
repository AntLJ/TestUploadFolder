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

#include "stdafx.h"

#include <sindy/workspace.h>
#include <TDC/useful_headers/scope_guard.h>
#include "LogUtil.h"

//�����[�U���擾�p
#define SECURITY_WIN32
#include <sspi.h>
#include <security.h>
//��

#include "ADAMSchemaWriter.h"

using namespace adam_v2;

ADAMSchemaWriter::ADAMSchemaWriter(void)
{
	_schema = std::make_shared<AdamSchema>();
}


ADAMSchemaWriter::~ADAMSchemaWriter(void)
{
}

void ADAMSchemaWriter::Clear(void)
{
	//_workspace = nullptr;
	SchemaWriter::Clear();

	_schema->Clear();
	_csv->Clear();
}

void ADAMSchemaWriter::SetCSV(const SurveyCSVPtr& csv)
{
	_csv = csv;
}

bool ADAMSchemaWriter::SetSpatialReferenceId()
{
	if(!_csv->listSpatialRef.size() == 1)
		return false;

	return SetSRID(_csv->listSpatialRef.begin()->srid.GetValue());
}

bool ADAMSchemaWriter::StartEdit()
{
	if(!_workspace)
		return false;

	return SchemaWriter::StartEdit(IWorkspaceEditPtr(_workspace));
}
bool ADAMSchemaWriter::StopEdit()
{
	if(!_workspace)
		return false;

	return SchemaWriter::StopEdit(IWorkspaceEditPtr(_workspace));
}
bool ADAMSchemaWriter::AbortEdit()
{
	if(!_workspace)
		return false;

	return SchemaWriter::AbortEdit(IWorkspaceEditPtr(_workspace));
}

bool ADAMSchemaWriter::Write(const uh::tstring& tableName)
{
	LogUtil::printDoubleLine();
	LogUtil::print("DB�ւ̃f�[�^�C���|�[�g���J�n���܂�");

	bool ret = true;

	try
	{
		//�C���|�[�g����
		//�m�[�h�A�����N�A�m�[�h��A�֘A�e�[�u�� �̏��ɏ���

			 if( tableName == AdamSchema::LaneLinkGroup::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertLaneLinkGroup,				tableName );	// �Ԑ������N�O���[�v
		else if( tableName == AdamSchema::LaneNode::TableName )					ret &=	Insert( &ADAMSchemaWriter::InsertLaneNode,					tableName );	// �Ԑ��m�[�h
		else if( tableName == AdamSchema::LaneLink::TableName )					ret &=	Insert( &ADAMSchemaWriter::InsertLaneLink,					tableName );	// �Ԑ������N
		else if( tableName == AdamSchema::NQLaneNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertNQLaneNode,				tableName );	// �Ԑ��m�[�h��
		else if( tableName == AdamSchema::RelRoadLinkLaneLink::TableName )				Insert( &ADAMSchemaWriter::InsertRelRoadLinkLaneLink,		tableName );	// �Ԑ��֘A
		else if( tableName == AdamSchema::CompartNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertCompartNode,				tableName );	// �����m�[�h
		else if( tableName == AdamSchema::CompartLink::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertCompartLink,				tableName );	// ���������N
		else if( tableName == AdamSchema::NQCompartNode::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertNQCompartNode,				tableName );	// �����m�[�h��
		else if( tableName == AdamSchema::RelCompartLinkLaneLink::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelCompartLinkLaneLink,	tableName );	// �����֘A
		else if( tableName == AdamSchema::BorderNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertBorderNode,				tableName );	// ���E���m�[�h
		else if( tableName == AdamSchema::BorderLink::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertBorderLink,				tableName );	// ���E�������N
		else if( tableName == AdamSchema::NQBorderNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertNQBorderNode,				tableName );	// ���E���m�[�h��
		else if( tableName == AdamSchema::RelBorderLinkLaneLink::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelBorderLinkLaneLink,		tableName );	// ���E���֘A
		else if( tableName == AdamSchema::SignalPoint::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertSignalPoint,				tableName );	// �M���@
		else if( tableName == AdamSchema::RelLaneNodeSignalPoint::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelLaneNodeSignalPoint,	tableName );	// �M���@�֘A
		else if( tableName == AdamSchema::RoadSignPoint::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertRoadSignPoint,				tableName );	// ���H�W��
		else if( tableName == AdamSchema::RelLaneNodeRoadSignPoint::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelLaneNodeRoadSignPoint,	tableName );	// ���H�W���֘A
		else if( tableName == AdamSchema::CrossWalkLine::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertCrossWalkLine,				tableName );	// ���f����
		else if( tableName == AdamSchema::RelLaneNodeCrossWalkLine::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelLaneNodeCrossWalkLine,	tableName );	// ���f�����֘A
		else if( tableName == AdamSchema::VirtualCompartLine::TableName )				Insert( &ADAMSchemaWriter::InsertVirtualCompartLine,		tableName );	// ���z�������N
		else if( tableName == AdamSchema::CoveringArea::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertCoveringArea,				tableName );	// ������
		else _ASSERT( false );

		if(!ret)
		{
			LogUtil::printError("�C���|�[�g�����Ɏ��s���܂����B�ҏW��j�����܂�");
		}
	}
	catch(const _com_error& e)
	{
		LogUtil::printError("�ُ�I�����܂���");
		LogUtil::printError("��O���o\t" +  uh::toStr(atl2::stringize(e)));
		LogUtil::printError("�ҏW��j�����܂�");
		LogUtil::printError("�C���|�[�g�����Ɏ��s���܂����B���O���m�F���Ă�������");
		return false;
	}
	catch(const std::exception& e)
	{
		LogUtil::printError("�ُ�I�����܂���");
		LogUtil::printError("��O���o\t" +  uh::toStr(e.what()));
		LogUtil::printError("�ҏW��j�����܂�");
		LogUtil::printError("�C���|�[�g�����Ɏ��s���܂����B���O���m�F���Ă�������");
		return false;
	}
	catch(...)
	{
		LogUtil::printError("�ُ�I�����܂���");
		LogUtil::printError("��O���o");
		LogUtil::printError("�ҏW��j�����܂�");
		LogUtil::printError("�C���|�[�g�����Ɏ��s���܂����B���O���m�F���Ă�������");
		return false;
	}

	if(!ret)
	{
		LogUtil::printError("�C���|�[�g�����Ɏ��s���܂����B���O���m�F���Ă�������");
	}	
	else
	{
		LogUtil::print("�C���|�[�g�����ɐ������܂���");
	}

	LogUtil::printDoubleLine();

	return ret;
}

bool ADAMSchemaWriter::ConnectDB()
{
	LogUtil::print("DB�ւ̐ڑ����m�F���܂�");

	bool ret = true;

	//���[�N�X�y�[�X�ڑ�
	if(!SetWorkspace())
	{
		ret = false;
	}
	if(!_schema)
	{
		ret = false;
	}
	if(!_workspace)
	{
		ret = false;
	}

	//���p�\���ADAMv2�ɑS���ڑ�
	ret &= SetFeature(	AdamSchema::LaneLink::TableName,					&(_schema->featureLaneLink)					);
	ret &= SetFeature(	AdamSchema::LaneNode::TableName,					&(_schema->featureLaneNode)					);
	ret &= SetFeature(	AdamSchema::CompartLink::TableName,					&(_schema->featureCompartLink)				);
	ret &= SetFeature(	AdamSchema::CompartNode::TableName,					&(_schema->featureCompartNode)				);
	ret &= SetFeature(	AdamSchema::BorderLink::TableName,					&(_schema->featureBorderLink)				);
	ret &= SetFeature(	AdamSchema::BorderNode::TableName,					&(_schema->featureBorderNode)				);
	ret &= SetFeature(	AdamSchema::SignalPoint::TableName,					&(_schema->featureSignalPoint)				);
	ret &= SetFeature(	AdamSchema::CrossWalkLine::TableName,				&(_schema->featureCrossWalkLine)			);
	ret &= SetFeature(	AdamSchema::RoadSignPoint::TableName,				&(_schema->featureRoadSignPoint)			);
	ret &= SetFeature(	AdamSchema::CoveringArea::TableName,				&(_schema->featureCoveringArea)				);
	SetFeature(	AdamSchema::VirtualCompartLine::TableName,					&(_schema->featureVirtualCompartLine)		);

	ret &= SetTable(	AdamSchema::RelRoadLinkLaneLink::TableName,			&(_schema->tableRelRoadLinkLaneLink)		);
	ret &= SetTable(	AdamSchema::LaneLinkGroup::TableName,				&(_schema->tableLaneLinkGroup)				);
	ret &= SetTable(	AdamSchema::RelCompartLinkLaneLink::TableName,		&(_schema->tableRelCompartLinkLaneLink)		);
	ret &= SetTable(	AdamSchema::RelBorderLinkLaneLink::TableName,		&(_schema->tableRelBorderLinkLaneLink)		);
	ret &= SetTable(	AdamSchema::RelLaneNodeSignalPoint::TableName,		&(_schema->tableRelLaneNodeSignalPoint)		);
	ret &= SetTable(	AdamSchema::RelLaneNodeCrossWalkLine::TableName,	&(_schema->tableRelLaneNodeCrossWalkLine)	);
	ret &= SetTable(	AdamSchema::RelLaneNodeRoadSignPoint::TableName,	&(_schema->tableRelLaneNodeRoadSignPoint)	);
	ret &= SetTable(	AdamSchema::NQLaneNode::TableName,					&(_schema->tableNQLaneNode)					);
	ret &= SetTable(	AdamSchema::NQCompartNode::TableName,				&(_schema->tableNQCompartNode)				);
	ret &= SetTable(	AdamSchema::NQBorderNode::TableName,				&(_schema->tableNQBorderNode)				);
	
	if(ret)
	{
		LogUtil::print("���ׂẴt�B�[�`���ڑ��ɐ������܂���");
	}
	else
	{
		LogUtil::print("�t�B�[�`���ڑ��Ɏ��s���܂����B���O���m�F���Ă�������");
	}

	return ret;
}

bool ADAMSchemaWriter::Insert(
		bool (ADAMSchemaWriter::*func)(const uh::tstring&), 
		const uh::tstring& tablename
		)
{
	LogUtil::print(uh::toStr( tablename ) + " �̃C���|�[�g���J�n���܂�...");

	bool ret = (this->*func)(tablename);

	if(!ret)
	{
		LogUtil::printError("�C���|�[�g�Ɏ��s���܂����B���O���m�F���Ă�������\t" + uh::toStr( tablename ));
	}
	else
	{
		LogUtil::print("����ɃC���|�[�g���܂���\t" + uh::toStr( tablename ));
	}

	LogUtil::print(uh::toStr( tablename ) + " �̃C���|�[�g���I�����܂�...");

	return ret;
}

bool ADAMSchemaWriter::InsertLaneLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�Ԑ������N
	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureLaneLink))
	{
		assert(false);
		return false;
	}
	AdamSchema::LaneLink index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	std::map<long long, OGID> dicRegNode;
	{
		//�Ԑ��m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegGroup;
	{
		//�Ԑ������N�O���[�v��
		ITablePtr ipGroupFC;
		if(!(ipGroupFC = _schema->tableLaneLinkGroup))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegGroup, ipGroupFC, _T("SOURCEID"), _T("OBJECTID"), _T(""));
	}

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	IFeatureCursorPtr ipFCursor;
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}


	long count = 0;
	long listSize = _csv->listLaneLink.size();
	//�f�[�^�C���|�[�g
	//��������̂��ɂ����
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listLaneLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID�������SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(100, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "�Ԑ������NSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^
		//FROM_NODE_* �͂���Γ����
		auto fFromNode = dicRegNode.find(csv.from_node_id.GetValue());
		uh::tstring from_node_gid = (fFromNode != dicRegNode.end()) ? fFromNode->second.GLOBALID : nullStr;
		long long   from_node_oid = (fFromNode != dicRegNode.end()) ? fFromNode->second.OBJECTID : nullInt;
		if(from_node_gid != nullStr && from_node_oid != nullInt)
		{
			SetValue(buf, index.from_node_gid.index, from_node_gid, nullStr);
			SetValue(buf, index.from_node_id.index, from_node_oid, 0LL);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(101, "�O���L�[�`�F�b�N", "�n�_�m�[�hID�����݂��܂���", "�Ԑ������NSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�n�_�m�[�hSID(from_node_id):" + std::to_string(csv.from_node_id.GetValue()));
			continue;
		}
		//TO_NODE_* �͂���Γ����
		auto fToNode = dicRegNode.find(csv.to_node_id.GetValue());
		uh::tstring to_node_gid = (fToNode != dicRegNode.end()) ? fToNode->second.GLOBALID : nullStr;
		long long   to_node_oid = (fToNode != dicRegNode.end()) ? fToNode->second.OBJECTID : nullInt;
		if(to_node_gid != nullStr && to_node_oid != nullInt)
		{
			SetValue(buf, index.to_node_gid.index, to_node_gid, nullStr);
			SetValue(buf, index.to_node_id.index, to_node_oid, 0LL);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(102, "�O���L�[�`�F�b�N", "�I�_�m�[�hID�����݂��܂���", "�Ԑ������NSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�I�_�m�[�hSID(to_node_id):" + std::to_string(csv.to_node_id.GetValue()));
			continue;
		}
		//���݂��Ȃ��ꍇ��NULL(���݂��Ȃ��ꍇ GetOID��-1�ŕԂ��Ă���̂ŕϊ�)
		auto fGroup = dicRegGroup.find(csv.lane_group_id.GetValue());
		long long gid = (fGroup != dicRegGroup.end()) ? fGroup->second.OBJECTID : nullInt;	//LANE_GROUP_ID
		if(gid == nullInt)
		{
			SetNULL(buf, index.lane_group_id.index);
		}
		else
		{
			SetValue(buf, index.lane_group_id.index, gid, 0LL);	
		}
		SetValue(buf, index.lane_number.index, csv.lane_no, 0);	//LANE_NUMBER
		SetValue(buf, index.lane_count.index, csv.lane_count, 0);	//LANE_COUNT
		SetValue(buf, index.max_legal_speed_c.index, csv.max_legal_speed_c, nullInt);	//MAX_LEGAL_SPEED_C
		SetValue(buf, index.min_legal_speed_c.index, csv.min_legal_speed_c, nullInt);	//MIN_LEGAL_SPEED_C
		SetValue(buf, index.travel_direction_c.index, csv.travel_direction_c, nullInt);	//TRAVEL_DIRECTION_C
		SetValue(buf, index.road_structure_c.index, csv.road_structure_c, nullInt);	//ROAD_STRUCTURE_C

		//ONEWAY_C
		if(csv.both_link_direction_f.GetIsNull())
		{
			//NULL�s�Ȃ̂� �����l��ݒ�
			SetValue(buf, index.oneway_c.index, nullInt, nullInt);
		}
		else
		{
			//ONEWAY_C�͕ϊ����Ċi�[
			//@NOTE	0/1�ȊO�𗘗p����ꍇ�́A�I�[�T�����O�c�[���Ɨv����(�t�����͍l�����Ă��Ȃ�)
			int convertOnewayC = -1;
			switch(csv.both_link_direction_f.GetValue())
			{
			case 0:	//�����Ȃ�����ʍs�i�������j
				convertOnewayC = 0;
				break;
			case 1:	//����ʍs�Ȃ�
				convertOnewayC = 1;
				break;
			}
			SetValue(buf, index.oneway_c.index, convertOnewayC, nullInt);	//ONEWAY_C
		}	
		SetValue(buf, index.left_change_f.index, csv.left_change_f, 0);	//LEFT_CHANGE_F
		SetValue(buf, index.right_change_f.index, csv.right_change_f, 0);	//RIGHT_CHANGE_F
		SetValue(buf, index.left_protrusion_f.index, csv.left_protrusion_f, 0);	//LEFT_PROTRUSION_F
		SetValue(buf, index.right_protrusion_f.index, csv.right_protrusion_f, 0);	//RIGHT_PROTRUSION_F
		SetValue(buf, index.rubbing_f.index, csv.rubbing_f, 0);	//RUBBING_F
		SetValue(buf, index.tunnel_f.index, csv.tunnel_f, 0);	//TUNNEL_F
			
		//�`��ݒ�
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif
	
	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertLaneNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureLaneNode))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::LaneNode index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listLanePoint.size();

	//�f�[�^�C���|�[�g
	//�m�[�h�t�B�[�`��(�d���|�C���g�̓C���|�[�g���Ȃ�)
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listLanePoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.node_id.GetValue()) != listRegSID.end())
		{
			//�d���͂���
			LogUtil::printDebug("�d���I�u�W�F�N�g�̂���SKIP���܂�\t�Ԑ��|�C���gSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ��m�[�hSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.node_id, 0LL);	//SOURCEID ��csv.object_id�ł͂Ȃ�csv.node_id
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		SetValue(buf, index.nodeclass_c.index, csv.point_class_c, nullInt);	//NODECLASS_C
		SetValue(buf, index.stop_point_f.index, csv.stop_point_f, 0);	//STOP_POINT_F
		SetValue(buf, index.covering_f.index, csv.cover_f, 0);	//COVERING_F

		//�`��ݒ�
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.node_id.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertNQLaneNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	//�Ԑ��m�[�h��
	ITablePtr ipT;
	if(!(ipT = _schema->tableNQLaneNode))
	{
		assert(false);
		return false;
	}
	AdamSchema::NQLaneNode index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegNode;
	{
		//�Ԑ��m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegLink;
	{
		//�Ԑ������N
		IFeatureClassPtr ipLinkFC;
		if(!(ipLinkFC = _schema->featureLaneLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLink, ipLinkFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listLanePoint.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	//�m�[�h��e�[�u��(�d���|�C���g���C���|�[�g����)
	for(const auto& csv: _csv->listLanePoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(100, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "�Ԑ��|�C���gSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//NQ_LANE_NODE.LANE_NODE_GID
		//LANE_NODE����GID�擾
		auto fNode = dicRegNode.find(csv.node_id.GetValue());
		uh::tstring node_gid = (fNode != dicRegNode.end()) ? fNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(152, "�O���L�[�`�F�b�N", "�Ԑ��m�[�hID�����݂��܂���", "�Ԑ��|�C���gSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ��m�[�hSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}
		//NQ_LANE_NODE.LANE_NODE_GID
		//LANE_LINK����GID�擾
		auto fLink = dicRegLink.find(csv.lane_link_id.GetValue());
		uh::tstring link_gid = (fLink != dicRegLink.end()) ? fLink->second.GLOBALID : nullStr;
		if(link_gid != nullStr)
		{
			SetValue(buf, index.lane_link_gid.index, link_gid, nullStr);	
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(141, "�O���L�[�`�F�b�N", "�Ԑ������NID�����݂��܂���", "�Ԑ��|�C���gSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ������NSID(lane_link_id):" + std::to_string(csv.lane_link_id.GetValue()));
			continue;
		}
		
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//NQ_LANE_NODE.SEQUENCE
		SetValue(buf, index.lastnode_f.index, csv.lastnode_f, 0);	//NQ_LANE_NODE.LASTNODE_F
		SetValue(buf, index.width.index, csv.width, 0.0);	//WIDTH
		SetValue(buf, index.linear_element_c.index, csv.linear_element_c, nullInt);	//LINEAR_ELEMENT_C
		SetValue(buf, index.curvature.index, csv.curvature, 0.0);	//CURVATURE
		SetValue(buf, index.curvature_radius.index, csv.curvature_radius, 0.0);	//CURVATURE_RADIUS
		SetValue(buf, index.curvature_change_rate.index, csv.curvature_change_rate, 0.0);	//CURVATURE_CHANGE_RATE
		SetValue(buf, index.clothoid_parameter.index, csv.clothoid_parameter, 0.0);	//CLOTHOID_PARAMETER
		SetValue(buf, index.longitudinal_slope.index, csv.longitudinal_slope, 0.0);	//LONGITUDINAL_SLOPE
		SetValue(buf, index.cross_slope.index, csv.cross_slope, 0.0);	//CROSS_SLOPE

		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif
	
	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

//�C���|�[�g�ΏۊO
bool ADAMSchemaWriter::InsertRelRoadLinkLaneLink(const uh::tstring& tablename)
{
	return true;
}

bool ADAMSchemaWriter::InsertLaneLinkGroup(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableLaneLinkGroup))
	{
		assert(false);
		return false;
	}
	_ICursorPtr ipTCursor;
	if(!ipT)
	{
		assert(false);
		return false;
	}
	AdamSchema::LaneLinkGroup index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	//�C���T�[�g�J�[�\���ݒ�
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listLaneLink.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listLaneLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ݒ��SKIP
		if(csv.lane_group_id.GetIsNull())
		{
			continue;
		}

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.lane_group_id.GetValue()) != listRegSID.end())
		{
			//�d���͂���
			LogUtil::printDebug("�d���I�u�W�F�N�g�̂���SKIP���܂�\tSID(lane_group_id):" + std::to_string(csv.lane_group_id.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		//SetGlobalId(buf, index.globalid.index);						//GLOBALID
		SetValue(buf, index.sourceid.index, csv.lane_group_id, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//�㉺�������N�O���[�v�͑ΏۊO�̂���NULL�w��
		SetNULL(buf, index.updown_link_group_id.index);				//LANE_LINK_GROUP.UPDOWN_LINK_GROUP_ID
		SetValue(buf, index.group_c.index, csv.lane_group_c, nullInt);	//LANE_LINK_GROUP.GROUP_C

		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.lane_group_id.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertCompartLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCompartLink))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CompartLink index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	std::map<long long, OGID> dicRegNode;
	{
		//�����m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureCompartNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}


	long count = 0;
	long listSize = _csv->listCompartLink.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCompartLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(200, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���������NSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//FROM_NODE_* �͂���Γ����
		auto fFromNode = dicRegNode.find(csv.from_node_id.GetValue());
		uh::tstring from_node_gid = (fFromNode != dicRegNode.end()) ? fFromNode->second.GLOBALID : nullStr;
		long long   from_node_oid = (fFromNode != dicRegNode.end()) ? fFromNode->second.OBJECTID : -1LL;
		if(from_node_gid != nullStr && from_node_oid != nullInt)
		{
			SetValue(buf, index.from_node_gid.index, from_node_gid, nullStr);
			SetValue(buf, index.from_node_id.index, from_node_oid, 0LL);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(201, "�O���L�[�`�F�b�N", "�n�_�m�[�hID�����݂��܂���", "�Ԑ������NSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�n�_�m�[�hSID(from_node_id):" + std::to_string(csv.from_node_id.GetValue()));
			continue;
		}
		//TO_NODE_* �͂���Γ����
		auto fToNode = dicRegNode.find(csv.to_node_id.GetValue());
		uh::tstring to_node_gid = (fToNode != dicRegNode.end()) ? fToNode->second.GLOBALID : nullStr;
		long long   to_node_oid = (fToNode != dicRegNode.end()) ? fToNode->second.OBJECTID : -1LL;
		if(to_node_gid != nullStr && to_node_oid != nullInt)
		{
			SetValue(buf, index.to_node_gid.index, to_node_gid, nullStr);
			SetValue(buf, index.to_node_id.index, to_node_oid, 0LL);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(202, "�O���L�[�`�F�b�N", "�I�_�m�[�hID�����݂��܂���", "�Ԑ������NSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�I�_�m�[�hSID(to_node_id):" + std::to_string(csv.to_node_id.GetValue()));
			continue;
		}
		SetValue(buf, index.compart_line_c.index, csv.compart_line_c, nullInt);	//COMPART_LINE_C
		SetValue(buf, index.compart_style_c.index, csv.compart_style_c, nullInt);	//COMPART_STYLE_C

		SetValue(buf, index.slowdown_label_c.index, csv.slowdown_label_c, nullInt);	//SLOWDOWN_LABEL_C

		SetValue(buf, index.installation_f.index, csv.installation_f, 0);	//INSTALLATION_F

		//�`��ݒ�
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertCompartNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCompartNode))
	{
		assert(false);
		return false;
	}
	//Search��Insert�p��Cursor�͕�����
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CompartNode index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCompartPoint.size();

	//�f�[�^�C���|�[�g
	//�m�[�h(�d�����Ȃ��|�C���g)���Ώ�
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCompartPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.node_id.GetValue()) != listRegSID.end())
		{
			//�d���͂���
			LogUtil::printDebug("�d���I�u�W�F�N�g�̂���SKIP���܂�\t�����|�C���gSID(objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�����m�[�hSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.node_id, 0LL);	//SOURCEID	��csv.object_id�ł͂Ȃ�csv.node_id
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		SetValue(buf, index.nodeclass_c.index, csv.point_class_c, nullInt);	//NODECLASS_C

		//�`��ݒ�
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.node_id.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertNQCompartNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableNQCompartNode))
	{
		assert(false);
		return false;
	}
	AdamSchema::NQCompartNode index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegNode;
	{
		//�����m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureCompartNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegLink;
	{
		//���������N
		IFeatureClassPtr ipLinkFC;
		if(!(ipLinkFC = _schema->featureCompartLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLink, ipLinkFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}



	long count = 0;
	long listSize = _csv->listCompartPoint.size();

	//�f�[�^�C���|�[�g
	//�|�C���g(�d�������|�C���g)���Ώ�
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCompartPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(240, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "�����|�C���gSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//NQ_COMPART_NODE.GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//NQ_COMPART_NODE.SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//NQ_COMPART_NODE.OBJECTID ��ArcObject�����t�^

		//NQ_COMPART_NODE.COMPART_NODE_GID
		//COMPART_NODE ����GID�擾
		auto fNode = dicRegNode.find(csv.node_id.GetValue());
		uh::tstring node_gid = (fNode != dicRegNode.end()) ? fNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.compart_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(247, "�O���L�[�`�F�b�N", "�����m�[�hID�����݂��܂���", "�����|�C���gSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�����m�[�hSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}
		//NQ_COMPART_NODE.COMPART_LINK_GID
		//COMPART_LINK����GID�擾
		auto fLink = dicRegLink.find(csv.compart_link_id.GetValue());
		uh::tstring link_gid = (fLink != dicRegLink.end()) ? fLink->second.GLOBALID : nullStr;
		if(link_gid != nullStr)
		{
			SetValue(buf, index.compart_link_gid.index, link_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(241, "�O���L�[�`�F�b�N", "���������NID�����݂��܂���", "�����|�C���gSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t���������NSID(compart_link_id):" + std::to_string(csv.compart_link_id.GetValue()));
			continue;
		}
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);		//NQ_COMPART_NODE.SEQUENCE
		SetValue(buf, index.lastnode_f.index, csv.lastnode_f, 0);	//NQ_COMPART_NODE.LASTNODE_F
		SetValue(buf, index.width.index, csv.width, 0.0);	//WIDTH
		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertRelCompartLinkLaneLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelCompartLinkLaneLink))
	{
		assert(false);
		return false;
	}
	_ICursorPtr ipTCursor;
	if(!ipT)
	{
		assert(false);
		return false;
	}
	AdamSchema::RelCompartLinkLaneLink index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//�Ԑ������N
		IFeatureClassPtr ipLaneLinkFC;
		if(!(ipLaneLinkFC = _schema->featureLaneLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipLaneLinkFC);
	}

	std::map<long long, OGID> dicRegCompart;
	{
		//���������N
		IFeatureClassPtr ipCompartLinkFC;
		if(!(ipCompartLinkFC = _schema->featureCompartLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegCompart, ipCompartLinkFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelCompartLink.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelCompartLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(280, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "�����֘ASID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);		//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//LANE_LINK_GID
		//LANE_LINK ����GID�擾
		auto fLaneLink = dicRegLane.find(csv.lane_link_id.GetValue());
		uh::tstring lane_gid = (fLaneLink != dicRegLane.end()) ? fLaneLink->second.GLOBALID : nullStr;
		if(lane_gid != nullStr)
		{
			SetValue(buf, index.lane_link_gid.index, lane_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(281, "�O���L�[�`�F�b�N", "�Ԑ������NID�����݂��܂���", "�����֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ������NSID(object_id):" + std::to_string(csv.lane_link_id.GetValue()));
			continue;
		}
		//COMPART_LINK_GID
		//COMPART_LINK ����GID�擾
		auto fCompartLink = dicRegCompart.find(csv.compart_link_id.GetValue());
		uh::tstring comp_gid = (fCompartLink != dicRegCompart.end()) ? fCompartLink->second.GLOBALID : nullStr;
		if(comp_gid != nullStr)
		{
			SetValue(buf, index.compart_link_gid.index, comp_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(282, "�O���L�[�`�F�b�N", "���������NID�����݂��܂���", "�����֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t���������NSID(object_id):" + std::to_string(csv.compart_link_id.GetValue()));
			continue;
		}

		SetValue(buf, index.lr_c.index, csv.lr_f, nullInt);	//LR_C
		SetValue(buf, index.compart_link_direction_c.index, csv.compart_link_direction_c, nullInt);	//COMPART_LINK_DIRECTION_C
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//SEQUENCE

		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}


bool ADAMSchemaWriter::InsertBorderLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureBorderLink))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::BorderLink index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	std::map<long long, OGID> dicRegNode;
	{
		//���E���m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureBorderNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listBorderLink.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listBorderLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(300, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���E�������NSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//FROM_NODE_* �͂���Γ����
		auto fFromNode = dicRegNode.find(csv.from_node_id.GetValue());
		uh::tstring from_node_gid = (fFromNode != dicRegNode.end()) ? fFromNode->second.GLOBALID : nullStr;
		long long   from_node_oid = (fFromNode != dicRegNode.end()) ? fFromNode->second.OBJECTID : -1LL;
		if(from_node_gid != nullStr && from_node_oid != nullInt)
		{
			SetValue(buf, index.from_node_gid.index, from_node_gid, nullStr);
			SetValue(buf, index.from_node_id.index, from_node_oid, 0LL);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(301, "�O���L�[�`�F�b�N", "�n�_�m�[�hID�����݂��܂���", "���E�������NSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�n�_�m�[�hSID(from_node_id):" + std::to_string(csv.from_node_id.GetValue()));
			continue;
		}
		//TO_NODE_* �͂���Γ����
		auto fToNode = dicRegNode.find(csv.to_node_id.GetValue());
		uh::tstring to_node_gid = (fToNode != dicRegNode.end()) ? fToNode->second.GLOBALID : nullStr;
		long long   to_node_oid = (fToNode != dicRegNode.end()) ? fToNode->second.OBJECTID : -1LL;
		if(to_node_gid != nullStr && to_node_oid != nullInt)
		{
			SetValue(buf, index.to_node_gid.index, to_node_gid, nullStr);
			SetValue(buf, index.to_node_id.index, to_node_oid, 0LL);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(302, "�O���L�[�`�F�b�N", "�I�_�m�[�hID�����݂��܂���", "���E�������NSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�I�_�m�[�hSID(to_node_id):" + std::to_string(csv.to_node_id.GetValue()));
			continue;
		}
		SetValue(buf, index.imaginary_border_f.index, csv.imaginary_boder_f, 0);//IMAGINARY_BODER_F

		//�`��ݒ�
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertBorderNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureBorderNode))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::BorderNode index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listBorderPoint.size();

	//�f�[�^�C���|�[�g
	//�m�[�h(�d�����Ȃ��|�C���g)���Ώ�
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listBorderPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.node_id.GetValue()) != listRegSID.end())
		{
			LogUtil::printDebug("�d���I�u�W�F�N�g�̂���SKIP���܂�\t���E���|�C���gSID(objectid)" + std::to_string(csv.objectid.GetValue()) + "\t���E���m�[�hSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.node_id, 0LL);	//SOURCEID	��csv.object_id�ł͂Ȃ�csv.node_id
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		SetValue(buf, index.nodeclass_c.index, csv.point_class_c, nullInt);	//NODECLASS_C

		//�`��ݒ�
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.node_id.GetValue());
		
#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertNQBorderNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableNQBorderNode))
	{
		assert(false);
		return false;
	}
	AdamSchema::NQBorderNode index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegNode;
	{
		//���E���m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureBorderNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegLink;
	{
		//���E�������N
		IFeatureClassPtr ipLinkFC;
		if(!(ipLinkFC = _schema->featureBorderLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLink, ipLinkFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listBorderPoint.size();

	//�f�[�^�C���|�[�g
	//�|�C���g(�d�������|�C���g)���Ώ�
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listBorderPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(340, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���E���|�C���gSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//NQ_BORDER_NODE.GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//NQ_BORDER_NODE.SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//NQ_BORDER_NODE.SOURCE_UPDATE_C
		//NQ_BORDER_NODE.OBJECTID ��ArcObject�����t�^

		//NQ_BORDER_NODE.BORDER_NODE_GID
		//BORDER_NODE ����GID�擾
		auto fNode = dicRegNode.find(csv.node_id.GetValue());
		uh::tstring node_gid = (fNode != dicRegNode.end()) ? fNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.border_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(347, "�O���L�[�`�F�b�N", "���E���m�[�hID�����݂��܂���", "���E���|�C���gSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t���E���m�[�hSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}
		//NQ_BORDER_NODE.BORDER_LINK_GID
		//BORDER_LINK����GID�擾
		auto fLink = dicRegLink.find(csv.border_link_id.GetValue());
		uh::tstring border_gid = (fLink != dicRegLink.end()) ? fLink->second.GLOBALID : nullStr;
		if(border_gid != nullStr)
		{
			SetValue(buf, index.border_link_gid.index, border_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(341, "�O���L�[�`�F�b�N", "���E�������NID�����݂��܂���", "���E���|�C���gSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t���E�������NSID(border_link_id):" + std::to_string(csv.border_link_id.GetValue()));
			continue;
		}

		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//NQ_BORDER_NODE.SEQUENCE
		SetValue(buf, index.lastnode_f.index, csv.lastnode_f, 0);	//NQ_BORDER_NODE.LASTNODE_F
		
		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());
		
#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertRelBorderLinkLaneLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelBorderLinkLaneLink))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelBorderLinkLaneLink index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//�Ԑ������N
		IFeatureClassPtr ipLaneLinkFC;
		if(!(ipLaneLinkFC = _schema->featureLaneLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipLaneLinkFC);
	}

	std::map<long long, OGID> dicRegBorder;
	{
		//���E�������N
		IFeatureClassPtr ipBorderLinkFC;
		if(!(ipBorderLinkFC = _schema->featureBorderLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegBorder, ipBorderLinkFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelBorderLink.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelBorderLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(380, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���E���֘ASID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//LANE_LINK_GID
		//LANE_LINK ����GID�擾
		auto fLaneLink = dicRegLane.find(csv.lane_link_id.GetValue());
		uh::tstring lane_gid = (fLaneLink != dicRegLane.end()) ? fLaneLink->second.GLOBALID : nullStr;
		if(lane_gid != nullStr)
		{
			SetValue(buf, index.lane_link_gid.index, lane_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(381, "�O���L�[�`�F�b�N", "�Ԑ������NID�����݂��܂���", "���E���֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ������NSID(object_id):" + std::to_string(csv.lane_link_id.GetValue()));
			continue;
		}
		//BORDER_LINK_GID
		//COMPART_LINK ����GID�擾
		auto fBorderLink = dicRegBorder.find(csv.border_link_id.GetValue());
		uh::tstring border_gid = (fBorderLink != dicRegBorder.end()) ? fBorderLink->second.GLOBALID : nullStr;
		if(border_gid != nullStr)
		{
			SetValue(buf, index.border_link_gid.index, border_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(382, "�O���L�[�`�F�b�N", "���E�������NID�����݂��܂���", "���E���֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t���E�������NSID(object_id):" + std::to_string(csv.border_link_id.GetValue()));
			continue;
		}

		SetValue(buf, index.lr_c.index, csv.lr_f, nullInt);	//LR_C
		SetValue(buf, index.border_link_direction_c.index, csv.border_link_direction_c, nullInt);	//BORDER_LINK_DIRECTION_C
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//SEQUENCE

		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}


bool ADAMSchemaWriter::InsertSignalPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureSignalPoint))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::SignalPoint index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listSignalPoint.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listSignalPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(400, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "�M���@�|�C���gSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		SetValue(buf, index.horizontal_angle.index, csv.horizontal_angle, 0.0);	//HORIZONTAL_ANGLE
		SetValue(buf, index.signal_mold_c.index, csv.signal_mold_c, nullInt);	//SIGNAL_MOLD_C
		SetValue(buf, index.lamp_count.index, csv.lamp_count, 0);	//LAMP_COUNT
		SetValue(buf, index.arrow_f.index, csv.arrow_f, 0);	//ARROW_F
		SetValue(buf, index.warning_f.index, csv.warning_f, 0);	//WARNING_F
		SetValue(buf, index.independent_f.index, csv.independent_f, 0);	//INDEPENDENT_F

		//�`��ݒ�
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertRelLaneNodeSignalPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelLaneNodeSignalPoint))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelLaneNodeSignalPoint index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//�Ԑ��m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipNodeFC);
	}

	std::map<long long, OGID> dicRegSignal;
	{
		//�M���@�|�C���g
		IFeatureClassPtr ipSignalFC;
		if(!(ipSignalFC = _schema->featureSignalPoint))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegSignal, ipSignalFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelSignalPoint.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelSignalPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(440, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "�M���@�֘ASID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//LANE_NODE_GID
		//LANE_NODE ����GID�擾
		auto fLaneNode = dicRegLane.find(csv.lane_node_id.GetValue());
		uh::tstring node_gid = (fLaneNode != dicRegLane.end()) ? fLaneNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(441, "�O���L�[�`�F�b�N", "�Ԑ��m�[�hID�����݂��܂���", "�M���@�֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ��m�[�hSID(object_id):" + std::to_string(csv.lane_node_id.GetValue()));
			continue;
		}
		//SIGNAL_POINT_GID
		//SIGNAL_POINT ����GID�擾
		auto fSignalPoint = dicRegSignal.find(csv.signal_point_id.GetValue());
		uh::tstring obj_gid = (fSignalPoint != dicRegSignal.end()) ? fSignalPoint->second.GLOBALID : nullStr;
		if(obj_gid != nullStr)
		{
			SetValue(buf, index.signal_point_gid.index, obj_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(442, "�O���L�[�`�F�b�N", "�M���@�|�C���gID�����݂��܂���", "�M���@�֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�M���@�|�C���gSID(object_id):" + std::to_string(csv.signal_point_id.GetValue()));
			continue;
		}

		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}


bool ADAMSchemaWriter::InsertCrossWalkLine(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCrossWalkLine))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CrossWalkLine index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCrossWalkLine.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCrossWalkLine)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(500, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���f����SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		SetValue(buf, index.width.index, csv.width, 0.0);	//WIDTH

		//�`��ݒ�
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertRelLaneNodeCrossWalkLine(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelLaneNodeCrossWalkLine))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelLaneNodeCrossWalkLine index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//�Ԑ��m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipNodeFC);
	}

	std::map<long  long, OGID> dicRegCrossWalk;
	{
		//���f�����|�C���g
		IFeatureClassPtr ipCrossWalkFC;
		if(!(ipCrossWalkFC = _schema->featureCrossWalkLine))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegCrossWalk, ipCrossWalkFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelCrossWalkLine.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelCrossWalkLine)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(540, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���f�����֘ASID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//LANE_NODE_GID
		//LANE_NODE ����GID�擾
		auto fLaneNode = dicRegLane.find(csv.lane_node_id.GetValue());
		uh::tstring node_gid = (fLaneNode != dicRegLane.end()) ? fLaneNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(641, "�O���L�[�`�F�b�N", "�Ԑ��m�[�hID�����݂��܂���", "���f�����֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ��m�[�hSID(object_id):" + std::to_string(csv.lane_node_id.GetValue()));
			continue;
		}
		//CROSSWALK_LINE_GID
		//CROSSWALK_LINE ����GID�擾
		auto fCrossWalk = dicRegCrossWalk.find(csv.crosswalk_line_id.GetValue());
		uh::tstring obj_gid = (fCrossWalk != dicRegCrossWalk.end()) ? fCrossWalk->second.GLOBALID : nullStr;
		if(obj_gid != nullStr)
		{
			SetValue(buf, index.crosswalk_line_gid.index, obj_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(642, "�O���L�[�`�F�b�N", "���f�������C��ID�����݂��܂���", "���f�����֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t���f�������C��SID(object_id):" + std::to_string(csv.crosswalk_line_id.GetValue()));
			continue;
		}

		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}


bool ADAMSchemaWriter::InsertRoadSignPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureRoadSignPoint))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::RoadSignPoint index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRoadSignPoint.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("-1");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRoadSignPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(600, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���H�W��SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		SetValue(buf, index.horizontal_angle.index, csv.horizontal_angle, 0.0);	//HORIZONTAL_ANGLE
		SetValue(buf, index.roadsign_c.index, csv.roadsign_c, nullStr);	//ROADSIGN_C
		SetValue(buf, index.variable_f.index, csv.variable_f, 0);	//VARIABLE_F

		//�`��ݒ�
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertRelLaneNodeRoadSignPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelLaneNodeRoadSignPoint))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelLaneNodeRoadSignPoint index(ipT);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//�Ԑ��m�[�h
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipNodeFC);
	}

	std::map<long long, OGID> dicRegRoadSign;
	{
		//�H�ʕW���|�C���g
		IFeatureClassPtr ipRoadSignFC;
		if(!(ipRoadSignFC = _schema->featureRoadSignPoint))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegRoadSign, ipRoadSignFC);
	}

	//�C���T�[�g�J�[�\���ݒ�
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelRoadSignPoint.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelRoadSignPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(640, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���H�W���֘ASID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//LANE_NODE_GID
		//LANE_NODE ����GID�擾
		auto fLaneNode = dicRegLane.find(csv.lane_node_id.GetValue());
		uh::tstring node_gid = (fLaneNode != dicRegLane.end()) ? fLaneNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(541, "�O���L�[�`�F�b�N", "�Ԑ��m�[�hID�����݂��܂���", "���H�W���֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t�Ԑ��m�[�hSID(object_id):" + std::to_string(csv.lane_node_id.GetValue()));
			continue;
		}
		//ROADSIGN_POINT_GID
		//ROADSIGN_POINT ����GID�擾
		auto fRoadSign = dicRegRoadSign.find(csv.roadsign_point_id.GetValue());
		uh::tstring obj_gid = (fRoadSign != dicRegRoadSign.end()) ? fRoadSign->second.GLOBALID : nullStr;
		if(obj_gid != nullStr)
		{
			SetValue(buf, index.roadsign_point_gid.index, obj_gid, nullStr);
		}
		else
		{
			//�Ȃ���΃G���[
			LogUtil::printError(542, "�O���L�[�`�F�b�N", "���H�W���|�C���gID�����݂��܂���", "���H�W���֘ASID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t���H�W���|�C���gSID(object_id):" + std::to_string(csv.roadsign_point_id.GetValue()));
			continue;
		}

		//�C���T�[�g
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertVirtualCompartLine		(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureVirtualCompartLine))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::VirtualCompartLine index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCompartLink.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listVirtualCompartLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(200, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "���z�����C��SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		//OBJECTID ��ArcObject�����t�^

		//�`��ݒ�
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::InsertCoveringArea			(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //�C���T�[�g�ς݌���

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//�C���f�b�N�X�ݒ�
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCoveringArea))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CoveringArea index(ipFC);

	//�o�^�ς�SID���X�g
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// �X�L�[�}���b�N������Ń��[�h�I�����[���[�h��ON�ɂ��A�A�����b�N�{OFF�p��scope_guard�𒣂��Ă���
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//�C���T�[�g�J�[�\���ݒ�
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCoveringArea.size();

	//�f�[�^�C���|�[�g
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCoveringArea)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//���ɓ���SID������΃`�F�b�N����SKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(200, "���j�[�N�`�F�b�N", "�d���I�u�W�F�N�g�̂���SKIP���܂�", "�������G���ASID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//�ҏW�������ݒ�
		SetModifyData(index, buf);

		//�����ݒ�
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID ��ArcObject�����t�^

		//�`��ݒ�
		IPolygonPtr ipPolygon = CreatePolygonZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolygon);

		//���Ȍ����`�F�b�N
		{
			ITopologicalOperatorPtr ipTopo(ipPolygon);
			VARIANT_BOOL IsSimple;
			if (ipTopo) { ipTopo->get_IsSimple(&IsSimple); }

			if(IsSimple == VARIANT_FALSE)
			{
				LogUtil::printWarning(900, "�W�I���g���`�F�b�N", "�|���S�������Ȍ������Ă��邽��SKIP���܂�", "�������G���ASID(object_id):" + std::to_string(csv.objectid.GetValue()));
				continue;
			}
		}

		//�C���T�[�g
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//�C���T�[�g���s
			LogUtil::printError(65, "�C���T�[�g���s\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//�o�^�ς݃��X�g�ɒǉ�
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "�� �C���|�[�g���܂���");

	return ret;
}

bool ADAMSchemaWriter::CheckAdamVersion()
{
	LogUtil::print("ADAM�o�[�W�����`�F�b�N�����{���܂�");

	bool ret = SetTable(	AdamSchema::AdamVersion::TableName,				&(_schema->tableAdamVersion));

	if( !ret )
	{
		LogUtil::printError( 73, "�X�L�[�}�o�[�W�����s��v", "�X�L�[�}�o�[�W�������قȂ�܂�", uh::toStr( AdamSchema::AdamVersion::TableName ) + "�����݂��܂���" );
		return false;
	}
	LogUtil::print( "ADAM�o�[�W�����`�F�b�N�ɐ������܂���" );

	LogUtil::printLine();

	return true;
}

IPointPtr ADAMSchemaWriter::CreatePointZ(const Location& loc)
{
	bool ret = true;

	//��ԎQ�Ǝ擾���� LANE_NODE
	IFeatureClassPtr ipPntFC = _schema->featureLaneNode;
	if(!ipPntFC)
	{
		assert(false);
		return nullptr;
	}

	IPointPtr ipPnt;

	//�|�C���g�W�I���g���쐬
	SetZPoint(ipPntFC, ipPnt);

	//���n�n�ϊ�
	WKSPointZ wksPnt = {loc.longitude.GetValue(), loc.latitude.GetValue(), loc.height.GetValue()};
	WKSPointZ cnvPnt = wksPnt;
	if(!crdcnvTokyo2Ref(wksPnt, cnvPnt))
	{
		assert(false);
		return nullptr;
	}

	ipPnt->put_X(cnvPnt.X);
	ipPnt->put_Y(cnvPnt.Y);
	ipPnt->put_Z(cnvPnt.Z);

	return ipPnt;
}

IPointPtr ADAMSchemaWriter::CreatePointZ(const adam_v2::Point& loc)
{
	return CreatePointZ(loc.GetGeometry());
}

IPolylinePtr ADAMSchemaWriter::CreatePolylineZ(const std::vector<Location>& loc)
{
	//��ԎQ�Ǝ擾���� LANE_LINK
	IFeatureClassPtr ipLinkFC = _schema->featureLaneLink;

	if(!ipLinkFC)
	{
		assert(false);
		return nullptr;
	}

	IPolylinePtr ipPolyline;

	//�W�I���g��������	
	SetZPolyline(ipLinkFC, ipPolyline);

	for(const auto& pnt : loc)
	{
		//�|�C���g�W�I���g���쐬
		IPointPtr ipPoint = CreatePointZ(pnt);

		//�|�����C���ɒǉ�
		((IPointCollectionPtr)ipPolyline)->AddPoint(ipPoint);
	}

	return ipPolyline;
}

IPolylinePtr ADAMSchemaWriter::CreatePolylineZ(const adam_v2::Polyline& loc)
{
	return CreatePolylineZ(loc.GetGeometry());
}

IPolygonPtr ADAMSchemaWriter::CreatePolygonZ(const std::vector<Location>& loc)
{
	//��ԎQ�Ǝ擾���� COVERING_AREA
	IFeatureClassPtr ipPolygonFC = _schema->featureCoveringArea;

	if(!ipPolygonFC)
	{
		assert(false);
		return nullptr;
	}

	IPolygonPtr ipPolygon;

	//�W�I���g��������	
	SetZPolygon(ipPolygonFC, ipPolygon);

	for(const auto& pnt : loc)
	{
		//�|�C���g�W�I���g���쐬
		IPointPtr ipPoint = CreatePointZ(pnt);

		//�|�����C���ɒǉ�
		((IPointCollectionPtr)ipPolygon)->AddPoint(ipPoint);
	}

	return ipPolygon;
}

IPolygonPtr ADAMSchemaWriter::CreatePolygonZ(const adam_v2::Polygon& loc)
{
	return CreatePolygonZ(loc.GetGeometry());
}

void ADAMSchemaWriter::SetModifyData(const AdamSchema::AdamiPCFeature& adamSchema, IRowBufferPtr ipRow)
{
	SYSTEMTIME st = {};
	::GetLocalTime(&st);

	CString strDate;
	strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	//���[�U���擾
	TCHAR szName[256];
	DWORD dwName = 255;
	if(!GetUserName(szName, &dwName))
	{
		_tcscpy_s(szName, 256 * sizeof(TCHAR),_T(""));	//���O���Ȃ������疼����
	}

	ipRow->put_Value(adamSchema.operator_name.index,	CComVariant(szName)									);
	ipRow->put_Value(adamSchema.purpose_c.index,		CComVariant(long(0))								);
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.updatetype_c.index,		CComVariant(long(1))								);	//�V�K�쐬
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.modifyprogname.index,	CComVariant(CString(_T("ADAMSurveyImporter")))		);
	ipRow->put_Value(adamSchema.userclaim_f.index,		CComVariant(long(0))								);
	ipRow->put_Value(adamSchema.speculation_c.index,	CComVariant(long(1))								);

	ipRow->put_Value(adamSchema.source.index,			CComVariant(CString(_source.c_str()))				);
}

void ADAMSchemaWriter::SetModifyData(const AdamSchema::TableClass& adamSchema, IRowBufferPtr ipRow)
{
	SYSTEMTIME st = {};
	::GetLocalTime(&st);

	CString strDate;
	strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	//���[�U���擾
	TCHAR szName[256];
	DWORD dwName = 255;
	if(!GetUserName(szName, &dwName))
	{
		_tcscpy_s(szName, 256 * sizeof(TCHAR),_T(""));	//���O���Ȃ������疼����
	}

	ipRow->put_Value(adamSchema.operator_name.index,	CComVariant(szName)									);
	ipRow->put_Value(adamSchema.purpose_c.index,		CComVariant(long(0))								);
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.updatetype_c.index,		CComVariant(long(1))								);	//�V�K�쐬
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.modifyprogname.index,	CComVariant(CString(_T("ADAMSurveyImporter")))		);
	ipRow->put_Value(adamSchema.userclaim_f.index,		CComVariant(long(0))								);

	ipRow->put_Value(adamSchema.source.index,			CComVariant(CString(_source.c_str()))				);
}

void ADAMSchemaWriter::StartProcess(ITablePtr& ipT)
{
	ChangeSchemeLock(ipT, esriExclusiveSchemaLock);	// �X�L�[�}���b�N
	ChangeLoadOnlyMode(ipT, VARIANT_TRUE);			// RoadOnlyMode ON
}

void ADAMSchemaWriter::FinishProcess(ITablePtr& ipT)
{
	ChangeLoadOnlyMode(ipT, VARIANT_FALSE);			// RoadOnlyMode OFF
	ChangeSchemeLock(ipT, esriSharedSchemaLock);	// �X�L�[�}�A�����b�N
}

void ADAMSchemaWriter::ChangeSchemeLock(ITablePtr& ipT, esriSchemaLock eStatus)
{
	ISchemaLockPtr ipSchemaLock(ipT);
	ipSchemaLock->ChangeSchemaLock(eStatus);
}

void ADAMSchemaWriter::ChangeLoadOnlyMode(ITablePtr& ipT, VARIANT_BOOL vbOnOff)
{
	IFeatureClassLoadPtr ipFcLoad(ipT);
	ipFcLoad->PutLoadOnlyMode(vbOnOff);
}