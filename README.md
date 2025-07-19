# Katana Zero - 2D Action Game

A fast-paced 2D action game built from scratch in C++ using SDL2, featuring fluid combat mechanics, advanced AI behavior, and polished game feel elements.

## 🎮 Game Overview

https://github.com/user-attachments/assets/5e42358e-ede2-4be3-ba68-1fa83491453b

### Key Features
- **Fluid Combat System**: Multi-directional attacks with visual feedback and combo potential
- **Advanced AI Boss**: Dynamic behavior patterns that adapt based on health and positioning
- **Bullet-Time Mechanics**: Slow-motion gameplay for precise timing and tactical advantage
- **Physics-Based Movement**: Realistic gravity, momentum, and collision detection
- **Rich Audio-Visual Feedback**: Dynamic camera shake, particle effects, and spatial audio

## 🛠️ Technical Implementation

### Architecture & Design Patterns
- **State Machine Pattern**: Robust character behavior management with 12+ enemy states and 7 player states
- **Component-Based Architecture**: Modular systems for animation, collision, physics, and audio
- **Singleton Pattern**: Centralized resource and system management
- **Observer Pattern**: Event-driven collision and timer systems

### Core Systems

#### **Character System**
- Inheritance-based hierarchy with shared `Character` base class
- Separate hit/hurt collision boxes for complex interaction mechanics
- HP management with visual and audio feedback
- Invulnerability frames and status effects

#### **State Machine Framework**
- Generic state system supporting complex AI behaviors
- Smooth state transitions with enter/update/exit lifecycle
- Timer-based and condition-based state switching
- Extensible architecture for easy behavior addition

#### **Animation System**
- Frame-based sprite animation with configurable timing
- Bidirectional animation support with automatic sprite flipping
- VFX system with particle effects and screen shake
- Asset management with texture atlases

#### **Physics & Collision**
- Layer-based collision detection with selective interaction
- Gravity simulation with configurable parameters
- Boundary constraints and screen wrapping
- Projectile physics with trajectory calculation

#### **AI Behavior**
- HP-dependent behavior modification
- Probabilistic decision making for unpredictable encounters
- Spatial awareness and player tracking
- Multiple attack patterns (projectiles, dash attacks, area denial)

## 🎯 Gameplay Mechanics

### Player Abilities
- **Multi-directional Combat**: Attack in 4 directions with visual feedback
- **Agile Movement**: Running, jumping, and rolling with momentum
- **Bullet-Time**: Slow-motion mode for tactical advantage
- **Combo System**: Chain attacks for maximum effectiveness

### Boss AI Features
- **Adaptive Difficulty**: Behavior changes based on remaining health
- **Varied Attack Patterns**: Sword throws, barb rain, silk traps, and dash attacks
- **Intelligent Positioning**: Uses environment and player location strategically
- **Dynamic State Transitions**: Fluid switching between 12 unique behavior states

### Combat Mechanics
- **Precision Timing**: Frame-perfect dodge and attack windows
- **Spatial Awareness**: Position-based tactics and area control
- **Risk/Reward Balance**: Aggressive play rewarded, mistakes punished
- **Visual Clarity**: Clear attack telegraphs and feedback systems

## 🔧 Technical Specifications

**Language**: C++20  
**Graphics**: SDL2 with custom rendering pipeline  
**Audio**: SDL2_mixer with spatial audio support  
**Build System**: Make with cross-platform compatibility  
**Architecture**: Component-based with modern C++ practices  

### Dependencies
- SDL2 (Core graphics and input)
- SDL2_image (Texture loading)
- SDL2_mixer (Audio playback)
- SDL2_ttf (Text rendering)

### Performance Features
- **60+ FPS**: Optimized rendering and update loops
- **Memory Management**: RAII principles and proper resource cleanup
- **Asset Streaming**: Efficient texture and audio loading
- **Collision Optimization**: Spatial partitioning for complex interactions

## 🚀 Building and Running

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

# macOS (with Homebrew)
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf

# Windows (MSYS2)
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_ttf
```

### Build Commands
```bash
# Build release version
make

# Run the game
make run

# Build debug version
make debug

# Memory leak detection
make valgrind

# Generate compile_commands.json for IDE support
make clangd

# Clean build files
make clean
```

## 🎨 Game Feel & Polish

### Visual Effects
- **Screen Shake**: Dynamic camera shake for impact feedback
- **Particle Systems**: Attack effects, landing particles, dash trails
- **Animation Blending**: Smooth transitions between character states
- **Visual Clarity**: Clear distinction between gameplay elements

### Audio Design
- **Spatial Audio**: Position-based sound effects
- **Dynamic Music**: Adaptive soundtrack responding to gameplay
- **Impact Feedback**: Satisfying hit sounds and environmental audio
- **Audio Mixing**: Balanced levels with volume controls

### User Experience
- **Responsive Controls**: Frame-perfect input handling
- **Clear Feedback**: Visual and audio confirmation for all actions
- **Consistent Feel**: Unified art style and interaction patterns
- **Accessibility**: Keyboard and mouse support with customizable bindings

## 📈 Learning Outcomes & Skills Demonstrated

### Game Programming Concepts
- **Game Loop Architecture**: Update/render separation with fixed timestep
- **Resource Management**: Efficient loading and cleanup of game assets
- **Input Handling**: Responsive control schemes with state management
- **Game Balance**: Tuning mechanics for engaging gameplay experience

### Software Engineering Practices
- **Clean Architecture**: Separation of concerns and modular design
- **Design Patterns**: Appropriate pattern usage for maintainable code
- **Performance Optimization**: Memory and CPU efficiency considerations
- **Cross-Platform Development**: Portable C++ with SDL2 abstraction

### Problem-Solving Skills
- **Complex Systems Integration**: Coordinating multiple game systems
- **Debugging Techniques**: Systematic approach to finding and fixing issues
- **Feature Implementation**: Adding new mechanics without breaking existing systems
- **Code Optimization**: Identifying and resolving performance bottlenecks

## 🔮 Future Enhancements

- **Multiple Enemy Types**: Expand AI system with additional boss variants
- **Level System**: Multiple stages with varying challenges
- **Combo System**: Extended attack combinations with scoring
- **Save System**: Progress persistence and achievement tracking
- **Particle Effects**: Enhanced visual feedback and environmental effects
- **Sound Design**: Advanced audio mixing and dynamic soundtrack

---

## 🤝 Portfolio Highlights

This project demonstrates proficiency in:
- **Modern C++ Development** (C++20 features, RAII, smart pointers)
- **Game Engine Programming** (Custom systems from scratch)
- **Real-Time Systems** (60+ FPS with complex interactions)
- **Software Architecture** (Scalable, maintainable design patterns)
- **Cross-Platform Development** (Windows, macOS, Linux compatibility)

Built as a showcase of game programming fundamentals and software engineering best practices for junior game developer positions.
