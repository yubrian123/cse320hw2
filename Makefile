all:
	gcc main.c fake_mem.o -o defrag_tool
clean:
	_rm -f defrag_tool
