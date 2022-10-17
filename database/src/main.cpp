#include <database.hpp>

ARK_API Ark::mapping* getFunctionsMapping()
{
    Ark::mapping* map = Ark::mapping_create(5);

    // Sqlite3
    Ark::mapping_add(map[0], "database:sqlite:open", Database::database_sqlite_open);
    Ark::mapping_add(map[1], "database:sqlite:close", Database::database_sqlite_close);
    Ark::mapping_add(map[2], "database:sqlite:exec", Database::database_sqlite_exec);
    Ark::mapping_add(map[3], "database:sqlite:exec_with_callback", Database::database_sqlite_exec_with_callback);
    Ark::mapping_add(map[4], "database:sqlite:last_insert_row_id", Database::database_sqlite_last_insert_row_id);

    return map;
}
