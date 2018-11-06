#include "McSonglistDelegate.h"
#include "qlineedit.h"

McSonglistDelegate::McSonglistDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

McSonglistDelegate::~McSonglistDelegate(){
}

QWidget *McSonglistDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	QLineEdit *edit = new QLineEdit(parent);
	return edit;
}

void McSonglistDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QString value = index.data(Qt::EditRole).toString();
	QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
	if(edit)
		edit->setText(value);
}

void McSonglistDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
	QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
	if(edit)
		model->setData(index, edit->text(), Qt::EditRole);
}

void McSonglistDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	editor->setGeometry(option.rect);
}
