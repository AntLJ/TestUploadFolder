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

#include "CurveInfoData.h"

namespace sindy
{
	class CRoadLinkTable;
	class CRowContainer;
	typedef boost::shared_ptr< CRowContainer > CSPRowContainer;
	class CRoadLinkTable;
	typedef boost::shared_ptr< CRoadLinkTable > CSPRoadLinkTable;
}

namespace curve_info
{
	/// カーブ情報関連のユーティリティ
	namespace utility
	{
		static std::string fd = "\t";		///< フィールドデリミタ

		/**
		 * @brief ２セグメントのなす角の角度を返す
		 * @param[in]	crSegment1	対象セグメント1
		 * @param[in]	crSegment2	対象セグメント2
		 * @return	角度（範囲は(-180,180]、反時計回りが正）
		 */
		double angle( const Segment& crSegment1, const Segment& crSegment2 );

#if 0	// 未使用だが、使えそうなので残しておく
		/**
		 * @brief 対象セグメントの長さをメートル単位で返す
		 * @param[in]	cpSegment	対象セグメント
		 * @return	長さ(m)
		 */
		double meterLength( const SegmentPtr cpSegment );
#endif
		/// ログのフォーマットに依存する処理はここに集約する
		namespace log_format
		{
			/// ログのヘッダに列挙するフィールド名列を返す
			std::vector< std::string > fieldNames();

			std::string freeComment( std::string& crMessage );

			/**
			 * @brief ログの点情報レコードの文字列を作成して返す
			 * @param[in]	cLon		経度
			 * @param[in]	cLat		緯度
			 * @param[in]	cMessage	メッセージ
			 * @return	レコード文字列
			 */
			std::string pointRecord( double cLon, double cLat, const std::string& crMessage );

			/**
			 * @brief ログのリンク情報レコードの文字列を作成して返す
			 * @param[in]	cTable		テーブル名
			 * @param[in]	cOID		オブジェクトID
			 * @param[in]	cMessage	メッセージ
			 * @return	レコード文字列
			 */
			std::string linkRecord( const std::string& cTable, long cOID, const std::string& crMessage );

		} // namespace log_record

		/**
		 * @brief ２点間の距離を返す
		 * @note	単に三平方の定理で算出した結果を返すだけなので、長さの単位は問わない。
		 */
		double distance( double x1, double y1, double x2, double y2 );

		/**
		 * @brief 長さの単位を度からmに変換するために掛けるべき係数を返す
		 * @note	指定した２点間の距離を度とmのそれぞれで算出し、その比で決定する。
		 * @param[in]	cLon1	１点目の経度
		 * @param[in]	cLat1	１点目の緯度
		 * @param[in]	cLon2	２点目の経度
		 * @param[in]	cLat2	２点目の緯度
		 */
		double degreeToMeterCoef( double cLon1, double cLat1, double cLon2, double cLat2 );

		/**
		 * @brief 円弧が円周を占める角度を返す
		 * @note	例えば円弧の長さが円周と同じなら、360が返る。
		 * @param[in]	cCircularArcLength	円弧の長さ
		 * @param[in]	cRadius				円の半径
		 * @return	角度（度）
		 */
		double circularArcAngle( double cCircularArcLength, double cRadius );

		/**
		 * @brief 対象リンクが通れるリンクであるか否かを判定する（Bug9642で追加）
		 * @note	通れないリンクとは、条件無し通行禁止または通行不可のリンク。
		 * @return	判定結果
		 */
		bool isPassable( IFeaturePtr cpLink );

		/**
		 * @brief 対象リンク１から対象リンク２へ通行できるか否かを右左折禁止規制によって判定する
		 * @param[in]	cpLink1	対象リンク１
		 * @param[in]	cpLink2	対象リンク２
		 * @param[in]	crLinkTable	対象リンク１、２を含むリンクテーブル（含まれない場合は通行可能と判定される）
		 * @return	判定結果
		 */
		bool isPassableByTurnreg( IFeaturePtr cpLink1, IFeaturePtr cpLink2, sindy::CRoadLinkTable& crLinkTable );

		/**
		 * @brief 対象リンクが高速域であるか否（低速域である）かを判定する
		 * @param[in]	cpLink	対象リンク
		 * @return	判定結果
		 */
		bool isHighSpeedArea( IFeaturePtr cpLink );

		namespace id_lists
		{
			typedef std::set< long > IDs;

			/**
			 * @brief IDを列挙した文字列を作成するクラス
			 * @note	「IDを1000個ずつ列挙したWhere句群」を作るのに使われるのを想定。
			 */
			class IDLists : public std::vector< std::string >
			{
			public:
				/**
				 * @brief コンストラクタ
				 * @param[in]	crIDs		対象ID群
				 * @param[in]	cDelimiter	デリミタ
				 * @param[in]	cMax		列挙する数の最大値
				 */
				IDLists( const IDs& crIDs, const std::string& cDelimiter, unsigned cMax );
			};

		} // namespace id_lists

		// sindylib_base関連のシンタックスシュガー
		namespace sindylib_base
		{
			/// CSPRowContainerへのキャスト
			template < typename T > sindy::CSPRowContainer row( T& t )
			{
				return boost::dynamic_pointer_cast< sindy::CRowContainer >( t );
			}

			/// _IRow*へのキャスト
			template < typename T > _IRow* irow( T& t )
			{
				return (_IRow*)*boost::dynamic_pointer_cast< sindy::CRowContainer >( t );
			}

			/// CSPRoadLinkTableへのキャスト
			template < typename T > sindy::CSPRoadLinkTable road_link_table( T& t )
			{
				return boost::dynamic_pointer_cast< CRoadLinkTable >( t );
			}
		}

		/**
		 * @brief 指定されたディレクトリがなければ作成する
		 * @param[in]	crDirPath	対象ディレクトリパス
		 */
		void createDirectory( const boost::filesystem::path& crDirPath );

		/**
		 * @brief 空間参照関連の機能	（[Bug9717]で追加）
		 * @note	このツールにおける空間参照設定の方針は下記の通り。
		 *			(1) 最初に着目したリンクの空間参照を基準とし、全てそれで処理する。
		 *			(2) 形状を含むインスタンスを生成する際は必ずnewGeometry()を使い、生成と同時に空間参照をセットする。
		 *			(3) ジオメトリ演算系の関数を使用後、空間参照が変わる可能性がある場合はsetSpatialReference()でセットし直す。
		 */
		namespace spatial_reference
		{
			/**
			 * @brief 対象ジオメトリに空間参照がセットされているか否かを判定する
			 * @param[in]	cpGeom	対象ジオメトリ
			 * @note	記述を短くするためだけの関数。
			 * @return	判定結果
			 */
			bool hasSpatialReference( const IGeometryPtr cpGeom );

			/**
			 * @brief 対象ジオメトリに空間参照がセットされていなければセットする
			 * @note	目的は、実行後に対象ジオメトリに指定した空間参照がセットされている事を保障する事。
			 * @param[in]	cpGeom			対象ジオメトリ
			 * @param[in]	cpSpatialRef	セットする空間参照
			 * @exception		失敗
			 * @todo	例外送出は念のため。まず失敗しない事が確認できたらassertにして良い。
			 */
			void setSpatialReference( IGeometryPtr cpGeom, const ISpatialReferencePtr cpSpatialRef );

			/**
			 * @brief 形状を含むインスタンスを生成（CreateInstance）し、指定された空間参照をセットして返す
			 * @note	一行で簡潔に書きたい＆型が複数あるので関数テンプレート化してみたが、呼び出し自体が長いので微妙な感じ…。
			 * @param	TPtr	生成されたインスタンスを指せるCOMインターフェイスポインタの型（IPointPtr, IPolylinePtrなど）
			 * @param[in]	clsid			生成するインスタンスのCLSID
			 * @param[in]	cpSpatialRef	セットする空間参照
			 * @return	生成されたインスタンス
			 */
			template <typename TPtr>
			TPtr newGeometry( const CLSID& clsid, const ISpatialReferencePtr cpSpatialRef )
			{
				TPtr pInstance( clsid );
				setSpatialReference( pInstance, cpSpatialRef );
				return pInstance;
			}
		} // namespace spatial_reference

		/**
		 * @brief double値を文字列化して返す
		 * @note	精度指定あり。boost::lexical_castはboostのバージョン1.34.0だと16桁、1.51.0だとそれ以上（未確認）と動作が違ったので、
		 *			結果が異なる事を防ぐためにstd::stringstreamを使用して精度を明示的に指定するようにした。
		 * @param[in]	cNum		対象double値
		 * @param[in]	cPrecision	精度（10進数の桁数。123.456なら6桁）
		 * @return	文字列
		 */
		std::string toStr( double cNum, std::streamsize cPrecision );

	} // namespace utility
} // namespace curve_info

