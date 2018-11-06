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
		ԭ����Դ��ַ��Ȼ���ã����޷�����
		����Ӧ���������ֵļ��ܴ��䣬ͨ�������㷨���»�ȡ����Դ���ѿ�����
		ԭ��ַ����̭�����Կ��ã���������
		2018-8-31: ��������ԭ����api���¼���api������������������
	*/
	QString url = QString(SONG_LINK).arg(songId);
	/*if (!d->m_netTool.isValid(url))
		url = "";*/
	return url;
}
