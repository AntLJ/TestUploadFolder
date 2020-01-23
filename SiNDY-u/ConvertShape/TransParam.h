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
class CTransParam
{
public:
	CTransParam(void);
	~CTransParam(void);

	bool parse(int argc, _TCHAR* argv[]);

	std::string GetTaregetDB() const { return uh::toStr( m_tstrTargetDB ); };
	std::string GetLayer() const { return uh::toStr( m_tstrLayer ); };
	std::string GetMeshDB() const { return uh::toStr( m_tstrMesh ); };
	std::string GetMeshSchema() const { return uh::toStr( m_tstrMeshSchema ); };
	std::string GetMeshlist() const { return uh::toStr( m_tstrMeshlist ); };

	std::string GetFileName() const { return uh::toStr( m_tstrFileName ); };
	std::string GetOutDir() const { return uh::toStr( m_tstrOutDir ); };
	std::string GetIdentifier() const { return uh::toStr( m_tstrIdentifier ); };
	std::string GetColList() const { return uh::toStr( m_tstrColumnlist ); };
	bool GetNoShape() const { return m_bNoShape; };
	std::string GetWhereQuery() const { return uh::toStr( m_tstrWhereQuery ); };
	std::string GetWhereLoop() const { return uh::toStr( m_tstrWhereLoop ); };
	std::string GetRunlogFile() const { return uh::toStr( m_tstrRunLog ); };
	std::string GetErrlogFile() const { return uh::toStr( m_tstrErrLog ); };

	std::string GetOption();

private:
	bool check_required_option(const po::variables_map& vm, LPCSTR lpszOption, LPCSTR lpszDesc);
	std::string GetOptionLine(const char* const cParam, std::string strValue);

private:
	uh::tstring		m_tstrTargetDB;					// 変換対象DB
	uh::tstring		m_tstrLayer;					// 変換レイヤ
	uh::tstring		m_tstrMesh;						// メッシュDB
	uh::tstring		m_tstrMeshSchema;				// メッシュレイヤのスキーマ(PostgreSQLの場合)
	uh::tstring		m_tstrMeshlist;					// メッシュリスト
	uh::tstring		m_tstrIdentifier;				// 識別子
	uh::tstring		m_tstrOutDir;					// 出力先ディレクトリ
	uh::tstring		m_tstrFileName;					// 出力ファイル名(拡張子なし)
	uh::tstring		m_tstrColumnlist;				// 変換カラム定義ファイルのパス
	bool            m_bNoShape;                     // 形状を出力させないフラグ
	uh::tstring		m_tstrWhereQuery;				// QueryFilter時のWhere句
	uh::tstring		m_tstrWhereLoop;				// フィーチャ取得＋ループ処理時の属性フィルタ
	uh::tstring		m_tstrRunLog;					// ランログ
	uh::tstring		m_tstrErrLog;					// エラーログ
};

