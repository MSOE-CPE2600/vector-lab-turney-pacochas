CC=gcc
CFLAGS= -Wall
LDFLAGS=
SOURCES= vector.c mini_matlab.c
HEADERS= vector.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE= lab4

all: $(SOURCES) $(EXECUTABLE)

# pull in depencecy info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.depencecy

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d