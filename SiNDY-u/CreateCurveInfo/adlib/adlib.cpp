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
#include "ADLib.h"
#include <boost/assign.hpp>

namespace adlib
{
	void identifiedFileName( boost::filesystem::path& cFilePath, long cMaxCount )
	{
		using namespace boost;
		using namespace boost::filesystem;
		using namespace boost::posix_time;
		using namespace boost::gregorian;
		using namespace boost::filesystem;
		path aFilePath;
		ptime aPTime = second_clock::local_time();
		std::string aTimeStr;
		for (long i=1; i<cMaxCount; ++i)
		{
			aTimeStr = lexical_cast<std::string>(aPTime.date()) + "_" + lexical_cast<std::string>(i);

			aFilePath =
				(cFilePath.branch_path() / basename( cFilePath )).string() + "_" + aTimeStr + extension( cFilePath );
			if (! exists( aFilePath ))
			{
				break;
			}
		}
		cFilePath = aFilePath;
	}

	void outputSindyFreeStyleLogHeader( std::ostream& co, std::vector<std::string> cFieldNames )
	{
		co << "# FREESTYLELOG" << std::endl;
		for (std::vector<std::string>::const_iterator it = cFieldNames.begin(); it != cFieldNames.end(); ++it)
		{
			if (it != cFieldNames.begin())
			{
				co << "\t";
			}
			co << *it;
		}
		co << std::endl;
	}

	void setJapaneseLocaleWithCNumpunctFacet(void)
	{
		std::locale loc( std::locale( "japanese" ), "C", std::locale::numeric );
		std::locale::global( loc );
	}

	IWorkspacePtr getWorkspace( tstring cDBStr, boost::function<void (std::string)> cErrorFunc )
	{
		IWorkspacePtr ipWorkspace;
		bool success = true;
		try
		{
			HRESULT hr;
			ipWorkspace = sindy::_create_workspace( cDBStr.c_str(), hr, true, 10 );
			if (! ipWorkspace)
				{ success = false; }
		}
		catch (_com_error& e) {
			success = false;
		}

		if (! success)
		{
			cErrorFunc( std::string(CT2A(cDBStr.c_str())) );
		}

		// 例外が出ずにipWorkspaceがNULLになることがあるかわからないが、一応フォローしておく
		ATLASSERT(ipWorkspace);
		if (! ipWorkspace) {
			cErrorFunc( std::string(CT2A(cDBStr.c_str())) );
		}

		return ipWorkspace;
	}

	IFeatureClassPtr getFeatureClass(IWorkspace* cpWorkspace, tstring cTableName, boost::function<void (std::string)> cErrorFunc)
	{
		IFeatureClassPtr ipFeatureClass;
		try
		{
			ipFeatureClass = IFeatureWorkspacePtr(cpWorkspace)->_OpenFeatureClass(cTableName.c_str());
		}
		catch (_com_error& e) {
			cErrorFunc( std::string(CT2A(cTableName.c_str())) );
		}

		// 例外が出ずにipFeatureClassがNULLになることがあるかわからないが、一応フォローしておく
		ATLASSERT(ipFeatureClass);
		if (! ipFeatureClass) {
			cErrorFunc( std::string(CT2A(cTableName.c_str())) );
		}

		return ipFeatureClass;
	}

	bool isBaseMeshCode( std::string cStr )
	{
		using namespace std::tr1;
		return regex_match(cStr, smatch(), regex("[1-9][0-9]{5,5}"));
	}

	BaseMeshList loadMeshList(
		boost::filesystem::path cBaseMeshListFilePath,
		boost::function<void (std::string)> cOpenErrorFunc,
		boost::function<void (long)> cFormatErrorFunc
	)
	{
		BaseMeshList aBaseMeshList;

		// cBaseMeshListFilePathが２次メッシュコードを表していたら、そのメッシュだけを対象にする
		if ( isBaseMeshCode( cBaseMeshListFilePath.string() ) )
		{
			aBaseMeshList.insert( boost::lexical_cast<long>( cBaseMeshListFilePath.string() ) );
			return aBaseMeshList;
		}

		// メッシュリストを読み込む
		std::ifstream aFin( CString( cBaseMeshListFilePath.string().c_str() ) );
		if (! aFin)
		{
			cOpenErrorFunc( cBaseMeshListFilePath.string() );
		}
		std::string aRecord;
		long aLineCount = 0;
		while (std::getline(aFin, aRecord))
		{
			++aLineCount;

			// ６桁の数字でなければエラー
			if (! isBaseMeshCode( aRecord ))
			{
				cFormatErrorFunc( aLineCount );
			}

			// コンテナに追加
			aBaseMeshList.insert( boost::lexical_cast<long>( aRecord ) );
		}

		return aBaseMeshList;
	}

	IFeaturePtr getMeshFeature( IFeatureClassPtr cpBaseMeshClass, long cMeshCode )
	{
		IFeaturePtr ipMesh;
		if (cpBaseMeshClass)
		{
			CString aWhereClause;
			aWhereClause.Format(_T("%s = %ld"), sindy::schema::basemesh::kMeshCode, cMeshCode);
			IFeatureCursorPtr ipFeatureCursor(
				cpBaseMeshClass->_Search( AheInitQueryFilter( NULL, NULL, aWhereClause), VARIANT_FALSE ) );
			if (ipFeatureCursor)
			{
				ipMesh = ipFeatureCursor->_NextFeature();	// 存在するとしても１つしかないはず
			}
		}
		return ipMesh;
	}

	long fieldIndex( _IRowPtr cpRow, tstring cTableName, tstring cFieldName )
	{
		ITablePtr ipTable( cpRow->GetTable() );

		typedef tstring TableName, FieldName;
		typedef std::pair<TableName, FieldName> LayerFieldName;
		typedef long FieldIndex;
		typedef std::map< LayerFieldName, FieldIndex > FieldIndexes;

		static FieldIndexes aFieldIndexes;
		FieldIndexes::const_iterator itIndex = aFieldIndexes.find( std::make_pair( cTableName, cFieldName ) );
		if (itIndex == aFieldIndexes.end())
		{
			long aIndex = cpRow->Fields->_FindField( cFieldName.c_str() );
			aFieldIndexes.insert( std::make_pair(
				std::make_pair( cTableName, cFieldName ),
				aIndex ) );
			return aIndex;
		}
		else
		{
			return itIndex->second;
		}
	}

	long roundOff( double cNum )
	{
		return static_cast<long>( cNum > 0 ? cNum + 0.5 : cNum - 0.5 );
	}

	bool isHwy( long cUpDownClass )
	{
		switch (cUpDownClass)
		{
			using namespace sindy::schema::road_link::up_down_class;
			case kUndirected:
			case kUpInWest:
			case kDownOutEast:
			case kCommon:
			case kEtcRamp:
				return true;
		}
		return false;
	}

	bool isHwy( IFeaturePtr cpLink )
	{
		using namespace sindy::schema;
		long aUpDownClass = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kUpDownClass ) );
		return isHwy( aUpDownClass );
	}

	MeshCodes surroundingMeshCodes( long cMeshCode )
	{
		return boost::assign::list_of
			( cMeshCode )
			( CMeshCode( cMeshCode ).east().code() )
			( CMeshCode( cMeshCode ).west().code() )
			( CMeshCode( cMeshCode ).north().code() )
			( CMeshCode( cMeshCode ).south().code() )
			( CMeshCode( cMeshCode ).east().north().code() )
			( CMeshCode( cMeshCode ).east().south().code() )
			( CMeshCode( cMeshCode ).west().north().code() )
			( CMeshCode( cMeshCode ).west().south().code() );
	}

	CMeshCode& CMeshCode::east()
	{
		// 1の位が7の場合は繰り上がる
		if (m_MeshCode % 10 == 7)
		{
			m_MeshCode += 100;
			m_MeshCode -= 7;
		}
		else
		{
			++m_MeshCode;
		}

		return *this;
	}

	CMeshCode& CMeshCode::west()
	{
		// 1の位が0の場合は繰り下がる
		if (m_MeshCode % 10 == 0)
		{
			m_MeshCode -= 100;
			m_MeshCode += 7;
		}
		else
		{
			--m_MeshCode;
		}

		return *this;
	}

	CMeshCode& CMeshCode::north()
	{
		// 10の位が7の場合は繰り上がる
		if (m_MeshCode % 100 / 10 == 7)
		{
			m_MeshCode += 10000;
			m_MeshCode -= 70;
		}
		else
		{
			m_MeshCode += 10;
		}

		return *this;
	}

	CMeshCode& CMeshCode::south()
	{
		// 10の位が0の場合は繰り上がる
		if (m_MeshCode % 100 / 10 == 0)
		{
			m_MeshCode -= 10000;
			m_MeshCode += 70;
		}
		else
		{
			m_MeshCode -= 10;
		}

		return *this;
	}

	mesh_relation::ECode CMeshCode::relation( long cMeshCode )
	{
		if (cMeshCode == code())										{ return mesh_relation::kSame; }
		if (cMeshCode == CMeshCode( code() ).east().code())				{ return mesh_relation::kEast; }
		if (cMeshCode == CMeshCode( code() ).west().code())				{ return mesh_relation::kWest; }
		if (cMeshCode == CMeshCode( code() ).north().code())			{ return mesh_relation::kNorth; }
		if (cMeshCode == CMeshCode( code() ).south().code())			{ return mesh_relation::kSouth; }
		if (cMeshCode == CMeshCode( code() ).north().east().code())		{ return mesh_relation::kNorthEast; }
		if (cMeshCode == CMeshCode( code() ).north().west().code())		{ return mesh_relation::kNorthWest; }
		if (cMeshCode == CMeshCode( code() ).south().east().code())		{ return mesh_relation::kSouthEast; }
		if (cMeshCode == CMeshCode( code() ).south().west().code())		{ return mesh_relation::kSouthWest; }
		return mesh_relation::kNone;
	}
} // namespace adlib
