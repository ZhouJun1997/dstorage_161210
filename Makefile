
CC=gcc
CPPFLAGS= -I./include -I/usr/include/fastdfs -I/usr/include/fastcommon 
CFLAGS=-Wall 
LIBS= -lfdfsclient -lfastcommon

main=./main


target=$(main) 


ALL:$(target)


#生成所有的.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#main程序
$(main):./main.o ./fdfs_api.o make_log.o
	$(CC) $^ -o $@ $(LIBS)


#clean指令

clean:
	-rm -rf ./*.o $(target) ./test/*.o

distclean:
	-rm -rf ./*.o $(target) ./test/*.o

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
