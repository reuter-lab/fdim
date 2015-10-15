ifeq ($(OS),Windows_NT)
    include Makefile.mingw
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        include Makefile.linux
    endif
    ifeq ($(UNAME_S),Darwin)
        include Makefile.darwin
    endif
endif
