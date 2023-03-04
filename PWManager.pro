QT -= gui
QT += sql

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ConsoleOptions/appcommand.cpp \
        ConsoleOptions/optiondefinition.cpp \
        ConsoleOptions/optionparser.cpp \
        ConsoleOptions/optiontable.cpp \
        PasswordGenerator/characterdefinition.cpp \
        PasswordGenerator/characterdefinitionlist.cpp \
        PasswordGenerator/pwgenerator.cpp \
        Persistence/credentials.cpp \
        Persistence/filepersistence.cpp \
        Persistence/persistence.cpp \
        Persistence/persistencefactory.cpp \
        Persistence/postgresql.cpp \
        SearchAccount/matchobject.cpp \
        SearchAccount/matchstring.cpp \
        UserInterface/columnwidth.cpp \
        UserInterface/consoleinterface.cpp \
        Utility/range.cpp \
        main.cpp \
        commandprocessor.cpp

HEADERS += \
        ConsoleOptions/appcommand.h \
        ConsoleOptions/optiondefinition.h \
        ConsoleOptions/optionparser.h \
        ConsoleOptions/optiontable.h \
        PasswordGenerator/characterdefinition.h \
        PasswordGenerator/characterdefinitionlist.h \
        PasswordGenerator/pwgenerator.h \
        Persistence/credentials.h \
        Persistence/filepersistence.h \
        Persistence/persistence.h \
        Persistence/persistencefactory.h \
        Persistence/postgresql.h \
        SearchAccount/matchobject.h \
        SearchAccount/matchstring.h \
        UserInterface/columnwidth.h \
        UserInterface/consoleinterface.h \
        Utility/range.h \
        Utility/sortlist.h \
        commandprocessor.h

TRANSLATIONS += \
    PWManager_de_DE.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
