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
 * @brief shared_ptr�̒��g�Ŕ�r����N���X
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
 * @brief �G�b�W�Ǘ��N���X
 * 
 * �e���v���[�g������T�́AGetAttribute()�Ƃ����֐����K�v�ł��B
 * T�̃T���v�������Ƃ���CItem������̂ŁA�Q�l�ɂ��ĉ������B
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
	/// �������g����_target�ɗ���邩�ǂ���
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
	std::list<boost::shared_ptr<edge> > __neighbours; // __edge_native����H���_tree�W��
};

/**
 * @brief �G�b�W�Ǘ��N���X
 *
 * edge�ƈقȂ�̂́A������͌������l���ł���Ƃ����_�ł��B
 * ����ȊO��edge�ƈꏏ�ł��B
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
	/// �������g����_target�ɗ���邩�ǂ���
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
	std::list<boost::shared_ptr<directed_edge> > __neighbours; // __edge_native����H���_tree�W��
};

/**
 * @brief �p�X�Ǘ��N���X
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
	long __start_node_id, __end_node_id; //!< �J�n�m�[�hID
};

/**
 * @brief �O���t�N���X
 *
 * �^����ꂽ�G�b�W��񂩂�A�p�X����͂��܂��B
 * �ȉ��́A�P���ȏ\���H�̓샊���N�i100�j���炽�ǂ����ꍇ��
 * �S�Ẵp�X����͂��o�͂��܂��B
 *
 * @code
 *	CItems items;
 *	std::list<path<directed_edge<CItem> > > listPath;
 *
 *	// CItem( from_node_id, to_node_id, edge_id, �L�� or ���� )
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
	/// �O���t���\�z����
	void create_graph( const T& _network_links )
	{
		// cNetworkLinks�����ׂ�edge�Ɋi�[
		for( T::const_iterator it = _network_links.begin(); it != _network_links.end(); ++it )
			push_back( boost::shared_ptr<TEdge>(new TEdge( *it )) );

		// �O���t���\�z
		for( const_iterator it = begin(); it != end(); ++it )
		{
			for( const_iterator it2 = begin(); it2 != end(); ++it2 )
			{
				if( (*it)->__edge_native != (*it2)->__edge_native && // �������g�͂͂����Ȃ��ƁA�ׂƂ��ēo�^����Ă��܂�
					it->get()->is_neighbour( *(it2->get()) ) )
				{
					it->get()->__neighbours.push_back( *it2 );
					//ATLTRACE(_T("%d-%d\n"), (*it)->__edge_native.GetOID(), (*it2)->__edge_native.GetOID() );
				}
			}
		}
	}
	/// ���ׂẴ��[�g���쐬����
	void create_path( long _start_node_id, long _end_node_id, std::list<path<TEdge> >& _listPath ) const
	{
		std::list<long> _list_start_node_id, _list_end_node_id;
		_list_start_node_id.push_back(_start_node_id);
		_list_end_node_id.push_back(_end_node_id);
		create_path( _list_start_node_id, _list_end_node_id, _listPath );
	}
	/// ���ׂẴ��[�g���쐬����
	void create_path( const std::list<long>& _list_start_node_id, const std::list<long>& _list_end_node_id, std::list<path<TEdge> >& _listPath ) const
	{
		// _start_node_id�������̂Ɋւ��ăp�X���쐬����
		for( const_iterator itPaths = begin(); itPaths != end(); ++itPaths )
		{
			for( std::list<long>::const_iterator itStart = _list_start_node_id.begin(); itStart != _list_start_node_id.end(); ++itStart )
			{
				if( (*itPaths)->has_node( *itStart ) )
				{
					if( _list_end_node_id.empty() )
					{
						// �I�_���w���
						path<TEdge> _path( *itStart, -1 ); // �n�_��o�^���č쐬
						_path.push_back( *itPaths );
						create_path( _path, _listPath );
						_listPath.push_back( _path );
					}
					else {
						// �I�_�w���
						for( std::list<long>::const_iterator itEnd = _list_end_node_id.begin(); itEnd != _list_end_node_id.end(); ++itEnd )
						{
							if( *itStart != *itEnd )
							{
								path<TEdge> _path( *itStart, *itEnd ); // �n�I�_��o�^���č쐬
								_path.push_back( *itPaths );
								create_path( _path, _listPath );
								_listPath.push_back( _path );
							}
						}
					}
				}
			}
		}

		// _end_node_id���w�肳��Ă���ꍇ�͂���ŏI���Ȃ����̂��폜����K�v������
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
		// _path�̃��X�g�̍ŏI�̎��𒲂ׂ�
		// ���̍ہA_path�̃��X�g�̒��Ɂu���v�����݂����烋�[�v���Ă���Ɣ��f���A����͔�΂�
		// ��������ꍇ��_path�̃��X�g�̃R�s�[��_listPath�ɒǉ�
		boost::shared_ptr<TEdge> _path_last = *_path.rbegin(); // _path�̍Ō�

		// _path_last���w�肳��Ă���A���Ō�̃G�b�W���I���n�_�Ȃ炱���ŏI���
		if( 0 <= _path.__end_node_id && _path_last->has_node( _path.__end_node_id ) )
			return;

		// _path_last�Ƃ��̑O�̊֘A�t���ɁA_path_last��from�Ato�̂ǂ��炪�g���Ă��邩��
		// ����āAit�̐���������������K�v������
		path<TEdge>::reverse_iterator it_path_last_next = _path.rbegin();
		ATLTRACE(_T("FROM,TO:(%ld,%ld)\n"), (*it_path_last_next)->__from, (*it_path_last_next)->__to );
		++it_path_last_next; // TODO: 5,4 ���� 0,4�Ɉڂ�Ȃ��Ƃ����Ȃ��̂ɁA����Ȃ��B�B�B
		if( _path.rend() != it_path_last_next )
			ATLTRACE(_T("FROM,TO:(%ld,%ld)\n"), (*it_path_last_next)->__from, (*it_path_last_next)->__to );
		bool _brel_from = false, _brel_to = false;
		if( it_path_last_next != _path.rend() ) // ����ȊO
		{
			_brel_from = _path_last->is_neighbour_from( *(*it_path_last_next).get() );
			_brel_to   = _path_last->is_neighbour_to(   *(*it_path_last_next).get() );
		}
		else { // ����
			// ����� �J�n�m�[�h �� path ���̂��̂ɐݒ肳��Ă���
			_brel_from = _path_last->is_neighbour_from( _path.__start_node_id );
			_brel_to   = _path_last->is_neighbour_to(   _path.__start_node_id );
		}

		bool _shuld_be_add = false; // �V�������X�g�����K�v�����邩�ǂ����i�������X�g�ɒǉ��������ǂ����j
		path<TEdge> _path_backup( _path ); // _path�̃o�b�N�A�b�v�i����p�j
		for( path<TEdge>::const_iterator it = _path_last.get()->__neighbours.begin(); it != _path_last.get()->__neighbours.end(); ++it )
		{
			if( std::find_if( _path_backup.begin(), _path_backup.end(), shared_ptr_equal_to<TEdge>( *it ) ) == _path_backup.end() && // it��_path�̒��Ɋ܂܂�邩�ǂ������f�i���[�v����j
				( _brel_from && _path_last->is_neighbour_to(   *it->get() ) ||
				  _brel_to   && _path_last->is_neighbour_from( *it->get() ) ||
				  ( false == _brel_from && false == _brel_to ) ) && 
				  _path_last->is_flow_to( *it->get() ) )
			{
				if( ! _shuld_be_add ) // == ����
				{
					// ���̎}����������Ƃ��p��_path�̃R�s�[�擾
					_path_backup = _path;

					// ���̂܂�_path��it��ǉ�
					_path.push_back( *it );
					// �ċA
					create_path( _path, _listPath );

					_shuld_be_add = true; // ���̃��[�v�ł͎}������ƂȂ�
				}
				else { // == �ʂ̎}
					// _path���R�s�[�������̂�it��ǉ��A�����_listPath�ɒǉ�
					path<TEdge> _path_backup_copy( _path_backup );
					_path_backup_copy.push_back( *it );
					// �ċA
					create_path( _path_backup_copy, _listPath );
					// �}������̍ŏI�_�Ȃ̂ŁA_listPath�ɓo�^
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
 * @brief edge�̃e���v���[�g�����ł���T�̃T���v������
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
