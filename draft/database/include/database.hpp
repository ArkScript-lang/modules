#ifndef database_module
#define database_module

#include <Ark/Module.hpp>

#include <stdio.h>
#include <sqlite3.h>

namespace Database
{
    using namespace Ark;

    Value database_sqlite_open(std::vector<Value>& args, VM* vm);
    Value database_sqlite_close(std::vector<Value>& args, VM* vm);
    Value database_sqlite_exec(std::vector<Value>& args, VM* vm);
    Value database_sqlite_exec_with_callback(std::vector<Value>& args, VM* vm);
    Value database_sqlite_last_insert_row_id(std::vector<Value>& args, VM* vm);
}

#endif
