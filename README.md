# Pod

Pod is a modern, ricing-friendly powermenu for i3wm, inspired by `rofi` and built with C++ and Qt6.

## Features
- **Minimalist Design**: Undecorated, floating window with rounded corners.
- **Visual Feedback**: Black theme with white accents, reversing on selection.
- **Custom Images**: Load your own icons from `~/.config/pod/assets/`.
- **Keyboard Driven**: Close with Esc.

## Requirements
- `qt6-base-dev` (or equivalent Qt6 Widgets package)
- `cmake`
- `make`
- `i3lock` (for Lock functionality)
- `systemd` (for poweroff/reboot/suspend)

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
Pod looks for `.png` images in `~/.config/pod/assets/` or `./assets/` with the following names:
- `shutdown.png`
- `reboot.png`
- `lock.png`
- `suspend.png`
- `logout.png`

If an image is not found, it falls back to the system icon theme.

### Styling
Customize the look by editing `style.qss`. Place your custom file at `~/.config/pod/style.qss`.

### i3wm Keybinding
Add the following to your `~/.config/i3/config`:

```i3
bindsym Mod4+Shift+e exec --no-startup-id /usr/local/bin/pod
```

## Usage
- **Shutdown**: Click
- **Reboot**: Click
- **Lock**: Click
- **Suspend**: Click
- **Logout**: Click
- **Escape**: Close menu