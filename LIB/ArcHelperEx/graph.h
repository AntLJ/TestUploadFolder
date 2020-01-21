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

#ifndef _ARCHELPEREX_GRAPH_H_
#define _ARCHELPEREX_GRAPH_H_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

/**
 * @brief shared_ptrの中身で比較するクラス
 */
template <class T>
class shared_ptr_equal_to
{
public:
	boost::shared_ptr<T> obj_;
	shared_ptr_equal_to(){}
	shared_ptr_equal_to( const boost::shared_ptr<T>& obj ) : obj_(obj)
	{
	}
	bool operator()( const boost::shared_ptr<T>& obj ) const
	{
		return *(obj_.get()) == *(obj.get());
	}
};

namespace path_analyzer
{

/**
 * @brief エッジ管理クラス
 * 
 * テンプレート引数のTは、GetAttribute()という関数が必要です。
 * Tのサンプル実装としてCItemがあるので、参考にして下さい。
 */
template <class T>
class edge {
public:
	edge() : __from(-1), __to(-1)
	{
	}
	edge( const T& _path ) : __edge_native(_path)
	{
		__from = __edge_native.GetAttribute(_T("FROM_NODE_ID")).lVal;
		__to   = __edge_native.GetAttribute(_T("TO_NODE_ID")).lVal;
	}
	bool operator==( const edge& _edge ) const
	{
		return __edge_native == _edge.__edge_native;
	}
	bool is_neighbour( const edge& _path ) const
	{
		return is_flow_to( _path );
	}
	bool is_neighbour_from( const edge& _path ) const
	{
		return( __from == _path.__from || __from == _path.__to );
	}
	bool is_neighbour_to( const edge& _path ) const
	{
		return( __to   == _path.__from || __to   == _path.__to );
	}
	bool is_neighbour_from( long _node_id ) const
	{
		return __from == _node_id;
	}
	bool is_neighbour_to( long _node_id ) const
	{
		return __to   == _node_id;
	}
	bool is_directed() const { return __edge_native.m_bDirected; }
	/// 自分自身から_targetに流れるかどうか
	bool is_flow_to( const edge& _target ) const
	{
		return _target.has_node( __from ) || _target.has_node( __to );
	}
	bool has_node( long _node_id ) const
	{
		return ( _node_id == __from || _node_id == __to );
	}
	void trace() const
	{
		for( std::list<boost::shared_ptr<edge> >::const_iterator it = __neighbours->begin(); it != __neighbours->end(); ++it )
			it->get()->trace();
	}
	long __from, __to;
	T __edge_native;
	std::list<boost::shared_ptr<edge> > __neighbours; // __edge_nativeから辿れる_tree集合
};

/**
 * @brief エッジ管理クラス
 *
 * edgeと異なるのは、こちらは向きを考慮できるという点です。
 * それ以外はedgeと一緒です。
 */
template <class T>
class directed_edge {
public:
	directed_edge() : __from(-1), __to(-1)
	{
	}
	directed_edge( const T& _path ) : __edge_native(_path)
	{
		__from = __edge_native.GetAttribute(_T("FROM_NODE_ID")).lVal;
		__to   = __edge_native.GetAttribute(_T("TO_NODE_ID")).lVal;
	}
	bool operator==( const directed_edge& _edge ) const
	{
		return __edge_native == _edge.__edge_native;
	}
	bool is_neighbour( const directed_edge& _path ) const
	{
		return is_flow_to( _path ) || _path.is_flow_to( *this );
	}
	bool is_neighbour_from( const directed_edge& _path ) const
	{
		if( __edge_native.m_bDirected  && _path.__edge_native.m_bDirected )
			return __from == _path.__to;
		else 
			return __from == _path.__from || __from == _path.__to;
	}
	bool is_neighbour_to( const directed_edge& _path ) const
	{
		if( __edge_native.m_bDirected && _path.__edge_native.m_bDirected )
			return __to == _path.__from;
		else
			return __to == _path.__from || __to == _path.__to;
	}
	bool is_neighbour_from( long _node_id ) const
	{
		return __from == _node_id;
	}
	bool is_neighbour_to( long _node_id ) const
	{
		return __to   == _node_id;
	}
	bool is_directed() const { return __edge_native.m_bDirected; }
	/// 自分自身から_targetに流れるかどうか
	bool is_flow_to( const directed_edge& _target ) const
	{
		if( is_directed() )
		{
			if( _target.is_directed() )
				return __to == _target.__from;
			else
				return __to == _target.__from || __to == _target.__to;
		}
		else {
			if( _target.is_directed() )
				return __from == _target.__from || __to == _target.__from;
			else
				return __to == _target.__from || __to == _target.__to || __from == _target.__from || __from == _target.__to;
		}
	}
	bool has_node( long _node_id ) const
	{
		return ( _node_id == __from || _node_id == __to );
	}
	void trace() const
	{
		for( std::list<boost::shared_ptr<edge> >::const_iterator it = __neighbours->begin(); it != __neighbours->end(); ++it )
			it->get()->trace();
	}
	long __from, __to;
	T __edge_native;
	std::list<boost::shared_ptr<directed_edge> > __neighbours; // __edge_nativeから辿れる_tree集合
};

/**
 * @brief パス管理クラス
 */
template <class TEdge>
class path : public std::list<boost::shared_ptr<TEdge> >
{
public:
	path() : __start_node_id(-1), __end_node_id(-1)
	{
	}
	path( long _start_node_id, long _end_node_id ) : __start_node_id(_start_node_id), __end_node_id(_end_node_id)
	{
	}
	long __start_node_id, __end_node_id; //!< 開始ノードID
};

/**
 * @brief グラフクラス
 *
 * 与えられたエッジ情報から、パスを解析します。
 * 以下は、単純な十字路の南リンク（100）からたどった場合の
 * 全てのパスを解析し出力します。
 *
 * @code
 *	CItems items;
 *	std::list<path<directed_edge<CItem> > > listPath;
 *
 *	// CItem( from_node_id, to_node_id, edge_id, 有向 or 無向 )
 *                                          //            2
 *	items.push_back( CItem( 0, 1, 100 ) );  //            | 103
 *	items.push_back( CItem( 3, 1, 101 ) );  //            |
 *	items.push_back( CItem( 1, 4, 102 ) );  //  3 - 101 - 1 - 102 - 4
 *	items.push_back( CItem( 1, 2, 103 ) );  //            |
 *                                          //            | 100
 *                                          //            0
 *
 *	graph<CItems,directed_edge<CItem> > g( items );
 *
 *	std::list<long> listStart;
 *	listStart.push_back( 0 );
 *	g.create_path( listStart, std::list<long>(), listPath );
 *	g.trace_path( listPath );
 * @endcode
 */
template <class T, class TEdge>
class graph : public std::list<boost::shared_ptr<TEdge> >
{
public:
	graph()
	{
	}
	graph( const T& _network_links )
	{
		create_graph( _network_links );
	}
	/// グラフを構築する
	void create_graph( const T& _network_links )
	{
		// cNetworkLinksをすべてedgeに格納
		for( T::const_iterator it = _network_links.begin(); it != _network_links.end(); ++it )
			push_back( boost::shared_ptr<TEdge>(new TEdge( *it )) );

		// グラフを構築
		for( const_iterator it = begin(); it != end(); ++it )
		{
			for( const_iterator it2 = begin(); it2 != end(); ++it2 )
			{
				if( (*it)->__edge_native != (*it2)->__edge_native && // 自分自身ははじかないと、隣として登録されてしまう
					it->get()->is_neighbour( *(it2->get()) ) )
				{
					it->get()->__neighbours.push_back( *it2 );
					//ATLTRACE(_T("%d-%d\n"), (*it)->__edge_native.GetOID(), (*it2)->__edge_native.GetOID() );
				}
			}
		}
	}
	/// すべてのルートを作成する
	void create_path( long _start_node_id, long _end_node_id, std::list<path<TEdge> >& _listPath ) const
	{
		std::list<long> _list_start_node_id, _list_end_node_id;
		_list_start_node_id.push_back(_start_node_id);
		_list_end_node_id.push_back(_end_node_id);
		create_path( _list_start_node_id, _list_end_node_id, _listPath );
	}
	/// すべてのルートを作成する
	void create_path( const std::list<long>& _list_start_node_id, const std::list<long>& _list_end_node_id, std::list<path<TEdge> >& _listPath ) const
	{
		// _start_node_idを持つものに関してパスを作成する
		for( const_iterator itPaths = begin(); itPaths != end(); ++itPaths )
		{
			for( std::list<long>::const_iterator itStart = _list_start_node_id.begin(); itStart != _list_start_node_id.end(); ++itStart )
			{
				if( (*itPaths)->has_node( *itStart ) )
				{
					if( _list_end_node_id.empty() )
					{
						// 終点未指定版
						path<TEdge> _path( *itStart, -1 ); // 始点を登録して作成
						_path.push_back( *itPaths );
						create_path( _path, _listPath );
						_listPath.push_back( _path );
					}
					else {
						// 終点指定版
						for( std::list<long>::const_iterator itEnd = _list_end_node_id.begin(); itEnd != _list_end_node_id.end(); ++itEnd )
						{
							if( *itStart != *itEnd )
							{
								path<TEdge> _path( *itStart, *itEnd ); // 始終点を登録して作成
								_path.push_back( *itPaths );
								create_path( _path, _listPath );
								_listPath.push_back( _path );
							}
						}
					}
				}
			}
		}

		// _end_node_idが指定されている場合はそれで終わらないものを削除する必要がある
		if( ! _list_end_node_id.empty() )
		{
			std::list<path<TEdge> >::iterator _itPath = _listPath.begin();
			while( _itPath != _listPath.end() )
			{
				const TEdge& _path_last = *_itPath->rbegin()->get();
				bool _bfind = false;
				for( std::list<long>::const_iterator it = _list_end_node_id.begin(); it != _list_end_node_id.end(); ++it )
				{
					if( _path_last.has_node(*it) )
					{
						_bfind = true;
						break;
					}
				}
				if( ! _bfind )
					_itPath = _listPath.erase( _itPath );
				else
					++_itPath;
			}
		}
	}
	void create_path( path<TEdge>& _path, std::list<path<TEdge> >& _listPath ) const
	{
		// _pathのリストの最終の次を調べる
		// その際、_pathのリストの中に「次」が存在したらループしていると判断し、それは飛ばす
		// 複数ある場合は_pathのリストのコピーを_listPathに追加
		boost::shared_ptr<TEdge> _path_last = *_path.rbegin(); // _pathの最後

		// _path_lastが指定されており、且つ最後のエッジが終了地点ならここで終わる
		if( 0 <= _path.__end_node_id && _path_last->has_node( _path.__end_node_id ) )
			return;

		// _path_lastとその前の関連付けに、_path_lastのfrom、toのどちらが使われているかに
		// よって、itの正当性を検査する必要がある
		path<TEdge>::reverse_iterator it_path_last_next = _path.rbegin();
		ATLTRACE(_T("FROM,TO:(%ld,%ld)\n"), (*it_path_last_next)->__from, (*it_path_last_next)->__to );
		++it_path_last_next; // TODO: 5,4 から 0,4に移らないといけないのに、うつらない。。。
		if( _path.rend() != it_path_last_next )
			ATLTRACE(_T("FROM,TO:(%ld,%ld)\n"), (*it_path_last_next)->__from, (*it_path_last_next)->__to );
		bool _brel_from = false, _brel_to = false;
		if( it_path_last_next != _path.rend() ) // 初回以外
		{
			_brel_from = _path_last->is_neighbour_from( *(*it_path_last_next).get() );
			_brel_to   = _path_last->is_neighbour_to(   *(*it_path_last_next).get() );
		}
		else { // 初回
			// 初回は 開始ノード が path そのものに設定されている
			_brel_from = _path_last->is_neighbour_from( _path.__start_node_id );
			_brel_to   = _path_last->is_neighbour_to(   _path.__start_node_id );
		}

		bool _shuld_be_add = false; // 新しいリストを作る必要があるかどうか（既存リストに追加したかどうか）
		path<TEdge> _path_backup( _path ); // _pathのバックアップ（分岐用）
		for( path<TEdge>::const_iterator it = _path_last.get()->__neighbours.begin(); it != _path_last.get()->__neighbours.end(); ++it )
		{
			if( std::find_if( _path_backup.begin(), _path_backup.end(), shared_ptr_equal_to<TEdge>( *it ) ) == _path_backup.end() && // itが_pathの中に含まれるかどうか判断（ループ回避）
				( _brel_from && _path_last->is_neighbour_to(   *it->get() ) ||
				  _brel_to   && _path_last->is_neighbour_from( *it->get() ) ||
				  ( false == _brel_from && false == _brel_to ) ) && 
				  _path_last->is_flow_to( *it->get() ) )
			{
				if( ! _shuld_be_add ) // == 初回
				{
					// 他の枝を検索するとき用に_pathのコピー取得
					_path_backup = _path;

					// そのまま_pathにitを追加
					_path.push_back( *it );
					// 再帰
					create_path( _path, _listPath );

					_shuld_be_add = true; // 次のループでは枝分かれとなる
				}
				else { // == 別の枝
					// _pathをコピーしたものにitを追加、さらに_listPathに追加
					path<TEdge> _path_backup_copy( _path_backup );
					_path_backup_copy.push_back( *it );
					// 再帰
					create_path( _path_backup_copy, _listPath );
					// 枝分かれの最終点なので、_listPathに登録
					_listPath.push_back( _path_backup_copy );
				}
			}
		}
	}
	void trace_path( const std::list<path<TEdge> >& _listPath ) const
	{
		for( std::list<path<TEdge> >::const_iterator it = _listPath.begin(); it != _listPath.end(); ++it )
		{
			ATLTRACE(_T("[%d]"), it->__start_node_id );
			trace_path( *it );
			ATLTRACE(_T("-[%d]\n"), it->__end_node_id );
		}
	}
	void trace_path( const path<TEdge>& _path ) const
	{
		for( path<TEdge>::const_iterator it = _path.begin(); it != _path.end(); ++it )
		{
			ATLTRACE(_T("-%d"), it->get()->__edge_native.GetOID() );
		}
	}
	void trace_graph() const
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			for( path<TEdge>::const_iterator it2 = (*it)->__neighbours.begin(); it2 != (*it)->__neighbours.end(); ++it2 )
				ATLTRACE(_T("[%d] - [%d]\n"), (*it)->__edge_native.GetOID(), (*it2)->__edge_native.GetOID() );
		}
	}
};

/**
 * @brief edgeのテンプレート引数であるTのサンプル実装
 */
class CItem
{
public:
	CItem() : m_vaFrom(-1L), m_vaTo(-1L), m_lOID(-1), m_bDirected(false)
	{
	}
	CItem( long from, long to, long oid, bool directed ) : m_vaFrom(from), m_vaTo(to), m_lOID(oid), m_bDirected(directed)
	{
	}
	bool operator == ( const CItem& item ) const
	{
		return ( GetOID() == item.GetOID() );
	}
	bool operator != ( const CItem& item ) const
	{
		return ( GetOID() != item.GetOID() );
	}
	CComVariant GetAttribute(LPCTSTR lpcsz) const
	{
		if( 0 == _tcsicmp(_T("FROM_NODE_ID"), lpcsz) )
			return m_vaFrom;
		else if( 0 == _tcsicmp(_T("TO_NODE_ID"), lpcsz) )
			return m_vaTo;
		return CComVariant();
	}
	long GetOID() const
	{
		return m_lOID;
	}
	CComVariant m_vaFrom, m_vaTo;
	long m_lOID;
	bool m_bDirected;
};

typedef std::list<CItem> CItems;

} // namespace path_analyzer

#endif // ifndef _ARCHELPEREX_GRAPH_H_
