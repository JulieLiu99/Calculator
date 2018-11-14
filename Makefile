mycalc: mycalc.o evaluate.o 
	g++ mycalc.o evaluate.o -o mycalc
calc.o: mycalc.cpp
	g++ -c mycalc.cpp
evaluate.o: evaluate.cpp evaluate.h
	g++ -c evaluate.cpp
clean:
	rm *.o mycalc

