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

// マルチパート形状を別フィーチャとして分離する
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

// find_by_key用のキーリストをexpTypeの条件から作成する
std::list<CString> createKeyList( const SPConditionExpressions& cond, ExpressionType expType, const CTableContainer& table )  // TODO: ProcessorUtility.cpp とか作ってそっちに
{
	std::list<CString> ret; // 戻り値

	if( !cond )
		return ret;

	if( cond->isContainer() )
	{
		const ConditionExpressionContainer& con = *std::static_pointer_cast<ConditionExpressionContainer>(cond);
		if( !con.isAndContainer )
		{
			notifier::critical::versatile(_T("ERROR : 比較条件にorは使えません"));
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
		// 直接指定、is null、is not nullが使用されている条件は無視する
		// そうしないと、keyとvalueの数が揃わなくてfind_by_keyが使用できない。
		// この関数はTargetConditionの条件でfind_by_keyを使う時用なので、これを
		// 仕様とする
		if( ( exp.leftValue.type  == ExpressionTypeSrc || exp.leftValue.type  == ExpressionTypeDst ) && 
			( exp.rightValue.type == ExpressionTypeSrc || exp.rightValue.type == ExpressionTypeDst ) )
		{
			if( exp.leftValue.type == expType )
				ret.push_back( exp.leftValue.col.c_str() );
			else if( exp.rightValue.type == expType )
				ret.push_back( exp.rightValue.col.c_str() );
		}
	}
	// 形状フィールドを指定されている場合は不正な条件なので異常終了する
	for( const auto& key : ret )
	{
		long fieldIndex = table.GetFieldMap()->HasField(key);
		if( fieldIndex == table.GetFieldMap()->GetShapeIndex() )
		{
			std::wstringstream ss;
			ss << L"比較条件に形状は使えません：" << (LPCWSTR)CStringW(key);
			ret.clear();
			notifier::critical::versatile(ss.str());
		}
	}

	// 存在しないカラムが指定されていないかチェック
	// 存在しないカラムがある場合は空にする
	for( const auto& key : ret )
	{
		long fieldIndex = table.GetFieldMap()->HasField(key);
		if( 0 > fieldIndex )
		{
			std::wstringstream ss;
			ss << L"TABLE[" << (LPCWSTR)CStringW(table.GetTableName()) << L"] 指定されているカラムが存在しません : " << (LPCWSTR)CStringW(key);
			notifier::versatile(ss.str(), std::wcout);
			ret.clear();
			break;
		}
	}
	return ret;
}

// find_by_key用の値リストを作成する
std::list<CComVariant> createValueList( const sindy::CRowBase& row, const std::list<CString>& cols ) // // TODO: ProcessorUtility.cpp とか作ってそっちに
{
	std::list<CComVariant> ret; // 戻り値

	for( const auto& col : cols )
	{
		long idx = row.GetFieldMap()->HasField(col);
		if( 0 > idx )
		{
			std::wstringstream ss;
			ss << _T("存在しないカラムを指定しています：") << (LPCWSTR)CStringW(col);
			notifier::critical::versatile(ss.str());
			return ret;
		}
		else
			ret.push_back( row.GetValue(idx) );
	}
	return ret;
}

/// 名前が共通しているカラムのインデックスのペアを作成する
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
				// 更新対象のフィールドに含まれない、あるいは、更新対象ではない
				auto it = idxs.find(fieldName);
				if( it == idxs.end() )
				{
					idxs.copyshape = false;
					continue;
				}

				// 更新対象なら、ソース側の値が形状フィールドでない場合にエラーとなる
				for( auto& idxPair : it->second )
				{
					if( srcFields.HasField(idxPair.first.srcName.c_str()) != srcFields.GetShapeIndex() )
					{
						std::wstringstream ss;
						ss << L"形状フィールドを更新できるのは形状フィールドだけです";
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

		// 未登録の場合はレコード値タイプとする
		auto& it = idxs.find(fieldName);
		if( it == idxs.end() )
		{
			// 全カラムコピーの場合は、不足分を補う
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
			// Cacheタイプはひとまず、srcの値を格納する
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
 * @brief 属性と形状をコピーする
 *
 * @param listIdx [in] 属性コピー可能なインデックスの対応表（first:ソース側 second:dst側）
 * @param src [in] コピー元
 * @param dst [in] 新規作成されたレコード
 * @param args [in] オプション管理クラス
 * @param geom [in] nullじゃない時はsrcの形状より優先されてdstにセットする
 * @param hasGeom [in] ジオメトリ型を持っているか（共有フィールド名のみを検査）
 * @return 成功すればtrue
 */
bool Update( CTableContainer& table, const IndexList& listIdx, const CSPRowBase& src, CSPRowBase& dst, const Arguments& args, IGeometryPtr geom, bool hasGeom )
{
	// キャッシュ作成
	// 全てが更新対象になるわけではないので、キャッシュ作成は更新対象のみに限定する
	// これによりパフォーマンスの低下がある場合は、一括キャッシュを検討する（多分大丈夫だと思う）
	dst->CreateCache();

	// 属性
	for( const auto& idxExps : listIdx )
	{
		for( const auto& idxExp : idxExps.second )
		{
			const IndexPair& idx = idxExp.first;
			const SPConditionExpressions& exps = idxExp.second;
			if( !exps || IndexPair::Cache == idx.type || exps->eval( src, dst, args, std::list<uh::tstring>() ) ) // 条件なし＝無条件
			{
				if( IndexPair::None == idx.type || 				        // コピー対象ではない
					IndexPair::RowValue == idx.type && 0 > idx.srcIdx ) // レコード値なのにインデックス番号が有効ではない
					continue;

				dst->SetValue( idx.dstIdx, idx.value(*src) );
				break; // 採用される値は一つだけ（先勝ち）
			}
		}
	}

	// ジオメトリ型を持っていない場合はここで終了
	if(!hasGeom)
		return true;

	if( geom )
	{
		dst->SetShape( geom );
	}
	else if( listIdx.copyshape || listIdx.copyall )
	{
		// 形状
		dst->SetShape(src->GetShapeCopy());
	}

	// マルチパート解消
	if( args.m_dissolveMulti )
		MultiGeometry2Geometries( *dst, table );

	return true;
}

/**
 * @brief レコードを新規作成する
 *
 * @note 新規作成後、srcから属性をコピーする（カラム名が同一のものだけ）
 * @param table [in] 作成対象テーブル
 * @param listIdx [in] 属性コピー可能なインデックスの対応表（first:ソース側 second:dst側）
 * @param src [in] コピー元
 * @param args [in,out] コマンドライン引数
 * @param hasGeom [in] ジオメトリ型を持っているか
 * @param doCache [in] キャッシュを行うテーブルである場合、true
 * @param relationField [in] first:SRCフィールド second:DSTフィールド
 * @return 成功すればtrue
 */
bool Create( CTableContainer& table, const IndexList& listIdx, const CSPRowBase& src, Arguments& args,
			bool hasGeom, bool doCache, const std::map<uh::tstring, uh::tstring>& relationField, RelationValues& relationValues)
{
	auto row = table.CreateDummyRowNoPush(); // 属性を付与してからコンテナに突っ込む（find_by_key対策）
	auto ret = Update( table, listIdx, src, CAST_ROWB(CAST_ROWC(row)), args, nullptr, hasGeom );
	table.push_back( row, false );
	// キャッシュを行う必要のある親テーブルの場合
	if(doCache)
	{
		for(const auto& field : relationField)
		{
			const auto& key = std::make_pair(table.GetTableName(), field.second.c_str()); // 親テーブル名、フィールド名をキーとする。
			relationValues[key][src->GetValue(field.first.c_str())] = row; // dst側はrowごと格納、src側は指定されたフィールドの値のみ格納
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

			// エラーレベル文字列作成
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

// 対象フィーチャを登録する
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

// 検索する（全データ）
void Select(CTableContainer& con)
{
	IQueryFilterPtr query(AheInitQueryFilter());
	con._Select(query, false, false);
	con.CreateCache();
}

// 検索する（矩形範囲内）
void Select( IEnvelopePtr env, CTableContainer& con )
{
	// 矩形でまずは検索
	IQueryFilterPtr query( AheInitSpatialFilter( nullptr, env ) );
	con._Select( query, false, false );
	con.CreateCache();
	// 矩形に接している向こう側を弾く
	IRelationalOperatorPtr rel(env);
	IGeometryPtr clipGeom;
	for( auto it = con.begin(); it != con.end(); )
	{
		IGeometryPtr geom = (IGeometry*)*CAST_ROWC(*it)->GetShape();
		IEnvelopePtr geomEnv;
		geom->get_Envelope(&geomEnv);
		// 検索矩形に含まれるものは無条件でOK
		VARIANT_BOOL isContain = VARIANT_FALSE;
		rel->Contains(geomEnv, &isContain);
		if( isContain )
		{
			++it;
			continue;
		}
		// 検索形状でクリップした結果何も残らないものはコンテナから削除
		if( !clipGeom )
			clipGeom = AheCreateClone(geom);
		((ITopologicalOperatorPtr)geom)->QueryClipped(env, clipGeom);
		if( IsEmpty(clipGeom) )
			it = con.erase(it);
		else
			++it;
	}
}

// 編集セッション開始する
bool StratEditOperation(CEditor& output)
{
	// 編集セッション開始
	if( !output.StartOperation() )
	{
		notifier::critical::versatile(_T("編集開始に失敗"));
		return false;
	}
	return true;
}

// 編集セッション終了する。エラーがあった場合はAbort
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

// 実体化する。
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
				// レコード作成に失敗
				return false;
			}
			row.Materialization(ipRow);
			// 二重処理（削除）防止
			limitter.regist( row.GetTableName(), row.GetOID(), false );
		}
	}
	return true;
}

// TODO: Conditions.cppで処理できるようにする。
// 現状、EnumOperatorを無視して、relationValuesに値があれば更新するという処理になっている。。(Cacheタイプの場合のみ)
// EnumOperatorに新しい定義を作成しないと、eval()での比較はできない。
// 現状、eval()は value 対 value の比較だが、この関数は value 対 map のため、eval()に入れる場合は検討が必要。。
bool SetRelation(const IndexList& idxList, const RelationValues& relationValues, CSPTableContainer& table)
{
	for(const auto& tblInfo : idxList)
	{
		// Column情報
		for(const auto& columnInfo : tblInfo.second)
		{
			// Cacheタイプのみ
			if(columnInfo.first.type != IndexPair::ValType::Cache)
				continue;
			ConditionExpression& cond( *std::static_pointer_cast<ConditionExpression>(columnInfo.second) );
			CString dstField = (cond.rightValue.type == ExpressionType::ExpressionTypeDst) ? cond.rightValue.col.c_str() : cond.leftValue.col.c_str();

			const auto foreignInfo = relationValues.find(std::make_pair(columnInfo.first.foreignTable, columnInfo.first.foreignField)); // キャッシュリストの中から関連テーブルを取得
			if(foreignInfo == relationValues.end())
				continue;

			// 更新をかけた全件に対して処理を行う。
			for( auto& col : *table )
			{
				CSPRowBase& row = CAST_ROWB(CAST_ROWC(col));
				// 削除レコードは無視
				if(row->IsDeleted())
					continue;

				const auto& foreignVal = foreignInfo->second.find(row->GetValue(dstField)); // 関連テーブルから該当するレコード取得
				// 該当のレコードがあれば外部キーフィールドを更新する
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
	// ログ
	{
		std::wstringstream ss;
		ss << _T("コンバート開始") <<  _T(" mesh:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	// フォルダ指定ならここでインプット側ワークスペースをオープン
	if( isDirectory( m_strInput ) )
	{
		uh::tstring dbpath = GetWorkspaceString(mesh);
		if( dbpath.empty() )
		{
			std::wstringstream ss;
			ss << _T("メッシュリストに対応するファイルが見つかりません メッシュコード : ") <<  mesh;
			std::cerr << uh::toStr(ss.str()) << std::endl;
			notifier::versatile(ss.str(), std::wcout);
			notifier::versatile(ss.str(), std::wcerr);
			return true; // 処理は継続する
		}
		m_input.Workspace().Connect( dbpath.c_str() );
		if( !m_input.Workspace() )
		{
			std::wstringstream ss;
			ss << _T("ワークスペースに接続できませんでした : ") <<  dbpath;
			std::cerr << uh::toStr(ss.str()) << std::endl;
			notifier::critical::versatile(ss.str());
			return false;
		}
	}

	// 条件式参照用の値をArgmentsに設定
	m_args.m_systemValue[Arguments::MESHCODE] = mesh;

	// 編集開始
	if(!StratEditOperation(m_output))
		return false;

	bool hasError = false; // 処理エラーがあったかどうか

	// コンバート処理
	ConvertTables(hasError, mesh);

	// 編集セッション終了
	StopEditOperation(m_output, hasError);

	std::wstringstream ss;
	if( !hasError )
	{
		ss << _T("コンバート終了") <<  _T(" mesh:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}
	else {
		ss << _T("コンバート終了") <<  _T(" mesh:") <<  mesh << _T(" エラーあり");
		notifier::critical::versatile(ss.str());
	}

	return !hasError;
}

bool PGFGDBConvertProcessor<uh::tstring>::preprocess()
{
	// 接続確認(input)
	if( !isDirectory( m_strInput ) )
	{
		if( !m_input.Workspace() )
		{
			notifier::critical::versatile(_T("input側に接続失敗"));
			return false;
		}
	}

	// 接続確認(output)
	if( !m_output.Workspace() )
	{
		notifier::critical::versatile(_T("output側に接続失敗"));
		return false;
	}

	std::list<uh::tstring> listTarget;
	for( const auto& table : m_manager.m_listTarget )
	{
		// 編集対象がバージョン対応か非対応かに応じて、編集を開始する
		listTarget.push_back( table.first );

		// 外部キーに関連するテーブル、フィールドをあらかじめ解析しておく
		if(!getRelationField(table.second, m_relationFields))
			return false;
	}

	bool good = true;
	// 外部キーに関連するテーブル、フィールドが存在するか事前に確認する
	// テーブルがない場合は処理が飛ばされるので、処理を続けて問題なし。ログだけ出力
	for( const auto& relTable : m_relationFields )
	{
		for( const auto& relField : relTable.second )
		{
			// input側
			if( m_input.Workspace() )
			{
				ITablePtr ipSrcT( m_input.Workspace().OpenTable(relTable.first.c_str()) );
				if( !ipSrcT )
				{
					std::wstringstream ss;
					ss << _T("テーブルがありません（SRC側：") << relTable.first << _T("）");
					notifier::versatile(ss.str(), std::wcerr);
				}
				else if( -1 == ipSrcT->GetFields()->_FindField(relField.first.c_str()) )
				{
					std::wstringstream ss;
					ss << _T("フィールドがありません（SRC側：") << relField.first << _T("）");
					notifier::versatile(ss.str(), std::wcerr);
					good = false;
				}
			}
			// output側
			ITablePtr ipDstT( m_output.Workspace().OpenTable(relTable.first.c_str()) );
			if( !ipDstT )
			{
				std::wstringstream ss;
				ss << _T("テーブルがありません（DST側：") << relTable.first << _T("）");
				notifier::versatile(ss.str(), std::wcerr);
			}
			else if( -1 == ipDstT->GetFields()->_FindField(relField.second.c_str()) )
			{
				std::wstringstream ss;
				ss << _T("フィールドがありません（DST側：") << relField.second << _T("）");
				notifier::versatile(ss.str(), std::wcerr);
				good = false;
			}
		}
	}
	if( !good )
		return false;

	return m_output.StartEditingByObjectType(listTarget); // エラーメッセージはCEditorで出す
}

bool PGFGDBConvertProcessor<uh::tstring>::postprocess()
{
	return m_output.StopEditing(true); // エラーメッセージはCEditorで出す
}

bool PGFGDBConvertProcessor<uh::tstring>::bulkproc(const std::vector<uh::tstring>& srcs)
{
	// ログ
	notifier::versatile(_T("コンバート開始 bulk:"), std::wcout);

	m_args.m_systemValue[Arguments::MESHCODE].clear();

	bool hasError = false;
	// ディレクトリの場合
	// TODO: [Relation]タグを使用していて、ディレクトリを指定した場合も動作するようにしてあるが、
	//       外部キーが重複する可能性が高いため、正常に動作しない。。
	//       src1でOBJECTID_ORG:100が新規作成され、src2でもOBJECTID_ORG:100が新規作成された場合、
	//       src1とsrc2の上記オブジェクトはそれぞれ別のオブジェクトであるが、外部キー情報のOBJECTID_ORGが重複してしまう。
	//       m_relationValuesのキーとして、メッシュコードも追加すれば参照先が分かるようになる、はず。
	if( isDirectory( m_strInput ) )
	{
		for(const auto& mesh : srcs)
		{
			uh::tstring dbpath = GetWorkspaceString(mesh);
			if( dbpath.empty() )
			{
				std::wstringstream ss;
				ss << _T("メッシュリストに対応するファイルが見つかりません メッシュコード : ") <<  mesh;
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
				ss << _T("ワークスペースに接続できませんでした : ") <<  dbpath;
				std::cerr << uh::toStr(ss.str()) << std::endl;
				notifier::critical::versatile(ss.str());
				hasError = true;
				continue;
			}

			// 条件式参照用の値をArgmentsに設定
			m_args.m_systemValue[Arguments::MESHCODE] = mesh;

			// 編集開始
			if(!StratEditOperation(m_output))
				return false;

			// コンバート処理(メッシュ矩形単位で処理をしないため、meshを引数として渡さない)
			ConvertTables(hasError);

			// 編集セッション終了
			StopEditOperation(m_output, hasError);
		}
	}

	else
	{
		// 編集開始
		if(!StratEditOperation(m_output))
			return false;

		// コンバート処理
		ConvertTables(hasError);

		// 編集セッション終了
		StopEditOperation(m_output, hasError);
	}

	std::wstringstream ss;
	if( !hasError )
	{
		ss << _T("コンバート終了 bulk:");
		notifier::versatile(ss.str(), std::wcout);
	}
	else {
		ss << _T("コンバート終了 bulk:") << _T(" エラーあり");
		notifier::critical::versatile(ss.str());
	}

	return !hasError;
}

// フォルダ指定の場合のメッシュ単位のDBパスを取得する
template<>
uh::tstring PGFGDBConvertProcessor<uh::tstring>::GetWorkspaceString( const uh::tstring& mesh ) const
{
	// メッシュコードに対応するPGFGDBファイルパスを取得
	std::wstringstream s;
	s << "\\\\" << mesh << "\\.[mg]db$"; // 「\\59414111.mdb」とかにマッチ
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
		// モードによって、処理しないテーブルは飛ばす
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
		// コンバート処理
		if(!ConvertTable(table, dstT, mesh))
			continue;

		// 実体化
		if(!Materiarize(dstT, m_limitter))
		{
			std::wstringstream ss;
			ss << _T("レコードの作成に失敗しましたので処理を中断します");
			if(!mesh.empty())
				ss << _T("mesh:") << mesh;
			notifier::critical::versatile(ss.str());
			hasError = true;
		}

		CString dstTableName = dstT->GetTableName();
		CErrorObjectsBase errs;
#if 0 // レイヤごとのエラーが出てきても、かえってうざい。そういうのは後でチェッカーでやるということで
		// 論理チェック
		CLogicRule rule;
		dstT->CheckLogic( rule, errs );
		ErrorObject2Notifier( dstTableName, errs );
#endif // if 0

		if( !SetRelation(table.second, m_relationValues, dstT) )
			hasError = true;

		// Store
		// 更新履歴を付与するかどうか
		bool backup = g_bSetHistory;
		g_bSetHistory = m_manager.m_globalAdditionalUpdate.sindyHistory;
		if( errorcode::sindyErr_NoErr != dstT->Store(errs) )
		{
			hasError = true;
			ErrorObject2Notifier( dstTableName.GetString(), errs );
		}
		g_bSetHistory = backup;

		// TODO: レイヤ単位での各処理の件数を出力すべきか？
		// 多数のレイヤ分毎回出るのもうざいので、オプションで出す出さないを決められるようにするか？
	}
}

template<>
bool PGFGDBConvertProcessor<uh::tstring>::ConvertTable(const std::pair<uh::tstring,IndexList>& table, CSPTableContainer& dstT, const uh::tstring& mesh)
{
	bool ret = true;
	// オーナー名付きのテーブル名取得
	uh::tstring srcTableName = m_args.getInputTableName(table.first);
	uh::tstring dstTableName = m_args.getOutputTableName(table.first);
	// ■更新対象取得
	// テーブルファインダ初期化
	CTableFinder srcFinder, dstFinder;
	{
		CModel model;
		model.Add( table.first.c_str(), sindy::schema::kObjectID, nullptr, nullptr );
		ITablePtr ipSrcT( m_input.Workspace().OpenTable(srcTableName.c_str()) );
		if( !ipSrcT )
		{
			std::wstringstream ss;
			ss << _T("テーブルがないため、処理を飛ばします（SRC側：") << srcTableName << _T("）");
			notifier::versatile(ss.str(), std::wcerr);
			return false;
		}
		ITablePtr ipDstT( m_output.Workspace().OpenTable(dstTableName.c_str()) );
		if( !ipDstT )
		{
			std::wstringstream ss;
			ss << _T("テーブルがないため、処理を飛ばします（DST側：") << dstTableName << _T("）");
			notifier::versatile(ss.str(), std::wcerr);
			return false;
		}

		srcFinder.InitCollection( ipSrcT, model );
		dstFinder.InitCollection( ipDstT, model );
	}

	CSPTableContainer srcT( srcFinder.FindTable( srcTableName.c_str() ) );
	dstT = dstFinder.FindTable( dstTableName.c_str() );

	// メッシュ形状で検索する
	// オプションが指定されていたらオンデマンド検索とする（bug 11710）
	// 但し、オンデマンド検索する場合はDST側でのループができない事に注意！！
	// メッシュが指定されていなかった全件(bulkモード)
	if(!mesh.empty())
	{
		if( m_args.m_output_ondemand_search )
			dstT->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach ); // オンデマンド検索
		IEnvelopePtr env(MESH2ENV( _ttol(mesh.c_str()), AheGetSpatialReference( (ITable*)*srcT ) ));
		Select( env, *srcT );
		// dst側の検索はdst側の条件が存在するときのみに限定する
		// こうすることで、単純なインポート処理時にはインデックスを削除して実行できる
		if( m_manager.hasDstCondition() && dstT->GetAutoCache() == sindyContainerAutoCacheNone )
			Select( env, *dstT );
	}
	else
	{
		Select(*srcT);
		// レコード数が多いと落ちるので、目安の値よりもレコード数が多い場合はオンデマンド検索にする
		if( m_args.m_output_ondemand_search )
			dstT->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach ); // オンデマンド検索
		else if( INDICATIONROWCOUNT <= dstT->RowCount(AheInitQueryFilter()) )
		{
			std::wstringstream ss;
			ss << L"レコード数が多いため、オンデマンド検索にします：" << uh::toWStr(dstT->GetTableName());
			notifier::versatile(ss.str());
			dstT->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach ); // オンデマンド検索
		}
		if( m_manager.hasDstCondition() && dstT->GetAutoCache() == sindyContainerAutoCacheNone )
			Select(*dstT);
	}

	// 更新をするための共通インデックスリストを作成する
	IndexList listIdx = table.second;
	bool hasGeom = false;
	createIndexList( *srcT->GetFieldMap(), *dstT->GetFieldMap(), m_args, listIdx, hasGeom );
	// キャッシュを行うテーブルか
	const auto& tgt = m_relationFields.find(table.first);
	bool doCache = (tgt != m_relationFields.end());
	std::map<uh::tstring, uh::tstring> relationField;
	if(doCache)
		relationField = tgt->second;

	// ループタイプでループ対象を切り替え
	for( const auto& condition : m_manager.m_listConditions )
	{
		// ループ対象を決定
		CSPTableContainer outerT, innerT;
		ExpressionType keyType = ExpressionTypeUnknown; //!< find_by_keyで使用するキーリストを作成する際の条件
		ExpressionType valType = ExpressionTypeUnknown; //!< find_by_keyで使用する値リストを作成する際の条件
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
			// TODO: エラーメッセージ
			// return false;
			break;
		}
		const auto& targetCondition = condition.second.first;
		const auto& modifyCondition = condition.second.second;
		// ■更新処理
		// 比較をするためのキーリストを作成する（内部で存在チェックもする）
		auto innerKeyList = createKeyList(targetCondition.exps, keyType, *innerT);
		auto outerKeyList = createKeyList(targetCondition.exps, valType, *outerT);

		// LoopTargetでループ
		for( auto& con : *outerT )
		{
			std::list<uh::tstring> messages;
			CSPRowBase outerRow = CAST_ROWB(CAST_ROWC(con));
			CSPRowBase srcRow = ( LoopType::LoopSRC == condition.first ) ? outerRow : CSPRowBase();
			CSPRowBase dstRow = ( LoopType::LoopDST == condition.first ) ? outerRow : CSPRowBase();
			// find_by_keyするための値のリストを作成する
			std::list<CComVariant> valueList = createValueList( *outerRow, outerKeyList );
			// 更新対象検索
			// 数が合わないなら、条件不成立として検索しない
			if( !innerKeyList.empty() && 
				innerKeyList.size() == outerKeyList.size() )
			{
				// キャッシュコンテナから条件が成立するものを探す
				auto& it = innerT->find_by_key( innerKeyList, valueList );
				if( it != innerT->end() )
				{
					if( LoopType::LoopSRC == condition.first )
						dstRow = CAST_ROWB(CAST_ROWC(*it));
					else if( LoopType::LoopDST == condition.first )
						srcRow = CAST_ROWB(CAST_ROWC(*it));
				}
			}
			// TargetConditionにmessageがあるかもしれないので、ここで条件評価する
			if( targetCondition.exps )
				targetCondition.exps->eval( srcRow, dstRow, m_args, messages );

			// 更新、削除、作成と種類があるが、処理の順番はxml記載順とする
			for( const auto& mcond : modifyCondition )
			{
				// 比較をするペアを見つける
				switch( mcond->type )
				{
				case Condition::ConditionType::Create:
					{
						// SRCがない場合はそもそも作ることができない
						if( ! srcRow )
							break;

						// 新規作成
						// ■新規作成処理
						// CreateConditonに書かれている条件に合致するならsrcの内容でdestに新規作成する
						if( !mcond->exps || // 条件なしなら、無条件ということ
							mcond->exps->eval( srcRow, dstRow, m_args, messages ) )
						{
							if( m_limitter.regist( srcRow->GetTableName(), srcRow->GetOID(), true ) ) // 二重処理防止
							{
								if( !Create( *dstT, listIdx, srcRow, m_args, hasGeom, doCache, relationField, m_relationValues ) )
								{
									std::wstringstream ss;
									ss << _T("新規作成に失敗しました（SRC側：") << srcRow->GetOID() << _T("）");
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
							break; // dstが存在しなければ処理のしようがない

						// 一度更新したものは二度と更新させない
						// 特に削除の場合、前の処理で新規作成されていると、「is empty」条件で
						// 対応するものがないものを削除するように定義している場合に、折角作った
						// ものが全て消えてしまう。
						// 同じことは属性更新にも言えること（属性更新した結果削除の条件に合うよ
						// うになってしまったとか）なので、変更がかかっているものはそれ以上更新
						// させない。
						// 更新に関してはそれほど問題にもならなそうだが、処理として統一させておく
						if( dstRow->Changed() )
							break;

						// 更新/削除
						// ■削除処理
						// DeleteConditionに書かれている条件に合致するなら削除
						// その場合更新処理以降は行わない
						// ■更新処理
						// UpdateConditionに書かれている条件に合致するならsrcの内容をdstに反映する
						// 但し、双方でカラム数が一致している保証がないので、事前に共通のカラム名リスト
						// を作成しておく（編集可能カラムのみ）。形状に関しては、フィーチャクラスなら必ず対象とする
						// この時、更新対象カラムを絞り込めた方が部分的な作業のマージが可能になって良いかもしれないが、
						// ExportPGDB,DiffImportPGDBではそのような絞り込みは行っていないため、とりあえず全てのカラムを対象とする TODO:後で要件定義にたす？
						// ・属性更新
						// ・形状更新
						if( !mcond->exps->eval( srcRow, dstRow, m_args, messages ) )
							break;

						if( mcond->type == Condition::ConditionType::Update )
						{
							if( m_limitter.regist( srcRow->GetTableName(), srcRow->GetOID(), true ) ) // 二重処理防止
							{
								// 更新
								if( !Update( *dstT, listIdx, srcRow, dstRow, m_args, nullptr, hasGeom ) )
								{
									std::wstringstream ss;
									ss << _T("属性/形状の更新に失敗しました（SRC側：") << srcRow->GetOID() << _T("）");
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
							if( m_limitter.regist( dstRow->GetTableName(), dstRow->GetOID(), false ) ) // 二重処理防止
							{
								// 削除
								dstRow->Delete(); // 削除
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
		// ■追加更新
		//   追加更新は、AdditionalUpdateに書かれている内容
		//   処理順番的に、更新と追加に対して反映される <- これはCreate,Updateの時に一緒に行う
	}
	return ret;
}
