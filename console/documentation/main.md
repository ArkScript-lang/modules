# Console module

## console:clear

This function takes no argument, and will clear the terminal.

Returns `nil`.

## console:color

This function changes the color of the text written to the terminal.

Must give a String representing the wanted color.

Returns `nil`.

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