@page bitwise_module Bitwise module

For bitwise operations.

## bitwise:rshift

Right shifts a given number.

**Parameters**
- `number`: the number to shift
- `shift`: shifting to do

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
~~~~{.lisp}
(import "bitwise.arkm")
(print (bitwise:rshift 4096 3))  # 512
~~~~

## bitwise:lshift

Left shifts a given number.

**Parameters**
- `number`: the number to shift
- `shift`: shifting to do

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
~~~~{.lisp}
(import "bitwise.arkm")
(print (bitwise:lshift 512 3))  # 4096
~~~~

## bitwise:xor

Xor a number given a mask.

**Parameters**
- `number`: the number to xor
- `mask`: the mask as a number

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
~~~~{.lisp}
(import "bitwise.arkm")
(print (bitwise:xor 99 13))  # 110
~~~~

## bitwise:or

Compute the bitwise `a OR b` operation.

**Parameters**
- `a`: first number
- `b`: second number

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
~~~~{.lisp}
(import "bitwise.arkm")
(print (bitwise:or 89 13))  # 93
~~~~

## bitwise:and

Compute the bitwise `a AND b` operation.

**Parameters**
- `a`: first number
- `b`: second number

**Return value** `Number`

**Author**
- [@rakista112](https://github.com/rakista112)

**Example**
~~~~{.lisp}
(import "bitwise.arkm")
(print (bitwise:and 89 13))  # 9
~~~~