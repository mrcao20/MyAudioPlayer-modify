#pragma once

#include <QtWidgets/QWidget>
#include <qscopedpointer.h>

struct McAudioPlayerData;

class McAudioPlayer : public QWidget{
	Q_OBJECT

public:
	explicit McAudioPlayer(QWidget *parent = Q_NULLPTR);
	~McAudioPlayer();

	void init();

	void playFile(const QString& filePath);

public slots:
	void quit();

private:
	void changeDisplay();
	void initTrayMenu();

private:
	QScopedPointer<McAudioPlayerData> d;
};
