################## My Make File #######################
CC=g++
#OPT_FLAG=-O3
FILE_NAME=nn
FILE_EXTENTION=.cpp
STD=c++11
LIB=-ltiff -lgraph -lm  -L/usr/X11R6/lib/ -lGL -lGLU -lglut
BACK_UP_FILE_NAME=date +%s

compile:
	$(CC) -std=$(STD) $(FILE_NAME)$(FILE_EXTENTION)  -o $(FILE_NAME) $(LIB)
#print : echo "hello"
run_programe:
	./$(FILE_NAME)
make_back_up:
	cp * ../back_up/
clear_ex:
	rm $(FILE_NAME)
clear_all:
	tar -czvf ../back_up/$(BACK_UP_FILE_NAME).tar.gz *;rm *
compiling_with_profiling:
	$(CC) -std=$(STD)  -o $(FILE_NAME) -pg $(FILE_NAME)$(FILE_EXTENTION) $(LIB)

#run_by_gprof:
#	gprof $(FILE_NAME) gmon.out -p
run_by_gprof_profile: 
	gprof $(FILE_NAME) gmon.out -p
run_by_gprof_call_graph: 
	gprof $(FILE_NAME) gmon.out -q
run_by_time:
	time ./$(FILE_NAME) 
