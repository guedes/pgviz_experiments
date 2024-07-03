##
# @Makefile
# @version 0.1

pgviz_buffercache:
	cc pgviz_buffercache.c -I/usr/include/postgresql -o pgviz_buffercache  -I./raylib/include -L./raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lpq -ggdb

pgviz_buffercache/run: pgviz_buffercache
	LD_LIBRARY_PATH=./raylib/lib ./pgviz_buffercache --conninfo "host=/var/run/postgresql port=5437"

pgviz_logs:
	cc pgviz_logs.c -I/usr/include/postgresql -o pgviz_logs  -I./raylib/include -L./raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lpq -ggdb

pgviz_logs/run: pgviz_logs
	LD_LIBRARY_PATH=./raylib/lib ./pgviz_logs --log-file "./postgres.log"

# end
