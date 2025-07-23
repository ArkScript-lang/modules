---
title: "Database module"
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

## database:sqlite:open

Open a connection to a SQLite database.

**Parameters**
- `database_filename`: path to the database file

**Return value** `UserType`: an open connection to a SQLite database

**Author**
- [@rstefanic](https://github.com/rstefanic)

**Example**
{{< highlight_arkscript >}}
(let db (database:sqlite:open "test.db"))
{{< /highlight_arkscript >}}

## database:sqlite:close

Close an open connection to a SQLite database.

**Parameters**
- `database`: an open SQLite database object

**Return value** `number`: SQLite exit code (0 on success).

**Author**
- [@rstefanic](https://github.com/rstefanic)

**Example**
{{< highlight_arkscript >}}
(database:sqlite:close db)
{{< /highlight_arkscript >}}

## database:sqlite:exec

Execute a SQL query.

**Parameters**
- `database`: an open SQLite database object
- `sql`: SQL statement(s)

**Return value** `number`: SQLite exit code (0 on success).

**Author**
- [@rstefanic](https://github.com/rstefanic)

**Example**
{{< highlight_arkscript >}}
(database:sqlite:exec db "CREATE TABLE company(id INT PRIMARY KEY NOT NULL, name TEXT NOT NULL);")
{{< /highlight_arkscript >}}

## database:sqlite:exec_with_callback

Run a SQL statement on the database and run a callback function on each row in the results. The callback function takes one argument which will be passed a list of the results from a row.

**Parameters**
- `database`: SQLite Object
- `sql`: a string of the query or queries that you'd like to run
- `callback`: a function to run on every row returned

**Return value** `number`: SQLite exit code.

**Author**
- [@rstefanic](https://github.com/rstefanic)

**Example**
{{< highlight_arkscript >}}
# CREATE TABLE user (id INT PRIMARY KEY NOT NULL, name TEXT NOT NULL);
# INSERT INTO user (id, name) VALUES (1, 'Jane Doe'), (2, 'John Doe');

(database:sqlite:exec_with_callback
  db
  "SELECT * FROM user;"
  (fun (row) (print row)))
# ["1" "Jane Doe"]
# ["2" "John Doe"]
{{< /highlight_arkscript >}}

## database:sqlite:last_insert_row_id

Get the row id of the last row inserted into the database.

**Parameters**
- `database`: SQLite Object

**Return value** `number`: id of the last row inserted

**Author**
- [@rstefanic](https://github.com/rstefanic)

**Example**
{{< highlight_arkscript >}}
(database:sqlite:last_insert_row_id db)
{{< /highlight_arkscript >}}
