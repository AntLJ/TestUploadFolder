/**
 * @file ErrObj.h
 * 
 * @biref 論理チェック等のエラー情報をオブジェクト別に管理する機構を提供する
 * クラス群の定義ファイルです。
 *
 * このクラス群は、SiNDYLibにあるErrorObject.hを参考に作成しました。
 *
 * @author コンテンツ本部技術開発システム開発グループ 古川貴宏
 * @date 2008/06/25
 * @copy Copyright(c) 2008 INCREMENT P CORP., All Rights Reserved.
 */
#pragma once

enum sindyeLogFormat {
	sindyeLogFormat_Common,		//!< ログハンドル標準フォーマット
	sindyeLogFormat_Freestyle,	//!< 自由形式ログ
};

enum sindyeLogLevel {
	sindyeLogLevel_INFO,		//!< 情報
	sindyeLogLevel_WARN,		//!< 警告
	sindyeLogLevel_ERR,			//!< エラー
};

/**
 * @class CErrInfo
 * 
 * @brief エラー情報を管理するクラス

 * 個々のエラー情報を管理します。
 * フィーチャとの関連付けは上位クラスで行います。
 */
class CErrInfo
{
public:
	CErrInfo( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszMessage );

	void init( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... );
	void initV( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, va_list args );

	CString ToString() const;
	CString ToLog( sindyeLogFormat emFormat ) const;
public:
	WKSPoint				m_cPoint;		//!< エラーポイント
	long					m_lField;		//!< フィールドインデックス番号
	CString					m_strMsg;		//!< エラーメッセージ
	long					m_lScale;		//!< エラー参照スケール
	sindyeLogLevel			m_emLevel;		//!< エラーレベル
};

/**
 * @clas CErrInfoContainer
 * 
 * @brief 複数のエラー情報を管理するクラス
 *
 * CErrInfoを複数管理します。
 * フィーチャとの関連付けは上位クラスで行います。
 */
class CErrInfoContainer : public std::list<CErrInfo>
{
public:
	void add( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... );
	void addV( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, va_list args );

	CString ToString() const;
};

/** 
 * @class CErrObj
 * 
 * @biref フィーチャ単位のエラー情報を管理するクラス
 *
 * CErrInfoContainerとフィーチャ（レコード）の関連を
 * 管理します。
 */
class CErrObj : public CErrInfoContainer
{
public:
	CErrObj(){}
	CErrObj( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom )
	{
		init( lpcszTableName, lOID, ipGeom );
	}
	~CErrObj(){}

	void init( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom );

	CString ToString() const;
	void ToLog( sindyeLogFormat emFormat, std::list<CString>& listLog ) const;
public:
	CString m_strTableName; //!< テーブル名
	long m_lOID; //!< OBJECTID
	IGeometryPtr m_ipGeom; //!< 形状
};

/**
 * @class CErrObjContainer
 *
 * @brief 複数のフィーチャのエラー情報を管理するクラス
 * 
 * CErrObjを複数管理します。
 * また、エラー情報をダイアログ等で使用する文字列に変換したり、
 * LogHandleToolで扱えるログ形式で吐き出したりすることができます。
 */
class CErrObjContainer : public std::list<CErrObj>
{
public:
	void add( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom, long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... );
	iterator find( LPCTSTR lpcszTableName, long lOID );
	const_iterator find( LPCTSTR lpcszTableName, long lOID ) const;

	CString ToString() const;
	void ToLog( sindyeLogFormat emFormat, FILE* out ) const;
	void ToLog( sindyeLogFormat emFormat, std::list<CString>& listLog ) const;

	/// 最高値のログレベルを返す
	sindyeLogLevel GetMaxLogLevel() const;
};