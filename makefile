# Copyright 2023 CSCE 311

cc = g++  # use g++ compiler
path = ..
standard = c++17

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -g  # include gdb instrumentation
flags += -I $(path)  # add parent to preprocessor include path

link = $(cc) $(flags) -o  # final linked build to binary executable

compile = $(cc) $(flags) -c  # compilation to intermediary .o files


# executables
#

all: text-server text-client


text-client: src/text-client.cc bin/client.o
	$(link) $@ $^

text-server: src/text-server.cc bin/server.o
	$(link) $@ $^

style: inc/*.h src/*.cpp
	cpplint --root=$(path) $^

bin/client.o: src/client.cc inc/client.h
	$(compile) $< -o $@

bin/server.o: src/server.cc inc/server.h inc/unix_domain_socket.h
	$(compile) $< -o $@

clean:
	$(RM) bin/*.o text-client text-server
	rm -r *.dSYM
