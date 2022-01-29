#include <string>
#include <random>
#include <chrono>
#include <termcolor/termcolor.hpp>

#include <Ark/Module.hpp>

namespace ArkConsole
{
    Value clear(std::vector<Value>& n [[maybe_unused]], Ark::VM* vm [[maybe_unused]])
    {
#if defined(OS_WINDOWS)
        COORD topLeft = { 0, 0 };
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screen;
        DWORD written;

        GetConsoleScreenBufferInfo(console, &screen);
        FillConsoleOutputCharacterA(
            console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
        FillConsoleOutputAttribute(
            console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
            screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
        SetConsoleCursorPosition(console, topLeft);
#elif defined(OS_LINUX)
        std::cout << "\x1B[2J\x1B[H";
#endif
        return Nil;
    }

    Value color(std::vector<Value>& n, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::String))
            types::generateError(
                "console:color",
                { { types::Contract { { types::Typedef("color_name", ValueType::String) } } } },
                n);

        auto value = n[0].string();
        if (value == "reset")
            std::cout << termcolor::reset;
        else if (value == "bold")
            std::cout << termcolor::bold;
        else if (value == "dark")
            std::cout << termcolor::dark;
        else if (value == "underline")
            std::cout << termcolor::underline;
        else if (value == "blink")
            std::cout << termcolor::blink;
        else if (value == "reverse")
            std::cout << termcolor::reverse;
        else if (value == "concealed")
            std::cout << termcolor::concealed;
        else if (value == "grey")
            std::cout << termcolor::grey;
        else if (value == "red")
            std::cout << termcolor::red;
        else if (value == "green")
            std::cout << termcolor::green;
        else if (value == "yellow")
            std::cout << termcolor::yellow;
        else if (value == "blue")
            std::cout << termcolor::blue;
        else if (value == "magenta")
            std::cout << termcolor::magenta;
        else if (value == "cyan")
            std::cout << termcolor::cyan;
        else if (value == "white")
            std::cout << termcolor::white;
        else if (value == "on_grey")
            std::cout << termcolor::on_grey;
        else if (value == "on_red")
            std::cout << termcolor::on_red;
        else if (value == "on_green")
            std::cout << termcolor::on_green;
        else if (value == "on_yellow")
            std::cout << termcolor::on_yellow;
        else if (value == "on_blue")
            std::cout << termcolor::on_blue;
        else if (value == "on_magenta")
            std::cout << termcolor::on_magenta;
        else if (value == "on_cyan")
            std::cout << termcolor::on_cyan;
        else if (value == "on_white")
            std::cout << termcolor::on_white;
        else
            throw std::runtime_error("Couldn't identify argument given to console:color: '" + value.toString() + "'");

        return Nil;
    }
}

ARK_API mapping* getFunctionsMapping()
{
    mapping* map = mapping_create(2);
    mapping_add(map[0], "console:clear", ArkConsole::clear);
    mapping_add(map[1], "console:color", ArkConsole::color);

    return map;
}
