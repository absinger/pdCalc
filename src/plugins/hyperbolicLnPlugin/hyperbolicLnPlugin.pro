HOME = ../../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = hyperbolicLnPlugin
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin
QT += widgets

# Input
HEADERS += HyperbolicLnPlugin.h
SOURCES += HyperbolicLnPlugin.cpp

unix:QMAKE_PRE_LINK+=cp $$PWD/../plugins.pdp.unix $$HOME/bin/plugins.pdp

win32{
  IN = $$PWD/../plugins.pdp.win
  IN ~= s,/,\\,g
  OUT = $$HOME/bin/plugins.pdp
  OUT ~= s,/,\\,g
  QMAKE_PRE_LINK+=copy $$IN $$OUT
}
win32:LIBS += -L$$HOME/bin -lpdCalcShared1 -lpdCalcBackend1
