all: problem_generator every_stable_marriage compute_stable_marriage
every_stable_marriage: every_stable_marriage.cpp stable_marriage
	clang++ -std=c++11 -Wall -O3 every_stable_marriage.cpp stable_marriage.o -o every_stable_marriage
problem_generator: problem_generator.cpp
	clang++ -std=c++11 -Wall -O3 problem_generator.cpp -o problem_generator
compute_stable_marriage: stable_marriage compute_stable_marriage.cpp
	clang++ -std=c++11 -Wall -O3 compute_stable_marriage.cpp stable_marriage.o -o compute_stable_marriage

stable_marriage: stable_marriage.cpp
	clang++ -std=c++11 -Wall -O3 stable_marriage.cpp -o stable_marriage.o -c

clean:
	rm a.out *~ stable_marriage every_stable_marriage compute_stable_marriage problem_generator *.o
