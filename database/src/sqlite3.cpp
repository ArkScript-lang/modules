#include <database.hpp>

namespace Database
{
    using namespace Ark;

    Value database_sqlite_open(std::vector<Value>& args, VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::String))
            types::generateError(
                "database:sqlite:open",
                { { types::Contract { { types::Typedef("database_filename", ValueType::String) } } } },
                args);

        sqlite3* db;
        const char* database_name = args[0].string().c_str();

        if (sqlite3_open(database_name, &db) != SQLITE_OK)
            return Nil;

        return Value(UserType(db));
    }

    Value database_sqlite_close(std::vector<Value>& args, VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::User) || !args[0].usertype().is<sqlite3>())
            types::generateError(
                "database:sqlite:close",
                { { types::Contract { { types::Typedef("database", ValueType::User) } } } },
                args);

        sqlite3& db = args[0].usertypeRef().as<sqlite3>();
        return Value(sqlite3_close(&db));
    }

    Value database_sqlite_exec(std::vector<Value>& args, VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::User, ValueType::String) || !args[0].usertype().is<sqlite3>())
            types::generateError(
                "database:sqlite:exec",
                { { types::Contract { { types::Typedef("database", ValueType::User),
                                        types::Typedef("sql", ValueType::String) } } } },
                args);

        sqlite3& db = args[0].usertypeRef().as<sqlite3>();
        const char* sql = args[1].string().c_str();
        char* error_message = nullptr;

        int code = sqlite3_exec(&db, sql, nullptr, nullptr, &error_message);
        if (code != SQLITE_OK)
        {
            std::runtime_error error = std::runtime_error(
                "database:sqlite:exec: exited with code " + std::to_string(code) + "\n" + std::string(error_message));
            sqlite3_free(error_message);
            throw error;
        }

        return Value(code);
    }

    Value database_sqlite_exec_with_callback(std::vector<Value>& args, VM* vm [[maybe_unused]])
    {
        if (args.size() != 3 || args[0].valueType() != ValueType::User || !args[0].usertype().is<sqlite3>() || args[1].valueType() != ValueType::String || !args[2].isFunction())
            types::generateError(
                "database:sqlite:exec_with_callback",
                { { types::Contract { { types::Typedef("database", ValueType::User),
                                        types::Typedef("sql", ValueType::String),
                                        types::Typedef("callback", ValueType::Closure) } } } },
                args);

        sqlite3& db = args[0].usertypeRef().as<sqlite3>();
        const char* sql = args[1].string().c_str();
        const Value user_defined_callback = args[2];

        // 'sqlite3_exec' accepts a callback function which we will define here. This callback is invoked for
        // each result row coming out of the evaluated SQL statement(s). The first argument is a void*
        // where we can pass a pointer to a vector containing all the results.
        auto callback = [](void* all_results, int argc, char** argv, char** col_names [[maybe_unused]]) -> int {
            auto all_results_casted = static_cast<std::vector<std::vector<Value>>*>(all_results);
            std::vector<Value> current_row;

            // Add each element as an Ark String to the current row results
            for (int i = 0; i < argc; i++)
                current_row.push_back(Value(std::string(argv[i])));

            // Add the current row to our collection of results
            all_results_casted->push_back(current_row);
            return 0;
        };

        char* error_message = nullptr;
        std::vector<std::vector<Value>> all_results;
        int code = sqlite3_exec(&db, sql, callback, &all_results, &error_message);

        // For each row returned from the SQL query, run the user's callback
        // function on each row passing in an Ark List of the row results.
        for (std::vector<Value>& row : all_results)
            vm->resolve(&user_defined_callback, Value(std::move(row)));

        if (code != SQLITE_OK)
        {
            std::runtime_error error = std::runtime_error(
                "database:sqlite:exec: exited with code " + std::to_string(code) + "\n" + std::string(error_message));
            sqlite3_free(error_message);
            throw error;
        }

        return Value(code);
    }

    Value database_sqlite_last_insert_row_id(std::vector<Value>& args, VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::User) || !args[0].usertype().is<sqlite3>())
            types::generateError(
                "database:sqlite:last_insert_row_id",
                { { types::Contract { { types::Typedef("database", ValueType::User) } } } },
                args);

        sqlite3& db = args[0].usertypeRef().as<sqlite3>();
        int row_id = sqlite3_last_insert_rowid(&db);
        return Value(row_id);
    }
}
