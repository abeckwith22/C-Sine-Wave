main: app.c
	cc $? -o $@ -lm 

clean:
	rm -f *.o