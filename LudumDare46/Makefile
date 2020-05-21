all: main.js

SRC_FILES := $(wildcard *.cc)
HDR_FILES := $(wildcard *.h)
MAP_DIR := asset_dir
TMX_FILES := $(wildcard $(MAP_DIR)/*.tmx)
CSV_FILES := $(patsubst $(MAP_DIR)/%.tmx,$(MAP_DIR)/%.csv,$(TMX_FILES))
CFLAGS := -std=c++17 -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file asset_dir -Wall -Werror

$(MAP_DIR)/%.csv: $(MAP_DIR)/%.tmx
	tiled --export-map $< $@

main.js: $(SRC_FILES) $(HDR_FILES) asset_dir
	em++ $(SRC_FILES) $(CFLAGS) -g -s DEMANGLE_SUPPORT=1 -o $@

clean:
	-rm -r release
