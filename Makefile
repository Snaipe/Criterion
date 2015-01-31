CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -Isrc/ -Iinclude/ -g

SRCS = runner.c
OBJS = $(addprefix src/,$(subst .c,.o,$(SRCS)))

sample: samples/simple.o libcriterion.a
	$(LINK.o) -o $@ $^

libcriterion.a: $(OBJS)
	$(AR) rcs $@ $^

clean:
	$(RM) $(OBJS) samples/simple.o

distclean: clean
	$(RM) libcriterion.a sample
