all: problem-generator every_stable_marriage compute-stable-marriage
every_stable_marriage: every_stable_marriage.cpp stable-marriage
	clang++ -std=c++11 -Wall -O3 every_stable_marriage.cpp stable-marriage.o -o every_stable_marriage
problem-generator: problem-generator.cpp
	clang++ -std=c++11 -Wall -O3 problem-generator.cpp -o problem-generator
compute-stable-marriage: stable-marriage compute-stable-marriage.cpp
	clang++ -std=c++11 -Wall -O3 compute-stable-marriage.cpp stable-marriage.o -o compute-stable-marriage

stable-marriage: stable-marriage.cpp
	clang++ -std=c++11 -Wall -O3 stable-marriage.cpp -o stable-marriage.o -c

clean:
	rm a.out *~ stable-marriage every_stable_marriage compute-stable-marriage problem-generator *.o
