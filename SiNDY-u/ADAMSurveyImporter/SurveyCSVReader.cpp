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

#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>

#include <TDC/useful_headers/directory_util.h>

#include "LogUtil.h"
#include "SurveyCSVReader.h"

using namespace adam_v2;

SurveyCSVReader::SurveyCSVReader(void) : 
	_csv(nullptr),
	_skipDeleteRecord(false),
	_hasError(false)
{
	_csv = std::make_shared<SurveyCSV>();
}


SurveyCSVReader::~SurveyCSVReader(void)
{
}

bool SurveyCSVReader::Read(const uh::tstring& fileName)
{
	bool ret = true;
	bool readSkiped = false; // �ǂݍ��݃X�L�b�v������

	LogUtil::printDoubleLine();
	LogUtil::print("���̓t�@�C���̓ǂݍ��݂��J�n���܂�");

	// CSV�t�@�C������f�[�^��ǂݍ���
	// ���ɓǂݍ��ݍς݂̏ꍇ�̓X�L�b�v���A�W�I���g���`�F�b�N���s��Ȃ�
	     if( fileName == SurveyCSV::SpatialRef::FILE_NAME )
		_csv->listSpatialRef.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadSpatialRef )			:	readSkiped = true;	// ��ԎQ��
	else if( fileName == SurveyCSV::Version::FILE_NAME )
		_csv->listVersion.empty() ?				ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadVersion )				:	readSkiped = true;	// CSV�o�[�W����
	else if( fileName == SurveyCSV::LaneLink::FILE_NAME )
		_csv->listLaneLink.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadLaneLink )				:	readSkiped = true;	// �Ԑ������N
	else if( fileName == SurveyCSV::LanePoint::FILE_NAME )
		_csv->listLanePoint.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadLanePoint )				:	readSkiped = true;	// �Ԑ��m�[�h
	else if( fileName == SurveyCSV::RelLaneLink::FILE_NAME )
		_csv->listRelLaneLink.empty() ?					ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelLaneLink )			:	readSkiped = true;	// �Ԑ��֘A
	else if( fileName == SurveyCSV::CompartLink::FILE_NAME )
		_csv->listCompartLink.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadCompartLink )			:	readSkiped = true;	// ���������N
	else if( fileName == SurveyCSV::CompartPoint::FILE_NAME )
		_csv->listCompartPoint.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadCompartPoint )			:	readSkiped = true;	// �����m�[�h
	else if( fileName == SurveyCSV::RelCompartLink::FILE_NAME )
		_csv->listRelCompartLink.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelCompartLink )		:	readSkiped = true;	// �����֘A
	else if( fileName == SurveyCSV::BorderLink::FILE_NAME )
		_csv->listBorderLink.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadBorderLink )			:	readSkiped = true;	// ���E�������N
	else if( fileName == SurveyCSV::BorderPoint::FILE_NAME )
		_csv->listBorderPoint.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadBorderPoint )			:	readSkiped = true;	// ���E���m�[�h
	else if( fileName == SurveyCSV::RelBorderLink::FILE_NAME )
		_csv->listRelBorderLink.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelBorderLink )			:	readSkiped = true;	// ���E���֘A
	else if( fileName == SurveyCSV::SignalPoint::FILE_NAME )
		_csv->listSignalPoint.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadSignalPoint )			:	readSkiped = true;	// �M���@
	else if( fileName == SurveyCSV::RelSignalPoint::FILE_NAME )
		_csv->listRelSignalPoint.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelSignalPoint )		:	readSkiped = true;	// �M���@�֘A
	else if( fileName == SurveyCSV::RoadSignPoint::FILE_NAME )
		_csv->listRoadSignPoint.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRoadSignPoint )			:	readSkiped = true;	// ���H�W��
	else if( fileName == SurveyCSV::RelRoadSignPoint::FILE_NAME )
		_csv->listRelRoadSignPoint.empty() ?	ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelRoadSignPoint )		:	readSkiped = true;	// ���H�W���֘A
	else if( fileName == SurveyCSV::CrossWalkLine::FILE_NAME )
		_csv->listCrossWalkLine.empty() ?				ReadCSV( _path, fileName, &SurveyCSVReader::ReadCrossWalkLine )			:	readSkiped = true;	// ���f����
	else if( fileName == SurveyCSV::RelCrossWalkLine::FILE_NAME )
		_csv->listRelCrossWalkLine.empty() ?			ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelCrossWalkLine )		:	readSkiped = true;	// ���f�����֘A
	else if( fileName == SurveyCSV::VirtualCompartLink::FILE_NAME )
		_csv->listVirtualCompartLink.empty() ?			ReadCSV( _path, fileName, &SurveyCSVReader::ReadVirtualCompartLink )	:	readSkiped = true;	// ���z�������N
	else if( fileName == SurveyCSV::VirtualCompartPoint::FILE_NAME )
		_csv->listVirtualCompartPoint.empty() ?			ReadCSV( _path, fileName, &SurveyCSVReader::ReadVirtualCompartPoint )	:	readSkiped = true;	// ���z���m�[�h
	else if( fileName == SurveyCSV::RelVirtualCompartLink::FILE_NAME )
		_csv->listRelVirtualCompartLink.empty() ?		ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelVirtualCompartLink )	:	readSkiped = true;	// ���z���֘A
	else if( fileName == SurveyCSV::CoveringArea::FILE_NAME )
		_csv->listCoveringArea.empty() ?				ReadCSV( _path, fileName, &SurveyCSVReader::ReadCoveringArea )			:	readSkiped = true;	// ������
	else
		_ASSERT( false );

	if(ret)
	{
		if(!readSkiped)
		{
			LogUtil::print("���̓t�@�C���̓ǂݍ��݂ɐ������܂���");
		}
		else
		{
			LogUtil::print("���̓t�@�C���͓ǂݍ��ݍς݂ł�");
		}
	}
	else
	{
		LogUtil::printError("���̓t�@�C���̓ǂݍ��ݏ������I�����܂����B�G���[�����邽�߃��O���m�F���Ă�������");
	}
	
	//�W�I���g���`�F�b�N
	if(ret && !readSkiped)
	{
		LogUtil::printLine();
		LogUtil::print("���̓t�@�C���̃W�I���g���`�F�b�N�����s���܂�");

		ret &= CheckGeometryIntegrity();
		ret &= CheckSortByOID();

		if(ret)
		{
			LogUtil::print("�W�I���g���`�F�b�N�ɐ������܂���");
		}
		else
		{
			LogUtil::printError("�W�I���g���`�F�b�N�Ɏ��s���܂����B�G���[�����邽�߃��O���m�F���Ă�������");
		}
	}
	
	LogUtil::printDoubleLine();

	return ret;
}

void SurveyCSVReader::Clear()
{
	_csv->Clear();
}


bool SurveyCSVReader::ReadCSV(
	const uh::tstring& dirPath,
	const uh::tstring& fileName,
	bool (SurveyCSVReader::*func)(const boost::tokenizer<boost::char_separator<char>>&)
	)
{
	uh::tstring filePath = dirPath + _T( "\\" ) + fileName;

	LogUtil::print(uh::toStr( filePath ) + " �̓ǂݍ��݂��J�n���܂�...");

	if(!_csv)
	{
		return false;
	}

	if( ! uh::isFile( filePath.c_str() ))
	{
		LogUtil::printError(50, "�t�@�C���I�[�v�����s\t" + uh::toStr( filePath ));
		LogUtil::printError("�ǂݍ��݂Ɏ��s���܂���");
		return false;
	}

	//�t�@�C���ǂݍ���
	_hasError = false;
	boost::filesystem::path const path(filePath);
	std::ifstream ifs(filePath.c_str());

	//BOM �X�L�b�v
	//http://qiita.com/yumetodo/items/3744afa94ab029f4e1d5
	{
		int dst[3];
		for (auto& i : dst)
		{
			i = ifs.get();
		}
		const int utf8[] = { 0xEF, 0xBB, 0xBF };
		if (!std::equal(std::begin(dst), std::end(dst), utf8))
		{
			ifs.seekg(0);
		}
	}

	std::string line;
	boost::char_separator<char> sep(uh::toStr(SurveyCSV::DELIMITER).c_str(), "", boost::keep_empty_tokens);

	long line_count = 0;
	long error_count = LogUtil::getErrCount();
	long normal_count = 0;
	long indexUpdateC = GetIndexUpdateC( fileName );
	bool isCheched = true;	//�t�H�[�}�b�g�`�F�b�N����
	
	while (std::getline(ifs, line)) 
	{
		++line_count;
		LogUtil::flushLine((boost::format("%10ld reading...") % line_count).str());

		// ���̓t�@�C���� BOM�Ȃ�UTF-8(UTF-8N)�Œ�
		// UTF-8 �� Unicode�ϊ�
		std::string lineStr;
		{
			int buflen = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, NULL, 0);
			WCHAR* wbuf = new wchar_t[buflen];

			// ����������C�h������iUnicode�j�Ƀ}�b�v
			MultiByteToWideChar(
				CP_UTF8,			// �R�[�h�y�[�W�FUTF-8 ���g�����ϊ�
				0,					// �����̎�ނ��w�肷��t���O
				line.c_str(),		// �ϊ���������
				-1,					// �ϊ���������T�C�Y(-1�w��Ŏ����v�Z)
				wbuf,				// �ϊ���o�b�t�@
				buflen);			// �ϊ���o�b�t�@�T�C�Y	

			lineStr = uh::toStr(wbuf);

			delete[] wbuf;
		}

		boost::tokenizer<boost::char_separator<char>> tokens(lineStr, sep);

		if(line_count == SurveyCSV::HEADER_COUNT)
		{
			// �R�����g�s
			// 12	WAR	�w�b�_�s�L���`�F�b�N	FMT�d�l�ʂ肩
			if(lineStr.size() > 0 && lineStr.compare(0, 1, SurveyCSV::HEADER_PREFIX) == 0)
				continue;
			LogUtil::printWarning(12, "�w�b�_�s����`����Ă��܂���");
		}
		
		// �t���O�������Ă��āA���폜���R�[�h�ł���΃X�L�b�v
		if( _skipDeleteRecord && IsDeleteRecord( fileName, tokens, indexUpdateC ) )
			continue;

		if(!((this->*func)(tokens)))
		{
			LogUtil::printError(std::to_string(line_count) + "�s�ڂɃt�H�[�}�b�g�ᔽ������܂��B���O���m�F���Ă�������");
			isCheched = false;
		}
		else
		{
			normal_count++;
		}
	}

	ifs.close();

	if(!isCheched)
	{
		LogUtil::printError(std::to_string(LogUtil::getErrCount() - error_count) + "�� ���̓f�[�^�Ƀt�H�[�}�b�g�ᔽ������܂��B���O���m�F���Ă�������");
	}
	else
	{
		LogUtil::print("����ɓǂݍ��݂܂���\t" + uh::toStr( filePath ));
	}

	LogUtil::print(std::to_string(normal_count) + " �s �ǂݍ��݂܂���");
	LogUtil::print( uh::toStr( filePath ) + " �̓ǂݍ��݂��I�����܂�...");

	return isCheched;
}

bool SurveyCSVReader::IsDeleteRecord( const uh::tstring& fileName, const boost::tokenizer<boost::char_separator<char>>& tokens, long index )
{
	// �X�V�R�[�h�u�폜�v�̃��R�[�h���ʗp
	static bool isDeleteRecord = false;

	// �ȉ��͔���ΏۊO
	if( fileName == SurveyCSV::SpatialRef::FILE_NAME ||
		fileName == SurveyCSV::Version::FILE_NAME ||
		fileName == SurveyCSV::RelLaneLink::FILE_NAME ||
		fileName == SurveyCSV::CrossWalkLine::FILE_NAME ||
		fileName == SurveyCSV::RelCrossWalkLine::FILE_NAME ||
		fileName == SurveyCSV::VirtualCompartLink::FILE_NAME ||
		fileName == SurveyCSV::VirtualCompartPoint::FILE_NAME ||
		fileName == SurveyCSV::RelVirtualCompartLink::FILE_NAME ||
		fileName == SurveyCSV::CoveringArea::FILE_NAME )
		return false;

	// �W�I���g���݂̂�����
	if( getTokenCount( tokens ) == SurveyCSV::GEOMETRY_ROW_COUNT )
	{
		// �폜�t���O���R�[�h��Ԃ�
		// �Ԑ��E�����E���E���́A�[��FMT�̎d�l��A�ŏ��̍\���_�̃��R�[�h�ɑ��������݂��A
		// �ȍ~�̍\���_�̃��R�[�h�̓W�I���g���݂̂ƂȂ��Ă���
		// ���̂��߁A�u�X�V�R�[�h�̃J�����ɒl�����݂��Ȃ����폜�ł͂Ȃ��v�ƂȂ�Ȃ��悤�A
		// ����̃I�u�W�F�N�g��\���\���_�Q�ŁA��������ƂȂ�悤�Ǘ����Ă���
		return isDeleteRecord;
	}

	// �J���������[�v
	long column = 0;
	for( const auto& token : tokens )
	{
		++column;
		if( index != column )
			continue;

		// �폜���R�[�h������
		if( token == std::to_string( update_code::kDelete ) )
		{
			// �Ԑ��E�����E���E��������
			if( fileName == SurveyCSV::LaneLink::FILE_NAME ||
				fileName == SurveyCSV::CompartLink::FILE_NAME ||
				fileName == SurveyCSV::BorderLink::FILE_NAME )
			{
				// �폜���R�[�h�t���O�𗧂Ă�
				isDeleteRecord = true;
			}

			return true;
		}
		// �폜���R�[�h�t���O��|��
		isDeleteRecord = false;
	}

	return false;
}

long SurveyCSVReader::GetIndexUpdateC( const uh::tstring& fileName )
{
	long indexUpdateC = -1;

	if( fileName == SurveyCSV::LaneLink::FILE_NAME )
	{
		SurveyCSV::LaneLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::LanePoint::FILE_NAME )
	{
		SurveyCSV::LanePoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::CompartLink::FILE_NAME )
	{
		SurveyCSV::CompartLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::CompartPoint::FILE_NAME )
	{
		SurveyCSV::CompartPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelCompartLink::FILE_NAME )
	{
		SurveyCSV::RelCompartLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::BorderLink::FILE_NAME )
	{
		SurveyCSV::BorderLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::BorderPoint::FILE_NAME )
	{
		SurveyCSV::BorderPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelBorderLink::FILE_NAME )
	{
		SurveyCSV::RelBorderLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::SignalPoint::FILE_NAME )
	{
		SurveyCSV::SignalPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelSignalPoint::FILE_NAME )
	{
		SurveyCSV::RelSignalPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RoadSignPoint::FILE_NAME )
	{
		SurveyCSV::RoadSignPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelRoadSignPoint::FILE_NAME )
	{
		SurveyCSV::RelRoadSignPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}

	return indexUpdateC;
}

long SurveyCSVReader::getTokenCount(const boost::tokenizer<boost::char_separator<char>>& token)
{
	long count = 0;

	BOOST_FOREACH(std::string s, token)
	{
		count++;
	}

	return count;
}

bool SurveyCSVReader::CheckGeometryIntegrity()
{
	//�W�I���g���t�B�[�`����ΏۂɃW�I���g���������`�F�b�N���{

	bool ret = true;

	//�Ԑ������N
	for(auto& data : _csv->listLaneLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\t�W�I���g���G���[������܂��B���O���m�F���Ă�������\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//�Ԑ��|�C���g�̐������`�F�b�N�͂Ȃ��̂�Skip

	//���������N
	for(auto& data : _csv->listCompartLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\t�W�I���g���G���[������܂��B���O���m�F���Ă�������\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//�����|�C���g�̐������`�F�b�N�͂Ȃ��̂�Skip

	//���E�������N
	for(auto& data : _csv->listBorderLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\t�W�I���g���G���[������܂��B���O���m�F���Ă�������\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//���E���|�C���g�̐������`�F�b�N�͂Ȃ��̂�Skip

	//�M���@�|�C���g�̐������`�F�b�N�͂Ȃ��̂�Skip

	//�W���|�C���g�̐������`�F�b�N�͂Ȃ��̂�Skip

	//���f�������C��
	for(auto& data : _csv->listCrossWalkLine)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\t�W�I���g���G���[������܂��B���O���m�F���Ă�������\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//���z�������N
	for(auto& data : _csv->listVirtualCompartLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\t�W�I���g���G���[������܂��B���O���m�F���Ă�������\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//�������G���A
	for(auto& data : _csv->listCoveringArea)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\t�W�I���g���G���[������܂��B���O���m�F���Ă�������\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	if(ret)
	{
		LogUtil::print("�W�I���g���������`�F�b�N�ɐ������܂���");
	}
	else
	{
		LogUtil::printError("�W�I���g���������`�F�b�N�ɃG���[������܂��B���O���m�F���Ă�������");
	}

	return ret;
}

bool SurveyCSVReader::CheckSortByOID()
{
	//�Ԑ������N
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listLaneLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t�Ԑ������NOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//�Ԑ��|�C���g
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listLanePoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t�Ԑ��|�C���gOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//�Ԑ��֘A
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelLaneLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t�Ԑ��֘A�����NOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���������N
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listCompartLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���������NOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//�����|�C���g
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listCompartPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t�����|�C���gOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//�����֘A
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelCompartLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t�����֘AOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���E�������N
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listBorderLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���E�������NOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���E���|�C���g
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listBorderPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���E���|�C���gOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���E���֘A
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelBorderLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���E���֘AOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//�M���@
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listSignalPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t�M���@OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//�M���@�֘A
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelSignalPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t�M���@�֘AOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���H�W��
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRoadSignPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���H�W��OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���H�W���֘A
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelRoadSignPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���H�W���֘AOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���f����
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listCrossWalkLine)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���f����OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���f�����֘A
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelCrossWalkLine)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID�����ł͂���܂���\t���f�����֘AOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//���ׂ�WAR�Ȃ̂� true
	return true;
}

bool SurveyCSVReader::CheckCsvVersion()
{
	if(!Read(SurveyCSV::Version::FILE_NAME))
		return false;

	if(_csv->listVersion.size() == 0)
	{
		LogUtil::printError("�t�H�[�}�b�g�t�@�C�����ǂݍ��܂�Ă��܂���");
		return false;
	}

	uh::tstring version = _csv->listVersion.begin()->format_def.GetValue();

	//�o�[�W�������e�l�`�F�b�N
	for(const auto& allow_ver: SurveyCSV::CONST_ALLOW_FMT_VERSION_LIST)
	{
		if(allow_ver == version)
		{
			LogUtil::printWarning(801, "�_���`�F�b�N", "FMT�o�[�W�������ŐV�ł͂���܂��񂪁A�p���\�Ȃ��ߏ����𑱍s���܂�", "\t����Ver:" + uh::toStr(_csv->listVersion.begin()->format_def.GetValue()) + "\t���p�\Ver:" + uh::toStr(SurveyCSV::CONST_FMT_VERSION));
			return true;
		}
	}

	//�o�[�W������v�`�F�b�N
	if(SurveyCSV::CONST_FMT_VERSION != version)
	{
		LogUtil::printError(800, "�_���`�F�b�N", "FMT�o�[�W�������قȂ�܂�", "\t����Ver:" + uh::toStr(_csv->listVersion.begin()->format_def.GetValue()) + "\t���p�\Ver:" + uh::toStr(SurveyCSV::CONST_FMT_VERSION));
		return false;
	}

	return true;
}

bool SurveyCSVReader::ReadLaneLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::LaneLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::LaneLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::LaneLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listLaneLink.size() > 0)
					{
						//OID�s���ǉ��ς� ���� �ǉ��ς݂̃I�u�W�F�N�g�ɒǋL
						Polyline addedGeo = _csv->listLaneLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listLaneLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "�C���|�[�g�G���[\t������`�s���G���[�̂��߃C���|�[�g��Skip���܂���");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.lane_group_id.GetIndex())
		{
			isCheched &= csv.lane_group_id.SetText(s);
		}
		else if(row_count == csv.lane_group_c.GetIndex())
			isCheched &= csv.lane_group_c.SetText(s);
		else if(row_count == csv.lane_no.GetIndex())
			isCheched &= csv.lane_no.SetText(s);
		else if(row_count == csv.lane_count.GetIndex())
			isCheched &= csv.lane_count.SetText(s);
		else if(row_count == csv.max_legal_speed_c.GetIndex())
			isCheched &= csv.max_legal_speed_c.SetText(s);
		else if(row_count == csv.min_legal_speed_c.GetIndex())
			isCheched &= csv.min_legal_speed_c.SetText(s);
		else if(row_count == csv.travel_direction_c.GetIndex())
			isCheched &= csv.travel_direction_c.SetText(s);
		else if(row_count == csv.road_structure_c.GetIndex())
			isCheched &= csv.road_structure_c.SetText(s);
		else if(row_count == csv.both_link_direction_f.GetIndex())
			isCheched &= csv.both_link_direction_f.SetText(s);
		else if(row_count == csv.left_change_f.GetIndex())
			isCheched &= csv.left_change_f.SetText(s);
		else if(row_count == csv.right_change_f.GetIndex())
			isCheched &= csv.right_change_f.SetText(s);
		else if(row_count == csv.left_protrusion_f.GetIndex())
			isCheched &= csv.left_protrusion_f.SetText(s);
		else if(row_count == csv.right_protrusion_f.GetIndex())
			isCheched &= csv.right_protrusion_f.SetText(s);
		else if(row_count == csv.rubbing_f.GetIndex())
			isCheched &= csv.rubbing_f.SetText(s);
		else if(row_count == csv.tunnel_f.GetIndex())
			isCheched &= csv.tunnel_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listLaneLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadLanePoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::LanePoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::LanePoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::LanePoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.stop_point_f.GetIndex())
			isCheched &= csv.stop_point_f.SetText(s);
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else if(row_count == csv.linear_element_c.GetIndex())
			isCheched &= csv.linear_element_c.SetText(s);
		else if(row_count == csv.curvature.GetIndex())
			isCheched &= csv.curvature.SetText(s);
		else if(row_count == csv.curvature_radius.GetIndex())
			isCheched &= csv.curvature_radius.SetText(s);
		else if(row_count == csv.curvature_change_rate.GetIndex())
			isCheched &= csv.curvature_change_rate.SetText(s);
		else if(row_count == csv.clothoid_parameter.GetIndex())
			isCheched &= csv.clothoid_parameter.SetText(s);
		else if(row_count == csv.longitudinal_slope.GetIndex())
			isCheched &= csv.longitudinal_slope.SetText(s);
		else if(row_count == csv.cross_slope.GetIndex())
			isCheched &= csv.cross_slope.SetText(s);
		else if(row_count == csv.cover_f.GetIndex())
			isCheched &= csv.cover_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listLanePoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelLaneLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RelLaneLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RelLaneLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelLaneLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.road_link_id.GetIndex())
			isCheched &= csv.road_link_id.SetText(s);
		else if(row_count == csv.road_link_direction_c.GetIndex())
			isCheched &= csv.road_link_direction_c.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRelLaneLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCompartLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::CompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::CompartLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::CompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listCompartLink.size() > 0)
					{
						//OID�s���ǉ��ς� ���� �ǉ��ς݂̃I�u�W�F�N�g�ɒǋL
						Polyline addedGeo = _csv->listCompartLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listCompartLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "�C���|�[�g�G���[\t������`�s���G���[�̂��߃C���|�[�g��Skip���܂���");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.compart_line_c.GetIndex())
			isCheched &= csv.compart_line_c.SetText(s);
		else if(row_count == csv.compart_style_c.GetIndex())
			isCheched &= csv.compart_style_c.SetText(s);
		else if(row_count == csv.slowdown_label_c.GetIndex())
			isCheched &= csv.slowdown_label_c.SetText(s);
		else if(row_count == csv.installation_f.GetIndex())
			isCheched &= csv.installation_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listCompartLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCompartPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::CompartPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::CompartPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::CompartPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.compart_link_id.GetIndex())
			isCheched &= csv.compart_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listCompartPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelCompartLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RelCompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RelCompartLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelCompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.lr_f.GetIndex())
			isCheched &= csv.lr_f.SetText(s);
		else if(row_count == csv.compart_link_id.GetIndex())
			isCheched &= csv.compart_link_id.SetText(s);
		else if(row_count == csv.compart_link_direction_c.GetIndex())
			isCheched &= csv.compart_link_direction_c.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRelCompartLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadBorderLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::BorderLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::BorderLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::BorderLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listBorderLink.size() > 0)
					{
						//OID�s���ǉ��ς� ���� �ǉ��ς݂̃I�u�W�F�N�g�ɒǋL
						Polyline addedGeo = _csv->listBorderLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listBorderLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "�C���|�[�g�G���[\t������`�s���G���[�̂��߃C���|�[�g��Skip���܂���");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.imaginary_boder_f.GetIndex())
			isCheched &= csv.imaginary_boder_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listBorderLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadBorderPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::BorderPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::BorderPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::BorderPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.border_link_id.GetIndex())
			isCheched &= csv.border_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listBorderPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelBorderLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RelBorderLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RelBorderLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelBorderLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.lr_f.GetIndex())
			isCheched &= csv.lr_f.SetText(s);
		else if(row_count == csv.border_link_id.GetIndex())
			isCheched &= csv.border_link_id.SetText(s);
		else if(row_count == csv.border_link_direction_c.GetIndex())
			isCheched &= csv.border_link_direction_c.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRelBorderLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadSignalPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::SignalPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::SignalPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::SignalPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.horizontal_angle.GetIndex())
			isCheched &= csv.horizontal_angle.SetText(s);
		else if(row_count == csv.signal_mold_c.GetIndex())
			isCheched &= csv.signal_mold_c.SetText(s);
		else if(row_count == csv.lamp_count.GetIndex())
			isCheched &= csv.lamp_count.SetText(s);
		else if(row_count == csv.arrow_f.GetIndex())
			isCheched &= csv.arrow_f.SetText(s);
		else if(row_count == csv.warning_f.GetIndex())
			isCheched &= csv.warning_f.SetText(s);
		else if(row_count == csv.independent_f.GetIndex())
			isCheched &= csv.independent_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listSignalPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelSignalPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RelSignalPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RelSignalPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelSignalPoint csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_node_id.GetIndex())
			isCheched &= csv.lane_node_id.SetText(s);
		else if(row_count == csv.signal_point_id.GetIndex())
			isCheched &= csv.signal_point_id.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRelSignalPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRoadSignPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RoadSignPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RoadSignPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RoadSignPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.horizontal_angle.GetIndex())
			isCheched &= csv.horizontal_angle.SetText(s);
		else if(row_count == csv.roadsign_c.GetIndex())
			isCheched &= csv.roadsign_c.SetText(s);
		else if(row_count == csv.variable_f.GetIndex())
			isCheched &= csv.variable_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRoadSignPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelRoadSignPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RelRoadSignPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RelRoadSignPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelRoadSignPoint csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_node_id.GetIndex())
			isCheched &= csv.lane_node_id.SetText(s);
		else if(row_count == csv.roadsign_point_id.GetIndex())
			isCheched &= csv.roadsign_point_id.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRelRoadSignPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCrossWalkLine(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::CrossWalkLine::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::CrossWalkLine::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::CrossWalkLine csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listCrossWalkLine.size() > 0)
					{
						//OID�s���ǉ��ς� ���� �ǉ��ς݂̃I�u�W�F�N�g�ɒǋL
						Polyline addedGeo = _csv->listCrossWalkLine.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listCrossWalkLine.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "�C���|�[�g�G���[\t������`�s���G���[�̂��߃C���|�[�g��Skip���܂���");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listCrossWalkLine.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelCrossWalkLine(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RelCrossWalkLine::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RelCrossWalkLine::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelCrossWalkLine csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_node_id.GetIndex())
			isCheched &= csv.lane_node_id.SetText(s);
		else if(row_count == csv.crosswalk_line_id.GetIndex())
			isCheched &= csv.crosswalk_line_id.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRelCrossWalkLine.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadVirtualCompartLink	(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::VirtualCompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::VirtualCompartLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::VirtualCompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listVirtualCompartLink.size() > 0)
					{
						//OID�s���ǉ��ς� ���� �ǉ��ς݂̃I�u�W�F�N�g�ɒǋL
						Polyline addedGeo = _csv->listVirtualCompartLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listVirtualCompartLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "�C���|�[�g�G���[\t������`�s���G���[�̂��߃C���|�[�g��Skip���܂���");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.compart_line_c.GetIndex())
			isCheched &= csv.compart_line_c.SetText(s);
		else if(row_count == csv.compart_style_c.GetIndex())
			isCheched &= csv.compart_style_c.SetText(s);
		else if(row_count == csv.slowdown_label_c.GetIndex())
			isCheched &= csv.slowdown_label_c.SetText(s);
		else if(row_count == csv.installation_f.GetIndex())
			isCheched &= csv.installation_f.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listVirtualCompartLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadVirtualCompartPoint	(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	//�g��Ȃ����Ǔǂݍ���ł���
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::VirtualCompartPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::VirtualCompartPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::VirtualCompartPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.v_compart_link_id.GetIndex())
			isCheched &= csv.v_compart_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listVirtualCompartPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelVirtualCompartLink	(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	//�g��Ȃ����Ǔǂݍ���ł���
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::RelVirtualCompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::RelVirtualCompartLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelVirtualCompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.lr_f.GetIndex())
			isCheched &= csv.lr_f.SetText(s);
		else if(row_count == csv.v_compart_link_id.GetIndex())
			isCheched &= csv.v_compart_link_id.SetText(s);
		else if(row_count == csv.v_compart_link_direction_c.GetIndex())
			isCheched &= csv.v_compart_link_direction_c.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listRelVirtualCompartLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCoveringArea			(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::CoveringArea::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::CoveringArea::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::CoveringArea csv;
	long row_count = 1;
	bool isCheched = true;
	Polygon geometry;
	long long oid;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listCoveringArea.size() > 0)
					{
						//OID�s���ǉ��ς� ���� �ǉ��ς݂̃I�u�W�F�N�g�ɒǋL
						Polygon addedGeo = _csv->listCoveringArea.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listCoveringArea.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "�C���|�[�g�G���[\t������`�s���G���[�̂��߃C���|�[�g��Skip���܂���");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listCoveringArea.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "�C���|�[�g�G���[", "�G���[�̂��߃C���|�[�g��Skip���܂���","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadSpatialRef(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::SpatialRef::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::SpatialRef::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::SpatialRef csv;
	long row_count = 1;
	bool isCheched = true;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.srid.GetIndex())
			isCheched &= csv.srid.SetText(s);
		else if(row_count == csv.name.GetIndex())
			isCheched &= csv.name.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listSpatialRef.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[\t�G���[�̂��߃C���|�[�g��Skip���܂���");
	}

	return isCheched;
}

bool SurveyCSVReader::ReadVersion(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//�񐔃`�F�b�N
	if(tokenCount != SurveyCSV::Version::ROW_COUNT)
	{
		LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(tokenCount), "�񐔏��:" + std::to_string(SurveyCSV::Version::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::Version csv;
	long row_count = 1;
	bool isCheched = true;

	std::string lon, lat, height;
	// �v�f�̊i�[
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.data_def.GetIndex())
			isCheched &= csv.data_def.SetText(s);
		else if(row_count == csv.format_def.GetIndex())
			isCheched &= csv.format_def.SetText(s);
		else
		{
			LogUtil::printError(6, "�񐔃G���[","��:" + std::to_string(row_count), "�񐔏��:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// ���X�g�ɒǉ�
	if(isCheched)
	{
		_csv->listVersion.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "�C���|�[�g�G���[\t�G���[�̂��߃C���|�[�g��Skip���܂���");
	}

	return isCheched;
}

SurveyCSVPtr SurveyCSVReader::GetSurveyCSVPtr()
{
	return _csv;
}