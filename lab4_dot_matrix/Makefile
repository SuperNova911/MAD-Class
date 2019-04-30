obj-m = dot_matrix_driver.o

PWD = $(shell pwd)

all: driver app

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
	gcc -o test_dot_matrix test_dot_matrix.c -Wall -static

clean:
	rm -rf *.ko *.mod.* *.o test_dot_matrix Module.symvers modules.order .dot_matrix* .tmp*
