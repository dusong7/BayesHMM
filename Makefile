UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
	CC=g++
	LIBS=-L/usr/local/lib -L/opt/local/lib -I.  -I./headers/
	CFLAGS += -O3 -std=c++11 -fopenmp 
	LDFLAGS=-lboost_serialization# -larmadillo # -lnlopt -lm
    endif
    ifeq ($(UNAME_S),Darwin)
	CC=/opt/local/bin/g++-mp-4.8
    CFLAGS += -O3 -std=c++11 -fopenmp
  	LIBS=-L/usr/local/lib -L/opt/local/lib -I. -I./headers/ -I/usr/local/include/
	LDFLAGS=-lboost_serialization# -larmadillo # -lnlopt -lm
    endif



main: main.cpp
	$(CC) $(LIBS) $(CFLAGS) main.cpp -o test $(LDFLAGS)


trace: trace.cpp
	$(CC) $(LIBS) $(CFLAGS) trace.cpp -o trace $(LDFLAGS)


plot_tree: plot_tree.cpp
	$(CC) $(LIBS) $(CFLAGS) plot_tree.cpp -o plot_tree $(LDFLAGS)