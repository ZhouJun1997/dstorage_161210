
CC=gcc
CPPFLAGS=-I ./include -I /usr/include/fastdfs -I/usr/include/fastcommon
CFLAGS=-Wall
LIBS=-lfdfsclient -lfastcommon

main=./main


target=$(main)


ALL:$(target)


%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

%(main):./%.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	-rm -rf ./*.o $(target) ./test/*.o

distclean:
	-rm -rf ./*.o $(target) ./test/*.o

.PHONY: clean ALL distclean
