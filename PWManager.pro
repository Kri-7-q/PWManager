#-------------------------------------------------
#
# Project created by QtCreator 2015-01-14T15:58:43
#
#-------------------------------------------------

QT       += core sql xml

QT       -= gui

TARGET = PWManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Persistence/persistence.cpp \
    UserInterface/consoleinterface.cpp \
    UserInterface/columnwidth.cpp \
    PasswordGenerator/characterdefinition.cpp \
    PasswordGenerator/pwgenerator.cpp \
    ConsoleOptions/appcommand.cpp \
    ConsoleOptions/optiontable.cpp \
    ConsoleOptions/optionparser.cpp \
    ConsoleOptions/optiondefinition.cpp \
    Persistence/credentials.cpp \
    Persistence/postgresql.cpp \
    PasswordGenerator/characterdefinitionlist.cpp \
    Persistence/persistencefactory.cpp \
    Persistence/filepersistence.cpp \
    commandprocessor.cpp \
    Utility/range.cpp \
    SearchAccount/matchobject.cpp \
    SearchAccount/matchstring.cpp

HEADERS += \
    Persistence/persistence.h \
    UserInterface/consoleinterface.h \
    UserInterface/columnwidth.h \
    PasswordGenerator/characterdefinition.h \
    PasswordGenerator/pwgenerator.h \
    ConsoleOptions/appcommand.h \
    ConsoleOptions/optiontable.h \
    ConsoleOptions/optiondefinition.h \
    ConsoleOptions/optionparser.h \
    Persistence/credentials.h \
    Persistence/postgresql.h \
    PasswordGenerator/characterdefinitionlist.h \
    Persistence/persistencefactory.h \
    Persistence/filepersistence.h \
    commandprocessor.h \
    Utility/range.h \
    Utility/sortlist.h \
    SearchAccount/matchobject.h \
    SearchAccount/matchstring.h
