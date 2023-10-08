CC=gcc
CFLAGS=-I.

sequential_objects = utils.o find_min_max.o utils.h find_min_max.h
parallel_objects = utils.o find_min_max.o utils.h find_min_max.h

all: sequential_min_max parallel_min_max make_exercise_5

sequential_min_max :  $(sequential_objects)
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : $(parallel_objects)
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

make_exercise_5: exercise5.c
	$(CC) -o make_exercise_5 exercise5.c $(CFLAGS)
	chmod u+x make_exercise_5
	
exercise5.c:
	$(CC) -o make_exercise_5 -c exercise5.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max make_exercise_5
