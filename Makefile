##
# @Makefile
# @version 0.1

pg_shmem_visualizer:
	cc pgviz_buffercache.c -I/usr/include/postgresql -o pgviz_buffercache  -I./raylib/include -L./raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lpq -ggdb

pg_shmem_visualizer/run: pg_shmem_visualizer
	LD_LIBRARY_PATH=./raylib/lib ./pgviz_buffercache
# end
