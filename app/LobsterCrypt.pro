QT += core gui widgets

TARGET = LobsterCrypt
TEMPLATE = app
CONFIG += c++17

# Указываем, где искать файлы
SOURCES += \
    src/main.cpp \
    src/main_window.cpp \
    src/core.cpp

HEADERS += \
    src/main_window.h \
    src/core.h

# Если у тебя есть файл интерфейса
FORMS += \
    src/main_window.ui

# Чтобы не мусорить, создадим папки для сборки
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
