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
#include "PGFGDBConvertProcessor.h"
#include <regex>
#include <mutex>
#include <memory>
#include <meshutil.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ErrorObject2.h>
#include <GeometryRule.h>
#include <RowContainer.h>
#include <TableContainer.h>
#include "Notifier.h"

using namespace sindy;

namespace sindy {
extern bool g_bSetHistory;
}

typedef std::basic_regex<TCHAR> tregex;
typedef std::match_results<const TCHAR *> tcmatch;
typedef std::match_results<uh::tstring::const_iterator> tsmatch;
typedef std::regex_token_iterator<uh::tstring::const_iterator> tregex_token_iterator;

bool IsEmpty( IGeometryPtr geom ); // ClipUtility.h

namespace{
	const long INDICATIONROWCOUNT = 50000;
}
namespace sindy {
_ISetPtr MultiGeometry2Geometries( IGeometry* geom ); // GeometryBase.cpp
} // sindy

// �}���`�p�[�g�`���ʃt�B�[�`���Ƃ��ĕ�������
void MultiGeometry2Geometries( CRowBase& row, CTableContainer& table )
{
	_ISetPtr clipGeoms( sindy::MultiGeometry2Geometries( (IGeometry*)*row.GetShape() ) );
	if( ! clipGeoms )
		return;

	long count = 0;
	clipGeoms->Reset();
	clipGeoms->get_Count( &count );
	bool orgSetted = false;
	for( long i = 0; i < count; ++i )
	{
		IUnknownPtr unk;
		clipGeoms->Next( &unk );
		if( IsEmpty(unk) )
			continue;

		if( !orgSetted )
		{
			row.CRowBase::SetShape((IGeometryPtr)unk);
			orgSetted = true;
		}
		else {
			CRowBase& newRow = *table.Create();
			newRow.CreateCache();
			row.CopyTo( newRow, true, false );
			newRow.CRowBase::SetShape((IGeometryPtr)unk);
		}
	}
}

// find_by_key�p�̃L�[���X�g��expType�̏�������쐬����
std::list<CString> createKeyList( const SPConditionExpressions& cond, ExpressionType expType, const CTableContainer& table )  // TODO: ProcessorUtility.cpp �Ƃ�����Ă�������
{
	std::list<CString> ret; // �߂�l

	if( !cond )
		return ret;

	if( cond->isContainer() )
	{
		const ConditionExpressionContainer& con = *std::static_pointer_cast<ConditionExpressionContainer>(cond);
		if( !con.isAndContainer )
		{
			notifier::critical::versatile(_T("ERROR : ��r������or�͎g���܂���"));
			return ret;
		}
		for( const auto& cond2 : con.vecExp )
		{
			for( const auto& key : createKeyList( cond2, expType, table ) )
				ret.push_back(key);
		}
	}
	else {
		const ConditionExpression& exp = *std::static_pointer_cast<ConditionExpression>(cond);
		// ���ڎw��Ais null�Ais not null���g�p����Ă�������͖�������
		// �������Ȃ��ƁAkey��value�̐�������Ȃ���find_by_key���g�p�ł��Ȃ��B
		// ���̊֐���TargetCondition�̏�����find_by_key���g�����p�Ȃ̂ŁA�����
		// �d�l�Ƃ���
		if( ( exp.leftValue.type  == ExpressionTypeSrc || exp.leftValue.type  == ExpressionTypeDst ) && 
			( exp.rightValue.type == ExpressionTypeSrc || exp.rightValue.type == ExpressionTypeDst ) )
		{
			if( exp.leftValue.type == expType )
				ret.push_back( exp.leftValue.col.c_str() );
			else if( exp.rightValue.type == expType )
				ret.push_back( exp.rightValue.col.c_str() );
		}
	}
	// �`��t�B�[���h���w�肳��Ă���ꍇ�͕s���ȏ����Ȃ̂ňُ�I������
	for( const auto& key : ret )
	{
		long fieldIndex = table.GetFieldMap()->HasField(key);
		if( fieldIndex == table.GetFieldMap()->GetShapeIndex() )
		{
			std::wstringstream ss;
			ss << L"��r�����Ɍ`��͎g���܂���F" << (LPCWSTR)CStringW(key);
			ret.clear();
			notifier::critical::versatile(ss.str());
		}
	}

	// ���݂��Ȃ��J�������w�肳��Ă��Ȃ����`�F�b�N
	// ���݂��Ȃ��J����������ꍇ�͋�ɂ���
	for( const auto& key : ret )
	{
		long fieldIndex = table.GetFieldMap()->HasField(key);
		if( 0 > fieldIndex )
		{
			std::wstringstream ss;
			ss << L"TABLE[" << (LPCWSTR)CStringW(table.GetTableName()) << L"] �w�肳��Ă���J���������݂��܂��� : " << (LPCWSTR)CStringW(key);
			notifier::versatile(ss.str(), std::wcout);
			ret.clear();
			break;
		}
	}
	return ret;
}

// find_by_key�p�̒l���X�g���쐬����
std::list<CComVariant> createValueList( const sindy::CRowBase& row, const std::list<CString>& cols ) // // TODO: ProcessorUtility.cpp �Ƃ�����Ă�������
{
	std::list<CComVariant> ret; // �߂�l

	for( const auto& col : cols )
	{
		long idx = row.GetFieldMap()->HasField(col);
		if( 0 > idx )
		{
			std::wstringstream ss;
			ss << _T("���݂��Ȃ��J�������w�肵�Ă��܂��F") << (LPCWSTR)CStringW(col);
			notifier::critical::versatile(ss.str());
			return ret;
		}
		else
			ret.push_back( row.GetValue(idx) );
	}
	return ret;
}

/// ���O�����ʂ��Ă���J�����̃C���f�b�N�X�̃y�A���쐬����
void createIndexList( const CFieldMap& srcFields, const CFieldMap& dstFields, const Arguments& args, IndexList& idxs, bool& hasGeom )
{
	for( const auto& field : dstFields )
	{
		if( !field.editable ) continue;

		uh::tstring fieldName((LPCTSTR)field.name);
		if( esriFieldTypeGeometry == field.type )
		{
			hasGeom = true;
			if( idxs.copyall ) idxs.copyshape = true;
			else
			{
				// �X�V�Ώۂ̃t�B�[���h�Ɋ܂܂�Ȃ��A���邢�́A�X�V�Ώۂł͂Ȃ�
				auto it = idxs.find(fieldName);
				if( it == idxs.end() )
				{
					idxs.copyshape = false;
					continue;
				}

				// �X�V�ΏۂȂ�A�\�[�X���̒l���`��t�B�[���h�łȂ��ꍇ�ɃG���[�ƂȂ�
				for( auto& idxPair : it->second )
				{
					if( srcFields.HasField(idxPair.first.srcName.c_str()) != srcFields.GetShapeIndex() )
					{
						std::wstringstream ss;
						ss << L"�`��t�B�[���h���X�V�ł���̂͌`��t�B�[���h�����ł�";
						notifier::critical::versatile(ss.str());
					}
				}
				idxs.copyshape = true;
			}
			continue;
		}

		long srcIdx = srcFields.HasField( field.name );
		long dstIdx = dstFields.HasField( field.name );
		if( 0 > dstIdx )
			continue;

		// ���o�^�̏ꍇ�̓��R�[�h�l�^�C�v�Ƃ���
		auto& it = idxs.find(fieldName);
		if( it == idxs.end() )
		{
			// �S�J�����R�s�[�̏ꍇ�́A�s������₤
			if( idxs.copyall )
			{
				IndexPair idxPair;
				idxPair.type = IndexPair::ValType::RowValue;
				idxs[fieldName].push_back(std::make_pair(idxPair,SPConditionExpressions()));
			}
			else
				continue;
		}

		for( auto& idxPair : idxs[fieldName] )
		{
			idxPair.first.dstIdx = dstIdx;
			switch(idxPair.first.type)
			{
			case IndexPair::ValType::RowValue:
			// Cache�^�C�v�͂ЂƂ܂��Asrc�̒l���i�[����
			case IndexPair::ValType::Cache:
				idxPair.first.srcIdx = srcIdx;
				break;
			case IndexPair::ValType::OptValue:
				idxPair.first.val = args.GetValue(idxPair.first.srcName.c_str());
				break;
			default:
				break;
			}
		}
	}
}

/**
 * @brief �����ƌ`����R�s�[����
 *
 * @param listIdx [in] �����R�s�[�\�ȃC���f�b�N�X�̑Ή��\�ifirst:�\�[�X�� second:dst���j
 * @param src [in] �R�s�[��
 * @param dst [in] �V�K�쐬���ꂽ���R�[�h
 * @param args [in] �I�v�V�����Ǘ��N���X
 * @param geom [in] null����Ȃ�����src�̌`����D�悳���dst�ɃZ�b�g����
 * @param hasGeom [in] �W�I���g���^�������Ă��邩�i���L�t�B�[���h���݂̂������j
 * @return ���������true
 */
bool Update( CTableContainer& table, const IndexList& listIdx, const CSPRowBase& src, CSPRowBase& dst, const Arguments& args, IGeometryPtr geom, bool hasGeom )
{
	// �L���b�V���쐬
	// �S�Ă��X�V�ΏۂɂȂ�킯�ł͂Ȃ��̂ŁA�L���b�V���쐬�͍X�V�Ώۂ݂̂Ɍ��肷��
	// ����ɂ��p�t�H�[�}���X�̒ቺ������ꍇ�́A�ꊇ�L���b�V������������i�������v���Ǝv���j
	dst->CreateCache();

	// ����
	for( const auto& idxExps : listIdx )
	{
		for( const auto& idxExp : idxExps.second )
		{
			const IndexPair& idx = idxExp.first;
			const SPConditionExpressions& exps = idxExp.second;
			if( !exps || IndexPair::Cache == idx.type || exps->eval( src, dst, args, std::list<uh::tstring>() ) ) // �����Ȃ���������
			{
				if( IndexPair::None == idx.type || 				        // �R�s�[�Ώۂł͂Ȃ�
					IndexPair::RowValue == idx.type && 0 > idx.srcIdx ) // ���R�[�h�l�Ȃ̂ɃC���f�b�N�X�ԍ����L���ł͂Ȃ�
					continue;

				dst->SetValue( idx.dstIdx, idx.value(*src) );
				break; // �̗p�����l�͈�����i�揟���j
			}
		}
	}

	// �W�I���g���^�������Ă��Ȃ��ꍇ�͂����ŏI��
	if(!hasGeom)
		return true;

	if( geom )
	{
		dst->SetShape( geom );
	}
	else if( listIdx.copyshape || listIdx.copyall )
	{
		// �`��
		dst->SetShape(src->GetShapeCopy());
	}

	// �}���`�p�[�g����
	if( args.m_dissolveMulti )
		MultiGeometry2Geometries( *dst, table );

	return true;
}

/**
 * @brief ���R�[�h��V�K�쐬����
 *
 * @note �V�K�쐬��Asrc���瑮�����R�s�[����i�J������������̂��̂����j
 * @param table [in] �쐬�Ώۃe�[�u��
 * @param listIdx [in] �����R�s�[�\�ȃC���f�b�N�X�̑Ή��\�ifirst:�\�[�X�� second:dst���j
 * @param src [in] �R�s�[��
 * @param args [in,out] �R�}���h���C������
 * @param hasGeom [in] �W�I���g���^�������Ă��邩
 * @param doCache [in] �L���b�V�����s���e�[�u���ł���ꍇ�Atrue
 * @param relationField [in] first:SRC�t�B�[���h second:DST�t�B�[���h
 * @return ���������true
 */
bool Create( CTableContainer& table, const IndexList& listIdx, const CSPRowBase& src, Arguments& args,
			bool hasGeom, bool doCache, const std::map<uh::tstring, uh::tstring>& relationField, RelationValues& relationValues)
{
	auto row = table.CreateDummyRowNoPush(); // ������t�^���Ă���R���e�i�ɓ˂����ށifind_by_key�΍�j
	auto ret = Update( table, listIdx, src, CAST_ROWB(CAST_ROWC(row)), args, nullptr, hasGeom );
	table.push_back( row, false );
	// �L���b�V�����s���K�v�̂���e�e�[�u���̏ꍇ
	if(doCache)
	{
		for(const auto& field : relationField)
		{
			const auto& key = std::make_pair(table.GetTableName(), field.second.c_str()); // �e�e�[�u�����A�t�B�[���h�����L�[�Ƃ���B
			relationValues[key][src->GetValue(field.first.c_str())] = row; // dst����row���Ɗi�[�Asrc���͎w�肳�ꂽ�t�B�[���h�̒l�̂݊i�[
		}
	}
	return ret;
}

void ErrorObject2Notifier( const uh::tstring& tableName, const CErrorObjectsBase& errs )
{
	for( const auto& err : errs )
	{
		for( const auto& info : *err )
		{
			auto obj = err->GetErrorObject();
			std::pair<double,double> pairP( -1, -1 );
			if( info->GetErrorGeometry() )
			{
				CGeometryBase cErrGeom( info->GetErrorGeometry() );
				CGeometryBase::const_point_iterator itPoint = cErrGeom.point_begin();
				if( itPoint != cErrGeom.point_end() )
					pairP = itPoint.get_coords();
			}
			else {
				if( (IGeometry*)obj->get<2>() )
				{
					CGeometryBase::const_point_iterator itPoint = obj->get<2>().point_begin();
					if( itPoint != obj->get<2>().point_end() )
						pairP = itPoint.get_coords();
				}
			}

			// �G���[���x��������쐬
			notifier::error_level level = notifier::error_level::notice;
			switch( info->GetErrorLevel() )
			{
			case sindyErrLevel_INFO: level = notifier::error_level::notice; break;
			case sindyErrLevel_WARN: level = notifier::error_level::warning; break;
			default:                 level = notifier::error_level::error; break;
			}

			notifier::reportObjInfo( tableName, obj->get<1>(), pairP.first, pairP.second, level, uh::tstring((LPCTSTR)info->GetErrorMessage()), std::wcerr );
		}
	}
}

// �Ώۃt�B�[�`����o�^����
bool RowMutex::regist( const uh::tstring& table, long oid, bool isSrc ) const
{
	if( !m_bEnable )
		return true;

	std::mutex mtx;
	{
		std::lock_guard<std::mutex> lock(mtx);
		static std::map<std::pair<uh::tstring,bool>, std::set<long>> g_mapRegister;
		auto& reg = g_mapRegister[std::make_pair(table,isSrc)];
		if( reg.find(oid) == reg.end() )
		{
			reg.insert(oid);
			return true;
		}
	}
	return false;
}

// ��������i�S�f�[�^�j
void Select(CTableContainer& con)
{
	IQueryFilterPtr query(AheInitQueryFilter());
	con._Select(query, false, false);
	con.CreateCache();
}

// ��������i��`�͈͓��j
void Select( IEnvelopePtr env, CTableContainer& con )
{
	// ��`�ł܂��͌���
	IQueryFilterPtr query( AheInitSpatialFilter( nullptr, env ) );
	con._Select( query, false, false );
	con.CreateCache();
	// ��`�ɐڂ��Ă������������e��
	IRelationalOperatorPtr rel(env);
	IGeometryPtr clipGeom;
	for( auto it = con.begin(); it != con.end(); )
	{
		IGeometryPtr geom = (IGeometry*)*CAST_ROWC(*it)->GetShape();
		IEnvelopePtr geomEnv;
		geom->get_Envelope(&geomEnv);
		// ������`�Ɋ܂܂����͖̂�������OK
		VARIANT_BOOL isContain = VARIANT_FALSE;
		rel->Contains(geomEnv, &isContain);
		if( isContain )
		{
			++it;
			continue;
		}
		// �����`��ŃN���b�v�������ʉ����c��Ȃ����̂̓R���e�i����폜
		if( !clipGeom )
			clipGeom = AheCreateClone(geom);
		((ITopologicalOperatorPtr)geom)->QueryClipped(env, clipGeom);
		if( IsEmpty(clipGeom) )
			it = con.erase(it);
		else
			++it;
	}
}

// �ҏW�Z�b�V�����J�n����
bool StratEditOperation(CEditor& output)
{
	// �ҏW�Z�b�V�����J�n
	if( !output.StartOperation() )
	{
		notifier::critical::versatile(_T("�ҏW�J�n�Ɏ��s"));
		return false;
	}
	return true;
}

// �ҏW�Z�b�V�����I������B�G���[���������ꍇ��Abort
bool StopEditOperation(CEditor& output, bool& hasError)
{
	if( hasError )
		output.AbortOperation();
	else {
		if(!(output.StopOperation() && output.SaveEditing()) )
		{
			hasError = true;
			return false;
		}
	}
	return true;
}

// ���̉�����B
bool Materiarize(CSPTableContainer& table, RowMutex& limitter)
{
	for( auto& col : *table )
	{
		CRowBase& row = *CAST_ROWB(CAST_ROWC(col));
		if( !(_IRow*)row && row.IsCreated() )
		{
			ITablePtr tbl((ITable*)*table);
			_IRowPtr ipRow;
			if( FAILED( tbl->CreateRow( &ipRow ) ) )
			{
				// ���R�[�h�쐬�Ɏ��s
				return false;
			}
			row.Materialization(ipRow);
			// ��d�����i�폜�j�h�~
			limitter.regist( row.GetTableName(), row.GetOID(), false );
		}
	}
	return true;
}

// TODO: Conditions.cpp�ŏ����ł���悤�ɂ���B
// ����AEnumOperator�𖳎����āArelationValues�ɒl������΍X�V����Ƃ��������ɂȂ��Ă���B�B(Cache�^�C�v�̏ꍇ�̂�)
// EnumOperator�ɐV������`���쐬���Ȃ��ƁAeval()�ł̔�r�͂ł��Ȃ��B
// ����Aeval()�� value �� value �̔�r�����A���̊֐��� value �� map �̂��߁Aeval()�ɓ����ꍇ�͌������K�v�B�B
bool SetRelation(const IndexList& idxList, const RelationValues& relationValues, CSPTableContainer& table)
{
	for(const auto& tblInfo : idxList)
	{
		// Column���
		for(const auto& columnInfo : tblInfo.second)
		{
			// Cache�^�C�v�̂�
			if(columnInfo.first.type != IndexPair::ValType::Cache)
				continue;
			ConditionExpression& cond( *std::static_pointer_cast<ConditionExpression>(columnInfo.second) );
			CString dstField = (cond.rightValue.type == ExpressionType::ExpressionTypeDst) ? cond.rightValue.col.c_str() : cond.leftValue.col.c_str();

			const auto foreignInfo = relationValues.find(std::make_pair(columnInfo.first.foreignTable, columnInfo.first.foreignField)); // �L���b�V�����X�g�̒�����֘A�e�[�u�����擾
			if(foreignInfo == relationValues.end())
				continue;

			// �X�V���������S���ɑ΂��ď������s���B
			for( auto& col : *table )
			{
				CSPRowBase& row = CAST_ROWB(CAST_ROWC(col));
				// �폜���R�[�h�͖���
				if(row->IsDeleted())
					continue;

				const auto& foreignVal = foreignInfo->second.find(row->GetValue(dstField)); // �֘A�e�[�u������Y�����郌�R�[�h�擾
				// �Y���̃��R�[�h������ΊO���L�[�t�B�[���h���X�V����
				if(foreignVal != foreignInfo->second.end())
				{
					row->CreateCache();
					CComVariant foreign = foreignVal->second->GetValue(foreignInfo->first.second.c_str());
					row->SetValue( dstField, foreign);
				}
			}
		}
	}

	return true;
}

bool PGFGDBConvertProcessor<uh::tstring>::proc( const uh::tstring& mesh )
{
	// ���O
	{
		std::wstringstream ss;
		ss << _T("�R���o�[�g�J�n") <<  _T(" mesh:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	// �t�H���_�w��Ȃ炱���ŃC���v�b�g�����[�N�X�y�[�X���I�[�v��
	if( isDirectory( m_strInput ) )
	{
		uh::tstring dbpath = GetWorkspaceString(mesh);
		if( dbpath.empty() )
		{
			std::wstringstream ss;
			ss << _T("���b�V�����X�g�ɑΉ�����t�@�C����������܂��� ���b�V���R�[�h : ") <<  mesh;
			std::cerr << uh::toStr(ss.str()) << std::endl;
			notifier::versatile(ss.str(), std::wcout);
			notifier::versatile(ss.str(), std::wcerr);
			return true; // �����͌p������
		}
		m_input.Workspace().Connect( dbpath.c_str() );
		if( !m_input.Workspace() )
		{
			std::wstringstream ss;
			ss << _T("���[�N�X�y�[�X�ɐڑ��ł��܂���ł��� : ") <<  dbpath;
			std::cerr << uh::toStr(ss.str()) << std::endl;
			notifier::critical::versatile(ss.str());
			return false;
		}
	}

	// �������Q�Ɨp�̒l��Argments�ɐݒ�
	m_args.m_systemValue[Arguments::MESHCODE] = mesh;

	// �ҏW�J�n
	if(!StratEditOperation(m_output))
		return false;

	bool hasError = false; // �����G���[�����������ǂ���

	// �R���o�[�g����
	ConvertTables(hasError, mesh);

	// �ҏW�Z�b�V�����I��
	StopEditOperation(m_output, hasError);

	std::wstringstream ss;
	if( !hasError )
	{
		ss << _T("�R���o�[�g�I��") <<  _T(" mesh:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}
	else {
		ss << _T("�R���o�[�g�I��") <<  _T(" mesh:") <<  mesh << _T(" �G���[����");
		notifier::critical::versatile(ss.str());
	}

	return !hasError;
}

bool PGFGDBConvertProcessor<uh::tstring>::preprocess()
{
	// �ڑ��m�F(input)
	if( !isDirectory( m_strInput ) )
	{
		if( !m_input.Workspace() )
		{
			notifier::critical::versatile(_T("input���ɐڑ����s"));
			return false;
		}
	}

	// �ڑ��m�F(output)
	if( !m_output.Workspace() )
	{
		notifier::critical::versatile(_T("output���ɐڑ����s"));
		return false;
	}

	std::list<uh::tstring> listTarget;
	for( const auto& table : m_manager.m_listTarget )
	{
		// �ҏW�Ώۂ��o�[�W�����Ή�����Ή����ɉ����āA�ҏW���J�n����
		listTarget.push_back( table.first );

		// �O���L�[�Ɋ֘A����e�[�u���A�t�B�[���h�����炩���߉�͂��Ă���
		if(!getRelationField(table.second, m_relationFields))
			return false;
	}

	bool good = true;
	// �O���L�[�Ɋ֘A����e�[�u���A�t�B�[���h�����݂��邩���O�Ɋm�F����
	// �e�[�u�����Ȃ��ꍇ�͏�������΂����̂ŁA�����𑱂��Ė��Ȃ��B���O�����o��
	for( const auto& relTable : m_relationFields )
	{
		for( const auto& relField : relTable.second )
		{
			// input��
			if( m_input.Workspace() )
			{
				ITablePtr ipSrcT( m_input.Workspace().OpenTable(relTable.first.c_str()) );
				if( !ipSrcT )
				{
					std::wstringstream ss;
					ss << _T("�e�[�u��������܂���iSRC���F") << relTable.first << _T("�j");
					notifier::versatile(ss.str(), std::wcerr);
				}
				else if( -1 == ipSrcT->GetFields()->_FindField(relField.first.c_str()) )
				{
					std::wstringstream ss;
					ss << _T("�t�B�[���h������܂���iSRC���F") << relField.first << _T("�j");
					notifier::versatile(ss.str(), std::wcerr);
					good = false;
				}
			}
			// output��
			ITablePtr ipDstT( m_output.Workspace().OpenTable(relTable.first.c_str()) );
			if( !ipDstT )
			{
				std::wstringstream ss;
				ss << _T("�e�[�u��������܂���iDST���F") << relTable.first << _T("�j");
				notifier::versatile(ss.str(), std::wcerr);
			}
			else if( -1 == ipDstT->GetFields()->_FindField(relField.second.c_str()) )
			{
				std::wstringstream ss;
				ss << _T("�t�B�[���h������܂���iDST���F") << relField.second << _T("�j");
				notifier::versatile(ss.str(), std::wcerr);
				good = false;
			}
		}
	}
	if( !good )
		return false;

	return m_output.StartEditingByObjectType(listTarget); // �G���[���b�Z�[�W��CEditor�ŏo��
}

bool PGFGDBConvertProcessor<uh::tstring>::postprocess()
{
	return m_output.StopEditing(true); // �G���[���b�Z�[�W��CEditor�ŏo��
}

bool PGFGDBConvertProcessor<uh::tstring>::bulkproc(const std::vector<uh::tstring>& srcs)
{
	// ���O
	notifier::versatile(_T("�R���o�[�g�J�n bulk:"), std::wcout);

	m_args.m_systemValue[Arguments::MESHCODE].clear();

	bool hasError = false;
	// �f�B���N�g���̏ꍇ
	// TODO: [Relation]�^�O���g�p���Ă��āA�f�B���N�g�����w�肵���ꍇ�����삷��悤�ɂ��Ă��邪�A
	//       �O���L�[���d������\�����������߁A����ɓ��삵�Ȃ��B�B
	//       src1��OBJECTID_ORG:100���V�K�쐬����Asrc2�ł�OBJECTID_ORG:100���V�K�쐬���ꂽ�ꍇ�A
	//       src1��src2�̏�L�I�u�W�F�N�g�͂��ꂼ��ʂ̃I�u�W�F�N�g�ł��邪�A�O���L�[����OBJECTID_ORG���d�����Ă��܂��B
	//       m_relationValues�̃L�[�Ƃ��āA���b�V���R�[�h���ǉ�����ΎQ�Ɛ悪������悤�ɂȂ�A�͂��B
	if( isDirectory( m_strInput ) )
	{
		for(const auto& mesh : srcs)
		{
			uh::tstring dbpath = GetWorkspaceString(mesh);
			if( dbpath.empty() )
			{
				std::wstringstream ss;
				ss << _T("���b�V�����X�g�ɑΉ�����t�@�C����������܂��� ���b�V���R�[�h : ") <<  mesh;
				std::cerr << uh::toStr(ss.str()) << std::endl;
				notifier::versatile(ss.str(), std::wcout);
				notifier::versatile(ss.str(), std::wcerr);
				hasError = true;
				continue;
			}
			m_input.Workspace().Connect( dbpath.c_str() );
			if( !m_input.Workspace() )
			{
				std::wstringstream ss;
				ss << _T("���[�N�X�y�[�X�ɐڑ��ł��܂���ł��� : ") <<  dbpath;
				std::cerr << uh::toStr(ss.str()) << std::endl;
				notifier::critical::versatile(ss.str());
				hasError = true;
				continue;
			}

			// �������Q�Ɨp�̒l��Argments�ɐݒ�
			m_args.m_systemValue[Arguments::MESHCODE] = mesh;

			// �ҏW�J�n
			if(!StratEditOperation(m_output))
				return false;

			// �R���o�[�g����(���b�V����`�P�ʂŏ��������Ȃ����߁Amesh�������Ƃ��ēn���Ȃ�)
			ConvertTables(hasError);

			// �ҏW�Z�b�V�����I��
			StopEditOperation(m_output, hasError);
		}
	}

	else
	{
		// �ҏW�J�n
		if(!StratEditOperation(m_output))
			return false;

		// �R���o�[�g����
		ConvertTables(hasError);

		// �ҏW�Z�b�V�����I��
		StopEditOperation(m_output, hasError);
	}

	std::wstringstream ss;
	if( !hasError )
	{
		ss << _T("�R���o�[�g�I�� bulk:");
		notifier::versatile(ss.str(), std::wcout);
	}
	else {
		ss << _T("�R���o�[�g�I�� bulk:") << _T(" �G���[����");
		notifier::critical::versatile(ss.str());
	}

	return !hasError;
}

// �t�H���_�w��̏ꍇ�̃��b�V���P�ʂ�DB�p�X���擾����
template<>
uh::tstring PGFGDBConvertProcessor<uh::tstring>::GetWorkspaceString( const uh::tstring& mesh ) const
{
	// ���b�V���R�[�h�ɑΉ�����PGFGDB�t�@�C���p�X���擾
	std::wstringstream s;
	s << "\\\\" << mesh << "\\.[mg]db$"; // �u\\59414111.mdb�v�Ƃ��Ƀ}�b�`
	tregex r(s.str()); 
	auto it = std::find_if( m_args.m_paths.begin(), m_args.m_paths.end(), [&mesh, &r](const uh::tstring& str) { return std::regex_search( str, r ); } );
	if( it != m_args.m_paths.end() )
		return *it;
	return _T("");
}

template<>
void PGFGDBConvertProcessor<uh::tstring>::ConvertTables(bool& hasError, const uh::tstring& mesh)
{
	for( auto& table : m_manager.m_listTarget )
	{
		// ���[�h�ɂ���āA�������Ȃ��e�[�u���͔�΂�
		if(mesh.empty())
		{
			if(!table.second.bulkexe)
				continue;
		}
		else
		{
			if(table.second.bulkexe)
				continue;
		}

		CSPTableContainer dstT;
		// �R���o�[�g����
		if(!ConvertTable(table, dstT, mesh))
			continue;

		// ���̉�
		if(!Materiarize(dstT, m_limitter))
		{
			std::wstringstream ss;
			ss << _T("���R�[�h�̍쐬�Ɏ��s���܂����̂ŏ����𒆒f���܂�");
			if(!mesh.empty())
				ss << _T("mesh:") << mesh;
			notifier::critical::versatile(ss.str());
			hasError = true;
		}

		CString dstTableName = dstT->GetTableName();
		CErrorObjectsBase errs;
#if 0 // ���C�����Ƃ̃G���[���o�Ă��Ă��A�������Ă������B���������̂͌�Ń`�F�b�J�[�ł��Ƃ������Ƃ�
		// �_���`�F�b�N
		CLogicRule rule;
		dstT->CheckLogic( rule, errs );
		ErrorObject2Notifier( dstTableName, errs );
#endif // if 0

		if( !SetRelation(table.second, m_relationValues, dstT) )
			hasError = true;

		// Store
		// �X�V������t�^���邩�ǂ���
		bool backup = g_bSetHistory;
		g_bSetHistory = m_manager.m_globalAdditionalUpdate.sindyHistory;
		if( errorcode::sindyErr_NoErr != dstT->Store(errs) )
		{
			hasError = true;
			ErrorObject2Notifier( dstTableName.GetString(), errs );
		}
		g_bSetHistory = backup;

		// TODO: ���C���P�ʂł̊e�����̌������o�͂��ׂ����H
		// �����̃��C��������o��̂��������̂ŁA�I�v�V�����ŏo���o���Ȃ������߂���悤�ɂ��邩�H
	}
}

template<>
bool PGFGDBConvertProcessor<uh::tstring>::ConvertTable(const std::pair<uh::tstring,IndexList>& table, CSPTableContainer& dstT, const uh::tstring& mesh)
{
	bool ret = true;
	// �I�[�i�[���t���̃e�[�u�����擾
	uh::tstring srcTableName = m_args.getInputTableName(table.first);
	uh::tstring dstTableName = m_args.getOutputTableName(table.first);
	// ���X�V�Ώێ擾
	// �e�[�u���t�@�C���_������
	CTableFinder srcFinder, dstFinder;
	{
		CModel model;
		model.Add( table.first.c_str(), sindy::schema::kObjectID, nullptr, nullptr );
		ITablePtr ipSrcT( m_input.Workspace().OpenTable(srcTableName.c_str()) );
		if( !ipSrcT )
		{
			std::wstringstream ss;
			ss << _T("�e�[�u�����Ȃ����߁A�������΂��܂��iSRC���F") << srcTableName << _T("�j");
			notifier::versatile(ss.str(), std::wcerr);
			return false;
		}
		ITablePtr ipDstT( m_output.Workspace().OpenTable(dstTableName.c_str()) );
		if( !ipDstT )
		{
			std::wstringstream ss;
			ss << _T("�e�[�u�����Ȃ����߁A�������΂��܂��iDST���F") << dstTableName << _T("�j");
			notifier::versatile(ss.str(), std::wcerr);
			return false;
		}

		srcFinder.InitCollection( ipSrcT, model );
		dstFinder.InitCollection( ipDstT, model );
	}

	CSPTableContainer srcT( srcFinder.FindTable( srcTableName.c_str() ) );
	dstT = dstFinder.FindTable( dstTableName.c_str() );

	// ���b�V���`��Ō�������
	// �I�v�V�������w�肳��Ă�����I���f�}���h�����Ƃ���ibug 11710�j
	// �A���A�I���f�}���h��������ꍇ��DST���ł̃��[�v���ł��Ȃ����ɒ��ӁI�I
	// ���b�V�����w�肳��Ă��Ȃ������S��(bulk���[�h)
	if(!mesh.empty())
	{
		if( m_args.m_output_ondemand_search )
			dstT->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach ); // �I���f�}���h����
		IEnvelopePtr env(MESH2ENV( _ttol(mesh.c_str()), AheGetSpatialReference( (ITable*)*srcT ) ));
		Select( env, *srcT );
		// dst���̌�����dst���̏��������݂���Ƃ��݂̂Ɍ��肷��
		// �������邱�ƂŁA�P���ȃC���|�[�g�������ɂ̓C���f�b�N�X���폜���Ď��s�ł���
		if( m_manager.hasDstCondition() && dstT->GetAutoCache() == sindyContainerAutoCacheNone )
			Select( env, *dstT );
	}
	else
	{
		Select(*srcT);
		// ���R�[�h���������Ɨ�����̂ŁA�ڈ��̒l�������R�[�h���������ꍇ�̓I���f�}���h�����ɂ���
		if( m_args.m_output_ondemand_search )
			dstT->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach ); // �I���f�}���h����
		else if( INDICATIONROWCOUNT <= dstT->RowCount(AheInitQueryFilter()) )
		{
			std::wstringstream ss;
			ss << L"���R�[�h�����������߁A�I���f�}���h�����ɂ��܂��F" << uh::toWStr(dstT->GetTableName());
			notifier::versatile(ss.str());
			dstT->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach ); // �I���f�}���h����
		}
		if( m_manager.hasDstCondition() && dstT->GetAutoCache() == sindyContainerAutoCacheNone )
			Select(*dstT);
	}

	// �X�V�����邽�߂̋��ʃC���f�b�N�X���X�g���쐬����
	IndexList listIdx = table.second;
	bool hasGeom = false;
	createIndexList( *srcT->GetFieldMap(), *dstT->GetFieldMap(), m_args, listIdx, hasGeom );
	// �L���b�V�����s���e�[�u����
	const auto& tgt = m_relationFields.find(table.first);
	bool doCache = (tgt != m_relationFields.end());
	std::map<uh::tstring, uh::tstring> relationField;
	if(doCache)
		relationField = tgt->second;

	// ���[�v�^�C�v�Ń��[�v�Ώۂ�؂�ւ�
	for( const auto& condition : m_manager.m_listConditions )
	{
		// ���[�v�Ώۂ�����
		CSPTableContainer outerT, innerT;
		ExpressionType keyType = ExpressionTypeUnknown; //!< find_by_key�Ŏg�p����L�[���X�g���쐬����ۂ̏���
		ExpressionType valType = ExpressionTypeUnknown; //!< find_by_key�Ŏg�p����l���X�g���쐬����ۂ̏���
		switch( condition.first )
		{
		case LoopType::LoopSRC: 
			outerT = srcT;
			innerT = dstT;
			keyType = ExpressionTypeDst;
			valType = ExpressionTypeSrc;
			break;
		case LoopType::LoopDST:
			outerT = dstT;
			innerT = srcT;
			keyType = ExpressionTypeSrc;
			valType = ExpressionTypeDst;
			break;
		default:
			// TODO: �G���[���b�Z�[�W
			// return false;
			break;
		}
		const auto& targetCondition = condition.second.first;
		const auto& modifyCondition = condition.second.second;
		// ���X�V����
		// ��r�����邽�߂̃L�[���X�g���쐬����i�����ő��݃`�F�b�N������j
		auto innerKeyList = createKeyList(targetCondition.exps, keyType, *innerT);
		auto outerKeyList = createKeyList(targetCondition.exps, valType, *outerT);

		// LoopTarget�Ń��[�v
		for( auto& con : *outerT )
		{
			std::list<uh::tstring> messages;
			CSPRowBase outerRow = CAST_ROWB(CAST_ROWC(con));
			CSPRowBase srcRow = ( LoopType::LoopSRC == condition.first ) ? outerRow : CSPRowBase();
			CSPRowBase dstRow = ( LoopType::LoopDST == condition.first ) ? outerRow : CSPRowBase();
			// find_by_key���邽�߂̒l�̃��X�g���쐬����
			std::list<CComVariant> valueList = createValueList( *outerRow, outerKeyList );
			// �X�V�Ώی���
			// ��������Ȃ��Ȃ�A�����s�����Ƃ��Č������Ȃ�
			if( !innerKeyList.empty() && 
				innerKeyList.size() == outerKeyList.size() )
			{
				// �L���b�V���R���e�i�������������������̂�T��
				auto& it = innerT->find_by_key( innerKeyList, valueList );
				if( it != innerT->end() )
				{
					if( LoopType::LoopSRC == condition.first )
						dstRow = CAST_ROWB(CAST_ROWC(*it));
					else if( LoopType::LoopDST == condition.first )
						srcRow = CAST_ROWB(CAST_ROWC(*it));
				}
			}
			// TargetCondition��message�����邩������Ȃ��̂ŁA�����ŏ����]������
			if( targetCondition.exps )
				targetCondition.exps->eval( srcRow, dstRow, m_args, messages );

			// �X�V�A�폜�A�쐬�Ǝ�ނ����邪�A�����̏��Ԃ�xml�L�ڏ��Ƃ���
			for( const auto& mcond : modifyCondition )
			{
				// ��r������y�A��������
				switch( mcond->type )
				{
				case Condition::ConditionType::Create:
					{
						// SRC���Ȃ��ꍇ�͂���������邱�Ƃ��ł��Ȃ�
						if( ! srcRow )
							break;

						// �V�K�쐬
						// ���V�K�쐬����
						// CreateConditon�ɏ�����Ă�������ɍ��v����Ȃ�src�̓��e��dest�ɐV�K�쐬����
						if( !mcond->exps || // �����Ȃ��Ȃ�A�������Ƃ�������
							mcond->exps->eval( srcRow, dstRow, m_args, messages ) )
						{
							if( m_limitter.regist( srcRow->GetTableName(), srcRow->GetOID(), true ) ) // ��d�����h�~
							{
								if( !Create( *dstT, listIdx, srcRow, m_args, hasGeom, doCache, relationField, m_relationValues ) )
								{
									std::wstringstream ss;
									ss << _T("�V�K�쐬�Ɏ��s���܂����iSRC���F") << srcRow->GetOID() << _T("�j");
									notifier::reportObjInfo( dstTableName, -1L, ss.str(), std::wcerr );
									ret = false;
								}
							}
						}
					}
					break;
				case Condition::ConditionType::Update:
				case Condition::ConditionType::Delete:
					{
						if( !dstRow )
							break; // dst�����݂��Ȃ���Ώ����̂��悤���Ȃ�

						// ��x�X�V�������͓̂�x�ƍX�V�����Ȃ�
						// ���ɍ폜�̏ꍇ�A�O�̏����ŐV�K�쐬����Ă���ƁA�uis empty�v������
						// �Ή�������̂��Ȃ����̂��폜����悤�ɒ�`���Ă���ꍇ�ɁA�܊p�����
						// ���̂��S�ď����Ă��܂��B
						// �������Ƃ͑����X�V�ɂ������邱�Ɓi�����X�V�������ʍ폜�̏����ɍ�����
						// ���ɂȂ��Ă��܂����Ƃ��j�Ȃ̂ŁA�ύX���������Ă�����̂͂���ȏ�X�V
						// �����Ȃ��B
						// �X�V�Ɋւ��Ă͂���قǖ��ɂ��Ȃ�Ȃ��������A�����Ƃ��ē��ꂳ���Ă���
						if( dstRow->Changed() )
							break;

						// �X�V/�폜
						// ���폜����
						// DeleteCondition�ɏ�����Ă�������ɍ��v����Ȃ�폜
						// ���̏ꍇ�X�V�����ȍ~�͍s��Ȃ�
						// ���X�V����
						// UpdateCondition�ɏ�����Ă�������ɍ��v����Ȃ�src�̓��e��dst�ɔ��f����
						// �A���A�o���ŃJ����������v���Ă���ۏ؂��Ȃ��̂ŁA���O�ɋ��ʂ̃J���������X�g
						// ���쐬���Ă����i�ҏW�\�J�����̂݁j�B�`��Ɋւ��ẮA�t�B�[�`���N���X�Ȃ�K���ΏۂƂ���
						// ���̎��A�X�V�ΏۃJ�������i�荞�߂����������I�ȍ�Ƃ̃}�[�W���\�ɂȂ��ėǂ���������Ȃ����A
						// ExportPGDB,DiffImportPGDB�ł͂��̂悤�ȍi�荞�݂͍s���Ă��Ȃ����߁A�Ƃ肠�����S�ẴJ������ΏۂƂ��� TODO:��ŗv����`�ɂ����H
						// �E�����X�V
						// �E�`��X�V
						if( !mcond->exps->eval( srcRow, dstRow, m_args, messages ) )
							break;

						if( mcond->type == Condition::ConditionType::Update )
						{
							if( m_limitter.regist( srcRow->GetTableName(), srcRow->GetOID(), true ) ) // ��d�����h�~
							{
								// �X�V
								if( !Update( *dstT, listIdx, srcRow, dstRow, m_args, nullptr, hasGeom ) )
								{
									std::wstringstream ss;
									ss << _T("����/�`��̍X�V�Ɏ��s���܂����iSRC���F") << srcRow->GetOID() << _T("�j");
									notifier::reportObjInfo(
										dstTableName, 
										dstRow->GetOID(), 
										dstRow->GetShape()->GetPoint(0).GetX(),
										dstRow->GetShape()->GetPoint(0).GetY(),
										notifier::error_level::fatal,
										ss.str(),
										std::wcerr );
									ret = false;
								}
							}
						}
						else {
							if( m_limitter.regist( dstRow->GetTableName(), dstRow->GetOID(), false ) ) // ��d�����h�~
							{
								// �폜
								dstRow->Delete(); // �폜
							}
						}
					}
					break;
				default:
					_ASSERTE(false);
					break;
				}
			}
		}
		// ���ǉ��X�V
		//   �ǉ��X�V�́AAdditionalUpdate�ɏ�����Ă�����e
		//   �������ԓI�ɁA�X�V�ƒǉ��ɑ΂��Ĕ��f����� <- �����Create,Update�̎��Ɉꏏ�ɍs��
	}
	return ret;
}
