#include "McYApi.h"
#include "McNetTools.h"

#define SONG_LINK "http://music.163.com/song/media/outer/url?id=%1"

struct McYApiData {
	McNetTools m_netTool;
};

McYApi::McYApi()
	: d(new McYApiData)
{
}

McYApi::~McYApi(){
}

QString McYApi::getSongUrl(const QString &songId) {
	/*
		原音乐源地址仍然可用，但无法下载
		现适应网易云音乐的加密传输，通过加密算法重新获取音乐源，已可下载
		原地址被淘汰，但仍可用，继续保留
		2018-8-31: 重新启用原歌曲api，新加密api单独被用作下载链接
	*/
	QString url = QString(SONG_LINK).arg(songId);
	/*if (!d->m_netTool.isValid(url))
		url = "";*/
	return url;
}
