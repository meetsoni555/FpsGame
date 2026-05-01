
#!/bin/bash

# 1. Install Gum if missing (The Beauty Foundation)
#
#
gum style --italic --foreground 250 "I know you can't wait to play this amazing game "
sleep 1 
gum style --italic --foreground 250 "Wait .. a second ...darling .. "
sleep 1 
gum style --italic --foreground 250 "Hmmm.... coming ... for you .."

if ! command -v gum &> /dev/null; then
    echo "✨ Making the installation beautiful..."
    sudo mkdir -p /etc/apt/keyrings
    curl -fsSL https://charm.sh | sudo gpg --dearmor -o /etc/apt/keyrings/charm.gpg
    echo "deb [signed-by=/etc/apt/keyrings/charm.gpg] https://charm.sh * *" | sudo tee /etc/apt/sources.list.d/charm.list
    sudo apt update && sudo apt install gum -y
fi

clear
gum style --foreground 212 --border-foreground 212 --border double --align center --width 50 "🔫 FPS GAME INSTALLER"

# 2. Pre-flight checks (Checking if your PC is worthy)
gum style --italic --foreground 250 "Checking if your PC can handle this masterpiece..."
sleep 1

# Check for g++
if ! command -v g++ &> /dev/null; then
    gum style --foreground 9 "❌ Error: g++ (GCC) is not installed. Use 'sudo apt install build-essential' or you're stuck in 2D forever."
    exit 1
fi

# Check for Raylib headers (Usually in /usr/local/include or /usr/include)
if [ ! -f /usr/local/include/raylib.h ] && [ ! -f /usr/include/raylib.h ]; then
    gum style --foreground 214 "⚠️ Warning: raylib.h not found in standard paths."
    gum confirm "I'll try to compile anyway, but if it fails, go install raylib, bro. Continue?" || exit 1
fi

gum style --foreground 10 "✅ System looks solid. No potato PC here!"
sleep 1

# 3. The Hype Sequence
gum style --italic --foreground 250 "Optimizing the pixels..."
sleep 1 
gum style --italic --foreground 250 "Polishing the gun for you ... "
sleep 1
gum style --italic --foreground 250 "I know you can't wait to play this amazing game..."
sleep 1
gum style --bold --foreground 212 "ALMOST THERE... 🚀"
sleep 1

# 4. Compilation Step
if gum confirm "Ready to turn code into magic?"; then
    gum spin --spinner monkey --title "Sacrificing bugs to the compiler gods..." -- \
        g++ main.cpp -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    
    if [ $? -eq 0 ]; then
        gum style --foreground 10 --bold "✅ Build Success! Your CPU didn't even sweat."
    else
        gum style --border double --padding "1 2" --foreground 9 "❌ BUILD FAILED!"
        echo "Check if you have these dependencies: libraylib-dev, libx11-dev, libgl1-mesa-dev" | gum format
        exit 1
    fi
else
    gum style --foreground 214 "Compilation cancelled. The gun remains a ghost in the machine."
    exit 0
fi

# 5. Optional App Launcher Shortcut
echo ""
echo "Do you want to create a application shortcut ? "
CREATE_SHORTCUT=$(gum choose  "Yes, make me a pro gamer" "No, I like the terminal life")

if [ "$CREATE_SHORTCUT" == "Yes, make me a pro gamer" ]; then
    GAME_DIR=$(pwd)
    
    # Create the run wrapper to handle the working directory (Crucial for loading gun.glb)
    cat <<EOF > run_game.sh
#!/bin/bash
cd "$GAME_DIR"
./main
EOF
    chmod +x run_game.sh

    # Create the .desktop file
    DESKTOP_FILE="$HOME/.local/share/applications/raylib_fps.desktop"
    cat <<EOF > "$DESKTOP_FILE"
[Desktop Entry]
Name=Raylib FPS
Comment=The coolest game you ever compiled
Exec=$GAME_DIR/run_game.sh
Path=$GAME_DIR
Icon=$GAME_DIR/gun.glb
Type=Application
Terminal=false
Categories=Game;
EOF

    chmod +x "$DESKTOP_FILE"
    # Update desktop database so Rofi/App Menu picks it up immediately
    update-desktop-database ~/.local/share/applications 2>/dev/null
    
    gum style --border normal --margin "1 2" --padding "1 2" --border-foreground 57 "🔥 SUCCESS! Look for 'Raylib FPS' in Rofi. Go get 'em, champ!"
else
    gum style --foreground 214 "Skipped shortcut. Use './main' to launch and start blasting."
fi

# Final goodbye
gum style --foreground 212 "Have fun gaming, bro! 🔫✨"
