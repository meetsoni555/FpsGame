
#!/bin/bash

# 1. Install Gum if missing
if ! command -v gum &> /dev/null; then
    echo "✨ Making the installation beautiful..."
    sudo mkdir -p /etc/apt/keyrings
    curl -fsSL https://charm.sh | sudo gpg --dearmor -o /etc/apt/keyrings/charm.gpg
    echo "deb [signed-by=/etc/apt/keyrings/charm.gpg] https://charm.sh * *" | sudo tee /etc/apt/sources.list.d/charm.list
    sudo apt update && sudo apt install gum -y
fi

clear
# 2. Hype sequence
gum style --foreground 212 --border-foreground 212 --border double --align center --width 50 "FPS GAME INSTALLER"
sleep 1
gum style --italic --foreground 250 "Just wait a second..."
sleep 1
gum style --italic --foreground 250 "I know you can't wait to play this amazing game..."
sleep 2

# 3. Compilation Step
if gum confirm "🚀 Ready to compile the game?"; then
    gum spin --spinner dot --title "Compiling main.cpp..." -- \
        g++ main.cpp -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    
    if [ $? -eq 0 ]; then
        gum style --foreground 10 --bold "✅ Build Success!"
    else
        gum style --foreground 9 --bold "❌ Build Failed! Make sure raylib is installed."
        exit 1
    fi
else
    gum style --foreground 214 "Compilation cancelled."
    exit 0
fi

# 4. Optional App Launcher Shortcut
echo ""
CREATE_SHORTCUT=$(gum choose "Do you want to create an app launcher shortcut?" "Yes, add to my menu" "No, just keep the binary")

if [ "$CREATE_SHORTCUT" == "Yes, add to my menu" ]; then
    GAME_DIR=$(pwd)
    
    # Create the run wrapper to handle the working directory (so it finds gun.glb)
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
Exec=$GAME_DIR/run_game.sh
Path=$GAME_DIR
Icon=$GAME_DIR/gun.glb
Type=Application
Terminal=false
Categories=Game;
EOF

    chmod +x "$DESKTOP_FILE"
    update-desktop-database ~/.local/share/applications 2>/dev/null
    
    gum style --border normal --margin "1 2" --padding "1 2" --border-foreground 57 "🔥 SUCCESS! You can now find 'Raylib FPS' in Rofi or your App Menu."
else
    gum style --foreground 214 "Skipped shortcut creation. Run the game using ./main"
fi

# Final goodbye
gum style --foreground 212 "Have fun gaming, bro! 🔫"
