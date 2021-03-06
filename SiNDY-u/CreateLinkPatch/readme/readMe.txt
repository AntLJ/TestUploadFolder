リンクパッチ作成ツール(CreateLinkPatch)仕様
Written by f_adachi 2005/11/11

◆ 目的
　高松自動車道などの高速道路の一部で、リンクの道路表示種別が
　「高速⇔一般有料⇔高速」というパターンになっている箇所がある場合、
　高速道路から降りるような経路が引かれてしまう。
　この問題に対しては、問題となるリンクについてtdc_link.patchというファイルを作成し、
　目黒開発部でのオーサリング時に種別を変更するような処理を行なって対応する。
　現時点では高松自動車道のみの対応となっているが、今後別の場所にも同様の対応をしていくため、
　問題のパターンを持つ箇所を抽出するツールを開発した。


◆ 機能
　路線コードと道路標示種別を指定し、その路線内のリンクの内、指定されたものと異なる道路標示種別を持つリンクを抽出し、ログファイルとリンクパッチファイルに出力する。

◆ 使用法
　CreateLinkPatch -d <DB指定文字列> -r <路線コード指定ファイルパス> -p <リンクパッチファイルパス> -l <ログファイルパス> [-e]

-d <DB指定文字列>
　対象DBを指定する。
　<DB指定文字列>は「ユーザ名@サーバ名(バージョン名)」の形式の文字列。
　例：sindy2005@amber(SDE.DEFAULT)

-r <路線コード指定ファイルパス>
　入力する路線コード指定ファイルパスを指定する。
　路線コード指定ファイルパスとは、「抽出対象とする路線コードと、その路線のリンクが持つべき道路標示種別の組の集合」を記述したファイルである。
　Tab区切りのテキストファイルで、下記の形式で記述するものとする。
　　第１フィールド：路線コード
　　第２フィールド：道路標示種別

-p <リンクパッチファイルパス>
　出力するリンクパッチファイルを指定する。

-l <ログファイルパス>
　出力するログファイルを指定する。

-u
　出力されるリンクパッチファイルの改行コードをUNIX環境で使用されるLFにする。
　デフォルトはWindows環境で使用されるCR+LF。
