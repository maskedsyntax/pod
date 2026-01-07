<div align="center">

# Pod

Pod is a modern, ricing-friendly powermenu for i3wm, inspired by `rofi` and built with C++ and Qt6.

<img src="assets/pod.png" alt="Pod Screenshot">

</div>

## Features
- **Minimalist Design**: Undecorated, floating window with rounded corners.
- **Visual Feedback**: Black theme with white accents, reversing on selection.
- **Custom Icons**: Load your own SVGs from `~/.config/pod/assets/`.
- **Keyboard Driven**: Full Tab navigation and Enter key support.

## Requirements
- `qt6-base-dev`
- `cmake`
- `make`
- `i3lock` (for Lock functionality)
- `systemd` / `logind`

## Build & Install

```bash
mkdir -p build
cd build
cmake ..
make
sudo cp pod /usr/local/bin/
```

## Configuration

### Custom Icons/Images
Pod looks for `.svg` images in `~/.config/pod/assets/` or `./assets/` with the following names:
- `poweroff.svg`
- `reboot.svg`
- `lock.svg`
- `sleep.svg`
- `logout.svg`

If an SVG is not found, it falls back to the system icon theme.

### Styling
Customize the look by editing `style.qss`. Place your custom file at `~/.config/pod/style.qss`.

### i3wm Keybinding
Add the following to your `~/.config/i3/config`:

```i3
bindsym Mod4+Shift+e exec --no-startup-id /usr/local/bin/pod
```

## Usage
- **Tab**: Cycle between options.
- **Enter**: Execute selected option.
- **Escape**: Close menu.
- **Mouse**: Click to execute.