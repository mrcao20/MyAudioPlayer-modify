#include "McQQApi.h"
#include <qjsonvalue.h>
#include "McNetTools.h"

#define VKEY "https://c.y.qq.com/base/fcgi-bin/fcg_music_express_mobile3.fcg?g_tk=1109981464&loginUin=839566521&hostUin=0&format=json&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&cid=205361747&uin=839566521&songmid=%1&filename=%2.m4a&guid=2054016189"
#define SONG "http://dl.stream.qqmusic.qq.com/%1.m4a?vkey=%2&guid=2054016189&uin=839566521&fromtag=66"

struct McQQApiData {
	McNetTools m_netTool;
};

McQQApi::McQQApi()
	: d(new McQQApiData)
{
}

McQQApi::~McQQApi(){
}

QString McQQApi::getSongUrl(const QString &songMid) {
	QString fileName = "C400" + songMid;
	QString vkey = QString(VKEY).arg(songMid).arg(fileName);
	QByteArray json = d->m_netTool.getNetworkData(vkey);
	QString key = d->m_netTool.getJsonValue(json, "data.items.vkey").toString();
	QString url = "";
	if(!key.isEmpty())
		url = QString(SONG).arg(fileName).arg(key);
	/*if (!d->m_netTool.isValid(url))
		url = "";*/
	return url;
}
