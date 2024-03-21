CC=clang

.PHONY: clear fork shm shm_read

fork:
	$(CC) -o fork fork.c && ./fork

shm:
	$(CC) -o shm shm.c && ./shm

shm_read:
	$(CC) -o shm_read shm_read.c && ./shm_read

clear:
	rm -rf fork shm shm_read