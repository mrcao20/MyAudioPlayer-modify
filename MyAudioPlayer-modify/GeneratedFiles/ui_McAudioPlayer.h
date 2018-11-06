/********************************************************************************
** Form generated from reading UI file 'McAudioPlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MCAUDIOPLAYER_H
#define UI_MCAUDIOPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mclistview.h"
#include "mcslider.h"
#include "mctableview.h"

QT_BEGIN_NAMESPACE

class Ui_McAudioPlayer
{
public:
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *localSongButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *networkSearchBackButton;
    QPushButton *networkSearchButton;
    QComboBox *searchSrcBox;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QPushButton *playOrderButton;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *previousButton;
    QPushButton *playButton;
    QPushButton *nextButton;
    McSlider *positionSlider;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QPushButton *lyricButton;
    QPushButton *lyricTranslatedButton;
    QHBoxLayout *horizontalLayout_4;
    QLabel *infoLabel;
    QLabel *positionLabel;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_5;
    QPushButton *volumeButton;
    QSpacerItem *verticalSpacer_6;
    QVBoxLayout *verticalLayout_7;
    QSpacerItem *verticalSpacer_13;
    QPushButton *playlistButton;
    QSpacerItem *verticalSpacer_14;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *searchBackButton;
    QPushButton *searchNextButton;
    QSpacerItem *horizontalSpacer_6;
    QLabel *currentListLabel;
    QLabel *songNumLabel;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *searchLineEdit;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QPushButton *addSonglistButton;
    QPushButton *songlistButton;
    QSpacerItem *verticalSpacer;
    QListView *songlistListView;
    McTableView *songTableView;
    McListView *playlistListView;

    void setupUi(QWidget *McAudioPlayer)
    {
        if (McAudioPlayer->objectName().isEmpty())
            McAudioPlayer->setObjectName(QStringLiteral("McAudioPlayer"));
        McAudioPlayer->resize(872, 587);
        QIcon icon;
        icon.addFile(QStringLiteral(":/McAudioPlayer/data/icon/AudioPlayer.png"), QSize(), QIcon::Normal, QIcon::Off);
        McAudioPlayer->setWindowIcon(icon);
        McAudioPlayer->setStyleSheet(QLatin1String("QWidget#McAudioPlayer{\n"
"	background-color:rgba(107, 21, 0, 255);\n"
"}\n"
""));
        gridLayout_4 = new QGridLayout(McAudioPlayer);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 5, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        localSongButton = new QPushButton(McAudioPlayer);
        localSongButton->setObjectName(QStringLiteral("localSongButton"));
        localSongButton->setMaximumSize(QSize(25, 25));
        localSongButton->setStyleSheet(QStringLiteral("background-color:rgba(255, 255, 255, 0);"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/McAudioPlayer/data/icon/localSong.png"), QSize(), QIcon::Normal, QIcon::Off);
        localSongButton->setIcon(icon1);
        localSongButton->setIconSize(QSize(25, 25));

        horizontalLayout->addWidget(localSongButton);

        horizontalSpacer_2 = new QSpacerItem(74, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        networkSearchBackButton = new QPushButton(McAudioPlayer);
        networkSearchBackButton->setObjectName(QStringLiteral("networkSearchBackButton"));
        networkSearchBackButton->setMaximumSize(QSize(31, 23));
        networkSearchBackButton->setStyleSheet(QStringLiteral("QPushButton#networkSearchBackButton:!hover{ background-color:rgba(255, 255, 255, 0); color:rgb(97, 97, 97); }QPushButton#networkSearchBackButton:hover{ background-color:rgba(255, 255, 255, 0); color:rgb(150, 150, 150); }"));

        horizontalLayout->addWidget(networkSearchBackButton);

        networkSearchButton = new QPushButton(McAudioPlayer);
        networkSearchButton->setObjectName(QStringLiteral("networkSearchButton"));
        networkSearchButton->setMaximumSize(QSize(16, 16));
        networkSearchButton->setStyleSheet(QLatin1String("QPushButton#networkSearchButton{\n"
"	background-color:rgba(255, 255, 255, 0);\n"
"}\n"
"QPushButton#networkSearchButton:!hover{\n"
"	border-image:url(:/McAudioPlayer/data/icon/search.png);\n"
"}\n"
"QPushButton#networkSearchButton:hover{\n"
"	border-image:url(:/McAudioPlayer/data/icon/search_hover.png);\n"
"}\n"
""));

        horizontalLayout->addWidget(networkSearchButton);

        searchSrcBox = new QComboBox(McAudioPlayer);
        searchSrcBox->addItem(QString());
        searchSrcBox->addItem(QString());
        searchSrcBox->setObjectName(QStringLiteral("searchSrcBox"));

        horizontalLayout->addWidget(searchSrcBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_2);

        playOrderButton = new QPushButton(McAudioPlayer);
        playOrderButton->setObjectName(QStringLiteral("playOrderButton"));
        playOrderButton->setMaximumSize(QSize(25, 25));
        playOrderButton->setStyleSheet(QStringLiteral("background-color:rgba(255, 255, 255, 0);"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/McAudioPlayer/data/icon/shuffle.png"), QSize(), QIcon::Normal, QIcon::Off);
        playOrderButton->setIcon(icon2);
        playOrderButton->setIconSize(QSize(23, 23));

        verticalLayout_3->addWidget(playOrderButton);

        verticalSpacer_7 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_7);


        gridLayout_3->addLayout(verticalLayout_3, 3, 6, 2, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        previousButton = new QPushButton(McAudioPlayer);
        previousButton->setObjectName(QStringLiteral("previousButton"));
        previousButton->setMaximumSize(QSize(25, 25));
        previousButton->setStyleSheet(QLatin1String("QPushButton#previousButton{\n"
"	background-color:rgba(255, 255, 255, 0);\n"
"}\n"
"QPushButton#previousButton:hover{\n"
"	border-image:url(:/McAudioPlayer/data/icon/previous_hover.png);\n"
"}"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/McAudioPlayer/data/icon/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        previousButton->setIcon(icon3);
        previousButton->setIconSize(QSize(25, 25));

        horizontalLayout_3->addWidget(previousButton);

        playButton = new QPushButton(McAudioPlayer);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setMaximumSize(QSize(32, 32));
        playButton->setStyleSheet(QStringLiteral("background-color:rgba(255, 255, 255, 0);"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/McAudioPlayer/data/icon/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        playButton->setIcon(icon4);
        playButton->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(playButton);

        nextButton = new QPushButton(McAudioPlayer);
        nextButton->setObjectName(QStringLiteral("nextButton"));
        nextButton->setMaximumSize(QSize(25, 25));
        nextButton->setStyleSheet(QLatin1String("QPushButton#nextButton{\n"
"	background-color:rgba(255, 255, 255, 0);\n"
"}\n"
"QPushButton#nextButton:hover{\n"
"	border-image:url(:/McAudioPlayer/data/icon/next_hover.png);\n"
"}"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/McAudioPlayer/data/icon/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        nextButton->setIcon(icon5);
        nextButton->setIconSize(QSize(25, 25));

        horizontalLayout_3->addWidget(nextButton);


        gridLayout_3->addLayout(horizontalLayout_3, 1, 0, 4, 1);

        positionSlider = new McSlider(McAudioPlayer);
        positionSlider->setObjectName(QStringLiteral("positionSlider"));
        positionSlider->setMaximumSize(QSize(16777215, 22));
        positionSlider->setStyleSheet(QString::fromUtf8("/*\351\246\226\345\205\210\346\230\257\350\256\276\347\275\256\344\270\273\344\275\223*/\n"
"QSlider#positionSlider{\n"
"	border-color: #bcbcbc;\n"
"}\n"
"QSlider::groove:horizontal {                                \n"
"	border: 1px solid #999999;          \n"
"	height: 1px;                                           \n"
"	margin: 0px 0;                                         \n"
"	left: 5px; right: 5px; \n"
"}\n"
"/*\350\256\276\347\275\256\344\270\255\351\227\264\347\232\204\351\202\243\344\270\252\346\273\221\345\212\250\347\232\204\351\224\256*/\n"
"QSlider::handle:horizontal {                               \n"
"	border: 0px ;                           \n"
"	border-image:  url(:/McAudioPlayer/data/icon/Slider_handle.png);\n"
"	width: 15px;                                           \n"
"	margin: -7px -7px -7px -7px;                  \n"
"} \n"
"/*\350\277\230\346\262\241\346\234\211\346\273\221\344\270\212\345\216\273\347\232\204\345\234\260\346\226\271*/\n"
"QSlider::add-page:horizontal{\n"
"	border-image:"
                        "  url(:/McAudioPlayer/data/icon/Slider_bar.png);\n"
"	background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #bcbcbc, stop:0.25 #bcbcbc, stop:0.5 #bcbcbc, stop:1 #bcbcbc); \n"
"}\n"
"/*\345\267\262\347\273\217\345\210\222\350\277\207\347\232\204\344\273\216\345\234\260\346\226\271*/\n"
"QSlider::sub-page:horizontal{                               \n"
"	background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #439cf3, stop:0.25 #439cf3, stop:0.5 #439cf3, stop:1 #439cf3);                     \n"
"}\n"
""));
        positionSlider->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(positionSlider, 4, 2, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        lyricButton = new QPushButton(McAudioPlayer);
        lyricButton->setObjectName(QStringLiteral("lyricButton"));
        lyricButton->setMaximumSize(QSize(21, 21));
        QFont font;
        font.setPointSize(12);
        lyricButton->setFont(font);
        lyricButton->setStyleSheet(QStringLiteral("QPushButton#lyricButton:!hover{ background-color:rgba(255, 255, 255, 0); color:rgb(97, 97, 97); }QPushButton#lyricButton:hover{ background-color:rgba(255, 255, 255, 0); color:rgb(150, 150, 150); }"));

        verticalLayout->addWidget(lyricButton);

        lyricTranslatedButton = new QPushButton(McAudioPlayer);
        lyricTranslatedButton->setObjectName(QStringLiteral("lyricTranslatedButton"));
        lyricTranslatedButton->setMaximumSize(QSize(21, 21));
        lyricTranslatedButton->setFont(font);
        lyricTranslatedButton->setStyleSheet(QStringLiteral("QPushButton#lyricTranslatedButton:!hover{ background-color:rgba(255, 255, 255, 0); color:rgb(97, 97, 97); }QPushButton#lyricTranslatedButton:hover{ background-color:rgba(255, 255, 255, 0); color:rgb(150, 150, 150); }"));

        verticalLayout->addWidget(lyricTranslatedButton);


        gridLayout_3->addLayout(verticalLayout, 3, 3, 2, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        infoLabel = new QLabel(McAudioPlayer);
        infoLabel->setObjectName(QStringLiteral("infoLabel"));
        infoLabel->setStyleSheet(QStringLiteral("color:rgb(120, 120, 120);"));
        infoLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(infoLabel);

        positionLabel = new QLabel(McAudioPlayer);
        positionLabel->setObjectName(QStringLiteral("positionLabel"));
        positionLabel->setMaximumSize(QSize(101, 21));
        QFont font1;
        font1.setPointSize(10);
        positionLabel->setFont(font1);
        positionLabel->setStyleSheet(QStringLiteral("color:rgb(97, 97, 97);"));
        positionLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(positionLabel);


        gridLayout_3->addLayout(horizontalLayout_4, 3, 2, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer_5 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_5);

        volumeButton = new QPushButton(McAudioPlayer);
        volumeButton->setObjectName(QStringLiteral("volumeButton"));
        volumeButton->setMaximumSize(QSize(24, 24));
        volumeButton->setStyleSheet(QLatin1String("QPushButton#volumeButton{\n"
"	background-color:rgba(255, 255, 255, 0);\n"
"}\n"
"QPushButton#volumeButton:hover{\n"
"	border-image:url(:/McAudioPlayer/data/icon/volume_hover.png);\n"
"}"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/McAudioPlayer/data/icon/volume.png"), QSize(), QIcon::Normal, QIcon::Off);
        volumeButton->setIcon(icon6);
        volumeButton->setIconSize(QSize(24, 24));

        verticalLayout_2->addWidget(volumeButton);

        verticalSpacer_6 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_6);


        gridLayout_3->addLayout(verticalLayout_2, 3, 7, 2, 1);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalSpacer_13 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_13);

        playlistButton = new QPushButton(McAudioPlayer);
        playlistButton->setObjectName(QStringLiteral("playlistButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(playlistButton->sizePolicy().hasHeightForWidth());
        playlistButton->setSizePolicy(sizePolicy);
        playlistButton->setMinimumSize(QSize(46, 20));
        playlistButton->setMaximumSize(QSize(46, 20));
        playlistButton->setStyleSheet(QLatin1String("QPushButton#playlistButton{\n"
"	background-color:rgba(255, 255, 255, 0);\n"
"	color:rgb(140, 140, 140);\n"
"}\n"
"QPushButton#playlistButton:hover{\n"
"	color:rgb(222, 222, 222);\n"
"	background-image: url(:/McAudioPlayer/data/icon/playlist_hover.png);\n"
"    border: none;\n"
"    background-repeat: no-repeat;\n"
"	margin: 1 0 0 0; \n"
"	padding: 0 0 0 0;\n"
"    min-height: 24px;\n"
"    min-width: 24px;\n"
"    max-width: 24px;\n"
"    max-height: 24px;\n"
"}"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/McAudioPlayer/data/icon/playlist.png"), QSize(), QIcon::Normal, QIcon::Off);
        playlistButton->setIcon(icon7);
        playlistButton->setIconSize(QSize(24, 24));

        verticalLayout_7->addWidget(playlistButton);

        verticalSpacer_14 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_14);


        gridLayout_3->addLayout(verticalLayout_7, 3, 8, 2, 1);


        gridLayout_4->addLayout(gridLayout_3, 5, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_4->addItem(verticalSpacer_4, 6, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(109, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        searchBackButton = new QPushButton(McAudioPlayer);
        searchBackButton->setObjectName(QStringLiteral("searchBackButton"));
        searchBackButton->setMaximumSize(QSize(31, 23));
        searchBackButton->setFont(font1);
        searchBackButton->setStyleSheet(QStringLiteral("QPushButton#searchBackButton:!hover{ background-color:rgba(255, 255, 255, 0); color:rgb(97, 97, 97); }QPushButton#searchBackButton:hover{ background-color:rgba(255, 255, 255, 0); color:rgb(150, 150, 150); }"));

        horizontalLayout_2->addWidget(searchBackButton);

        searchNextButton = new QPushButton(McAudioPlayer);
        searchNextButton->setObjectName(QStringLiteral("searchNextButton"));
        searchNextButton->setMaximumSize(QSize(31, 23));
        searchNextButton->setFont(font1);
        searchNextButton->setStyleSheet(QStringLiteral("QPushButton#searchNextButton:!hover{ background-color:rgba(255, 255, 255, 0); color:rgb(97, 97, 97); }QPushButton#searchNextButton:hover{ background-color:rgba(255, 255, 255, 0); color:rgb(150, 150, 150); }"));

        horizontalLayout_2->addWidget(searchNextButton);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        currentListLabel = new QLabel(McAudioPlayer);
        currentListLabel->setObjectName(QStringLiteral("currentListLabel"));
        currentListLabel->setMinimumSize(QSize(131, 16));
        currentListLabel->setMaximumSize(QSize(131, 16));
        currentListLabel->setFont(font1);
        currentListLabel->setStyleSheet(QStringLiteral("color:rgb(200, 200, 200);"));
        currentListLabel->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        horizontalLayout_2->addWidget(currentListLabel);

        songNumLabel = new QLabel(McAudioPlayer);
        songNumLabel->setObjectName(QStringLiteral("songNumLabel"));
        songNumLabel->setFont(font1);
        songNumLabel->setStyleSheet(QStringLiteral("color:rgb(200, 200, 200);"));

        horizontalLayout_2->addWidget(songNumLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        searchLineEdit = new QLineEdit(McAudioPlayer);
        searchLineEdit->setObjectName(QStringLiteral("searchLineEdit"));
        searchLineEdit->setMaximumSize(QSize(151, 20));
        searchLineEdit->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(searchLineEdit);


        gridLayout_4->addLayout(horizontalLayout_2, 1, 0, 1, 3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(10);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        addSonglistButton = new QPushButton(McAudioPlayer);
        addSonglistButton->setObjectName(QStringLiteral("addSonglistButton"));
        addSonglistButton->setMaximumSize(QSize(21, 23));
        QFont font2;
        font2.setPointSize(17);
        addSonglistButton->setFont(font2);
        addSonglistButton->setStyleSheet(QStringLiteral("QPushButton#addSonglistButton:!hover{ background-color:rgba(255, 255, 255, 0); color:rgb(97, 97, 97); }QPushButton#addSonglistButton:hover{ background-color:rgba(255, 255, 255, 0); color:rgb(150, 150, 150); }"));

        gridLayout->addWidget(addSonglistButton, 0, 2, 1, 1);

        songlistButton = new QPushButton(McAudioPlayer);
        songlistButton->setObjectName(QStringLiteral("songlistButton"));
        songlistButton->setMaximumSize(QSize(61, 23));
        songlistButton->setStyleSheet(QLatin1String("QPushButton#songlistButton:!hover{ background-color:rgba(255, 255, 255, 0); color:rgb(97, 97, 97); }\n"
"QPushButton#songlistButton:hover{ background-color:rgba(255, 255, 255, 0); color:rgb(150, 150, 150); }"));

        gridLayout->addWidget(songlistButton, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 2);

        songlistListView = new QListView(McAudioPlayer);
        songlistListView->setObjectName(QStringLiteral("songlistListView"));
        songlistListView->setMinimumSize(QSize(100, 192));
        songlistListView->setMaximumSize(QSize(100, 192));
        songlistListView->setContextMenuPolicy(Qt::CustomContextMenu);
        songlistListView->setStyleSheet(QLatin1String("QListView{\n"
"	background-color:rgb(107, 21, 0);\n"
"	border: 0px;\n"
"	outline:0px;\n"
"}\n"
"\n"
"QListView::Item{\n"
"	border: 0px;\n"
"	color:rgb(150, 150, 150);\n"
"}\n"
"\n"
"QListView::Item:hover{\n"
"	background:rgba(97, 97, 97, 10%); \n"
"}\n"
"\n"
"QListView::Item:selected{\n"
"	background:rgba(97, 97, 97, 25%);  \n"
"}\n"
"\n"
"QListView::Item:selected:!active{\n"
"	background:rgba(97, 97, 97, 25%);  \n"
"}"));
        songlistListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        songlistListView->setEditTriggers(QAbstractItemView::EditKeyPressed);

        gridLayout->addWidget(songlistListView, 1, 1, 1, 2);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        songTableView = new McTableView(McAudioPlayer);
        songTableView->setObjectName(QStringLiteral("songTableView"));
        songTableView->setMouseTracking(true);
        songTableView->setStyleSheet(QString::fromUtf8("QTableView{\n"
"		color:rgb(181, 181, 181);\n"
"        border:none;\n"
"        background-color:rgba(107, 21, 0, 255);\n"
"}\n"
"QTableView::item:selected{\n"
"        color:white;\n"
"        background:rgb(34, 175, 75);    \n"
"}\n"
"QTableView::item:hover{\n"
"        color:rgb(24, 39, 255);\n"
"		background:rgb(28, 153, 62);\n"
"}\n"
"/*QTableView \345\267\246\344\270\212\350\247\222\346\240\267\345\274\217*/\n"
"QTableView QTableCornerButton::section {\n"
"   /*  background: red;\n"
"    border: 2px outset red;*/\n"
"    color: red;\n"
"    background-color: rgb(64, 64, 64);\n"
"    border: 5px solid #f6f7fa;\n"
"    border-radius:0px;\n"
"    border-color: rgb(64, 64, 64);\n"
" }\n"
"\n"
"QHeaderView {\n"
"    color: white;\n"
"    font: bold 10pt;\n"
"    background-color:rgba(107, 21, 0, 255);\n"
"    border: 0px solid rgb(144, 144, 144);\n"
"    border:0px solid rgb(191,191,191);\n"
"    border-left-color: rgba(255, 255, 255, 0);\n"
"    border-top-color: rgba(255, 255, 255, 0);\n"
"    border-radiu"
                        "s:0px;\n"
"    min-height:29px;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    color: white;\n"
"    background-color:rgba(107, 21, 0, 255);\n"
"    border: 1px solid #f6f7fa;\n"
"    border-radius:0px;\n"
"    border-color: rgb(64, 64, 64);\n"
"} \n"
""));
        songTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        songTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        songTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        songTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        songTableView->setShowGrid(false);
        songTableView->horizontalHeader()->setHighlightSections(false);
        songTableView->verticalHeader()->setVisible(false);

        gridLayout_2->addWidget(songTableView, 1, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 2, 0, 1, 3);

        playlistListView = new McListView(McAudioPlayer);
        playlistListView->setObjectName(QStringLiteral("playlistListView"));
        playlistListView->setMinimumSize(QSize(200, 0));
        playlistListView->setMaximumSize(QSize(200, 16777215));
        playlistListView->setMouseTracking(true);
        playlistListView->setStyleSheet(QString::fromUtf8("QListView{\n"
"		color:rgb(181, 181, 181);\n"
"		border:none;\n"
"        background-color:rgba(107, 21, 0, 255);\n"
"		outline:0px;\n"
"}\n"
"QListView::item{\n"
"		border: 1px outset gray;\n"
"		border-width: 1px 0px 0px 0px\n"
"}\n"
"QListView::item:selected{\n"
"		border:none;\n"
"        color:white;\n"
"        background:rgb(34, 175, 75);   \n"
"}\n"
"QListView::item:hover{\n"
"        color:rgb(24, 39, 255);\n"
"		background:rgb(28, 153, 62);\n"
"}\n"
"/*QListView \345\267\246\344\270\212\350\247\222\346\240\267\345\274\217*/\n"
"QListView QTableCornerButton::section {\n"
"   /*  background: red;\n"
"    border: 2px outset red;*/\n"
"    color: red;\n"
"    background-color: rgb(64, 64, 64);\n"
"    border: 5px solid #f6f7fa;\n"
"    border-radius:0px;\n"
"    border-color: rgb(64, 64, 64);\n"
" }\n"
""));
        playlistListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        playlistListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        playlistListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        gridLayout_4->addWidget(playlistListView, 0, 3, 7, 1);


        retranslateUi(McAudioPlayer);

        QMetaObject::connectSlotsByName(McAudioPlayer);
    } // setupUi

    void retranslateUi(QWidget *McAudioPlayer)
    {
        McAudioPlayer->setWindowTitle(QApplication::translate("McAudioPlayer", "McAudioPlayer", nullptr));
        localSongButton->setText(QString());
        networkSearchBackButton->setText(QApplication::translate("McAudioPlayer", "\350\277\224\345\233\236", nullptr));
        networkSearchButton->setText(QString());
        searchSrcBox->setItemText(0, QApplication::translate("McAudioPlayer", "ymusic", nullptr));
        searchSrcBox->setItemText(1, QApplication::translate("McAudioPlayer", "qqmusic", nullptr));

#ifndef QT_NO_ACCESSIBILITY
        playOrderButton->setAccessibleName(QApplication::translate("McAudioPlayer", "shuffle", nullptr));
#endif // QT_NO_ACCESSIBILITY
        playOrderButton->setText(QString());
        previousButton->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        playButton->setAccessibleName(QApplication::translate("McAudioPlayer", "play", nullptr));
#endif // QT_NO_ACCESSIBILITY
        playButton->setText(QString());
        nextButton->setText(QString());
        lyricButton->setText(QApplication::translate("McAudioPlayer", "\350\257\215", nullptr));
        lyricTranslatedButton->setText(QApplication::translate("McAudioPlayer", "\350\257\221", nullptr));
        volumeButton->setText(QString());
        playlistButton->setText(QApplication::translate("McAudioPlayer", "0  ", nullptr));
        searchBackButton->setText(QApplication::translate("McAudioPlayer", "back", nullptr));
        searchNextButton->setText(QApplication::translate("McAudioPlayer", "next", nullptr));
        currentListLabel->setText(QString());
        songNumLabel->setText(QString());
        addSonglistButton->setText(QApplication::translate("McAudioPlayer", "+", nullptr));
        songlistButton->setText(QApplication::translate("McAudioPlayer", "\346\210\221\347\232\204\346\255\214\345\215\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class McAudioPlayer: public Ui_McAudioPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MCAUDIOPLAYER_H
