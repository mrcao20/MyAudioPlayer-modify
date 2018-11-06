#pragma once

#include <QListView>

class McListView : public QListView {
	Q_OBJECT

public:
	explicit McListView(QWidget *parent = 0);
	~McListView();
};
