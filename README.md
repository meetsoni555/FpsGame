# Raylib FPS Aim Trainer

A lightweight, high-performance first-person shooter designed for aim training and mechanical practice. Built entirely in C++ using the **raylib** library, focusing on low-latency input and smooth 3D rendering.

## Overview

This project serves as a focused aim trainer. It utilizes a custom 3D engine workflow to render weapon models (GLB) and target entities, providing a clean environment for practicing tracking and flick shots.

### Technical Stack
- **Language:** C++17
- **Graphics:** raylib 5.0+ (OpenGL 3.3)
- **UI:** Gum (Installer TUI)
- **Platform:** Linux (Native)

## Gameplay
<img width="982" height="721" alt="Aim Trainer Gameplay" src="https://github.com/user-attachments/assets/a4746d01-ec50-4f50-a4ed-353e28376584" />

---

<img width="977" height="714" alt="Aim Trainer Interface" src="https://github.com" />

## Installation

The project includes an interactive shell script that handles compilation and system integration.

### Prerequisites
- **GCC/G++** compiler
- **raylib** development headers (`libraylib-dev`)
- **gum** (the installer will attempt to install this if missing)

### Quick Start
1. Clone the repository:
   ```bash
   git clone git@github.com:meetsoni555/FpsGame.git
   cd FpsGame
   ```
2. Run the installer:
   ```bash
   chmod +x install.sh
   ./install.sh
   ```

The installer will compile the source code and give you the option to create a desktop shortcut for your application launcher (Rofi, dmenu, etc.).

## Controls
- **WASD:** Movement
- **Mouse:** Look / Aim
- **Left Click:** Fire
- **ESC:** Pause
- **R:** Restart ( when you loose ) 

- **Currently Underdevelopment** 

## License
MIT License - Feel free to use this for your own learning or projects.
