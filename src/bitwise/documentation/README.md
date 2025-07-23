---
title: "Bitwise module"
description: "For bitwise operations"
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

## bitwise:rshift

Right shifts a given number.

**Parameters**
- `number`: the number to shift
- `shift`: shifting to do

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
{{< highlight_arkscript >}}
(import bitwise)
(print (bitwise:rshift 4096 3))  # 512
{{< /highlight_arkscript >}}

## bitwise:lshift

Left shifts a given number.

**Parameters**
- `number`: the number to shift
- `shift`: shifting to do

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
{{< highlight_arkscript >}}
(import bitwise)
(print (bitwise:lshift 512 3))  # 4096
{{< /highlight_arkscript >}}

## bitwise:xor

Xor a number given a mask.

**Parameters**
- `number`: the number to xor
- `mask`: the mask as a number

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
{{< highlight_arkscript >}}
(import bitwise)
(print (bitwise:xor 99 13))  # 110
{{< /highlight_arkscript >}}

## bitwise:or

Compute the bitwise `a OR b` operation.

**Parameters**
- `a`: first number
- `b`: second number

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
{{< highlight_arkscript >}}
(import bitwise)
(print (bitwise:or 89 13))  # 93
{{< /highlight_arkscript >}}

## bitwise:and

Compute the bitwise `a AND b` operation.

**Parameters**
- `a`: first number
- `b`: second number

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
{{< highlight_arkscript >}}
(import bitwise)
(print (bitwise:and 89 13))  # 9
{{< /highlight_arkscript >}}