CC = gcc -std=c11
CFLAGS = -Wall -Werror -Wextra
name = game\ of\ life

$(name):
	$(CC) $(CFLAGS) $(name).c -o $(name)

clean:
	rm -r $(name)

rebuild: clean $(name)