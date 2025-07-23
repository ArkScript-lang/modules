---
title: "Hash module"
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

## hash:sha256

Generates a SHA256 of a given string.

**Parameters**
- `data`: a string to hash

**Return value** `String`: hexadecimal hash

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
{{< highlight_arkscript >}}
(import hash)
(print (hash:sha256 "arkscript"))  # c56d0bb03e9e03e7000f7b596cdb333c315523af7e4781f02610c690441c43dd
{{< /highlight_arkscript >}}

## hash:md5

Generates a MD5 of a given string.

**Parameters**
- `data`: a string to hash

**Return value** `String`: hexadecimal hash

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
{{< highlight_arkscript >}}
(import hash)
(print (hash:md5 "arkscript"))  # a27ee01c49b7552b8d69a631b7bb23c9
{{< /highlight_arkscript >}}
