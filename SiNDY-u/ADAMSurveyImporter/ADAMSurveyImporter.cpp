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

#include "ADAMSurveyImporter.h"

#include "LogUtil.h"

#include "SurveyCSVReader.h"
#include "ADAMSchemaWriter.h"


using namespace std;

// ���s
bool ADAMSurveyImporter::execute()
{
	using namespace adam_v2;

	LogUtil::printLine();
	LogUtil::print("�Ή�ADAM�X�L�[�}:\t" + uh::toStr(AdamSchema::AdamVersion::TableName));
	LogUtil::print("�Ή��[��FMT:\t" + uh::toStr(SurveyCSV::CONST_FMT_VERSION));
	LogUtil::printLine();

	//���O�t�@�C���ݒ�
	LogUtil::setLogDirPath(_args.log_dir);

	// �C���|�[�g��̃e�[�u���ACSV�t�@�C���A�f�[�^�ێ��t���O���A"�C���|�[�g�p�Ή����"�Ƃ��ė\�ߗp��
	const vector<ImportInfo> vecImportInfo = boost::assign::list_of
		( ImportInfo( AdamSchema::LaneLinkGroup::TableName,				SurveyCSV::LaneLink::FILE_NAME,				true	) )
		( ImportInfo( AdamSchema::LaneNode::TableName,					SurveyCSV::LanePoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::LaneLink::TableName,					SurveyCSV::LaneLink::FILE_NAME,				true	) )
		( ImportInfo( AdamSchema::NQLaneNode::TableName,				SurveyCSV::LanePoint::FILE_NAME,			false	) )
		( ImportInfo( AdamSchema::CompartNode::TableName,				SurveyCSV::CompartPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::CompartLink::TableName,				SurveyCSV::CompartLink::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::NQCompartNode::TableName,				SurveyCSV::CompartPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::RelCompartLinkLaneLink::TableName,	SurveyCSV::RelCompartLink::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::BorderNode::TableName,				SurveyCSV::BorderPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::BorderLink::TableName,				SurveyCSV::BorderLink::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::NQBorderNode::TableName,				SurveyCSV::BorderPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::RelBorderLinkLaneLink::TableName,		SurveyCSV::RelBorderLink::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::SignalPoint::TableName,				SurveyCSV::SignalPoint::FILE_NAME,			false	) )
		( ImportInfo( AdamSchema::RelLaneNodeSignalPoint::TableName,	SurveyCSV::RelSignalPoint::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::CrossWalkLine::TableName,				SurveyCSV::CrossWalkLine::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::RelLaneNodeCrossWalkLine::TableName,	SurveyCSV::RelCrossWalkLine::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::RoadSignPoint::TableName,				SurveyCSV::RoadSignPoint::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::RelLaneNodeRoadSignPoint::TableName,	SurveyCSV::RelRoadSignPoint::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::VirtualCompartLine::TableName,		SurveyCSV::VirtualCompartLink::FILE_NAME,	false	) )
		( ImportInfo( AdamSchema::CoveringArea::TableName,				SurveyCSV::CoveringArea::FILE_NAME,			false	) )
		;

	// ���[�_�[����
	SurveyCSVReaderPtr reader = make_shared<SurveyCSVReader>();
	reader->SetParam(_args.survey_dir);
	reader->SetSkipDeleteRecord(_args.skip_delete_record);

	// CSV�o�[�W�����`�F�b�N
	if(!reader->CheckCsvVersion())
		return false;

	// ���C�^�[����
	ADAMSchemaWriterPtr writer = make_shared<ADAMSchemaWriter>();
	writer->SetParam(_args.import_db);
	writer->SetUnuseVersion(_args.unuse_version);
	writer->SetSource(_args.company, _args.date, _args.lot);

	// ���W�ϊ��p�p�����[�^�t�@�C����ǂݍ���ŃZ�b�g
	if(!writer->ReadCrdParam(_args.crd_param))
		return false;

	// SRID��ǂݍ���ŃZ�b�g
	if(!reader->Read(SurveyCSV::SpatialRef::FILE_NAME))
		return false;
	writer->SetCSV(reader->GetSurveyCSVPtr());
	if(!writer->SetSpatialReferenceId())
		return false;

	// ADAM�o�[�W�����`�F�b�N
	if(!writer->CheckAdamVersion())
		return false;

	// DB�ڑ�
	if(!writer->ConnectDB())
		return false;

	// �ҏW�J�n
	if(!writer->StartEdit())
		return false;

	// �C���|�[�g�p�Ή����Ń��[�v
	for(const auto& info : vecImportInfo)
	{
		// CSV�t�@�C���ǂݍ���
		if(!reader->Read(info.fileName))
		{
			writer->AbortEdit();
			return false;
		}

		// ADAM�փC���|�[�g
		writer->SetCSV(reader->GetSurveyCSVPtr());
		if(!writer->Write(info.tableName))
		{
			writer->AbortEdit();
			return false;
		}

		// �ێ��t���O���܂�Ă�����CSV�f�[�^�j���i�������ߖ�̂��߁j
		if(!info.bKeepData)
			reader->Clear();
	}

	// �ҏW�I��
	return writer->StopEdit();
}