EXEC=os_test

CC=g++
CFLAGS=-std=c++98 -I.


%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

${EXEC}: main.o
	g++ -o ${EXEC} main.o -I. -lpthread 


clean:
	rm -rf ./${EXEC}
	rm -rf ./*.o
