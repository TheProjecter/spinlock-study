testlock.exe : testlock.cpp alllocks.h microbench.h
	g++-4 testlock.cpp -o testlock -lpthread -Iopa -O2 -g

alllocks.h : abstractlock.h lock1.h lock2.h lock3.h

microbench.h : alllocks.h

timer_test.exe : timer_test.cpp alllocks.h
	g++-4 timer_test.cpp -o timer_test -Iopa -O2 -g

clean :
	rm -f *.exe