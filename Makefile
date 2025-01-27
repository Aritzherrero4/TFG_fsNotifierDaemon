CC = g++
CFLAGS = -std=c++17 -lstdc++fs -Wall -lsystemd -O3
IDIR = ./include/
SDIR = ./src/
ODIR = ./obj/
BDIR = ./bin/

main: $(SDIR)fsNotifierDaemon.cpp $(IDIR)fsNotifierDaemon.hpp $(ODIR)notifier.o $(ODIR)utils.o $(IDIR)utils.hpp
	@echo Compiling fsNotifierDaemon...
	@g++ -o fsNotifierDaemon $(SDIR)fsNotifierDaemon.cpp $(ODIR)notifier.o $(ODIR)utils.o $(CFLAGS)
	@echo Done

$(ODIR)utils.o: $(SDIR)utils.cpp $(IDIR)utils.hpp
	@echo Compiling utils...
	@g++ -c $(SDIR)utils.cpp -o $(ODIR)utils.o -std=c++17 -lstdc++fs -Wall

$(ODIR)notifier.o: $(SDIR)notifier.cpp $(IDIR)notifier.hpp
	@mkdir -p $(ODIR) $(LDIR)
	@echo Compiling notifier...
	@g++ -c $(SDIR)notifier.cpp -o $(ODIR)notifier.o $(CFLAGS)

.PHONY : clean, fresh
fresh: clean main
clean :
	@echo Cleaning object files and the executable...
	@rm -f $(ODIR)*
	@rm -f fsNotifierDaemon
	@echo Done

