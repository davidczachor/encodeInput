# this makefile will compile and and all source
# found in the "MyApp" directory.  This represents a sample
# development directory structure and project

# =======================================================
#                  MyApp Example
# =======================================================
# FINAL BINARY Target
./bin/encodeInput : ./obj/encodeInput.o ./obj/convert.o ./obj/utils.o
	cc ./obj/encodeInput.o ./obj/convert.o ./obj/utils.o -o ./bin/encodeInput

#
# =======================================================
#                     Dependencies
# =======================================================
./obj/encodeInput.o : ./src/encodeInput.c ./inc/convert.h ./inc/common.h ./inc/utils.h
	cc -c ./src/encodeInput.c -o ./obj/encodeInput.o

./obj/convert.o : ./src/convert.c ./inc/convert.h ./inc/common.h
	cc -c ./src/convert.c -o ./obj/convert.o

./obj/utils.o : ./src/utils.c ./inc/utils.h ./inc/common.h
	cc -c ./src/utils.c -o ./obj/utils.o

#
# =======================================================
# Other targets
# =======================================================
all : ./bin/encodeInput

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
