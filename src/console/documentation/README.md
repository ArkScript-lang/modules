---
title: "Console module"
description: ""
summary: ""
date: 2025-07-23T14:25:16+02:00
lastmod: 2025-07-23T14:25:16+02:00
draft: false
weight: 410
toc: true
seo:
  title: "" # custom title (optional)
  description: "" # custom description (recommended)
  canonical: "" # custom canonical URL (optional)
  noindex: false # false (default) or true
---

{{< highlight_scripts >}}

## console:clear

Clear the terminal on Windows and Posix.

**Parameters**
- None.

**Return value** `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
{{< highlight_arkscript >}}
(import console)
(print "hello world")  # hello world is printed
(console:clear)        # now the whole terminal is empty and nothing is printed
{{< /highlight_arkscript >}}

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

**Example**
{{< highlight_arkscript >}}
(import console)
(print "hello world")  # this is using the default color of the terminal
(console:color "red")
(print "this is in red")
(print "this line is still red")
(console:color "reset")    # now the colors are back to normal
(print "I am normal")
(console:color "on_cyan")
(print "I am a text on cyan background")
{{< /highlight_arkscript >}}