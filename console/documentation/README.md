@page console_module Console module

## console:clear

Clear the terminal on Windows and Posix.

**Parameters**
- None.

**Return value** `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

## console:color

This function changes the color of the text written to the terminal.

**Parameter**
- `color`: a string representing the wanted color.

**Return value** `nil`

Available colors:
```
reset - remove all color and modifiers
bold - bold text if available
dark - darker text if available
underline - underlined text if available
blink - blinking text if available
reverse - reverse text if available
concealed - concealed text if available

FOREGROUND COLORS:
grey
red
green
yellow
blue
magenta
cyan
white

BACKGROUND COLORS:
on_grey
on_red
on_green
on_yellow
on_blue
on_magenta
on_cyan
on_white
```
