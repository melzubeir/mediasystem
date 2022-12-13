# -------------------------------------------------
# Project created by QtCreator 2009-08-20T00:31:05
# -------------------------------------------------
QT += sql
TARGET = "Clipping Station"
TEMPLATE = app
SOURCES += main.cpp \
    clippingstation.cpp \
    authenticationdialog.cpp \
    globalfunctions.cpp \
    Importer/importerdialog.cpp \
    Importer/imageoutputformatdialog.cpp \
    section.cpp \
    user.cpp \
    publication.cpp \
    statusbar.cpp \
    tag.cpp \
    pushbuttondelegate.cpp \
    tagswrapper.cpp \
    imageitem.cpp \
    fullpagescene.cpp \
    imagecachingdialog.cpp \
    imagecachingthread.cpp \
    fullpageview.cpp \
    issue.cpp \
    drawerscene.cpp \
    pageitem.cpp \
    droppeditem.cpp \
    drawerviewstackedwidget.cpp \
    drawerview.cpp \
    pagemargin.cpp \
    droppeditemzoomdialog.cpp \
    systemconfiguration.cpp \
    thumbnail.cpp \
    article.cpp \
    importerthread.cpp \
    imagecutout.cpp \
    tagcoordinates.cpp \
    coordinates.cpp \
    prominencedialog.cpp \
    prominence.cpp \
    pagetextdialog.cpp \
    wordcoordinates.cpp \
    searchpublicationlistdialog.cpp \
    digitalclock.cpp
HEADERS += clippingstation.h \
    authenticationdialog.h \
    Importer/importerdialog.h \
    Importer/imageoutputformatdialog.h \
    section.h \
    user.h \
    publication.h \
    globalfunctions.h \
    constants.h \
    statusbar.h \
    tag.h \
    pushbuttondelegate.h \
    tagswrapper.h \
    fullpagescene.h \
    fullpageview.h \
    imagecachingthread.h \
    imagecachingdialog.h \
    imageitem.h \
    issue.h \
    drawerscene.h \
    drawerviewstackedwidget.h \
    drawerview.h \
    pagemargin.h \
    pageitem.h \
    droppeditem.h \
    droppeditemzoomdialog.h \
    systemconfiguration.h \
    thumbnail.h \
    article.h \
    importerthread.h \
    imagecutout.h \
    tagcoordinates.h \
    coordinates.h \
    prominencedialog.h \
    prominence.h \
    pagetextdialog.h \
    wordcoordinates.h \
    searchpublicationlistdialog.h \
    digitalclock.h
FORMS += clippingstation.ui \
    authenticationdialog.ui \
    Importer/importerdialog.ui \
    Importer/imageoutputformatdialog.ui \
    imagecachingdialog.ui \
    droppeditemzoomdialog.ui \
    prominencedialog.ui \
    pagetextdialog.ui \
    searchpublicationlistdialog.ui
ICON = socialhose.icns
RC_FILE = ClippingStation.rc
win32 {
    LIBS += -L"C:\curl-7.19.6\bin\"
    INCLUDEPATH += "C:\curl-7.19.6\include\"
}
LIBS += -lcurl
RESOURCES += icons.qrc
