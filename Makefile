FLAGS = -Werror

*.exe: *.o *.h
	g++ -o main.exe *.o

*.o: *.cpp *.h
	g++ $(FLAGS) -c *.cpp

clean:
	rm *.o
	rm main.exe