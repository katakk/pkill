CC=/usr/local/mingw32/bin/i386-mingw32-gcc
CC=/usr/bin/i686-w64-mingw32-gcc
OBJS=pkill.o
TARGET=pkill.exe 
INCLUDES=-I/usr/local/mingw32/include/
LIBS=/usr/i686-w64-mingw32/sys-root/mingw/lib/libpsapi.a

.SUFFIXES: .cpp .o

all:	$(TARGET)
	ln $(TARGET) pgrep.exe
	ln $(TARGET) ps.exe

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	-rm -f $(TARGET) $(OBJS) pgrep.exe ps.exe

.cpp.o:
	mv $< $(<:.cpp=.c)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(<:.cpp=.c)
	mv $(<:.cpp=.c) $<

