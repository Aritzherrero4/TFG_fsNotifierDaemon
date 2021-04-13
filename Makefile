CC = g++
CFLAGS = -std=c++17 -lstdc++fs -Wall -lsystemd
IDIR = ./include/
SDIR = ./src/
ODIR = ./obj/
BDIR = ./bin/

main: $(SDIR)fsNotifierDaemon.cpp $(IDIR)fsNotifierDaemon.hpp $(ODIR)notifier.o $(ODIR)utils.o $(IDIR)utils.hpp
	g++ -o fsNotifierDaemon $(SDIR)fsNotifierDaemon.cpp $(ODIR)notifier.o $(ODIR)utils.o $(CFLAGS)

$(ODIR)utils.o: $(SDIR)utils.cpp $(IDIR)utils.hpp
	g++ -c $(SDIR)utils.cpp -o $(ODIR)utils.o -std=c++17 -lstdc++fs -Wall

$(ODIR)notifier.o: $(SDIR)notifier.cpp $(IDIR)notifier.hpp
	g++ -c $(SDIR)notifier.cpp -o $(ODIR)notifier.o $(CFLAGS)

.PHONY : clean
clean :
	rm $(ODIR)*
	rm fsNotifierDaemon

