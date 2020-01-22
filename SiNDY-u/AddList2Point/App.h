#pragma once

#define IDLIST_MODE		1
#define MXYLIST_MODE	2

#define PROGNAME	_T("AddList2Point.exe")
#define OPERATOR	_T("sindy")

class CApp
{
public:
	CApp(void);
	~CApp(void);

	/**
	 * @brief	使用法
	 */
	static void printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	環境変数からＤＢ関係の情報取得
	 * @return	bool
	 */
	bool setDBInfoFromEnv();

	/**
	 * @brief	編集開始
	 * @note	編集用子バージョンを作成する
	 * @return	bool
	 */
	bool editStart();

	/**
	 * @brief	編集終了
	 * @return	bool
	 */
	bool editStop();

	/**
	 * @brief	ＩＤリストから追加
	 */
	void addFromID();

	/**
	 * @brief	MXYリストから追加
	 */
	void addFromMXY();

	/**
	* @brief	属性値セット
	* @warning  属性コピー元とセット先フィーチャは同じフィーチャクラスであること
	* @param	ipSrcFeature	[in]		属性値コピー元のフィーチャ
	* @param	ipFeatureBuffer	[in,out]	セット対象のフィーチャバッファ
	* @param	strError		[out]		エラーメッセージ(エラーあったとき)
	* @return	retval true  セット成功
	* @return	retval false セット失敗
	*/
	bool setAttribute(IFeaturePtr ipSrcFeature, IFeatureBufferPtr ipFeatureBuffer, std::string& strError);

	/**
	 * @brief	属性値セット
	 * @param	mapVal			[in]		属性値格納コンテナ
	 * @param	ipFeatureBuffer	[in,out]	セット対象のフィーチャバッファ
	 * @param	strError		[out]		エラーメッセージ(エラーあったとき)
	 * @return	bool
	 */
	bool setAttribute( const std::map<CString, CString>& mapVal, IFeatureBuffer* ipFeatureBuffer, std::string& strError );

	/**
	 * @brief	共通属性セット
	 * @param	ipFeatureBuffer	[in,out]	セット対象のフィーチャバッファ
 	 * @param	strError		[out]		エラーメッセージ(エラーあったとき)
	 * @return	bool
	 */
	bool setCommonAttribute( IFeatureBuffer* ipFeatureBuffer, std::string& strError );

	/**
	 * @brief	ログ出力
	 * @note	Bug8605
	 * @param	lOID		[in]	OID
	 * @param	ipFeature	[in]	フィーチャ
	 */
	void PrintLog( long lOID, IFeaturePtr ipFeature );

///////////////////////////////
//	メンバ変数
///////////////////////////////

	int		m_nListMode;					/// IDLIST_MODE or MXYLIST_MODE
	TCHAR	m_szListFile[_MAX_PATH];		/// リストファイル

	IWorkspacePtr	m_ipFromWorkspace;		//!< 追加元ＤＢワークスペース(IDリストのときのみ使用)
	IWorkspacePtr	m_ipToWorkspace;		//!< 追加先ＤＢワークスペース

	IWorkspaceEditPtr	m_ipWorkspaceEdit;	/// 編集用ワークスペース

	LPTSTR		m_lpszTargetFCName;			/// 追加元フィーチャクラス名

	IFeatureClassPtr	m_ipTargetFromFC;	/// 追加元フィーチャクラス
	IFeatureClassPtr	m_ipTargetToFC;		/// 追加先フィーチャクラス

	std::vector<long>	m_vecTargetFCFID;	//!< 追加先フィーチャクラスフィールドインデックス
};
