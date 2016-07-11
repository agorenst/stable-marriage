stable-marriage: stable-marriage.cpp
	clang++ -std=c++11 -Wall -g -O3 stable-marriage.cpp -o stable-marriage
	clang++ -std=c++11 -Wall -O3 stable-marriage.cpp -S -o stable-marriage.s

clean:
	rm a.out *~ stable-marriage
