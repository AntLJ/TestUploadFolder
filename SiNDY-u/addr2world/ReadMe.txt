プログラム名：addr2world.exe
目的：
　日本測地系の住所データを世界測地系に変換する。
　変換モードは2つあり、JGD2000変換と3パラメータ変換を行える
　JGD2000変換は、iPCで測地成果2000に合わせた離島を3パラメータで変換する

　ついでに郵便番号データも変換する

使用方法：
　-iオプションで入力フォルダを指定。
　-oオプションで出力フォルダを指定。
　JGD2000に変換する場合は-rオプションで使用するパラメータのパスをその後に記述する。

　例：
　　データの場所：
　　　　\\win\tdc\PRODENG_WORK\release_data\DB-130944S-000\A\jusyo\data
　　　　　　|---JUSYO
            |---JUSYO_EX
            |---ZIP

　　出力先：F:\Temp\World以下

　　[3パラメータ方式での変換]
　　　addr2world.exe -i \\win\tdc\PRODENG_WORK\release_data\DB-130944S-000\A\jusyo\data -o F:\Temp\World

    [地域パラメータでの変換]
　　　addr2world.exe -i \\win\tdc\PRODENG_WORK\release_data\DB-130944S-000\A\jusyo\data -o F:\Temp\World -r \\win\tdc\Common\dev2-6G\02_プロジェクト\測地系変換\パラメータ\TKY2JGD_ver212.par

　※出力先フォルダがない場合は、自動で作成する。


注意点：
　指定したフォルダ以下から以下の条件に合ったファイルを検索し実行する。
　　pref.txt
　　city.txt
    addr*.txt
　　gou*.txt
　　pnt*.txt
　　zip.txt
　そのため、文字付住所も存在する場合はそれも変換してしまう。
　その分時間を要するが、全部で10分程度の変換時間なので気にすることはない。
　変換したくない場合は、変換元データで必要なものだけ別フォルダにコピーしてそこを入力フォルダとして実行すればよい。

実行時間：
　文字付住所も含め、住所データ全セットで約10分程度。
