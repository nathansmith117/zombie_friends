FLCON = fltk-config
COMPILER = $(shell $(FLCON) --cxx)
CFLAGS = $(shell $(FLCON) --use-images --cxxflags)
LDFLAGS = $(shell $(FLCON) --use-images --ldstaticflags) \
	  -static-libstdc++ -static-libgcc

TARGET = ../zombie_friends
DEH = program_data.h
OBJS = main.o appWindow.o player.o image_loader.o game_tools.o weapon.o \
       sword.o map.o tile.o debug_window.o view_window.o top_menu.o \
       character.o tab_menu.o game_tab_menu.o chat_box.o game_commands.o \
       common_item.o common_tool.o player_info_display.o \
       submachine_gun.o gun.o
GAME_TOOLS = image_loader.o game_tools.o weapon.o map.o tile.o player.o \
	     debug_window.o view_window.o top_menu.o character.o tab_menu.o \
	     chat_box.o game_commands.o common_item.o common_tool.o \
	     player_info_display.o
WEAPONS = weapon.o sword.o submachine_gun.o gun.o
LIB_DIR = ../lib/

%.o: %.cpp
	@echo compiling $<
	@$(COMPILER) $(CFLAGS) -c -o $@ $<
$(TARGET): $(OBJS)
	@echo making exe
	@$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo Creating libgametools.a
	@ar rcs -o $(LIB_DIR)libgametools.a $(GAME_TOOLS)
	@echo Creating libweapons.a
	@ar rcs $(LIB_DIR)libweapons.a $(WEAPONS)
editor:
	@make -C level_edit
debug:
	$(COMPILER) $(CFLAGS) -c *.cpp -g
	$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) -g

# Objects.
main.o: *.cpp *.h
appWindow.o: appWindow.cpp appWindow.h map.cpp map.h debug_window.cpp debug_window.h view_window.cpp view_window.h \
	  player_info_display.cpp player_info_display.h $(DEH)
player.o: player.cpp player.h game_tools.cpp game_tools.h weapon.cpp weapon.h map.cpp map.h character.cpp character.h \
	player_info_display.cpp player_info_display.h common_tool.cpp common_tool.h $(DEH)
image_loader.o: image_loader.cpp image_loader.h $(DEH)
game_tools.o: game_tools.cpp game_tools.h $(DEH)
weapon.o: weapon.cpp weapon.h common_item.cpp common_item.h common_tool.cpp common_tool.h character.cpp character.h $(DEH)
sword.o: sword.cpp sword.h weapon.cpp weapon.h character.cpp character.h $(DEH)
map.o: map.cpp map.h tile.cpp tile.h game_tools.cpp game_tools.h common_item.cpp common_item.h chat_box.cpp chat_box.h $(DEH)
tile.o: tile.cpp tile.h $(DEH)
debug_window.o: debug_window.cpp debug_window.h game_tools.cpp game_tools.h character.cpp character.h $(DEH)
view_window.o: view_window.cpp view_window.h character.cpp character.h tab_menu.cpp tab_menu.h chat_box.cpp \
	chat_box.h   player.cpp player.h $(DEH)
top_menu.o: top_menu.cpp top_menu.h debug_window.cpp debug_window.h view_window.cpp view_window.h \
	game_tools.cpp game_tools.h $(DEH)
character.o: character.cpp character.h weapon.cpp weapon.h common_item.cpp common_item.h \
	  game_tools.cpp game_tools.h map.cpp map.h \
	common_tool.cpp common_tool.h gun.cpp gun.h sword.cpp sword.h submachine_gun.cpp submachine_gun.h $(DEH)
tab_menu.o: tab_menu.cpp tab_menu.h $(DEH)
game_tab_menu.o: game_tab_menu.cpp game_tab_menu.h tab_menu.cpp tab_menu.h debug_window.cpp debug_window.h \
	chat_box.cpp chat_box.h game_commands.cpp game_commands.h player_info_display.cpp player_info_display.h $(DEH)
chat_box.o: chat_box.cpp chat_box.h game_tools.cpp game_tools.h $(DEH)
game_commands.o: game_commands.cpp game_commands.h chat_box.cpp chat_box.h game_tools.cpp game_tools.h \
	map.cpp map.h view_window.cpp view_window.h player.cpp player.h $(DEH)
common_item.o: common_item.cpp common_item.h game_tools.cpp game_tools.h map.cpp map.h $(DEH)
common_tool.o: common_item.cpp common_item.h $(DEH)
player_info_display.o: player_info_display.cpp player_info_display.h common_item.cpp common_item.h common_tool.cpp common_tool.h \
	player.cpp player.h $(DEH)
submachine_gun.o: weapon.cpp weapon.h game_tools.cpp game_tools.h character.cpp character.h gun.cpp gun.h $(DEH)
gun.o: weapon.cpp weapon.h game_tools.cpp game_tools.h character.cpp character.h map.cpp map.h   \
	tile.cpp tile.h common_item.cpp common_item.h $(DEH)

clean:
	rm *.o
	rm $(TARGET)
	rm $(LIB_DIR)*.a
