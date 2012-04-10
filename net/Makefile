all:virt mainC

CFLAGS=`pkg-config --cflags --libs gtk+-2.0` -lvirt -g

#virt: virt.o
#	gcc -o $@ $^ -lvirt

#virt.o: virt.c
#	gcc -c $^

virt:virt.c
#virt_list:virt_list.c
mainC:mainC.c

#mainC: mainC.o 
#	gcc -o $@ $^ `pkg-config --libs gtk+-2.0`

#mainC.o: mainC.c
#	gcc -o $@ -c $^ `pkg-config --cflags gtk+-2.0`

.PHONY:clean
clean:
	rm *.o virt mainC virt_list
