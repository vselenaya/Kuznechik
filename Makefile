# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c -Wall -Werror -Wextra

all: kuznechik

kuznechik: main.o kuznechik.o test_speed.o test_correct.o
	$(CC) main.o kuznechik.o test_speed.o test_correct.o -o main

main.o: main.c
	$(CC) $(CFLAGS) main.c

kuznechik.o: kuznechik.c
	$(CC) $(CFLAGS) kuznechik.c

test_speed.o: test_speed.c
	$(CC) $(CFLAGS) test_speed.c

test_correct.o: test_correct.c
	$(CC) $(CFLAGS) test_correct.c

clean:
	rm -rf *.o main