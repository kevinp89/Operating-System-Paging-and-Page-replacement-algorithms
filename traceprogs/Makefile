SRCS = simpleloop.c matmul.c blocked.c mergesort.c
PROGS = simpleloop matmul blocked mergesort

all : $(PROGS)

$(PROGS) : % : %.c
	gcc -Wall -g -o $@ $<


traces: $(PROGS)
	./runit simpleloop
	./runit matmul 100
	./runit blocked 100 25
	./runit mergesort

.PHONY: clean
clean : 
	rm -f mergesort simpleloop matmul blocked tr-*.ref *.marker *~
