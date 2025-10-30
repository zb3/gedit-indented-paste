indentedpaste_sources = $(wildcard ../indentedpaste/*.c) $(wildcard ../indentedpaste/*.h)

run:
	cc -g test-core.c -I../indentedpaste `pkg-config --libs --cflags glib-2.0 gedit` -Wl,-rpath=/usr/lib64/gedit -o $@

.PHONY: run