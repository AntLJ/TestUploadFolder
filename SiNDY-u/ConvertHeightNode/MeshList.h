#pragma once
#include "LogSys.h"


/**
 *	@brief	メッシュリスト情報クラス
 */
class MeshList
{
public:
	MeshList(void);
	~MeshList(void);
	/*
	 *	@brief	メッシュリスト取得
	 *	@param	meshListPath [in] メッシュリストフルパス
	 *	@retval	true 成功
	 *	@retval	false 失敗
	 */
	bool SetMeshList(LPCTSTR meshListPath);

	/*
	 *	@brief	メッシュコンテナの先頭を指すイテレータ取得(const_iterator)
	 *	@retval コンストイテレータ
	 */
	std::set<long>::const_iterator begin()
	{
		return m_meshes.begin();
	}

	/*
	 *	@brief	メッシュコンテナの終端を指すイテレータ取得(const_iterator)
	 *	@retval コンストイテレータ
	 */
	std::set<long>::const_iterator end()
	{
		return m_meshes.end();
	}


private:
	std::set<long> m_meshes;

};

