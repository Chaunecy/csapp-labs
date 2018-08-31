CC = gcc
file = code.c
tar = test

test: $(file)
	$(CC) -o $(tar) $(file)





.PHONY: clean
clean:
	rm $(tar) 
