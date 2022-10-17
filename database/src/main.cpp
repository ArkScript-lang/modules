#include <database.hpp>

ARK_API Ark::mapping* getFunctionsMapping()
{
    static Ark::mapping map[] = {
        { "database:sqlite:open", Database::database_sqlite_open },
        { "database:sqlite:close", Database::database_sqlite_close },
        { "database:sqlite:exec", Database::database_sqlite_exec },
        { "database:sqlite:exec_with_callback", Database::database_sqlite_exec_with_callback },
        { "database:sqlite:last_insert_row_id", Database::database_sqlite_last_insert_row_id },
        { nullptr, nullptr }
    };

    return map;
}
