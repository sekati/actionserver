# Sekati ActionServer Makefile
# @author Jason M Horwitz <hello@sekati.com>, Sekati LLC
# @copyright (C) 2004-2012 Jason M Horwitz, Sekati LLC.
# @license http://www.opensource.org/licenses/mit-license.php 

CC=gcc
CFLAGS=-I.
DEPS=daemon.h logging.h
ODIR=./bin
SRC=./src
PREFIX=/usr/local
IDIR=${PREFIX}/bin

actionserver: $(SRC)/daemon.c $(SRC)/logging.c $(SRC)/actionserver.c
	@echo
	@echo Make: Compiling Sekati ActionServer ...
	@echo
	gcc -o $(ODIR)/actionserver $(SRC)/daemon.c $(SRC)/logging.c $(SRC)/actionserver.c

clean:
	@echo
	@echo Make Clean ...
	@echo
	rm -f $(ODIR)/actionserver

install:
	@echo
	@echo Make Install ...
	@echo
	@sudo install -cv ${ODIR}/actionserver ${IDIR}
	@sudo install -cv ${ODIR}/actionserverctl ${IDIR}
	@sudo install -cv ${ODIR}/socketpolicyserver ${IDIR}
	@echo 
	@echo Make Install OK! 
	@echo Run: ${IDIR}/actionserverctl to start the show!
	@echo

.PHONY: install

uninstall:
	@echo
	@echo Make Uninstall ...
	@echo
	@echo Removing install files from ${IDIR} ...
	@echo
	sudo rm -f ${IDIR}/actionserver 
	sudo rm -f ${IDIR}/actionserverctl 
	sudo rm -f ${IDIR}/socketpolicyserver

all:
	@echo
	@echo Make: Making All ...
	@make clean
	@make
	@make install
	@echo

help:
	@echo
	@echo ActionServer Make Help
	@echo
	@echo Build:
	@echo - Type 'make' to compile the actionserver daemon.
	@echo
	@echo Install:
	@echo - Type 'make install' to install the actionserver application binaries in: ${IDIR}.
	@echo
	@echo Run:
	@echo - Run the actionserver control script: ${IDIR}/actionserverctl
	@echo
	@echo Clean:
	@echo - Type 'make clean' to remove previously compiled actionserver binaries.
	@echo
	@echo Uninstall:
	@echo - Type 'make uninstall' to remove installed actionserver binaries from ${IDIR}.
	@echo
