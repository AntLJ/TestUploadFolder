#include "stdafx.h"
#include "RelativeLinks.h"
#include <ArcHelperEx/Map.h>

using namespace sindy::schema;

namespace {
	/// 歩行者種別取得
	CString GetWalkClass(const IFeaturePtr& link)
	{
		CString walkClass;

		// ROAD_LINKなら空文字
		long idx = AheFindField( AheGetFields( link ), walk_link::kWalkClass );
		if( idx < 0 )
			return walkClass;
		
		AheGetDomainNameAndValue( link, walk_link::kWalkClass, walkClass, CComVariant() );
		return walkClass;
	};
} // namespace

// コンストラクタ
CLinkHeight::CLinkHeight( const IFeaturePtr link )
{
	m_feature = link;
	m_id = AheGetOID( link );
	m_layer = AheGetFeatureClassName( link );
	m_walkclass = GetWalkClass( link );
	m_height = 0;
}

// 高さ比べ
height_info::eCode CLinkHeight::Compare( const CLinkHeight& other) const
{
	using namespace height_info;

	if( *this == other )
		return flat;

	return ( *this < other ) ? lower : higher;
}

CRelativeLinks::CRelativeLinks() :
	m_createF(false)
{
}


CRelativeLinks::~CRelativeLinks()
{
}

/// 交差する2リンク取得
std::list< CAdapt<IFeaturePtr> > CRelativeLinks::GetCrossedLinks( const IFeaturePtr& heightNode )
{
	// 接触するリンク取得
	auto SearchLink = [&heightNode]( const IFeatureClassPtr linkT, std::list< CAdapt<IFeaturePtr> >& links ){
		IFeatureCursorPtr cur;
		linkT->Search( AheInitSpatialFilter( nullptr, AheGetShapeCopy( heightNode ) ), VARIANT_FALSE, &cur );

		IFeaturePtr feature;
		while( ( S_OK == cur->NextFeature( &feature ) ) && feature )
		{
			// 人車共用の歩行者リンクは弾く
			// この結果、道路リンク ✕ 道路リンクのノードとなる場合があるが、それは論理チェックで弾くことにする
			long walkclass = AheFindField( AheGetFields(feature), walk_link::kWalkClass );
			if( 0 < walkclass &&
				walk_link::walk_class::kWalkableRoad == AheGetAttribute( feature, walk_link::kWalkClass).lVal )
			{
				continue;
			}

			links.push_back( feature );
		}

		return links;
	};

	// 交差する２リンクを検索する
	std::list< CAdapt<IFeaturePtr> > links;
	SearchLink( m_roadT, links );
	SearchLink( m_walkT, links );

	return links;

}

// 初期化
bool CRelativeLinks::Reset( const IFeaturePtr& heightNode, const CArcHelper& ah, CString& err )
{
	m_heightNode = heightNode;

	CMap cMap( ah.GetFocusMap() );
	m_roadT = AheGetFeatureClass( cMap.FindSameGroupLayer( AheGetFeatureClass( heightNode ), road_link::kTableName ) );
	m_walkT = AheGetFeatureClass( cMap.FindSameGroupLayer( AheGetFeatureClass( heightNode ), walk_link::kTableName ) );
	if( !(m_roadT && m_walkT) )
	{
		err = AheLoadString( IDS_ERR_NOT_FOUND_LAYER );
		return false;
	}

	// 「未整備」なら新規もの
	if( AheGetAttribute( heightNode, height_node::kLinkID1Table).lVal == height_node::link_table::kNone )
	{
		m_createF = true;
		auto links = GetCrossedLinks( heightNode );
		if( links.size() != 2 )
		{
			err = AheLoadString( IDS_ERR_NOT_ON_2LINKS );
			return false;
		}

		Reset( links );
		return true;
	}

	// LINK_TABLE_Cのテーブルを取得
	const auto getLinkTable = [&]( const CString& layerField )
	{
		switch( AheGetAttribute( heightNode, layerField).lVal )
		{
		case height_node::link_table::kRoadLink:
			return m_roadT; break;
		case height_node::link_table::kWalkLink:
			return m_walkT;	break;
		default:
			break;
		}
		return (IFeatureClassPtr)nullptr;
	};

	typedef boost::tuple<linkNo::eCode, CString, CString, CString> layer_id_height;
	static const std::vector< layer_id_height > tupleFields = boost::assign::tuple_list_of
		( linkNo::link1, height_node::kLinkID1Table, height_node::kLinkID1, height_node::kLinkID1Height )
		( linkNo::link2, height_node::kLinkID2Table, height_node::kLinkID2, height_node::kLinkID2Height );
	
	m_links.clear();
	for( const auto& fields : tupleFields )
	{
		// タプルを展開
		linkNo::eCode linkNo = linkNo::unkown;
		CString layerField, idField, heightField;
		boost::tie( linkNo, layerField, idField, heightField ) = fields;

		CLinkHeight cLink;

		// フィーチャクラス情報取得
		IFeatureClassPtr linkT = getLinkTable( layerField );
		cLink.m_layer = AheGetFeatureClassName( linkT );
		
		// フィーチャ情報取得
		IFeaturePtr link;
		long linkID = AheGetAttribute( heightNode, idField ).lVal;
		linkT->GetFeature( linkID, &link );
		cLink.m_id = linkID;
		cLink.m_feature = link;
		cLink.m_walkclass = GetWalkClass( link );

		// 高さ情報取得
		cLink.m_height = AheGetAttribute( heightNode, heightField ).lVal;

		m_links[linkNo] = cLink;
	}

	return true;
}

// 初期化（新規）
void CRelativeLinks::Reset( const std::list< CAdapt<IFeaturePtr> >& links )
{
	_ASSERT( links.size() == 2 );

	m_links[linkNo::link1] = CLinkHeight( links.begin()->m_T );
	m_links[linkNo::link2] = CLinkHeight( links.back().m_T );

}

height_info::eCode CRelativeLinks::GetRelativeHeight( linkNo::eCode linkNo )
{
	const auto& height1 =  GetLinkInfo( linkNo::link1 );
	const auto& height2 =  GetLinkInfo( linkNo::link2 );

	switch( linkNo )
	{
	case linkNo::link1:
		return height1.Compare( height2 );
		break;
	case linkNo::link2:
		return height2.Compare( height1 );
		break;
	default:
		_ASSERT( false );
		break;
	}

	// ここに来ることはないはず
	return height_info::flat;
}

// 論理チェック
bool CRelativeLinks::CheckLogic( CString& msg )
{
	// TODO: このやり方だと何度も m_links を回さなければならないため非効率な気がしてきた。
	//       が、m_links の size は高々2なので、それほどコストを気にすることはないだろう。
	//       むしろ、1回転のうちに色んなチェック詰め込むと、色々と絡まって面倒になる気がするので
	//       ここはこの方針で行くことにする。
	std::vector< std::function<bool()> > checks;

	// 歩行者リンクか
	auto& isWalkLink = [&]( const CLinkHeight& link ){
		return (link.m_layer.CompareNoCase( walk_link::kTableName ) == 0);
	};

	// 属性取得
	auto& getValue = [&]( const CLinkHeight& link, const CString field ){
		long idx = AheFindField( AheGetFields( link.m_feature ), field );
		return AheGetAttribute( link.m_feature, idx );
	};

	// [check] 交差する２リンクが存在するか
	checks.push_back( [&]()
	{
		for( const auto& link : m_links )
		{
			if( link.second.m_feature )
				continue;

			if( m_createF )
			{
				// リンクの交点以外に作成することはできません
				msg = AheLoadString( IDS_ERR_CANNOT_CREATE );
			}
			else
			{
				// リンクが存在しません
				msg = uh::str_util::format( AheLoadString( IDS_ERR_NOEXIST_LINK ), link.second.m_layer, link.second.m_id );
			}
			return false;
		}
		return true;
	});
	
	// [check] 道路リンク同士の交差はNG -> どちらか一方は必ず歩行者
	checks.push_back( [&]()
	{
		for( const auto& link : m_links )
		{
			if( isWalkLink( link.second ) )
			{
				return true;
			}
		}
		msg = AheLoadString( IDS_ERR_ON_ROADLINKS );
		return false;
	});

	// エラー文整形用
	auto errFormat = [&](const CLinkHeight& link, const CString& field){
		return uh::str_util::format(
			_T(" %s(OID:%ld) %s: %ld\n"), AheGetFeatureClassName(link.m_feature), AheGetOID(link.m_feature), field, getValue( link, field ).lVal );
	};

	// [check] 重なり高さノード作成対象外の種別
	checks.push_back( [&]()
	{
		using namespace sindy::schema::road_link;

		CString err;
		for( const auto& link : m_links )
		{
			if( isWalkLink( link.second ) )
			{
				auto walkC =getValue( link.second, walk_link::kWalkClass ).lVal;
				switch ( walkC )
				{
				// 仕様としては人車共用もだが、GetCrossedLinks()で弾いているのでそれはないという前提
				// ここに人車共用がくるようなら、そもそも何かおかしい。
				// あ、、、既存ものならあり得るか、と思ったが、それはそれであってはならないものだからやっぱりデータがおかしい
				case walk_link::walk_class::kConnect:     // 接続リンク
				case walk_link::walk_class::kUnderGround: // 地下通路
				//TODO:ロジカルコネクションは将来的には削除される予定。その時には削除すること
				case walk_link::walk_class::kLogicalConnection: // ロジカルコネクション
				case walk_link::walk_class::kInternalTicketGateUG: // 地下改札内
				case walk_link::walk_class::kPlatFormUG: // 地下プラットフォーム
					err = errFormat( link.second, walk_link::kWalkClass );
					break;
				default:
					break;
				}
			}
			else
			{
				// 駐車場リンク、マッチングリンク
				if( getValue( link.second, kRoadClass ).lVal >= road_class::kParking )
				{
					err = errFormat( link.second, kRoadClass );
				}

				// 細街路2005年度
				if( getValue( link.second, kAlleyMainte ).lVal == alley_mainte::k2005 )
				{
					err += errFormat( link.second, kRoadClass );
				}
			}
		}

		if( err.IsEmpty() )
			return true;
		
		msg.Format( AheLoadString( IDS_ERR_LINK_ATTR ), err );
		return false;
	});

	// 交点のポイント
	auto nodePoint = AheGetShapeCopy( m_heightNode );

	// [check] ちゃんと LINK_ID1 と LINK_ID2 の交点上にあるか
	checks.push_back( [&]()
	{
		std::vector< CAdapt<IGeometryPtr> > geoms;
		for( const auto& link : m_links )
		{
			geoms.push_back( AheGetShapeCopy( link.second.m_feature ) );
		}
		auto crossedP = AheGetCrossOrTouchPoints( geoms.front(), geoms.back(), 0);

		// リンクが移動されて交差しなくなった可能性がある（bug 11427）
		if( crossedP )
		{
			VARIANT_BOOL vb = VARIANT_FALSE;
			((IRelationalOperatorPtr)crossedP)->Disjoint( nodePoint, &vb );
			if( !vb )
			{
				return true;
			}
		}
		
		// 2リンクの存在判定をしているのでlink1とlink2は必ず存在する
		msg = uh::str_util::format( 
			AheLoadString( IDS_ERR_NO_CROSSED_LINK ),
			m_links[linkNo::link1].m_layer, m_links[linkNo::link1].m_id,
			m_links[linkNo::link2].m_layer, m_links[linkNo::link2].m_id );

		return false;
	});

	// [check] 高さノード同士が重ならないか
	checks.push_back( [&]()
	{
		if( !m_createF )
			return true;

		long num = 0;
		AheGetFeatureClass( m_heightNode )->FeatureCount( 
			AheInitSpatialFilter( nullptr, nodePoint ),
			&num );

		if( num > 0 )
		{
			msg = AheLoadString( IDS_ERR_ALREADY_EXIT );
			return false;
		}
		return true;
	});

	// 単発チェック実行
	for( auto& check : checks )
	{
		if( !check() )
			return false;
	}

	return true;
}