indentedpaste_sources = $(wildcard indentedpaste/*.c) $(wildcard indentedpaste/*.h)

libindentedpaste.so: ${indentedpaste_sources}
	cc ${indentedpaste_sources} `pkg-config --libs --cflags gedit` -fPIC -shared -o $@

test:
	make -C tests
	tests/run

.PHONY: test
