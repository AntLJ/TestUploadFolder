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

const esriFeatureType UNKOWN_FEATURE_TYPE = static_cast<esriFeatureType>(-1);
const esriFieldType   UNKOWN_FIELD_TYPE   = static_cast<esriFieldType>(-1);

// 出力先のタイプを定義
enum OUTPUT_TYPE
{
	UNKOWN, //!< 未定義
	SDE,    //!< SDE
	FOLDER, //!< フォルダ
	LOCAL,  //!< PDGB/FGDB
};

namespace create_info
{

/**
 * @brief レイヤ生成に必要な情報を格納
 */
class CreateLayerInfo
{
public:
	uh::tstring m_name;	                      //!< 生成するレイヤ名称
	boost::optional<uh::tstring> m_aliasName; //!< 生成するレイヤ名称
	IFieldsEditPtr m_addFields;               //!< レイヤに追加で生成するフィールド定義

};

/**
 * @brief ドメイン生成に必要な情報を格納
 */
class CreateDomainInfo
{
public:
	/// コンストラクタ
	CreateDomainInfo(){}
	/**
	 * @brief コンストラクタ
	 */
	CreateDomainInfo(const uh::tstring& name):
		m_name(name)
	{}

	bool operator <(const CreateDomainInfo& c) const
	{
		return m_name < c.m_name;
	}

	uh::tstring m_name;	//!< 生成するドメイン名称
};

/**
 * @brief フィーチャデータセット生成に必要な情報を格納
 */
class CreateDatasetInfo
{
public:
	/// コンストラクタ
	CreateDatasetInfo(){}
	/**
	 * @brief コンストラクタ
	 */
	CreateDatasetInfo(const uh::tstring& name):
		m_name(name)
	{}

	bool operator <(const CreateDatasetInfo& c) const
	{
		return m_name < c.m_name;
	}

	uh::tstring m_name;	//!< 生成するフィーチャデータセット名称
};

/**
 * @brief レイヤ・ドメイン・フィーチャデータセット生成のために必要な情報を格納
 */
class CreateInfo
{
public:
	std::vector<CreateLayerInfo> m_layerInfo;   //!< レイヤの情報
	std::set<CreateDomainInfo>   m_domainInfo;  //!< ドメインの情報
	std::set<CreateDatasetInfo>  m_datasetInfo; //!< フィーチャデータセットの情報
};

/**
 * @brief 入力DB毎のレイヤ・ドメイン・フィーチャデータセット生成のために必要な情報を格納
 * < 入力DB, 生成情報>
 * @detail 生成順を定義順にするため、実体はmapではなくvector<pair>で定義している
 */
typedef std::vector<std::pair<uh::tstring, CreateInfo>> CREATEINFOMAP;

} // create_info


namespace product
{

/**
 * @brief 生成するレイヤ定義クラス
 */
class ProductLayerDef
{
public:
	ProductLayerDef():
		m_srcFeatureType(UNKOWN_FEATURE_TYPE)
	{}
	virtual ~ProductLayerDef(){}

	uh::tstring m_name;	     //!< 作成するレイヤ名称
	uh::tstring m_aliasName; //!< 作成するレイヤのエイリアス名称
	IFieldsEditPtr m_fields; //!< 作成するフィールド群
	IUIDPtr m_srcCLSID;      //!< 元レイヤのCLSID
	IUIDPtr m_srcEXTCLSID;   //!< 元レイヤのEXTCLSID

	// 以下はフィーチャクラス作成で使用
	uh::tstring m_shapeFieldName;     //!< 元フィーチャクラスの形状フィールド名称
	esriFeatureType m_srcFeatureType; //!< 元フィーチャクラスのフィーチャクラスタイプ
	uh::tstring m_datasetName;        //!< 所属するデータセット
};

/**
 * @brief ドメイン生成のために必要な情報を定義するクラス
 */
class ProductDomainDef
{
public:
	uh::tstring m_name;      //!< 名称
	IDomainPtr  m_domainPtr; //!< ドメイン
};

/**
 * @brief フィーチャデータセット生成のために必要な情報を定義するクラス
 */
class ProductDatasetDef
{
public:
	uh::tstring m_name; //!< 名称
	ISpatialReferencePtr m_spRef; //!< 空間参照
};

/**
 * @brief レイヤ生成のために必要な情報を格納するクラス
 */
class ProductDef
{
public:
	std::vector<ProductLayerDef>   m_layerList;   //!< テーブル定義
	std::vector<ProductDomainDef>  m_domainList;  //!< ドメイン定義
	std::vector<ProductDatasetDef> m_datasetList; //!< フィーチャデータセット定義
};

} // product
