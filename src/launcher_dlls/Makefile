FLCON = fltk-config
COMPILER = $(shell $(FLCON) --cxx)
CFLAGS = $(shell $(FLCON) --use-images --cxxflags) -I../ -fPIC
GAME_LDFLAGS = -lzombie_friends -L../../lib
LDFLAGS = $(GAME_LDFLAGS) $(shell $(FLCON) --use-images --ldstaticflags) -L../../lib/ \
	  -static-libstdc++ -static-libgcc
DLLFLAGS = -Wl,--subsystem,windows,--kill-at

LAUNCHER_DLLS_DIR = ../../launcher_dlls/
TARGET = $(LAUNCHER_DLLS_DIR)zombie_friends.dll
OBJS = game.o

%.o: %.cpp
	@echo compiling $<
	@$(COMPILER) $(CFLAGS) -c -o $@ $<
$(TARGET): $(OBJS)
	@echo Creating $(TARGET)
	@$(COMPILER) $(CFLAGS) -shared -o $(TARGET) $(OBJS) $(LDFLAGS) $(DLLFLAGS)
debug:
	$(COMPILER) $(CFLAGS) -c *.cpp -g
	$(COMPILER) $(CFLAGS) -shared -o $(TARGET) $(OBJS) $(LDFLAGS) $(DLLFLAGS) -g


game.o: game.cpp game.h

clean:
	rm *.o
	rm $(TARGET)
