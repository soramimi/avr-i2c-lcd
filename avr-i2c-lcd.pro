
INCLUDEPATH += /usr/lib/avr/include
INCLUDEPATH += /usr/avr/include/

DEFINES += __AVR_ATmega32U2__

HEADERS += \
    avrgpio.h \
    lcd.h
SOURCES += \
    main.cpp \
    lcd.cpp
