#include <SDL.h>
#include <libtcod.hpp>

/// Return the data directory.
auto get_data_dir() -> std::filesystem::path {
  static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
  while (!std::filesystem::exists(root_directory / "res")) {
    // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
    root_directory /= "..";
    if (!std::filesystem::exists(root_directory)) {
      throw std::runtime_error("Could not find the data directory.");
    }
  }
  return root_directory / "res";
};

int main(int argc, char **argv) {
  TCODConsole console(80, 25);
  TCOD_ContextParams params{};

  params.tcod_version = TCOD_COMPILEDVERSION;  // This is required.
  params.console = console.get();  // Derive the window size from the console size.
  params.window_title = "Libtcod Project";
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.vsync = true;
  params.argc = argc;  // This allows some user-control of the context.
  params.argv = argv;

  // Tileset example using a Code Page 437 font.
  // "terminal8x8_gs_ro.png" must be in the working directory.
  auto tileset = tcod::load_tilesheet(get_data_dir() / "curses_640x300.png", {16, 16}, tcod::CHARMAP_CP437);
  params.tileset = tileset.get();

  auto context = tcod::Context(params);

  while (1) {  // Game loop.
    TCOD_console_clear(console.get());
    static constexpr std::array<int, 9> LEGEND = {TCOD_CHARMAP_CP437[218], TCOD_CHARMAP_CP437[196], TCOD_CHARMAP_CP437[191], TCOD_CHARMAP_CP437[179], ' ', TCOD_CHARMAP_CP437[179], TCOD_CHARMAP_CP437[192], TCOD_CHARMAP_CP437[196], TCOD_CHARMAP_CP437[217]};
    tcod::draw_frame(console, {0, 0, 20, 10}, LEGEND, {{255, 255, 255}}, {{0, 0, 0}});  
    tcod::print(console, {1, 1}, "Hello World", std::nullopt, std::nullopt);
  
    context.present(console);  // Updates the visible display.

    SDL_Event event;
    SDL_WaitEvent(nullptr);  // Optional, sleep until events are available.
    while (SDL_PollEvent(&event)) {
      context.convert_event_coordinates(event);  // Optional, converts pixel coordinates into tile coordinates.
      switch (event.type) {
        case SDL_QUIT:
          return 0;  // Exit.
      }
    }
  }
}