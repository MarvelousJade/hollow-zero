# Hollow Zero

A fast-paced 2D action game built from scratch in C++20 with SDL2. Fight a boss with 12+ dynamic behavior states using multi-directional attacks, dodge rolls, and bullet-time mechanics.

https://github.com/user-attachments/assets/5e42358e-ede2-4be3-ba68-1fa83491453b

## Features

- **Fluid Combat** — 4-directional attacks with visual feedback
- **Advanced Boss AI** — Adapts behavior based on health and positioning (sword throws, barb rain, silk traps, dash attacks)
- **Bullet-Time** — Slow-motion for precise timing
- **Physics-Based Movement** — Gravity, momentum, rolling with i-frames
- **Game Feel** — Camera shake, particle effects, spatial audio

## Tech Stack

- **C++20** — State machine architecture, component-based design, singleton managers
- **SDL2** — Rendering, input, audio (SDL2_image, SDL2_mixer, SDL2_ttf)
- **Custom Engine** — No game framework, everything built from scratch

## Build

```bash
# Native
make && make run

# WebAssembly (requires Emscripten SDK)
make web
```

### Dependencies

```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

# macOS
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf

# Windows (MSYS2)
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_ttf
```
