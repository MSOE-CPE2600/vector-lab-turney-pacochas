CC=gcc
CFLAGS=-c -Wall 
LDFLAGS=-lm
SOURCES= mini_matlab.c vector.c
HEADERS= vector.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE= lab5

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