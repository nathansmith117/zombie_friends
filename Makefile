all:
	@make -C src
	@make -C src/level_edit
	@make -C src/launcher_dlls
	@make -C src/launcher
	@make -C src/launcher/cmd_launcher
game:
	@make -C src
editor:
	@make -C src/level_edit
clean:
	@make -C src clean
	@make -C src/level_edit clean
	@make -C src/launcher_dlls clean
	@make -C src/launcher clean
	@make -C src/launcher/cmd_launcher clean
debug:
	@make -C src debug
	@make -C src/level_edit debug
	@make -C src/launcher_dlls debug
	@make -C src/launcher debug
	@make -C src/launcher/cmd_launcher debug
%:
	@make -C src $@
