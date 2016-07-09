stable-marriage: stable-marriage.cpp
	clang++ -std=c++11 -Wall -g -O3 stable-marriage.cpp -o stable-marriage

clean:
	rm a.out *~ stable-marriage
