CC=clang

.PHONY: clear fork shm shm_read pipe pthreads workers shm_list shm_list_read usb disks

fork:
	$(CC) -o fork fork.c && ./fork

shm:
	$(CC) -o shm shm.c && ./shm

shm_read:
	$(CC) -o shm_read shm_read.c && ./shm_read

shm_list:
	$(CC) -o shm_list shm_list.c && ./shm_list

shm_list_read:
	$(CC) -o shm_list_read shm_list_read.c && ./shm_list_read

pipe:
	$(CC) -o pipe pipe.c && ./pipe

pthreads:
	$(CC) -o pthreads pthreads.c -lpthread && ./pthreads 10

workers:
	$(CC) -o workers workers.c -lpthread && ./workers

usb:
	$(CC) -o usb usb.c -lusb-1.0 && ./usb

disks:
	$(CC) -o disks disks.c -lsystemd && ./disks

clear:
	rm -rf fork shm shm_read pipe pthreads workers shm_list shm_list_read usb disks