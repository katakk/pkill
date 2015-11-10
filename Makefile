CC=/usr/local/mingw32/bin/i386-mingw32-gcc
OBJS=pkill.obj
TARGET=pkill 
INCLUDES=-I/usr/local/mingw32/include/

.SUFFIXES: .cpp .obj

all:	$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	-rm -f $(TARGET) $(OBJS)

.cpp.obj:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

