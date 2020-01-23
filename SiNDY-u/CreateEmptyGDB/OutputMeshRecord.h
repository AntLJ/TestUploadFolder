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
#include "DataDef.h"
/**
 * @brief 対象のGDBにメッシュを出力する
 */
class COutputMeshRecord
{
public:
	/**
	 * @brief コンストラクタ
	 * @param targetPathList [in] 生成先のGDBパス一覧
	 * @param meshlist       [in] メッシュリストの読込み結果
	 * @param meshDB         [in] メッシュDB
	 * @param meshLayerName  [in] メッシュレイヤ名
	 */
	COutputMeshRecord(
		const std::vector<uh::tstring>& targetPathList
		, const std::vector<uh::tstring>& meshlist
		, const uh::tstring& meshDB
		, const uh::tstring& meshLayerName
		, OUTPUT_TYPE outputType):
		m_targetPathList(targetPathList)
		, m_meshlist(meshlist)
		, m_meshDB(meshDB)
		, m_meshLayerName(meshLayerName)
		, m_outputType(outputType)
	{}

	/**
	 * @brief 対象のGDBにメッシュを出力する
	 * @note 作成先にメッシュコードが同じレコードが存在する場合、出力しない
	 * @return 処理の成否
	 */
	bool output();

protected:
	/**
	 * @brief 出力先にメッシュリストに指定されたメッシュを出力
	 * @param path            [in] 出力先パス
	 * @param inputLayer      [in] メッシュ検索先レイヤ
	 * @param inputFM         [in] inputLayerのフィールドマップ
	 * @param inputShapeIndex [in] inputLayerの形状フィールドのインデックス番号
	 * @return 処理の成否
	 */
	bool outputAllTargetMesh(
		const uh::tstring& path, const IFeatureClassPtr& inputLayer
		, const std::map<uh::tstring,long>& inputFM, long inputShapeIndex);

	/**
	 * @brief 条件に合わせてレコードを出力
	 * @param outputLayer      [in]  出力先のレイヤ
	 * @param outputStr        [in]  出力先DBの接続文字列
	 * @param strWhereList     [in]  レコード検索条件
	 * @param inputLayer       [in]  メッシュ検索先レイヤ
	 * @param inputFM          [in]  inputLayerのフィールドマップ
	 * @param inputShapeIndex  [in]  inputLayerの形状フィールドのインデックス番号
	 * @param outputFM         [in]  outputLayerのフィールドマップ
	 * @param outputShapeIndex [in]  outputLayerの形状フィールドのインデックス番号
	 * @param createdMeshList  [out] outputLayerに生成されたメッシュコードの格納先
	 * @return 処理の成否
	 */
	bool outputRecord(
		IFeatureClassPtr& outputLayer
		, const uh::tstring& outputStr
		, const std::vector<CString>& strWhereList
		, const IFeatureClassPtr& inputLayer
		, const std::map<uh::tstring,long>& inputFM, long inputShapeIndex
		, const std::map<uh::tstring,long>& outputFM, long outputShapeIndex
		, std::vector<CString>& createdMeshList);

	/**
	 * @brief 出力対象となるメッシュのメッシュコードリストを作成
	 * @param path          [in] 出力先パス
	 * @param outputMeshList [in] outputLayerに存在するメッシュコード一覧
	 * @return 出力対象のメッシュコードリスト
	 */
	std::vector<CString> makeTargetMeshList(
		const uh::tstring& path
		, const std::vector<unsigned long>& outputMeshList);

	/**
	 * @brief メッシュ検索条件群を生成
	 * @param targetMeshList [in] 出力対象のメッシュコードリスト
	 * @return 出力対象のメッシュコードリスト
	 */
	std::vector<CString> makeWhereList(const std::vector<CString>& targetMeshList);

protected:
	const std::vector<uh::tstring>& m_targetPathList; //!< 生成先のGDBパス一覧
	const std::vector<uh::tstring>& m_meshlist;       //!< メッシュリストの読込み結果
	const uh::tstring m_meshDB;                       //!< メッシュDB
	const uh::tstring m_meshLayerName;                //!< メッシュレイヤ名
	OUTPUT_TYPE m_outputType;                         //!< 出力先タイプ
};
