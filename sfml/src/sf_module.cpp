#include <SFML/Graphics.hpp>
#include <list>
#include <Ark/Module.hpp>

bool& get_has_window() { static bool w = false; return w; }
sf::RenderWindow& get_window() { static sf::RenderWindow w; return w; }
sf::Event& get_event() { static sf::Event e; return e; }
std::list<sf::Texture>& get_textures() { static std::list<sf::Texture> t; return t; }
std::list<sf::Sprite>& get_sprites() { static std::list<sf::Sprite> s; return s; }
std::list<sf::Font>& get_fonts() { static std::list<sf::Font> f; return f; }
std::list<sf::Text>& get_texts() { static std::list<sf::Text> t; return t; }

// module functions
Value sf_window_init(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("sf:window:init needs 3 arguments: width, height and title");
    if (n[0].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:window:init: width must be a Number");
    if (n[1].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:window:init: height must be a Number");
    if (n[2].valueType() != ValueType::String)
        throw Ark::TypeError("sf:window:init: title must be a String");
    
    if (!get_has_window())
    {
        get_window().create(sf::VideoMode(static_cast<long>(n[0].number()), static_cast<long>(n[1].number())), n[2].string().toString());
        get_has_window() = true;
    }
    else
        throw std::runtime_error("sf:window:init: can't call the function twice");

    return Nil;
}

Value sf_window_isopen(std::vector<Value>& n, Ark::VM* vm)
{
    return get_window().isOpen() ? True : False;
}

Value sf_poll_event(std::vector<Value>& n, Ark::VM* vm)
{
    if (get_window().pollEvent(get_event()))
    {
        std::string out = "event-";

        if (get_event().type == sf::Event::Closed)
            out += "quit";
        else if (get_event().type == sf::Event::KeyReleased)
            out += "keyup";
        else if (get_event().type == sf::Event::KeyPressed)
            out += "keydown";
        else
            out += "unknown";

        if (get_event().type == sf::Event::KeyReleased || get_event().type == sf::Event::KeyPressed)
        {
            switch (get_event().key.code)
            {
            case sf::Keyboard::A: out += "a"; break;
            case sf::Keyboard::B: out += "b"; break;
            case sf::Keyboard::C: out += "c"; break;
            case sf::Keyboard::D: out += "d"; break;
            case sf::Keyboard::E: out += "e"; break;
            case sf::Keyboard::F: out += "f"; break;
            case sf::Keyboard::G: out += "g"; break;
            case sf::Keyboard::H: out += "h"; break;
            case sf::Keyboard::I: out += "i"; break;
            case sf::Keyboard::J: out += "j"; break;
            case sf::Keyboard::K: out += "k"; break;
            case sf::Keyboard::L: out += "l"; break;
            case sf::Keyboard::M: out += "m"; break;
            case sf::Keyboard::N: out += "n"; break;
            case sf::Keyboard::O: out += "o"; break;
            case sf::Keyboard::P: out += "p"; break;
            case sf::Keyboard::Q: out += "q"; break;
            case sf::Keyboard::R: out += "r"; break;
            case sf::Keyboard::S: out += "s"; break;
            case sf::Keyboard::T: out += "t"; break;
            case sf::Keyboard::U: out += "u"; break;
            case sf::Keyboard::V: out += "v"; break;
            case sf::Keyboard::W: out += "w"; break;
            case sf::Keyboard::X: out += "x"; break;
            case sf::Keyboard::Y: out += "y"; break;
            case sf::Keyboard::Z: out += "z"; break;
            case sf::Keyboard::Num0: out += "0"; break;
            case sf::Keyboard::Num1: out += "1"; break;
            case sf::Keyboard::Num2: out += "2"; break;
            case sf::Keyboard::Num3: out += "3"; break;
            case sf::Keyboard::Num4: out += "4"; break;
            case sf::Keyboard::Num5: out += "5"; break;
            case sf::Keyboard::Num6: out += "6"; break;
            case sf::Keyboard::Num7: out += "7"; break;
            case sf::Keyboard::Num8: out += "8"; break;
            case sf::Keyboard::Num9: out += "9"; break;
            case sf::Keyboard::Escape:   out += "escape"; break;
            case sf::Keyboard::LBracket: out += "["; break;
            case sf::Keyboard::RBracket: out += "]"; break;
            case sf::Keyboard::Semicolon: out += ";"; break;
            case sf::Keyboard::Comma:    out += ","; break;
            case sf::Keyboard::Period:   out += "."; break;
            case sf::Keyboard::Quote:    out += "'"; break;
            case sf::Keyboard::Slash:    out += "/"; break;
            case sf::Keyboard::Backslash: out += "\\"; break;
            case sf::Keyboard::Tilde:    out += "~"; break;
            case sf::Keyboard::Equal:    out += "="; break;
            case sf::Keyboard::Hyphen:   out += "-"; break;
            case sf::Keyboard::Space:    out += "space"; break;
            case sf::Keyboard::Enter:    out += "enter"; break;
            case sf::Keyboard::Tab:      out += "tab"; break;
            case sf::Keyboard::Add:      out += "+"; break;
            case sf::Keyboard::Subtract: out += "-"; break;
            case sf::Keyboard::Multiply: out += "*"; break;
            case sf::Keyboard::Divide:   out += "/"; break;
            case sf::Keyboard::Left:     out += "left"; break;
            case sf::Keyboard::Right:    out += "right"; break;
            case sf::Keyboard::Up:       out += "up"; break;
            case sf::Keyboard::Down:     out += "down"; break;
            default: out += "unknown"; break;
            }
        }

        return Value(out);
    }
    return Value("event-empty");
}

Value sf_window_clear(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("sf:window:clear needs 3 arguments: r, g and b");
    if (n[0].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:window:clear: r must be a Number");
    if (n[1].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:window:clear: g must be a Number");
    if (n[2].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:window:clear: b must be a Number");
    get_window().clear(sf::Color(static_cast<long>(n[0].number()), static_cast<long>(n[1].number()), static_cast<long>(n[2].number())));
    return Nil;
}

Value sf_draw(std::vector<Value>& n, Ark::VM* vm)
{
    for (Value::Iterator it=n.begin(); it != n.end(); ++it)
    {
        if (it->valueType() != ValueType::User)
            throw Ark::TypeError("sf:draw: invalid argument");

        std::size_t i = it->string().toString().find_first_of('-');
        std::string sub = it->string().toString().substr(0, i);

        if (it->usertype_ref().is<sf::Text>())
            get_window().draw(it->usertype_ref().as<sf::Text>());
        else if (it->usertype_ref().is<sf::Sprite>())
            get_window().draw(it->usertype_ref().as<sf::Sprite>());
        else
            throw Ark::TypeError("Object can not be drawn");
    }
    return Nil;
}

Value sf_window_display(std::vector<Value>& n, Ark::VM* vm)
{
    get_window().display();
    return Nil;
}

Value sf_window_set_fps(std::vector<Value>& n, Ark::VM* vm)
{
    if (n[0].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:window:setFPS: fps must be a Number");
    get_window().setFramerateLimit(static_cast<long>(n[0].number()));
    return Nil;
}

Value sf_load_texture(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 1)
        throw std::runtime_error("sf:load:texture: need 1 argument: path");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("sf:load:texture: need a path to the texture");

    sf::Texture& texture = get_textures().emplace_back();
    if (!texture.loadFromFile(n[0].string().toString()))
        throw std::runtime_error("sf:load:texture: Could not load texture: " + n[0].string().toString());

    return Value(UserType(&texture));
}

Value sf_load_sprite(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 1)
        throw std::runtime_error("sf:load:sprite: need 1 argument: texture");
    if (n[0].valueType() != ValueType::User || !n[0].usertype_ref().is<sf::Texture>())
        throw Ark::TypeError("sf:load:sprite: need a SFML texture");

    sf::Sprite& object = get_sprites().emplace_back();
    object.setTexture(n[0].usertype_ref().as<sf::Texture>());

    return Value(UserType(&object));
}

Value sf_load_font(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 1)
        throw std::runtime_error("sf:load:font: need 1 argument: path to font");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("sf:load:font: need a String");

    sf::Font& font = get_fonts().emplace_back();
    if (!font.loadFromFile(n[0].string().toString()))
        throw std::runtime_error("sf:load:font: Could not load font: " + n[0].string().toString());

    return Value(UserType(&font));
}

Value sf_make_text(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 4)
        throw std::runtime_error("sf:make:text: need 4 arguments: font, text, size, color");
    if (n[0].valueType() != ValueType::User || !n[0].usertype_ref().is<sf::Font>())
        throw Ark::TypeError("sf:make:text: invalid argument (font)");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("sf:make:text: invalid argument (text)");
    if (n[2].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:make:text: invalid argument (size)");
    if (n[3].valueType() != ValueType::List)
        throw Ark::TypeError("sf:make:text: invalid argument (color)");

    sf::Text& object = get_texts().emplace_back();
    object.setFont(n[0].usertype_ref().as<sf::Font>());
    object.setString(n[1].string().toString());
    object.setCharacterSize(static_cast<long>(n[2].number()));
    object.setFillColor(sf::Color(
        static_cast<long>(n[3].const_list()[0].number()),
        static_cast<long>(n[3].const_list()[1].number()),
        static_cast<long>(n[3].const_list()[2].number())
    ));

    return Value(UserType(&object));
}

Value sf_set_text(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("sf:set:text: need 2 arguments: text object, new value");
    if (n[0].valueType() != ValueType::User || !n[0].usertype_ref().is<sf::Text>())
        throw Ark::TypeError("sf:set:text: invalid argument (text object)");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("sf:set:text: invalid argument (new value)");

    n[0].usertype_ref().as<sf::Text>().setString(n[1].string().toString());
    return Nil;
}

Value sf_setpos(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("sf:set:pos: need 3 arguments: object, x, y");
    if (n[0].valueType() != ValueType::User)
        throw Ark::TypeError("sf:set:pos: invalid argument (object)");
    if (n[1].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:set:pos: invalid argument (x)");
    if (n[2].valueType() != ValueType::Number)
        throw Ark::TypeError("sf:set:pos: invalid argument (y)");

    if (n[0].usertype_ref().is<sf::Text>())
        n[0].usertype_ref().as<sf::Text>().setPosition(static_cast<long>(n[1].number()), static_cast<long>(n[2].number()));
    else if (n[0].usertype_ref().is<sf::Sprite>())
        n[0].usertype_ref().as<sf::Sprite>().setPosition(static_cast<long>(n[1].number()), static_cast<long>(n[2].number()));
    else
        throw Ark::TypeError("Object isn't a SFML object");

    return Nil;
}

Value sf_width(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 1)
        throw std::runtime_error("sf:width: need 1 argument: object");
    if (n[0].valueType() != ValueType::User)
        throw Ark::TypeError("sf:width: invalid argument");

    if (n[0].usertype_ref().is<sf::Text>())
        return Value(static_cast<int>(n[0].usertype_ref().as<sf::Text>().getGlobalBounds().width));
    else if (n[0].usertype_ref().is<sf::Sprite>())
        return Value(static_cast<int>(n[0].usertype_ref().as<sf::Sprite>().getGlobalBounds().width));
    throw Ark::TypeError("Object isn't a SFML object");
}

Value sf_height(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 1)
        throw std::runtime_error("sf:height: need 1 argument: object");
    if (n[0].valueType() != ValueType::User)
        throw Ark::TypeError("sf:height: invalid argument");

    if (n[0].usertype_ref().is<sf::Text>())
        return Value(static_cast<int>(n[0].usertype_ref().as<sf::Text>().getGlobalBounds().height));
    else if (n[0].usertype_ref().is<sf::Sprite>())
        return Value(static_cast<int>(n[0].usertype_ref().as<sf::Sprite>().getGlobalBounds().height));
    throw Ark::TypeError("Object isn't a SFML object");
}

Value sf_event(std::vector<Value>& n, Ark::VM* vm)
{
    std::string out = "event-";

    if (n.size() == 0)
        throw std::runtime_error("sf:event need at least 1 argument");

    for (Value::Iterator it=n.begin(); it != n.end(); ++it)
    {
        if (it->valueType() != ValueType::String)
            throw Ark::TypeError("sf:event: invalid argument");
        out += it->string().toString();
    }

    return Value(out);
}

Value sf_window_close(std::vector<Value>& n, Ark::VM* vm)
{
    get_window().close();
    return Nil;
}