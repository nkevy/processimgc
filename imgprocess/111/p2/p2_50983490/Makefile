EXEC=p2_exec
CC=g++
CFLAGS= -std=c++98 -I.

#----------------------------------------------
# complete this part
DEPS = 
OBJ = main.cpp types_p2.cpp p2_threads.cpp utils.cpp
%.o: %.cpp $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)
#----------------------------------------------

#----------------------------------------------
# complete this part
${EXEC}: $(OBJ)
	@$(CC) -o ${EXEC} $^ $(CFLAGS) -I. -lpthread 
#----------------------------------------------


clean:
	rm -rf ./${EXEC}
	rm -rf ./*.o
