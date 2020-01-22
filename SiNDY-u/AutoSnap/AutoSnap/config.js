/*
 * 自动Snap工具配置文件　v0.03
 * 自動化Snapツール設定ファイル v0.03
 */
{
    //Type 1:全部单一文件或者SDE
    //Type 2:复数PGDB,单个Log文件
    //Type 3:复数PGDB,复数Log文件
    //Type 1: 全部単一ファイルやSDE環境
    //Type 2:複数PGDB、単一Logファイル
    //Type 3:複数PGDB、複数Logファイル
    "type" : 3,

    //输出Log Type1 Type2:文件名  Type3：文件夹
    //出力Log　Type1 Type2:ファイル名  Type3：フォルダ
	"log": "E:\\Env\\snaplog",

    //Log文件名格式，只有Type3有效, %s为meshlist中的meshcode
    //Logファイル名フォーマット、Type３のみ、%sはmeshlist中のmeshcode
    "log_format" : "%s_snap.txt",

    //Mesh号
    //Mesh番号
	"mesh_list": "E:\\Env\\mesh.txt",

    //ShapeChecker的Log文件 Type1 Type2 :文件名   Type3：文件夹
    //ShapeCheckerのLogファイル名　Type1 Type2：ファイル名   Type3：フォルダ
	"snap": "E:\\Env\\log",

    //Snap Log文件名格式，只有Type3有效, %s为meshlist中的meshcode
    //ファイル名フォーマット　Type3のみ、%sはmeshlist中のmeshcode
    "snap_format" : "%s.txt",

    //SnapLayer的设置
    //SnapLayerの設定
	"snap_layer": "E:\\Env\\snap_layer.txt",

    //自动Snap的范围（半径）
    //自動化Snapの範囲（半径）
	"radius": 0.000005,

    //Mesh框数据 Type2或Type3的情况下，此参数无效
    //Meshデータ Type2とType3の場合、このパラメータが無効になる
    "mesh": "E:\\Env\\Data.mdb",

    //Mesh框数据的表名
    //Meshデータのテーブル名
    "mesh_name": "CITYMESH",

    //数据文件名 Type1 :文件名  Type2 Type3：文件夹
    //データファイル名　Type1 :ファイル名  Type2 Type3：フォルダ
	"lot": "E:\\Env\\Data",

    //各层数据的微小片断值，
    //如果自动添加的点和原来已存在的点之间的距离小于以下的值，不做处理并报错。
    //各レイヤの微小セグメント情報
    //追加される点と存在する点の距離は以下の値未満の場合：処理しなくて、エラーを出します。
	"mini_length" :
	{
		"city_site": 0.0000005,
		"city_line": 0.0000005,
		"city_railway": 0.0000005,
		"city_station": 0.0000005,
		"city_tunnel": 0.0000005,
		"building": 0.0000005,
		"building_step": 0.0000005,
		"building_line": 0.0000005,
		"base_site": 0.000005,
		"base_line": 0.000005,
		"base_railway": 0.000005,
		"base_station": 0.000005,
		"base_contour": 0.000005,
		"middle_site": 0.00005,
		"middle_line": 0.00005,
		"middle_contour": 0.00005,
		"city_admin": 0.0000005,
		"base_admin": 0.000005
	}
} 	//end

/*
 JSON解析库　 v0.1
 JSON格式　http://www.json.org
 支持类似javascript（C/C++）的单行和多行的注释

 JSONパーサーライブライ　v0.1 
 JSONフォーマット　http://www.json.org
 Javascript（C\C++）同様の単一行と複数行コメントがサポートされています。
*/
