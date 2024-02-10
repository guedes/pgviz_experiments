##
# Makezaum
#
# @file
# @version 0.1

pg_shmem_visualizer:
	cc pg_shmem_visualizer.c -I/usr/include/postgresql -o pg_shmem_visualizer  -I./raylib/include -L./raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lpq -ggdb

pg_shmem_visualizer/run: pg_shmem_visualizer
	LD_LIBRARY_PATH=./raylib/lib ./pg_shmem_visualizer
# end
