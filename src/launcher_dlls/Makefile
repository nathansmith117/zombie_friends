FLCON = fltk-config
COMPILER = $(shell $(FLCON) --cxx)
CFLAGS = $(shell $(FLCON) --use-images --cxxflags) -I../ -fPIC
GAME_LDFLAGS =  ../*.o
LDFLAGS = $(GAME_LDFLAGS) $(shell $(FLCON) --use-images --ldstaticflags) -L../../lib/

DEH = ../*.cpp ../*.h
OBJS = zombie_friends.o
TARGET = zombie_friends.so
LAUNCHER_DLLS_DIR = ../../launcher_dlls/

%.o: %.cpp
	@echo compiling $<
	@$(COMPILER) $(CFLAGS) -c -o $@ $<
$(TARGET): $(OBJS)
	@echo making $(TARGET)
	@$(COMPILER) $(CFLAGS) -shared -o $(LAUNCHER_DLLS_DIR)$(TARGET) $(OBJS) $(LDFLAGS)
debug:
	$(COMPILER) $(CFLAGS) -c *.cpp -g
	$(COMPILER) $(CFLAGS) -shared -o $(LAUNCHER_DLLS_DIR)$(TARGET) $(OBJS) $(LDFLAGS) -g

# Objects.
zombie_friends.o: $(DEH)

clean:
	rm *.o
	rm $(LAUNCHER_DLLS_DIR)$(TARGET)
