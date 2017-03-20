QT       += core gui
QT       += sql

CONFIG += release
CONFIG -= debug

TARGET = ErrorCheck
TEMPLATE = app

include(Src/ProEntry/ProEntry.pri)
include(Src/LogicalLayer/CustomInfoClass/CustomInfoClass.pri)
include(Src/LogicalLayer/DataAnalysis/DataAnalysis.pri)
include(Src/LogicalLayer/Database/Database.pri)
include(Src/LogicalLayer/Can/Can.pri)
include(Src/UiLayer/FailureRecord/FailureRecord.pri)
include(Src/UiLayer/GCU/GCU.pri)
include(Src/UiLayer/ImportExport/ImportExport.pri)
include(Src/UiLayer/LadderDiagram/LadderDiagram.pri)
include(Src/UiLayer/LCU/LCU.pri)
include(Src/UiLayer/MainInterface/MainInterface.pri)
include(Src/UiLayer/DCU/DCU.pri)
include(Src/UiLayer/Schematic/Schematic.pri)
include(Src/UiLayer/CustomMsgBox/CustomMsgBox.pri)
include(Src/UiLayer/Log/Log.pri)
include(Src/LogicalLayer/CBaseOperate/CBaseOperate.pri)

INCLUDEPATH += \
    $$PWD/Src/ProEntry \
    $$PWD/Src/LogicalLayer/CustomInfoClass \
    $$PWD/Src/LogicalLayer/DataAnalysis \
    $$PWD/Src/LogicalLayer/Database \
    $$PWD/Src/LogicalLayer/Can \
    $$PWD/Src/UiLayer/FailureRecord \
    $$PWD/Src/UiLayer/GCU \
    $$PWD/Src/UiLayer/ImportExport \
    $$PWD/Src/UiLayer/LadderDiagram \
    $$PWD/Src/UiLayer/LCU \
    $$PWD/Src/UiLayer/MainInterface \
    $$PWD/Src/UiLayer/DCU \
    $$PWD/Src/UiLayer/Schematic\
    $$PWD/Src/UiLayer/CustomMsgBox\
    $$PWD/Src/UiLayer/Log \
    $$PWD/Src/LogicalLayer/CBaseOperate

RESOURCES += \
    Qrc/Image.qrc

HEADERS +=

SOURCES +=
