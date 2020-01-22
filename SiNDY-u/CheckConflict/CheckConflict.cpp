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

#include "CheckConflict.h"
#include <boost/regex.hpp>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <GeometryBase.h>
#include <RowContainer.h>
#include <TableContainer.h>
#include <TDC/useful_headers/str_util.h>

using namespace std;
using namespace sindy;

namespace {
	const long EXECCOUNT = 50000; // 処理単位
}

// 実行
bool CheckConflict::execute()
{
	// 初期化
	if( !init() )
		return false;

	checkConflict();
	RUNLOGGER.writeExitLog(true);
	return true;
}

// 初期化
bool CheckConflict::init()
{
	// ログファイルオープン
	if(!logInit())
		return false;

	bool good = true;
	if(!m_tgtWork.Connect(m_args.m_tgtdb.c_str()))
	{
		ERRLOGGER.Log(_T("#チェック対象接続先に接続できませんでした\t%s"), m_args.m_tgtdb);
		good = false;
	}

	if(!m_diffWork.Connect(m_args.m_diffdb.c_str()))
	{
		ERRLOGGER.Log(_T("#差分DB接続先に接続できませんでした\t%s"), m_args.m_diffdb);
		good = false;
	}
	if(!readConfig())
		good = false;

	if(!good)
	{
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	return true;
}

bool CheckConflict::logInit()
{
	if(!RUNLOGGER.Initialize(m_args.m_runlog))
	{
		cerr << "実行ログファイルを開けませんでした\t" << uh::toStr(m_args.m_runlog.c_str()) << endl;
		return false;
	}

	if(!ERRLOGGER.Initialize(m_args.m_errlog))
	{
		cerr << "エラーログファイルを開けませんでした\t" << uh::toStr(m_args.m_errlog.c_str()) << endl;
		RUNLOGGER.writeExitLog(false);
		return false;
	}
	return true;
}

// チェックコンフリクト
void CheckConflict::checkConflict()
{
	using namespace sindy::schema;
	// 処理対象テーブルでループ
	for(const auto& table : m_tgtTables)
	{
		CTableFinder tgtFinder, diffFinder;
		// テーブルファインダ初期化
		// テーブルがない（差分がない）場合は飛ばす
		if( !initTableFinder(m_tgtWork, table, tgtFinder) ||
			!initTableFinder(m_diffWork, table, diffFinder) )
		{
			continue;
		}

		CSPTableContainer tgtT(tgtFinder.FindTable( CString(table.c_str()) ));
		CSPTableContainer diffT(diffFinder.FindTable( CString(table.c_str()) ));
		// 必要なフィールドが存在するか
		if( -1 == diffT->GetFieldMap()->FindField(global::ipc_diff_feature::kOrgId) ||
			-1 == diffT->GetFieldMap()->FindField(global::ipc_diff_feature::kChangeType) )
		{
			ERRLOGGER.Log(_T("#差分DBの%sに必要なフィールドが存在しません。チェックできません。"), uh::toTStr(table).c_str());
			continue;
		}

		// ある程度の件数に絞って処理を行う。
		// 1対1の差分比較の為、メッシュ矩形で絞るのではなく、OBJECTIDで絞る
		// 差分DBは、ExtractDifferenceToolによって機械的に作成されるDBなので、OBJECTIDは順番に振られているとして処理する。
		long maxOID = diffT->RowCount(AheInitQueryFilter()); // 厳密にはレコード数で、MAX(OBJECTID)ではないが、機械処理後のDBなので、問題なし

		for(long i = 0; (i*EXECCOUNT) < maxOID ; ++i)
		{
			std::list<long> listTgtOIDs; // 処理対象OBJECTID群
			getCheckRecords(i, diffT, listTgtOIDs); // diffTはここでレコードがキャッシュされる
			// 比較するレコードが一つもなければ次へ
			if(listTgtOIDs.empty())
				continue;

			tgtT->_Select(listTgtOIDs, kObjectID, false, false); // tgtTのデータキャッシュ
			checkConflict(listTgtOIDs, tgtT, diffT); // コンフリクトチェック
		}
	}
}

// 設定ファイルを読み取る
bool CheckConflict::readConfig()
{
	using namespace boost;
	std::ifstream ifs( uh::toStr(m_args.m_config.c_str()) );
	if( !ifs.is_open() )
	{
		ERRLOGGER.Log(_T("#設定ファイルの読み込みに失敗しました"));
		return false;
	}

	bool good = true;
	long count = 0;
	std::string line;
	// テーブル名が記載されたファイルであるため、テーブル名ではない文字があればエラーとする
	regex re1( "\\W" ); // アルファベット、数字、アンダースコア以外の文字検出用
	regex re2( "^[a-zA-Z].*" ); // 最初の文字がアルファベットであるか調査用
	while(std::getline(ifs, line))
	{
		++count;
		// 空行、コメント行除外
		if( line.empty() || line.front() == '#' )
			continue;

		if( regex_search( line, re1 ) || !regex_match( line, re2 ) )
		{
			ERRLOGGER.Log(_T("#設定ファイルに無効な文字列が存在します：%d行目\t%s"), count, CString(line.c_str()));
			good = false;
			continue;
		}
		m_tgtTables.push_back(line);
	}
	return good;
}

// テーブルファインダ初期化
bool CheckConflict::initTableFinder(const sindy::CWorkspace& workspace, const string& table, CTableFinder& finder)
{
	uh::tstring tableName = uh::toTStr(table);
	CModel model;
	ITablePtr ipTable( workspace.OpenTable(tableName.c_str()) );
	if(!ipTable)
	{
		CString info = uh::str_util::format(_T("Table[%s] を開けなかったので、チェックを飛ばします"), tableName.c_str());
		RUNLOGGER.Info(info.GetString());
		return false;
	}
	finder.InitCollection( ipTable, model );
	return true;
}

void CheckConflict::getCheckRecords(const long roopCount, sindy::CSPTableContainer& table, std::list<long>& listTgtOIDs)
{
	using namespace sindy::schema;
	// CHANGE_TYPEが「4：更新前」のオブジェクトが対象
	CString whereClause = uh::str_util::format(_T("%s = %d AND %s > %ld AND %s <= %ld"),
		global::ipc_diff_feature::kChangeType, changetypecode::kBeforeUpdate,
		kObjectID, roopCount * EXECCOUNT,
		kObjectID, (roopCount + 1) * EXECCOUNT); // WHERE句

	table->clear();
	IQueryFilterPtr ipQuery(AheInitQueryFilter(nullptr, _T(""), whereClause));
	table->_Select(ipQuery, false, false);
	for( const auto& con : *table )
	{
		CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
		CComVariant orgOID = row->GetValue(global::ipc_diff_feature::kOrgId);
		if( VT_I4 != orgOID.vt )
		{
			ERRLOGGER.Log(_T("#差分DBの%s[%s]の値が不正です。OBJECTID：%ld"),
				table->GetTableName(), global::ipc_diff_feature::kOrgId, row->GetOID());
			continue; // 不正な値は飛ばすが、正常な値はチェックする
		}
		listTgtOIDs.push_back(orgOID.lVal);
	}
}

void CheckConflict::checkConflict(const std::list<long>& listTgtOIDs, const sindy::CSPTableContainer& tgtT, const sindy::CSPTableContainer& diffT)
{
	using namespace sindy::schema;

	for( const auto& oid : listTgtOIDs )
	{
		CContainerBase::iterator itTgt = tgtT->find_by_key(kObjectID, oid); // 対象DB側レコードイテレータ
		CContainerBase::iterator itDiff = diffT->find_by_key(global::ipc_diff_feature::kOrgId, oid); // 差分DB側レコードイテレータ
		// 対象DBでレコードが削除されているか
		if(itTgt == tgtT->end())
		{
			ERRLOGGER.writeError( tgtT->GetTableName(), oid, _T("#対象DB側でレコードが削除されています") );
			continue;
		}
		
		CSPRowBase tgtRow = CAST_ROWB(CAST_ROWC(*itTgt));
		CSPRowBase diffRow = CAST_ROWB(CAST_ROWC(*itDiff));
		CString errorMessage;
		// チェック対象テーブルが持つフィールドを全て比較する。
		for(const auto& field : *tgtT->GetFieldMap())
		{
			// OBJECTIDは除外
			if(field.name.CompareNoCase(kObjectID) == 0)
				continue;

			// フィールドがない場合（SHAPE.LENなどはFGDBにはないので）
			if(0 > diffT->GetFieldMap()->FindField(field.name))
				continue;

			// 形状フィールドの場合
			if( esriFieldTypeGeometry == field.type )
			{
				// NULL形状は属性関係なくおかしいので、あったら終了
				CGeometryBase cTgtGeom(tgtRow->GetShapeCopy());
				if( !(IGeometry*)cTgtGeom )
				{
					errorMessage = _T("対象DB側のレコードにNULL形状があります。");
					break;
				}
				IGeometryPtr ipDiffGeom(diffRow->GetShapeCopy());
				if( !ipDiffGeom )
				{
					errorMessage.Format(_T("差分DB側のレコードにNULL形状があります。差分DB側の%s[%ld]を確認ください"), kObjectID, diffRow->GetOID());
					break;
				}
				if(!CGeometryBase(cTgtGeom).Equals(ipDiffGeom))
				{
					if( errorMessage.IsEmpty() )
						errorMessage = _T("対象DB側のレコードが更新されています。");
					errorMessage.AppendFormat(_T("[%s]"), field.name);
				}
			}
			// 形状以外のフィールドは値を比較
			else if( tgtRow->GetValue(field.name) != diffRow->GetValue(field.name) )
			{
				if( errorMessage.IsEmpty() )
					errorMessage = _T("対象DB側のレコードが更新されています。");
				errorMessage.AppendFormat(_T("[%s]"), field.name);
			}
		}

		// エラーがあれば出力
		if( !errorMessage.IsEmpty() )
			ERRLOGGER.writeError(tgtT->GetTableName(), oid, errorMessage.GetString());

	}
}
