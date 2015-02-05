CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -Isrc/ -Iinclude/ -g

SRCS = runner.c report.c
OBJS = $(addprefix src/,$(subst .c,.o,$(SRCS)))

sample: samples/simple.o libcriterion.a
	$(LINK.o) -o $@ $< -L. -lcriterion -lcsptr

libcriterion.a: $(OBJS)
	$(AR) cru $@ $^

clean:
	$(RM) $(OBJS) samples/simple.o

distclean: clean
	$(RM) libcriterion.a sample
