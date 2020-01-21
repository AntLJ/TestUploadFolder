/**
* @brief	概要の構造体
*/
function Library(strName,strSumm,strDeti,strSamp)
{
	this.strName = strName;		// ライブラリ名
	this.strSumm = strSumm;		// 概要
	this.strDeti = strDeti;		// 詳細
	this.strSamp = strSamp;		// サンプル
}

// HTTP通信用、共通関数
function createXMLHttpRequest(strFunc)
{
	var XMLhttpObject = null;
	try{
		XMLhttpObject = new XMLHttpRequest();
	}catch(e){
		try{
			XMLhttpObject = new ActiveXObject("Msxml2.XMLHTTP");
		}catch(e){
			try{
				XMLhttpObject = new ActiveXObject("Microsoft.XMLHTTP");
			}catch(e){
				return null;
			}
		}
	}
//	if(XMLhttpObject)
//		XMLhttpObject.onreadystatechange = strFunc;
	return XMLhttpObject;
}

function loadSummary(strFileName)
{
	var httpSummary = createXMLHttpRequest();
	if( httpSummary )
	{
		httpSummary.open("GET","outline/"+strFileName+".txt",false);
		httpSummary.send(null);
		var listSummary = httpSummary.responseText.split("\n");
		var strTemp = "";
		var nCount = 0;
		var nCount2 = 0;
		var cLib = new Library(strFileName,"","","");
		while(1)
		{
			if( listSummary[nCount].match(/========/i) )
			{
				while(1)
				{
					if( nCount == listSummary.length - 1 )
						break;
					nCount++;
					if( listSummary[nCount].match(/========/i) )
						break;
					strTemp += listSummary[nCount] + "<br>";
				}

				if( nCount2 == 0 )
					cLib.strSumm = strTemp;
				else if( nCount2 == 1 )
					cLib.strDeti = strTemp;
				else if( nCount2 == 2 )
					cLib.strSamp = strTemp;
				else
					break;
				strTemp = "";
				nCount2++;
			}
			if( nCount == listSummary.length - 1 )
				break;
		}
		return cLib;
	}
}

function loadAccordions() {
	var bottomAccordion = new accordion('vertical_container', { resizeSpeed : 10 });
	var nestedVerticalAccordion = new accordion('vertical_nested_container', {
		resizeSpeed : 10,
		classNames : {
			toggle : 'vertical_accordion_toggle',
			toggleActive : 'vertical_accordion_toggle_active',
			content : 'vertical_accordion_content'
		}
	});
	//nestedVerticalAccordion.activate($$('#vertical_nested_container .vertical_accordion_toggle')[0]);
}
