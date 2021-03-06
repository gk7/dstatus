PROG 		= dstatus
CC			= gcc
PREFIX 		= /usr/bin

# Comment if you don't want mpd information
MPDLIB 		= -lmpdclient
MPDFLAG 	= -DMPD

# Comment if you don't want to display clock
CLKFLAG		= -DCLK

LIBS		= -lasound -lX11 ${MPDLIB}
CPPFLAGS	= ${MPDFLAG} ${CLKFLAG}
CFLAGS		= -Os -Wall -Wextra ${CPPFLAGS}

${PROG}: ${PROG}.c
		@${CC} ${CFLAGS} ${LIBS} -o ${PROG} ${PROG}.c
		@strip ${PROG}

clean:
		rm -f ${PROG}

install:
		install -Dm755 ${PROG} ${DESTDIR}${PREFIX}/${PROG}

uninstall:
		rm -f ${PREFIX}/${PROG}
