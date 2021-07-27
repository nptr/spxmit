EXECUTABLE 	  := spxmit
CROSS_COMPILE := 
CC            := $(CROSS_COMPILE)gcc
LIBS		  :=
CFLAGS        := -Wall
LDFLAGS       := 

SRC	:= $(wildcard *.c)
OBJ := $(SRC:.c=.o)

all: CFLAGS += -O2
all: $(EXECUTABLE)

debug: CFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	@echo linking... $(notdir $@)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.c
	@echo compiling... $(notdir $<)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)