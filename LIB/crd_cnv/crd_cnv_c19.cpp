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

#include "crd_cnv.h"
#include "crd_cnv_defines.h"

//------------------------------------------------------------
/*
	１９座標系の表を作るメンバ関数
*/
//------------------------------------------------------------
void	crd_cnv::
Make19tbl	(	void	)
{
	c19_origin	origin19[19] = 
				{{{129,30,0.0},	{33,0,0.0}},	// 系番号１
				{{131,0,0.0},	{33,0,0.0}},	// 系番号２
				{{132,10,0.0},	{36,0,0.0}},	// 系番号３
				{{133,30,0.0},	{33,0,0.0}},	// 系番号４
				{{134,20,0.0},	{36,0,0.0}},	// 系番号５
				{{136,0,0.0},	{36,0,0.0}},	// 系番号６
				{{137,10,0.0},	{36,0,0.0}},	// 系番号７
				{{138,30,0.0},	{36,0,0.0}},	// 系番号８
				{{139,50,0.0},	{36,0,0.0}},	// 系番号９
				{{140,50,0.0},	{40,0,0.0}},	// 系番号１０
				{{140,15,0.0},	{44,0,0.0}},	// 系番号１１
				{{142,15,0.0},	{44,0,0.0}},	// 系番号１２
				{{144,15,0.0},	{44,0,0.0}},	// 系番号１３
				{{142,0,0.0},	{26,0,0.0}},	// 系番号１４
				{{127,30,0.0},	{26,0,0.0}},	// 系番号１５
				{{124,0,0.0},	{26,0,0.0}},	// 系番号１６
				{{131,0,0.0},	{26,0,0.0}},	// 系番号１７
				{{136,0,0.0},	{20,0,0.0}},	// 系番号１８
				{{154,0,0.0},	{26,0,0.0}}};	// 系番号１９

	// 平面直角座標(19系)のラジアンの表を作成する
	int	i;
	for (i = 0; i < 19; i++)
	{
		Rad_Origin[i].lat	= (origin19[i].lat.deg + (origin19[i].lat.min / 60.0)) * M_PI / 180.0;
		Rad_Origin[i].lon	= (origin19[i].lon.deg + (origin19[i].lon.min / 60.0)) * M_PI / 180.0;
	//	printf("Rad_Origin[%d].lat	= %0.60f;\n", i, Rad_Origin[i].lat);
	//	printf("Rad_Origin[%d].lon	= %0.60f;\n", i, Rad_Origin[i].lon);
	}
}	

//------------------------------------------------------------
/*
	赤道から緯度Phiまでの子午線孤長を得る
	@return 子午線孤長(m)

	【参考】
	http://vldb.gsi.go.jp/sokuchi/surveycalc/algorithm/b2s/b2s.htm
*/
//------------------------------------------------------------
inline	double	crd_cnv::
M_Arc_Len_Core	(	double	Ra,		// 地球楕円体の長半径（メートル）
					double	ea2,	// 第一離心率の2乗
					double	Phi	)	// 緯度
{
	double	tmp1 = 0.0;
	double	A=0.0, B=0.0, C=0.0, D=0.0, E=0.0, F=0.0, G=0.0, H=0.0, I=0.0;
	double	B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0, B7=0.0, B8=0.0, B9=0.0;
	double	arc_len=0.0;

	A	= 1.0 + 3.0*ea2/4.0 + 45.0*pow(ea2,2.0)/64.0 + 175.0*pow(ea2,3.0)/256.0 + 11025.0*pow(ea2,4.0)/16384 + 43659.0*pow(ea2,5.0)/65536.0
			+ 693693.0*pow(ea2,6.0)/1048576.0 + 19324305.0*pow(ea2,7.0)/29360128.0 + 4927697775.0*pow(ea2,8.0)/7516192768.0;

	B	= 3.0*ea2/4.0 + 15.0*pow(ea2,2.0)/16.0 + 525.0*pow(ea2,3.0)/512.0 + 2205.0*pow(ea2,4.0)/2048.0 + 72765.0*pow(ea2,5.0)/65536.0
			+ 297297.0*pow(ea2,6.0)/262144.0 + 135270135.0*pow(ea2,7.0)/117440512.0 + 547521975.0*pow(ea2,8.0)/469762048.0;

	C	= 15.0*pow(ea2,2.0)/64.0 + 105.0*pow(ea2,3.0)/256.0 + 2205.0*pow(ea2,4.0)/4096.0 + 10395.0*pow(ea2,5.0)/16384.0
			+ 1486485.0*pow(ea2,6.0)/2097152.0 + 45090045.0*pow(ea2,7.0)/58720256.0 + 766530765.0*pow(ea2,8.0)/939524096.0;

	D	= 35.0*pow(ea2,3.0)/512.0 + 315.0*pow(ea2,4.0)/2048.0 + 31185.0*pow(ea2,5.0)/131072.0 + 165165.0*pow(ea2,6.0)/524288.0
			+ 45090045.0*pow(ea2,7.0)/117440512.0 + 209053845.0*pow(ea2,8.0)/469762048.0;

	E	= 315.0*pow(ea2,4.0)/16384.0 + 3465.0*pow(ea2,5.0)/65536.0 + 99099.0*pow(ea2,6.0)/1048576.0 + 4099095.0*pow(ea2,7.0)/29360128.0
			+ 348423075.0*pow(ea2,8.0)/1879048192.0;

	F	= 693.0*pow(ea2,5.0)/131072.0 + 9009.0*pow(ea2,6.0)/524288.0 + 4099095.0*pow(ea2,7.0)/117440512.0 + 26801775.0*pow(ea2,8.0)/469762048.0;

	G	= 3003.0*pow(ea2,6.0)/2097152.0 + 315315.0*pow(ea2,7.0)/58720256.0 + 11486475.0*pow(ea2,8.0)/939524096.0;

	H	= 45045.0*pow(ea2,7.0)/117440512.0 + 765765.0*pow(ea2,8.0)/469762048.0;

	I	= 765765.0*pow(ea2,8.0)/7516192768.0;

	tmp1	= Ra*(1.0 - ea2);

	B1	= tmp1 * A;
	B2	= tmp1 * (-B/2.0);
	B3	= tmp1 * (C/4.0);
	B4	= tmp1 * (-D/6.0);
	B5	= tmp1 * (E/8.0);
	B6	= tmp1 * (-F/10.0);
	B7	= tmp1 * (G/12.0);
	B8	= tmp1 * (-H/14.0);
	B9	= tmp1 * (I/16.0);

	arc_len	= B1*Phi + B2*sin(2.0*Phi) + B3*sin(4.0*Phi) + B4*sin(6.0*Phi) + B5*sin(8.0*Phi)
				+ B6*sin(10.0*Phi) + B7*sin(12.0*Phi) + B8*sin(14.0*Phi) + B9*sin(16.0*Phi);

	return(arc_len);
}

//------------------------------------------------------------
/*
	赤道から緯度Phiまでの子午線孤長を得る
	@return 子午線孤長(m)
*/
//------------------------------------------------------------
inline	double	crd_cnv::
M_Arc_Len	(	double	Ra,		// 地球楕円体の長半径（メートル）
				double	ea2,	// 第一離心率の2乗
				double	Phi	)	// 緯度
{
	double	arc_len;

	arc_len	= Ra * (1.0 - ea2)
			* (C1 * Phi
			- C2 / 2.0 * sin(2.0 * Phi)
			+ C3 / 4.0 * sin(4.0 * Phi)
			- C4 / 6.0 * sin(6 * Phi)
			+ C5 / 8 * sin(8.0 * Phi)
			- C6 / 10.0 * sin(10.0 * Phi));

	return(arc_len);
}

//---------------------------------------------------------------------
/**
* 19座標系の北海道の5桁コードと系の対応を初期化
*/
//---------------------------------------------------------------------
bool	crd_cnv::
Sys_Number_Init(	char*	c_fname	)	///< [in]  読み込みファイル名
{
	if( c_fname != NULL ) {
		// ファイルを読み込む場合は最初にクリア
		m_mCode_Sys19.clear();

		const int	LINEMAX	= 512;
		FILE	*rfp	= NULL;
		char	r_buf[LINEMAX];
		int		max_field	= 0;	// 2000.11.30

		// ファイルオープン
		rfp		= fopen(c_fname, "r");
		if(rfp == NULL)
		{
			fprintf(stderr, "ファイルオープンエラー <%s> ！！\n", c_fname);
			return	( false );
		}

		memset(r_buf, '\0', LINEMAX);	// バッファの初期化

		int	a_iCurLine	= 0;
		for(; fgets(r_buf, LINEMAX, rfp) != NULL; a_iCurLine++)
		{
			if( r_buf[0] == '#' || r_buf[0] == '\n' ) {
				continue;
			}

			int	a_ilen	= strlen(r_buf);
			if( r_buf[a_ilen-1] == '\n' ) {
				r_buf[a_ilen-1] = '\0';
			}

			char	*delim		= "\t, ";			// タブ、コンマ、スペース区切り
			char	*field[2]	= { NULL, NULL };	// フィールドの文字列のポインタ

			field[0]	= strtok(r_buf, delim);		// 住所コード
			field[1]	= strtok(NULL, delim);		// 系番号

			// フィールドの読み取りチェック
			int	i;
			for(i = 0; i < 2; i++)
			{
				if(field[i] == NULL) {
					return	( false );
				}
			}

			std::string		a_sCode;	// コード
			int				a_iSys_Num;	// 系番号

			a_sCode		= field[0];
			a_iSys_Num	= atoi ( field[1] );

			m_mCode_Sys19.insert(std::pair<std::string, int>( a_sCode, a_iSys_Num ));	// データ格納
			memset(r_buf, '\0', LINEMAX);	// バッファの初期化
		}
		fclose	( rfp );
		return	( true );
	}else {
		// デフォルト値設定
		// 11系 140度15分0秒0 44度0分0秒0 小樽市　函館市　伊達市　北斗市　胆振支庁管内のうち有珠郡及び虻田郡　檜山支庁管内　後志支庁管内　渡島支庁管内  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01202", 11 ) );	// 北海道	渡島支庁	函館市	ハコダテシ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01203", 11 ) );	// 北海道	後志支庁	小樽市	オタルシ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01233", 11 ) );	// 北海道	胆振支庁	伊達市	ダテシ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01236", 11 ) );	// 北海道	渡島支庁	北斗市	ホクトシ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01331", 11 ) );	// 北海道	渡島支庁	松前町	マツマエチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01332", 11 ) );	// 北海道	渡島支庁	福島町	フクシマチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01333", 11 ) );	// 北海道	渡島支庁	知内町	シリウチチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01334", 11 ) );	// 北海道	渡島支庁	木古内町	キコナイチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01337", 11 ) );	// 北海道	渡島支庁	七飯町	ナナエチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01343", 11 ) );	// 北海道	渡島支庁	鹿部町	シカベチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01345", 11 ) );	// 北海道	渡島支庁	森町	モリマチ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01346", 11 ) );	// 北海道	渡島支庁	八雲町	ヤクモチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01347", 11 ) );	// 北海道	渡島支庁	長万部町	オシャマンベチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01361", 11 ) );	// 北海道	檜山支庁	江差町	エサシチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01362", 11 ) );	// 北海道	檜山支庁	上ノ国町	カミノクニチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01363", 11 ) );	// 北海道	檜山支庁	厚沢部町	アッサブチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01364", 11 ) );	// 北海道	檜山支庁	乙部町	オトベチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01367", 11 ) );	// 北海道	檜山支庁	奥尻町	オクシリチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01370", 11 ) );	// 北海道	檜山支庁	今金町	イマカネチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01371", 11 ) );	// 北海道	檜山支庁	せたな町	セタナチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01391", 11 ) );	// 北海道	後志支庁	島牧村	シママキムラ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01392", 11 ) );	// 北海道	後志支庁	寿都町	スッツチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01393", 11 ) );	// 北海道	後志支庁	黒松内町	クロマツナイチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01394", 11 ) );	// 北海道	後志支庁	蘭越町	ランコシチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01395", 11 ) );	// 北海道	後志支庁	ニセコ町	ニセコチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01396", 11 ) );	// 北海道	後志支庁	真狩村	マッカリムラ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01397", 11 ) );	// 北海道	後志支庁	留寿都村	ルスツムラ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01398", 11 ) );	// 北海道	後志支庁	喜茂別町	キモベツチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01399", 11 ) );	// 北海道	後志支庁	京極町	キョウゴクチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01400", 11 ) );	// 北海道	後志支庁	倶知安町	クッチャンチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01401", 11 ) );	// 北海道	後志支庁	共和町	キョウワチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01402", 11 ) );	// 北海道	後志支庁	岩内町	イワナイチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01403", 11 ) );	// 北海道	後志支庁	泊村	トマリムラ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01404", 11 ) );	// 北海道	後志支庁	神恵内村	カモエナイムラ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01405", 11 ) );	// 北海道	後志支庁	積丹町	シャコタンチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01406", 11 ) );	// 北海道	後志支庁	古平町	フルビラチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01407", 11 ) );	// 北海道	後志支庁	仁木町	ニキチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01408", 11 ) );	// 北海道	後志支庁	余市町	ヨイチチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01409", 11 ) );	// 北海道	後志支庁	赤井川村	アカイガワムラ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01571", 11 ) );	// 北海道	胆振支庁	豊浦町	トヨウラチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01575", 11 ) );	// 北海道	胆振支庁	壮瞥町	ソウベツチョウ	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01584", 11 ) );	// 北海道	胆振支庁	洞爺湖町	トウヤコチョウ	11
		// 12系 142度15分0秒0 44度0分0秒0 札幌市　旭川市　稚内市　留萌市　美唄市　夕張市 岩見沢市　苫小牧市　室蘭市　士別市　名寄市　芦別市　赤平市　三笠市　滝川市　砂川市　江別市 千歳市　歌志内市　深川市　紋別市　富良野市　登別市　恵庭市　北広島市　石狩市　石狩支庁管内 網走支庁管内のうち紋別郡　上川支庁管内　宗谷支庁管内　日高支庁管内　胆振支庁管内（有珠郡及び虻田郡を除く。)　空知支庁管内　留萌支庁管内  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01100", 12 ) );	// 北海道	石狩支庁	札幌市	サッポロシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01101", 12 ) );	// 北海道	石狩支庁	札幌市_中央区	サッポロシチュウオウク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01102", 12 ) );	// 北海道	石狩支庁	札幌市_北区	サッポロシキタク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01103", 12 ) );	// 北海道	石狩支庁	札幌市_東区	サッポロシヒガシク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01104", 12 ) );	// 北海道	石狩支庁	札幌市_白石区	サッポロシシロイシク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01105", 12 ) );	// 北海道	石狩支庁	札幌市_豊平区	サッポロシトヨヒラク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01106", 12 ) );	// 北海道	石狩支庁	札幌市_南区	サッポロシミナミク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01107", 12 ) );	// 北海道	石狩支庁	札幌市_西区	サッポロシニシク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01108", 12 ) );	// 北海道	石狩支庁	札幌市_厚別区	サッポロシアツベツク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01109", 12 ) );	// 北海道	石狩支庁	札幌市_手稲区	サッポロシテイネク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01110", 12 ) );	// 北海道	石狩支庁	札幌市_清田区	サッポロシキヨタク	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01204", 12 ) );	// 北海道	上川支庁	旭川市	アサヒカワシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01205", 12 ) );	// 北海道	胆振支庁	室蘭市	ムロランシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01209", 12 ) );	// 北海道	空知支庁	夕張市	ユウバリシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01210", 12 ) );	// 北海道	空知支庁	岩見沢市	イワミザワシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01212", 12 ) );	// 北海道	留萌支庁	留萌市	ルモイシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01213", 12 ) );	// 北海道	胆振支庁	苫小牧市	トマコマイシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01214", 12 ) );	// 北海道	宗谷支庁	稚内市	ワッカナイシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01215", 12 ) );	// 北海道	空知支庁	美唄市	ビバイシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01216", 12 ) );	// 北海道	空知支庁	芦別市	アシベツシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01217", 12 ) );	// 北海道	石狩支庁	江別市	エベツシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01218", 12 ) );	// 北海道	空知支庁	赤平市	アカビラシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01219", 12 ) );	// 北海道	網走支庁	紋別市	モンベツシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01220", 12 ) );	// 北海道	上川支庁	士別市	シベツシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01221", 12 ) );	// 北海道	上川支庁	名寄市	ナヨロシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01222", 12 ) );	// 北海道	空知支庁	三笠市	ミカサシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01224", 12 ) );	// 北海道	石狩支庁	千歳市	チトセシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01225", 12 ) );	// 北海道	空知支庁	滝川市	タキカワシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01226", 12 ) );	// 北海道	空知支庁	砂川市	スナガワシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01227", 12 ) );	// 北海道	空知支庁	歌志内市	ウタシナイシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01228", 12 ) );	// 北海道	空知支庁	深川市	フカガワシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01229", 12 ) );	// 北海道	上川支庁	富良野市	フラノシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01230", 12 ) );	// 北海道	胆振支庁	登別市	ノボリベツシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01231", 12 ) );	// 北海道	石狩支庁	恵庭市	エニワシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01234", 12 ) );	// 北海道	石狩支庁	北広島市	キタヒロシマシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01235", 12 ) );	// 北海道	石狩支庁	石狩市	イシカリシ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01303", 12 ) );	// 北海道	石狩支庁	当別町	トウベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01304", 12 ) );	// 北海道	石狩支庁	新篠津村	シンシノツムラ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01423", 12 ) );	// 北海道	空知支庁	南幌町	ナンポロチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01424", 12 ) );	// 北海道	空知支庁	奈井江町	ナイエチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01425", 12 ) );	// 北海道	空知支庁	上砂川町	カミスナガワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01427", 12 ) );	// 北海道	空知支庁	由仁町	ユニチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01428", 12 ) );	// 北海道	空知支庁	長沼町	ナガヌマチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01429", 12 ) );	// 北海道	空知支庁	栗山町	クリヤマチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01430", 12 ) );	// 北海道	空知支庁	月形町	ツキガタチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01431", 12 ) );	// 北海道	空知支庁	浦臼町	ウラウスチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01432", 12 ) );	// 北海道	空知支庁	新十津川町	シントツカワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01433", 12 ) );	// 北海道	空知支庁	妹背牛町	モセウシチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01434", 12 ) );	// 北海道	空知支庁	秩父別町	チップベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01436", 12 ) );	// 北海道	空知支庁	雨竜町	ウリュウチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01437", 12 ) );	// 北海道	空知支庁	北竜町	ホクリュウチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01438", 12 ) );	// 北海道	空知支庁	沼田町	ヌマタチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01452", 12 ) );	// 北海道	上川支庁	鷹栖町	タカスチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01453", 12 ) );	// 北海道	上川支庁	東神楽町	ヒガシカグラチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01454", 12 ) );	// 北海道	上川支庁	当麻町	トウマチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01455", 12 ) );	// 北海道	上川支庁	比布町	ピップチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01456", 12 ) );	// 北海道	上川支庁	愛別町	アイベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01457", 12 ) );	// 北海道	上川支庁	上川町	カミカワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01458", 12 ) );	// 北海道	上川支庁	東川町	ヒガシカワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01459", 12 ) );	// 北海道	上川支庁	美瑛町	ビエイチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01460", 12 ) );	// 北海道	上川支庁	上富良野町	カミフラノチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01461", 12 ) );	// 北海道	上川支庁	中富良野町	ナカフラノチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01462", 12 ) );	// 北海道	上川支庁	南富良野町	ミナミフラノチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01463", 12 ) );	// 北海道	上川支庁	占冠村	シムカップムラ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01464", 12 ) );	// 北海道	上川支庁	和寒町	ワッサムチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01465", 12 ) );	// 北海道	上川支庁	剣淵町	ケンブチチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01468", 12 ) );	// 北海道	上川支庁	下川町	シモカワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01469", 12 ) );	// 北海道	上川支庁	美深町	ビフカチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01470", 12 ) );	// 北海道	上川支庁	音威子府村	オトイネップムラ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01471", 12 ) );	// 北海道	上川支庁	中川町	ナカガワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01472", 12 ) );	// 北海道	上川支庁	幌加内町	ホロカナイチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01481", 12 ) );	// 北海道	留萌支庁	増毛町	マシケチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01482", 12 ) );	// 北海道	留萌支庁	小平町	オビラチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01483", 12 ) );	// 北海道	留萌支庁	苫前町	トママエチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01484", 12 ) );	// 北海道	留萌支庁	羽幌町	ハボロチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01485", 12 ) );	// 北海道	留萌支庁	初山別村	ショサンベツムラ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01486", 12 ) );	// 北海道	留萌支庁	遠別町	エンベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01487", 12 ) );	// 北海道	留萌支庁	天塩町	テシオチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01511", 12 ) );	// 北海道	宗谷支庁	猿払村	サルフツムラ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01512", 12 ) );	// 北海道	宗谷支庁	浜頓別町	ハマトンベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01513", 12 ) );	// 北海道	宗谷支庁	中頓別町	ナカトンベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01514", 12 ) );	// 北海道	宗谷支庁	枝幸町	エサシチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01516", 12 ) );	// 北海道	宗谷支庁	豊富町	トヨトミチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01517", 12 ) );	// 北海道	宗谷支庁	礼文町	レブンチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01518", 12 ) );	// 北海道	宗谷支庁	利尻町	リシリチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01519", 12 ) );	// 北海道	宗谷支庁	利尻富士町	リシリフジチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01520", 12 ) );	// 北海道	宗谷支庁	幌延町	ホロノベチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01555", 12 ) );	// 北海道	網走支庁	遠軽町	エンガルチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01558", 12 ) );	// 北海道	網走支庁	上湧別町	カミユウベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01559", 12 ) );	// 北海道	網走支庁	湧別町	ユウベツチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01560", 12 ) );	// 北海道	網走支庁	滝上町	タキノウエチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01561", 12 ) );	// 北海道	網走支庁	興部町	オコッペチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01562", 12 ) );	// 北海道	網走支庁	西興部村	ニシオコッペムラ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01563", 12 ) );	// 北海道	網走支庁	雄武町	オウムチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01578", 12 ) );	// 北海道	胆振支庁	白老町	シラオイチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01581", 12 ) );	// 北海道	胆振支庁	厚真町	アツマチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01585", 12 ) );	// 北海道	胆振支庁	安平町	アビラチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01586", 12 ) );	// 北海道	胆振支庁	むかわ町	ムカワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01601", 12 ) );	// 北海道	日高支庁	日高町	ヒダカチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01602", 12 ) );	// 北海道	日高支庁	平取町	ビラトリチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01604", 12 ) );	// 北海道	日高支庁	新冠町	ニイカップチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01607", 12 ) );	// 北海道	日高支庁	浦河町	ウラカワチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01608", 12 ) );	// 北海道	日高支庁	様似町	サマニチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01609", 12 ) );	// 北海道	日高支庁	えりも町	エリモチョウ	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01610", 12 ) );	// 北海道	日高支庁	新ひだか町	シンヒダカチョウ	12
		// 13系 144度15分0秒0 44度0分0秒0 北見市　帯広市　釧路市　網走市　根室市　根室支庁管内　釧路支庁管内　網走支庁管内（紋別郡を除く。)　十勝支庁管内  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01206", 13 ) );	// 北海道	釧路支庁	釧路市	クシロシ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01207", 13 ) );	// 北海道	十勝支庁	帯広市	オビヒロシ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01208", 13 ) );	// 北海道	網走支庁	北見市	キタミシ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01211", 13 ) );	// 北海道	網走支庁	網走市	アバシリシ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01223", 13 ) );	// 北海道	根室支庁	根室市	ネムロシ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01543", 13 ) );	// 北海道	網走支庁	美幌町	ビホロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01544", 13 ) );	// 北海道	網走支庁	津別町	ツベツチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01545", 13 ) );	// 北海道	網走支庁	斜里町	シャリチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01546", 13 ) );	// 北海道	網走支庁	清里町	キヨサトチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01547", 13 ) );	// 北海道	網走支庁	小清水町	コシミズチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01549", 13 ) );	// 北海道	網走支庁	訓子府町	クンネップチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01550", 13 ) );	// 北海道	網走支庁	置戸町	オケトチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01552", 13 ) );	// 北海道	網走支庁	佐呂間町	サロマチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01564", 13 ) );	// 北海道	網走支庁	大空町	オオゾラチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01631", 13 ) );	// 北海道	十勝支庁	音更町	オトフケチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01632", 13 ) );	// 北海道	十勝支庁	士幌町	シホロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01633", 13 ) );	// 北海道	十勝支庁	上士幌町	カミシホロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01634", 13 ) );	// 北海道	十勝支庁	鹿追町	シカオイチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01635", 13 ) );	// 北海道	十勝支庁	新得町	シントクチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01636", 13 ) );	// 北海道	十勝支庁	清水町	シミズチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01637", 13 ) );	// 北海道	十勝支庁	芽室町	メムロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01638", 13 ) );	// 北海道	十勝支庁	中札内村	ナカサツナイムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01639", 13 ) );	// 北海道	十勝支庁	更別村	サラベツムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01641", 13 ) );	// 北海道	十勝支庁	大樹町	タイキチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01642", 13 ) );	// 北海道	十勝支庁	広尾町	ヒロオチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01643", 13 ) );	// 北海道	十勝支庁	幕別町	マクベツチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01644", 13 ) );	// 北海道	十勝支庁	池田町	イケダチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01645", 13 ) );	// 北海道	十勝支庁	豊頃町	トヨコロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01646", 13 ) );	// 北海道	十勝支庁	本別町	ホンベツチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01647", 13 ) );	// 北海道	十勝支庁	足寄町	アショロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01648", 13 ) );	// 北海道	十勝支庁	陸別町	リクベツチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01649", 13 ) );	// 北海道	十勝支庁	浦幌町	ウラホロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01661", 13 ) );	// 北海道	釧路支庁	釧路町	クシロチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01662", 13 ) );	// 北海道	釧路支庁	厚岸町	アッケシチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01663", 13 ) );	// 北海道	釧路支庁	浜中町	ハマナカチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01664", 13 ) );	// 北海道	釧路支庁	標茶町	シベチャチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01665", 13 ) );	// 北海道	釧路支庁	弟子屈町	テシカガチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01667", 13 ) );	// 北海道	釧路支庁	鶴居村	ツルイムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01668", 13 ) );	// 北海道	釧路支庁	白糠町	シラヌカチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01691", 13 ) );	// 北海道	根室支庁	別海町	ベッカイチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01692", 13 ) );	// 北海道	根室支庁	中標津町	ナカシベツチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01693", 13 ) );	// 北海道	根室支庁	標津町	シベツチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01694", 13 ) );	// 北海道	根室支庁	羅臼町	ラウスチョウ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01695", 13 ) );	// 北海道	根室支庁	色丹村	シコタンムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01696", 13 ) );	// 北海道	根室支庁	泊村	トマリムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01697", 13 ) );	// 北海道	根室支庁	留夜別村	ルヨベツムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01698", 13 ) );	// 北海道	根室支庁	留別村	ルベツムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01699", 13 ) );	// 北海道	根室支庁	紗那村	シャナムラ	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01700", 13 ) );	// 北海道	根室支庁	蘂取村	シベトロムラ	13
	}
	return	( false );
}

//------------------------------------------------------------
/*
	経緯度から一番近いところの19座標系の系を得る
	（なんか無駄なことをしているっぽい）
	@return		系番号
*/
//------------------------------------------------------------
int	crd_cnv::
Sys_Number	(	double		lon,	// 経度
				double		lat)	// 緯度
{
	double	d_lat,	d_lon;
	double	dist			= 9999999.0;	// 現在調べている距離
	double	dist_n1			= 9999999.0;	// 最も近かった距離
	double	dist_n2			= 9999999.0;	// ２番目に近かった距離
	int		sys_n1			= 0;
	int		sys_n2			= 0;
	int		i;

//	Make19tbl();	// 19座標系のテーブルを作成
		
	// degreeをradianに変換
	lat	= lat * M_PI / 180.0;
	lon	= lon * M_PI / 180.0;

	for(i = 0; i < 19; i++)
	{
		d_lat	= Rad_Origin[i].lat - lat;
		d_lon	= Rad_Origin[i].lon - lon;
		dist	= sqrt(pow(d_lat, 2.0) + pow(d_lon, 2.0));
		if(dist < dist_n1)
		{
			sys_n1		= i + 1;
			dist_n1		= dist;
		}
		else
		if(dist < dist_n2)
		{
			sys_n2		= i + 1;
			dist_n2		= dist;
		}
	}
	return(sys_n1);
}

//------------------------------------------------------------
/*
	経緯度と市区町村コード(5桁)から19座標系の系を得る
	@return     系番号(市区町村コードや経緯度に間違いがある場合は0を返却)
*/
//------------------------------------------------------------
int	crd_cnv::
Sys_Number	(	double	lon,	// [in]  経度
				double	lat,	// [in]  緯度
				char*	code5 )	// [in]  市区町村コード(5桁)
{
	// テーブルが空だった場合、一度デフォルトの初期化をする。
	// （既に外部で初期化実施している場合、ここは通らない）
	if( m_mCode_Sys19.size() == 0 ) {
		Sys_Number_Init();
	}

	if( code5 == NULL ) {
		return ( 0 );
	}else
	if( strlen(code5) < 5 ) {
		return ( 0 ); 
	}
	// 1系 129度30分0秒0 33度0分0秒0 長崎県 鹿児島県のうち北方北緯32度南方北緯27度西方東経128度18分東方東経130度を境界線とする区域内（奄美群島は東経130度13分までを含む。)にあるすべての島、小島、環礁及び岩礁  
	if( strncmp( code5, "42", 2 ) == 0 ) {	// 長崎県
		return	( 1 );
	}else
	if( strncmp( code5, "46", 2 ) == 0 ){	// 鹿児島県
		// 北方北緯32度 南方北緯27度 西方東経128度18分 東方東経130度
		if(	27.0 <= lat && lat <= 32.0 && 
			128.30 <= lon && lon <= 130.0 ) {
			return	( 1 );
		}else {
			// 2系 131度 0分0秒0 33度0分0秒0 福岡県　佐賀県　熊本県　大分県　宮崎県　鹿児島県（第I系に規定する区域を除く。)  
			return	( 2 );
		}
	}else
	// 2系 131度 0分0秒0 33度0分0秒0 福岡県　佐賀県　熊本県　大分県　宮崎県　鹿児島県（第I系に規定する区域を除く。)  
	if( strncmp( code5, "40", 2 ) == 0 ||	// 福岡県
		strncmp( code5, "41", 2 ) == 0 ||	// 佐賀県
		strncmp( code5, "43", 2 ) == 0 ||	// 熊本県
		strncmp( code5, "44", 2 ) == 0 ||	// 大分県
		strncmp( code5, "45", 2 ) == 0 ) {	// 宮崎県
		return	( 2 );
	}else
	// 3系 132度10分0秒0 36度0分0秒0 山口県　島根県　広島県  
	if( strncmp( code5, "32", 2 ) == 0 ||	// 島根県
		strncmp( code5, "34", 2 ) == 0 ||	// 広島県
		strncmp( code5, "35", 2 ) == 0 ) {	// 山口県
		return	( 3 );
	}else
	// 4系 133度30分0秒0 33度0分0秒0 香川県　愛媛県　徳島県　高知県  
	if( strncmp( code5, "36", 2 ) == 0 ||	// 徳島県
		strncmp( code5, "37", 2 ) == 0 ||	// 香川県
		strncmp( code5, "38", 2 ) == 0 ||	// 愛媛県
		strncmp( code5, "39", 2 ) == 0 ) {	// 高知県
		return	( 4 );
	}else
	// 5系 134度20分0秒0 36度0分0秒0 兵庫県　鳥取県　岡山県  
	if( strncmp( code5, "28", 2 ) == 0 ||	// 兵庫県
		strncmp( code5, "31", 2 ) == 0 ||	// 鳥取県
		strncmp( code5, "33", 2 ) == 0 ) {	// 岡山県
		return	( 5 );
	}else
	// 6系 136度 0分0秒0 36度0分0秒0 京都府　大阪府　福井県　滋賀県　三重県　奈良県 和歌山県 
	if( strncmp( code5, "18", 2 ) == 0 ||	// 福井県
		strncmp( code5, "24", 2 ) == 0 ||	// 三重県
		strncmp( code5, "25", 2 ) == 0 ||	// 滋賀県
		strncmp( code5, "26", 2 ) == 0 ||	// 京都府
		strncmp( code5, "27", 2 ) == 0 ||	// 大阪府
		strncmp( code5, "29", 2 ) == 0 ||	// 奈良県
		strncmp( code5, "30", 2 ) == 0 ) {	// 和歌山県
		return	( 6 );
	}else
	// 7系 137度10分0秒0 36度0分0秒0 石川県　富山県　岐阜県　愛知県  
	if( strncmp( code5, "16", 2 ) == 0 ||	// 富山県	7
		strncmp( code5, "17", 2 ) == 0 ||	// 石川県	7
		strncmp( code5, "21", 2 ) == 0 ||	// 岐阜県	7
		strncmp( code5, "23", 2 ) == 0 ) {	// 愛知県	7
		return	( 7 );
	}else
	// 8系 138度30分0秒0 36度0分0秒0 新潟県　長野県　山梨県　静岡県  
	if( strncmp( code5, "15", 2 ) == 0 ||	// 新潟県	8
		strncmp( code5, "19", 2 ) == 0 ||	// 山梨県	8
		strncmp( code5, "20", 2 ) == 0 ||	// 長野県	8
		strncmp( code5, "22", 2 ) == 0 ) {	// 静岡県	8
		return	( 8 );
	}else
	// 9系 139度50分0秒0 36度0分0秒0 東京都（14系、18系及び19系に規定する区域を除く。)　福島県　栃木県　茨城県　埼玉県 千葉県 群馬県　神奈川県  
	if( strncmp( code5, "13", 2 ) == 0 ) {	// 東京都
		if(	lat <= 28.0 ) {		//  北緯28度から南
			// 14系 142度 0分0秒0 26度0分0秒0 東京都のうち北緯28度から南であり、かつ東経140度30分から東であり東経143度から西である区域  
			if( 140.50 <= lon && lon <= 143.0 ) {	// 東経140度30分から東であり東経143度から西
				return	( 14 );
			}else
			// 18系 136度 0分0秒0 20度0分0秒0 東京都のうち北緯28度から南であり、かつ東経140度30分から西である区域
			if( lon < 140.50 ) {	// 東経140度30分から西
				return	( 18 );
			}else
			// 19系 154度 0分0秒0 26度0分0秒0 東京都のうち北緯28度から南であり、かつ東経143度から東である区域
			if( lon > 143.0 ) {		// 東経143度から東
				return	( 19 );
			}
		}
		return	( 9 );
	}else
	// 9系 139度50分0秒0 36度0分0秒0 東京都（14系、18系及び19系に規定する区域を除く。)　福島県　栃木県　茨城県　埼玉県 千葉県 群馬県　神奈川県  
	if( strncmp( code5, "07", 2 ) == 0 ||	// 福島県	9
		strncmp( code5, "08", 2 ) == 0 ||	// 茨城県	9
		strncmp( code5, "09", 2 ) == 0 ||	// 栃木県	9
		strncmp( code5, "10", 2 ) == 0 ||	// 群馬県	9
		strncmp( code5, "11", 2 ) == 0 ||	// 埼玉県	9
		strncmp( code5, "12", 2 ) == 0 ||	// 千葉県	9
		strncmp( code5, "14", 2 ) == 0 ) {	// 神奈川県	9
		return	( 9 );
	}else
	// 10系 140度50分0秒0 40度0分0秒0 青森県　秋田県　山形県　岩手県　宮城県  
	if( strncmp( code5, "02", 2 ) == 0 ||	// 青森県	10
		strncmp( code5, "03", 2 ) == 0 ||	// 岩手県	10
		strncmp( code5, "04", 2 ) == 0 ||	// 宮城県	10
		strncmp( code5, "05", 2 ) == 0 ||	// 秋田県	10
		strncmp( code5, "06", 2 ) == 0 ) {	// 山形県	10
		return	( 10 );
	}else
	// 11系 140度15分0秒0 44度0分0秒0 小樽市　函館市　伊達市　北斗市　胆振支庁管内のうち有珠郡及び虻田郡　檜山支庁管内　後志支庁管内　渡島支庁管内  
	// 12系 142度15分0秒0 44度0分0秒0 札幌市　旭川市　稚内市　留萌市　美唄市　夕張市 岩見沢市　苫小牧市　室蘭市　士別市　名寄市　芦別市　赤平市　三笠市　滝川市　砂川市　江別市 千歳市　歌志内市　深川市　紋別市　富良野市　登別市　恵庭市　北広島市　石狩市　石狩支庁管内 網走支庁管内のうち紋別郡　上川支庁管内　宗谷支庁管内　日高支庁管内　胆振支庁管内（有珠郡及び虻田郡を除く。)　空知支庁管内　留萌支庁管内  
	// 13系 144度15分0秒0 44度0分0秒0 北見市　帯広市　釧路市　網走市　根室市　根室支庁管内　釧路支庁管内　網走支庁管内（紋別郡を除く。)　十勝支庁管内  
	if(	strncmp( code5, "01", 2 ) == 0 ) {
		std::map<std::string, int>::iterator	a_iIndx;
		a_iIndx	= m_mCode_Sys19.find ( code5 );
		if( a_iIndx != m_mCode_Sys19.end() ) {
			return	( (*a_iIndx).second );
		}else {
			return	( -1 );
		}
	}else
	if( strncmp( code5, "47", 2 ) == 0 ) {		// 沖縄県
		// 15系 127度30分0秒0 26度0分0秒0 沖縄県のうち東経126度から東であり、かつ東経130度から西である区域
		if( 126.0 <= lon && lon <= 130.0 ) {	// 東経126度から東であり、かつ東経130度から西
			return	( 15 );
		}else
		// 16系 124度 0分0秒0 26度0分0秒0 沖縄県のうち東経126度から西である区域
		if( lon < 126.0 ) {
			return	( 16 );
		}else
		// 17系 131度 0分0秒0 26度0分0秒0 沖縄県のうち東経130度から東である区域
		if( lon > 130.0 ) {
			return	( 17 );
		}
	}
	// エラー
	return	( -1 );
}

//------------------------------------------------------------
/*
	メッシュコードから該当する１９座標系の数と番号を得る \n
	該当する系がない場合、数、番号ともに0を返す
	@return 構造体 sys_num へのポインタ
*/
//------------------------------------------------------------
sys_num		*crd_cnv::
Sys_Number	(	int		meshcode	)		// メッシュコード
{
	static	sys_num		sys;

	switch(meshcode){
		case	392725:		//15	那覇市
			sys.total	= 1;
			sys.sys[0]	= 15;
			return(&sys);
		case	473024:		//2		鹿児島市
		case	473034:		//2		鹿児島市
		case	473163:		//2		宮崎市
		case	473173:		//2		宮崎市
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	492906:		//1		長崎市
		case	492907:		//1		長崎市
		case	492916:		//1		長崎市
		case	492917:		//1		長崎市
		case	492955:		//1		佐世保市
		case	492965:		//1		佐世保市
			sys.total	= 1;
			sys.sys[0]	= 1;
			return(&sys);
		case	493015:		//2		熊本市
		case	493062:		//2		佐賀市
		case	493072:		//2		佐賀市
		case	493074:		//2		久留米市
		case	493164:		//2		八代市	大分市
		case	493165:		//2		大分市
		case	493173:		//2		別府市
		case	493174:		//2		別府市	大分市
		case	503022:		//2		那珂川町
		case	503023:		//2		春日市	大野城市	那珂川町	志免町	須恵町
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	503032:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	503033:		//2		志免町	須恵町	粕屋町
		case	503065:		//2		水巻町
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	503066:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	503067:		//3		下関市
		case	503077:		//3		下関市
		case	503171:		//3		宇部市
		case	503172:		//3		宇部市
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	503255:		//4		松山市
		case	503256:		//4		松山市
		case	503265:		//4		松山市
		case	503266:		//4		松山市
		case	503324:		//4		高知市
		case	503372:		//4		新居浜市
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513104:		//3		山口市	防府市
		case	513106:		//3		徳山市
		case	513113:		//3		宇部市	山口市	防府市
		case	513123:		//3		山口市
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513207:		//4		今治市
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513211:		//3		岩国市
		case	513221:		//3		岩国市	和木町	大竹市
		case	513224:		//3		呉市
		case	513234:		//3		呉市	坂町
		case	513242:		//3		廿日市市
		case	513243:		//3		府中町	坂町
		case	513244:		//3		府中町	海田町	坂町
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513253:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	513254:		//3		東広島市	府中町
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513300:		//4		今治市
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513341:		//3		尾道市	福山市
		case	513352:		//3		福山市
		case	513353:		//3		尾道市	福山市
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513365:		//5		倉敷市
		case	513366:		//5		岡山市	倉敷市
		case	513375:		//5		倉敷市
		case	513376:		//5		岡山市	倉敷市
		case	513377:		//5		岡山市
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513404:		//4		徳島市
		case	513430:		//4		高松市
		case	513440:		//4		高松市
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513477:		//5		明石市
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513521:		//6		和歌山市
		case	513541:		//6		泉南市	阪南市	田尻町				
		case	513542:		//6		泉南市	阪南市	田尻町	熊取町	貝塚市	泉佐野市	
		case	513551:		//6		泉南市	阪南市	田尻町	貝塚市	泉佐野市		
		case	513552:		//6		忠岡町	田尻町	岸和田市	貝塚市	泉佐野市		
		case	513553:		//6		忠岡町	堺市	岸和田市	貝塚市	和泉市		
		case	513554:		//6		堺市	富田林市	河内長野市	和泉市	大阪狭山市		
		case	513562:		//6		堺市	岸和田市	泉大津市	貝塚市	高石市	忠岡町	
		case	513563:		//6		堺市	泉大津市	和泉市	高石市	忠岡町		
		case	513564:		//6		堺市 八尾市 寝屋川市 松原市 柏原市 羽曳野市 藤井寺市 大阪狭山市 美原町
		case	513565:		//6		富田林市 柏原市 羽曳野市 藤井寺市 大和高田市 香芝市 新庄町 當麻町 上牧町 王寺町 広陵町 河合町
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	513570:		//5		明石市						
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513571:		//6		忠岡町						
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	513573:		//5	6	尼崎市(5)	堺市(6)					
			sys.total	= 2;
			sys.sys[0]	= 5;
			sys.sys[1]	= 6;
			return(&sys);
		case	513574:		//6		堺市	八尾市	松原市	柏原市	藤井寺市	東大阪市	
		case	513575:		//6		八尾市	柏原市	東大阪市	奈良市	大和郡山市	生駒市	斑鳩町	安堵町	王寺町	河合町
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523307:		//5		岡山市						
		case	523406:		//5		姫路市	明石市	加古川市	高砂市	播磨町
		case	523407:		//5		明石市	加古川市	稲美町	播磨町
		case	523415:		//5		姫路市	高砂市		
		case	523416:		//5		高砂市	姫路市	加古川市	
		case	523417:		//5		加古川市	稲美町		
		case	523425:		//5		姫路市			
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523501:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523502:		//5		尼崎市	西宮市	芦屋市	
		case	523503:		//5		尼崎市	西宮市	明石市	
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523504:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523505:		//5		姫路市			
		case	523506:		//5		姫路市			
		case	523512:		//5		西宮市	芦屋市	伊丹市	宝塚市
		case	523513:		//5		尼崎市	明石市	西宮市	伊丹市	宝塚市	川西市
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523514:		//6		豊中市	吹田市	高槻市	守口市	枚方市	茨木市	寝屋川市	箕面市	摂津市
		case	523515:		//6		高槻市	枚方市	寝屋川市	交野市	生駒市	香芝市
		case	523523:		//6		池田市	茨木市	箕面市	宝塚市	川西市
		case	523524:		//6		高槻市	箕面市	高槻市
		case	523525:		//6		高槻市	枚方市	島本町	長岡京市	八幡市	大山崎町	久御山町
		case	523526:		//6		宇治市	城陽市	八幡市	久御山町	大津市
		case	523535:		//6		向日市	長岡京市	高槻市	島本町
		case	523536:		//6		宇治市	大津市
		case	523537:		//6		宇治市	大津市	栗東町
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523545:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523546:		//6		大津市	
		case	523547:		//6		大津市	守山市	栗東町	野洲町
		case	523603:		//6		津市	
		case	523604:		//6		津市	
		case	523634:		//6		四日市市	鈴鹿市
		case	523635:		//6		四日市市	桑名市	長島町
		case	523640:		//6		守山市	栗東町	野洲町
		case	523645:		//6		四日市市	長島町
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523656:		//7		七宝町	大治町					
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523657:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523666:		//7		一宮市 稲沢市 西枇杷島町 師勝町 西春町 春日町 清洲町 新川町 七宝町 美和町 甚目寺町 大治町
		case	523667:		//7		春日井市 西枇杷島町 豊山町 師勝町 西春町		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523671:		//6		彦根市						
		case	523672:		//6		彦根市						
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523676:		//7		岡崎市	尾西市	稲沢市	岩倉市	師勝町	西春町	木曽川町	羽島市
		case	523677:		//7		一宮市	春日井市	小牧市	岩倉市	豊山町	師勝町	西春町
		case	523702:		//7		豊橋市						
		case	523703:		//7		豊橋市						
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523705:		//8		浜松市						
		case	523706:		//8		浜松市						
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	523712:		//7		豊橋市						
		case	523713:		//7		豊橋市						
		case	523731:		//7		岡崎市	豊田市	安城市				
		case	523741:		//7		岡崎市	豊田市					
		case	523750:		//7		豊田市	豊明市	三好町
		case	523751:		//7		豊田市	三好町
		case	523760:		//7		瀬戸市	春日井市	豊田市	尾張旭市
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523832:		//8		静岡市
		case	523833:		//8		静岡市	清水市
		case	523837:		//8		沼津市	修善寺町
		case	523843:		//8		静岡市	清水市
		case	523846:		//8		沼津市
		case	523847:		//8		沼津市	三島市	伊豆長岡町	修善寺町	清水町
		case	523855:		//8		沼津市	富士市
		case	523856:		//8		沼津市	富士市
		case	523857:		//8		沼津市	三島市	清水町
		case	523930:		//8		伊東市	修善寺町
		case	523940:		//8		熱海市	伊東市	修善寺町
		case	523950:		//8		熱海市	三島市
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	523954:		//9		三浦市
		case	523955:		//9		横須賀市	三浦市	富津市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	523960:		//9	8	小田原市(9)	箱根町(9)	湯河原町(9)	三島市(8)
			sys.total	= 2;
			sys.sys[0]	= 8;
			sys.sys[0]	= 9;
			return(&sys);
		case	523961:		//9		小田原市	二宮町	湯河原町
		case	523964:		//9		横須賀市	三浦市	葉山町
		case	523965:		//9		横須賀市	三浦市	富津市
		case	523966:		//9		富津市	横須賀市
		case	523971:		//9		平塚市	小田原市	南足柄市	二宮町	中井町	開成町
		case	523972:		//9		平塚市	茅ヶ崎市	大磯町	二宮町	中井町
		case	523973:		//9		平塚市	鎌倉市	藤沢市	茅ヶ崎市	逗子市	葉山町
		case	523974:		//9		横須賀市	鎌倉市	藤沢市	逗子市	葉山町
		case	523975:		//9		横須賀市	逗子市	葉山町	富津市
		case	523976:		//9		君津市	富津市	横須賀市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533310:		//3		松江市
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	533411:		//5		鳥取市
		case	533421:		//5		鳥取市
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	533605:		//7		岐阜市	羽島市	笠松町	柳津町	巣南町
		case	533606:		//7		岐阜市	羽島市	岐南町	笠松町	柳津町	一宮市	木曽川町
		case	533616:		//7		岐阜市		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	533834:		//8		甲府市		
		case	533844:		//8		甲府市		
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	533902:		//9		平塚市	茅ヶ崎市	秦野市	厚木市	伊勢原市	海老名市	寒川町	中井町
		case	533903:		//9		平塚市	鎌倉市	藤沢市	茅ヶ崎市	厚木市	海老名市	綾瀬市	寒川町
		case	533904:		//9		鎌倉市	藤沢市	
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533905:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533912:		//9		厚木市	伊勢原市	海老名市	愛川町
		case	533913:		//9		藤沢市	相模原市	厚木市	大和市	海老名市	座間市	綾瀬市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533914:		//		該当なし
		case	533915:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533916:		//9		木更津市	君津市	
		case	533922:		//9		相模原市	厚木市	愛川町
		case	533923:		//9		相模原市	厚木市	大和市	座間市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533924:		//		該当なし
		case	533925:		//		該当なし
		case	533926:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533932:		//9		相模原市
		case	533933:		//9		相模原市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533934:		//		該当なし
		case	533935:		//		該当なし
		case	533936:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533937:		//9		市川市	船橋市	習志野市	浦安市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533942:		//		該当なし
		case	533943:		//		該当なし
		case	533944:		//		該当なし
		case	533945:		//		該当なし
		case	533946:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533947:		//9		市川市	船橋市	松戸市	鎌ヶ谷市	浦安市
		case	533952:		//9		飯能市	入間市
		case	533953:		//9		所沢市	狭山市	入間市	三芳町
		case	533954:		//9		浦和市	所沢市	戸田市	朝霞市	志木市	和光市	新座市	富士見市	三芳町
		case	533955:		//9		川口市	浦和市	蕨市	戸田市	鳩ヶ谷市	朝霞市	和光市
		case	533956:		//9		川口市	草加市	鳩ヶ谷市	八潮市	三郷市
		case	533957:		//9		三郷市	市川市	船橋市	松戸市	柏市	流山市	鎌ヶ谷市	沼南町
		case	533962:		//9		飯能市	狭山市	入間市	坂戸市	日高市
		case	533963:		//9		川越市	所沢市	飯能市	狭山市	入間市	鶴ケ島市	日高市	大井町	三芳町
		case	533964:		//9		川越市	浦和市	大宮市	与野市	志木市	富士見市	上福岡市	大井町	三芳町
		case	533965:		//9		川口市	浦和市	大宮市	岩槻市	与野市	越谷市	鳩ヶ谷市
		case	533966:		//9		川口市	岩槻市	草加市	越谷市	八潮市	三郷市	吉川市	松伏町	野田市
		case	533967:		//9		三郷市	吉川市	松戸市	野田市	柏市	流山市	我孫子市	沼南町	取手市	守谷町
		case	533973:		//9		川越市	東松山市	坂戸市	鶴ケ島市	日高市
		case	533974:		//9		川越市	大宮市	上尾市	桶川市	北本市	伊奈町	川島町
		case	533975:		//9		大宮市	岩槻市	春日部市	上尾市	越谷市	蓮田市	伊奈町	宮代町	白岡町	杉戸町
		case	533976:		//9		岩槻市	春日部市	越谷市	吉川市	杉戸町	松伏町	庄和町	野田市
		case	533977:		//9		野田市	柏市	水海道市	岩井市	谷和原村	守谷町
		case	534010:		//9		市原市	袖ヶ浦市			
		case	534020:		//9		市原市	袖ヶ浦市	
		case	534021:		//9		市原市		
		case	534022:		//9		東金市	市原市	八街市	大網白里町
		case	534030:		//9		船橋市	習志野市	
		case	534031:		//9		佐倉市	四街道市	
		case	534040:		//9		船橋市	習志野市	八千代市	鎌ヶ谷市
		case	534041:		//9		佐倉市	八千代市	四街道市	酒々井町	印旛村
		case	534050:		//9		船橋市	松戸市	柏市	八千代市	鎌ヶ谷市	印西市	沼南町	白井町
		case	534060:		//9		柏市	我孫子市	印西市	沼南町	取手市	守谷町	藤代町
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543601:		//6		福井市		
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	543665:		//7		金沢市		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	543727:		//8		松本市		
		case	543737:		//8		松本市		
		case	543800:		//8		岡谷市	諏訪市	下諏訪町
		case	543801:		//8		諏訪市	下諏訪町	
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	543837:		//9		高崎市		
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543845:		//8		軽井沢町		
		case	543871:		//8		長野市		
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	543924:		//9		館林市	板倉町	明和村	邑楽町	行田市	加須市	羽生市
		case	543930:		//9		前橋市	高崎市	藤岡市	新町	玉村町	上里町
		case	543931:		//9		前橋市	伊勢崎市	新町	境町	玉村町	本庄市	上里町
		case	543932:		//9		妻沼町	太田市	境町	尾島町	新田町	薮塚本町
		case	543933:		//9		足利市	太田市	館林市	大泉町	邑楽町
		case	543934:		//9		館林市	板倉町	邑楽町	足利市	佐野市	藤岡町	岩舟町	田沼町
		case	543936:		//9		結城市	小山市	野木町
		case	543940:		//9		前橋市	高崎市	
		case	543942:		//9		桐生市	太田市	赤堀町	新田町	薮塚本町	笠懸町
		case	543943:		//9		桐生市	太田市	足利市	田沼町
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543966:		//8	9	宇都宮市(8)	鹿沼市(9)	壬生町(9)
		case	543967:		//8	9	宇都宮市(8)	真岡市(9)	
			sys.total	= 2;
			sys.sys[0]	= 8;
			sys.sys[1]	= 9;
			return(&sys);
		case	544001:		//9		土浦市	牛久市	つくば市	阿見町	茎崎町
		case	544011:		//9		土浦市	石岡市	つくば市	千代田町	新治村
		case	544043:		//9		水戸市	ひたちなか市	茨城町	内原町
		case	544044:		//9		水戸市	ひたちなか市
		case	544065:		//9		日立市
		case	544075:		//9		日立市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	553701:		//7		富山市
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	554046:		//9		いわき市
		case	554047:		//9		いわき市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	563816:		//8		長岡市
		case	563817:		//8		長岡市
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	563917:		//9		会津若松市
		case	563927:		//9		会津若松市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	563960:		//8		新潟市	亀田町	黒埼町
		case	563970:		//8		新潟市
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	564002:		//9		郡山市
		case	564003:		//9		郡山市
		case	564043:		//9		福島市
		case	564053:		//9		福島市
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	574022:		//10	山形市	上山市
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	574026:		//		該当なし
		case	574027:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	574032:		//10	山形市	天童市
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	574036:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	574037:		//10	塩竃市	多賀城市	利府町
		case	574042:		//10	山形市	天童市
		case	574043:		//10	山形市	天童市
		case	594040:		//10	秋田市
		case	594041:		//10	秋田市
		case	594140:		//10	盛岡市
		case	594141:		//10	盛岡市
		case	604073:		//10	弘前市
		case	604163:		//10	八戸市
		case	604164:		//10	八戸市
		case	614015:		//10	青森市
		case	614016:		//10	青森市
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	624045:		//11	函館市	上磯町
		case	624055:		//11	6	函館市	上磯町	大野町(6)
		case	624056:		//11	函館市
		case	644067:		//11	小樽市
			sys.total	= 1;
			sys.sys[0]	= 11;
			return(&sys);
		case	644132:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	644133:		//12	北広島市
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644142:		//		該当なし
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	644143:		//12	江別市	北広島市
		case	644144:		//12	江別市	北広島市
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644151:		//11	小樽市
		case	644152:		//11	12	小樽市(11)	石狩市(12)
			sys.total	= 1;
			sys.sys[0]	= 11;
			sys.sys[1]	= 12;
			return(&sys);
		case	644153:		//12	江別市
		case	644154:		//12	江別市
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644160:		//11	小樽市
			sys.total	= 1;
			sys.sys[0]	= 11;
			return(&sys);
		case	654252:		//12	旭川市
		case	654253:		//12	旭川市
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		default:
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
	}
}

// ここから z19_add.cpp
//1999.12.27 ここまで
//------------------------------------------------------------
/*
	メッシュコードから該当する１９座標系の数と番号を得る \n
	該当する系がない場合、数、番号ともに0を返す
*/
//------------------------------------------------------------
void	crd_cnv::
Sys_Number	(	int		meshcode,		// メッシュコード
				sys_num	*sys)			// 系番号
{
	*sys	= *Sys_Number(meshcode);	// 2009.05.25 修正
}

//------------------------------------------------------------
/*
	経緯度から１９座標系のＸＹへ変換(楕円体情報を指定)
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19_Core	(	double		Ra,			// 地球楕円体の長半径（メートル）
					double		ea2,		// 第一離心率の2乗
					double		eb2,		// 第二離心率の2乗
					int			axis,		// 該当する座標系の番号
					double		lon,		// 経度
					double		lat,		// 緯度
					int			mode,		// 変換モード(0=日本測地系,1=世界測地系)
					double		*x,			// １９座標系のＸ
					double		*y)			// １９座標系のＹ
{

	double			d_lon,	Tau;
	double			Eta2,	Eta4;
	double			N,		Phi;
	double			DPhi,	t;
	double			tx,		ty;

//	Make19tbl();			// 19座標系のテーブルを作成

	d_lon	= lon * M_PI / 180.0 - Rad_Origin[axis - 1].lon;
	lat		= lat * M_PI / 180.0;

	Tau		= tan(lat);
	Eta2	= eb2 * pow(cos(lat), 2.0);
	Eta4	= pow(Eta2, 2.0);
	N		= Ra / sqrt(1.0 - ea2 * pow(sin(lat), 2.0));
	Phi		= Rad_Origin[axis - 1].lat;
	DPhi	= lat - Rad_Origin[axis - 1].lat;
	t		= tan(lat);

	double	arclen	= 0.0, arclen_org = 0.0;

	if( mode == 0 ) {
		arclen		= M_Arc_Len(Ra, ea2, lat);
		arclen_org	= M_Arc_Len(Ra, ea2, Rad_Origin[axis - 1].lat);
	}else {
		arclen		= M_Arc_Len_Core(Ra, ea2, lat);
		arclen_org	= M_Arc_Len_Core(Ra, ea2, Rad_Origin[axis - 1].lat);
	}

	tx		= arclen - arclen_org
  			+ N * pow(d_lon, 2.0) * sin(lat) * cos(lat) / 2.0
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((5.0 - pow(t, 2.0) + 9.0 * Eta2 + 4.0 * Eta4) / 12.0
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (61.0 - 58.0 * pow(t, 2.0) + pow(t, 4.0) + 270.0 * Eta2
			- 330.0 * pow(t, 2.0) * Eta2) / 30.0));

	ty		= N * d_lon * cos(lat)
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((1.0 - pow(t, 2.0) + Eta2)
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (5.0 - 18.0 * pow(t, 2.0) + pow(t, 4.0)
			+ 14.0 * Eta2 -58.0 * pow(t, 2.0) * Eta2) / 20.0) / 6.0);

	*x	= tx * M0;
	*y	= ty * M0;
}

//------------------------------------------------------------
/*
	経緯度から１９座標系のＸＹへ変換
	@return	構造体 xy_coord へのポインタ
*/
//------------------------------------------------------------
xy_coord*	crd_cnv::
LLtoXY19	(	int		axis,		// 該当する座標系の番号
				double	lon,		// 経度
				double	lat)		// 緯度
{
	static xy_coord	coord;

	LLtoXY19 ( axis, lon, lat, &coord.x, &coord.y );

	return(&coord);
}

//------------------------------------------------------------
/*
	経緯度から１９座標系のＸＹへ変換
	@return	構造体 coord19 へのポインタ
*/
//------------------------------------------------------------
coord19*	crd_cnv::
LLtoXY19	(	double	lon,		// 経度
				double	lat)		// 緯度
{
	static	coord19		sys_xy;	
	int					sys		= 0;

	sys			= Sys_Number(lon, lat);		
	sys_xy.sys	= sys;

	LLtoXY19 ( sys, lon, lat, &sys_xy.x, &sys_xy.y );

	return(&sys_xy);
}

//------------------------------------------------------------
/*
	経緯度から１９座標系のＸＹへ変換
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	int			axis,		// 該当する座標系の番号
				double		lon,		// 経度
				double		lat,		// 緯度
				xy_coord	*coord)		// 19座標系のＸＹ
{
	LLtoXY19 ( axis, lon, lat, &coord->x, &coord->y );
}

//------------------------------------------------------------
/*
	経緯度から１９座標系のＸＹへ変換		1998.7.14
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	double		lon,			// 経度
				double		lat,			// 緯度
				coord19		*sys_xy)		// 19座標系の系とＸＹを表す
{
	int			sys		= 0;

	sys			= Sys_Number(lon, lat);		
	sys_xy->sys = sys;
	LLtoXY19 ( sys, lon, lat, &sys_xy->x, &sys_xy->y );
}

//------------------------------------------------------------
/*
	経緯度から１９座標系のＸＹへ変換		2000.1.12
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	int			axis,		// 該当する座標系の番号
				double		lon,		// 経度
				double		lat,		// 緯度
				double		*x,			// １９座標系のＸ
				double		*y)			// １９座標系のＹ
{
	LLtoXY19_Core ( RA, EA2, EB2, axis, lon, lat, 0, x, y );
}

//------------------------------------------------------------
/*
	経緯度から１９座標系のＸＹへ変換（世界測地系版）
	@return	構造体 xy_coord へのポインタ
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19_JGD2000	(	int		axis,		// 該当する座標系の番号
						double	lon,		// 経度
						double	lat,		// 緯度
						double	*x,			// １９座標系のＸ
						double	*y)			// １９座標系のＹ
{
	LLtoXY19_Core ( GRS80_RA, GRS80_EA2, GRS80_EB2, axis, lon, lat, 1, x, y );
}

//------------------------------------------------------------
/*
	１９座標系のＸＹから経緯度へ変換(楕円体情報を指定)
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL_Core	(	double		Ra,		// 地球楕円体の長半径（メートル）
					double		ea2,	// 第一離心率の2乗
					double		eb2,	// 第二離心率の2乗
					double		f,		// 楕円体の扁平率
					int			axis,	// 該当する座標系の番号
					double		x,		// 北方向の座標値
					double		y,		// 東方向の座標値
					int			mode,	// 変換モード(0=日本測地系,1=世界測地系)
					double		*lon,	// 経度
					double		*lat)	// 緯度
{
	double	Rad_Phi,	Rad_Ramda0,	Rad_Ramda;
	double	Rad_Phi1,	Rad_Phi0;
	double	Rad_Phi_n,	Rad_Phi_c,	Gunma2;
	double	t1,	a1,	p;

//	Make19tbl();	// 19座標系のテーブルを作成

	// 原点経緯度の取り出し
	Rad_Phi0		= Rad_Origin[axis - 1].lat;

	Rad_Ramda0		= Rad_Origin[axis - 1].lon;

	Rad_Phi_c		= Rad_Phi0;

	double	arc_len1 = 0.0, arc_len2 = 0.0;

	// 垂足緯度を求める
	while(1)
	{
		if( mode == 0 ) {
			arc_len1	= M_Arc_Len(Ra, ea2, Rad_Phi_c);
			arc_len2	= M_Arc_Len(Ra, ea2, Rad_Phi0);
		}else {
			arc_len1	= M_Arc_Len_Core(Ra, ea2, Rad_Phi_c);
			arc_len2	= M_Arc_Len_Core(Ra, ea2, Rad_Phi0);
		}
		Rad_Phi_n	= Rad_Phi_c
					- ((arc_len1 - arc_len2 - (x / M0)) / Ra * (1.0 - ea2))
					* pow((1.0 - ea2 * pow(sin(Rad_Phi_c), 2.0)), 1.5);

		if(fabs(Rad_Phi_n - Rad_Phi_c) < 1.0e-12)
		{
			break;
		}
		Rad_Phi_c	= Rad_Phi_n;
	}
	Rad_Phi1	= Rad_Phi_n;

	t1		= tan(Rad_Phi1);
	Gunma2	= eb2 * pow(cos(Rad_Phi1), 2.0);
	a1		= Ra / sqrt(1.0 - ea2 * pow(sin(Rad_Phi1), 2.0));
	p		= y / M0;

	double p2, p4, t2, t4, a2, a4;
	p2		= pow(p,	2.0);
	p4		= pow(p,	4.0);
	t2		= pow(t1,	2.0);
	t4		= pow(t1,	4.0);
	a2		= pow(a1,	2.0);
	a4		= pow(a1,	4.0);

	Rad_Phi		= Rad_Phi1
				- (p2 * (1.0 + Gunma2) * t1) / (2.0 * a2)
				* (1.0 - p2 / (12.0 * a2)
				* (5.0 + 3.0 * t2 + Gunma2 - 9.0 * t2 * Gunma2)
				+ p4 / (360.0 * a4) 
				* (61.0 + 90.0 * t2 + 45.0 * t4));

	Rad_Ramda	= Rad_Ramda0
				+ p / (a1 * cos(Rad_Phi1))
				* (1.0 - p2 / (6.0 * a2)
				* (1.0 + 2.0 * t2 + Gunma2)
				+ p4 / (120.0 * a4)
				* (5.0 + 28.0 * t2 + 24.0 * t4));

	*lon	= Rad_Ramda	/ (M_PI/180.0);
	*lat	= Rad_Phi	/ (M_PI/180.0);
}

//------------------------------------------------------------
/*
	１９座標系のＸＹから経緯度へ変換
	@return	構造体 d_lonlat へのポインタ
*/
//------------------------------------------------------------
d_lonlat*	crd_cnv::
XY19toLL	(	int		axis,		// 該当する座標系の番号
				double	x,			// 北方向の座標値
				double	y)			// 東方向の座標値
{
	static	d_lonlat	lonlat;

	XY19toLL ( axis, x, y, &lonlat.lon, &lonlat.lat );

	return(&lonlat);
}

//------------------------------------------------------------
/*
	１９座標系のＸＹから経緯度へ変換
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL	(	int			axis,		// 該当する座標系の番号
				double		x,			// 北方向の座標値
				double		y,			// 東方向の座標値
				d_lonlat	*lonlat)	// 10進で表した経緯度(ex:130.23432342....)
{
	XY19toLL ( axis, x, y, &lonlat->lon, &lonlat->lat );
}

//------------------------------------------------------------
/*
	１９座標系のＸＹから経緯度へ変換
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL	(	int			axis,	// 該当する座標系の番号
				double		x,		// 北方向の座標値
				double		y,		// 東方向の座標値
				double		*lon,	// 経度
				double		*lat)	// 緯度
{
	double	f = 1.0 / BESSEL_INV_F;	// 楕円体の扁平率

	XY19toLL_Core (	RA, EA2, EB2, f, axis, x, y, 0, lon, lat );
}

//------------------------------------------------------------
/*
	１９座標系のＸＹから経緯度へ変換（世界測地系版）
	@return	構造体 d_lonlat へのポインタ
*/
//------------------------------------------------------------
void	crd_cnv::
XY19toLL_JGD2000	(	int		axis,		// 該当する座標系の番号
						double	x,			// 北方向の座標値
						double	y,			// 東方向の座標値
						double	*lon,		// 経度
						double	*lat	)	// 緯度
{
	double	f = 1.0 / GRS80_INV_F;	// 楕円体の扁平率

	XY19toLL_Core (	GRS80_RA, GRS80_EA2, GRS80_EB2, f, axis, x, y, 1, lon, lat );
}

//------------------------------------------------------------
/*
	１９座標系の(X,Y)から、メッシュコード + (x,y) を求める
	@retval		0	成功
	@retval		-1	失敗
*/
//------------------------------------------------------------
int		crd_cnv::
XY19toMesh	(	int		sys,		// １９座標系の系番号
				double	x_19,		// １９座標系のＸ（緯度方向）
				double	y_19,		// １９座標系のＹ（経度方向）
				int		level,		// メッシュのレベル (1,2,64)
				int		*meshcode,	// メッシュコード
				int		*x,			// 正規化Ｘ
				int		*y)			// 正規化Ｙ
{
	double	lat	= 0.0,	lon	= 0.0;

	XY19toLL(sys, x_19, y_19, &lon, &lat);
	if(LLtoMesh(lon, lat, level, meshcode, x, y) == 0)
	{
		return(0);
	}
	else
	{
		return(-1);
	}
}

//------------------------------------------------------------
/*
	１９座標系の(X,Y)から、メッシュコード + (x,y) を求める
*/
//------------------------------------------------------------
void	crd_cnv::
MeshtoXY19	(	int		sys,		// １９座標系の系番号
				int		meshcode,	// メッシュコード
				int		x,			// 正規化Ｘ
				int		y,			// 正規化Ｙ
				double	*x_19,		// １９座標系のＸ（緯度方向）
				double	*y_19)		// １９座標系のＹ（経度方向）
{
	double	lon	= 0.0,	lat	= 0.0;

	MeshtoLL(meshcode, x, y, &lon, &lat);
	LLtoXY19(sys, lon, lat, x_19, y_19); 
}
