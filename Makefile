

all: libbuddy.a app


libbuddy.a: 	buddy.c
	gcc -Wall -c buddy.c -lm
	ar -cvq libbuddy.a buddy.o
	ranlib libbuddy.a

app: 	app.c
	gcc -Wall -o app app.c  -L. -lbuddy -lm


clean: 
	rm -fr *.o *.a *~ a.out app
