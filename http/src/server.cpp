#include <httplib.hpp>
#include <http_module.hpp>
#include <list>

using namespace httplib;

/*
    ***********************************
                   Misc
    ***********************************
*/

int& get_logger_level()
{
    static int i = 0;
    return i;
}

bool& get_error_handler()
{
    static bool b = false;
    return b;
}

Server& create_server()
{
    static Server srv;

    if (get_logger_level() > 0)
        srv.set_logger([](const auto& req, const auto& res) {
            std::cout << "got request\n";
            std::cout << "method " << req.method << ", path " << req.path << ", body " << req.body << "\n";
            std::cout << "status " << res.status << "\n";
            std::cout << "==================\n\n";
        });

    if (get_error_handler())
        srv.set_error_handler([](const auto& req, const auto& res) {
            std::cout << "ERROR???\n";
            std::cout << "status " << res.status << "\n";
            std::cout << "==================\n\n";
        });

    return srv;
}

UserType::ControlFuncs& get_cfs()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "httpServer<0x" << a.data() << ">";
        return os;
    };
    cfs.deleter = [](void* data) {
        ///@todo won't work asis
    };
    return cfs;
}

/*
    ***********************************
                   Server
    ***********************************
*/

Value http_create_server(std::vector<Value>& n, Ark::VM* vm)
{
    Value server = Ark::Value(Ark::UserType(&create_server()));
    server.usertype_ref().setControlFuncs(&get_cfs());
    return server;
}

Value http_server_get(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() < 3 || n.size() > 4)
        throw std::runtime_error("http:server:get needs 3 to 4 arguments: server, route, content, [type=text/plain]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:get: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:get: route must be a String");
    if (n[2].valueType() != ValueType::String && !n[2].isFunction())
        throw Ark::TypeError("http:server:get: content must be a String or a Function");

    std::string type = "text/plain";
    if (n.size() == 4)
    {
        if (n[3].valueType() != ValueType::String)
            throw Ark::TypeError("http:server:get: type must be a String");
        else
            type = n[3].string_ref().toString();
    }

    Server& srv = n[0].usertype().as<Server>();
    srv.Get(n[1].string().c_str(), [n, type, vm](const Request& req, Response& res) {
        // TODO allow use of req.matches
        // TODO allow use of external functions (eg, httpServerStop when going to /stop)

        std::string content = (n[2].valueType() == ValueType::String) ? n[2].string().toString() : "";
        if (n[2].isFunction())
        {
            Value r = vm->resolve(&n[2], "test");
            if (r.valueType() == ValueType::String)
                content = r.string_ref().toString();
        }

        res.set_content(content, type.c_str());
    });

    return Nil;
}

Value http_server_stop(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 1)
        throw std::runtime_error("http:server:stop: needs a single argument: server");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:stop: server must be an httpServer");

    n[0].usertype().as<Server>().stop();

    return Nil;
}

Value http_server_listen(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("http:server:listen: needs 2 to 3 arguments: server, host, [port]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:listen: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:listen: host must be a String");
    if (n.size() == 3 && n[1].string() == "0.0.0.0" && n[2].valueType() != ValueType::Number)
        throw Ark::TypeError("http:server:listen: port must be a Number, and is a mandatory argument when host is 0.0.0.0");

    Server& srv = n[0].usertype().as<Server>();

    if (n[1].string() == "0.0.0.0")
    {
        srv.bind_to_any_port("0.0.0.0");
        srv.listen_after_bind();
    }
    else
        srv.listen(n[1].string().c_str(), static_cast<int>(n[2].number()));

    return Nil;
}

Value http_server_set_mount_point(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("http:server:setMountPoint: needs 3 arguments: server, folder, destination");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:setMountPoint: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setMountPoint: folder must be a String");
    if (n[2].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setMountPoint: destination must be a String");

    auto ret = n[0].usertype().as<Server>().set_mount_point(n[1].string().c_str(), n[2].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_remove_mount_point(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:server:rmMountPoint: needs 2 arguments: server, folder");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:rmMountPoint: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:rmMountPoint: folder must be a String");

    auto ret =n[0].usertype().as<Server>().remove_mount_point(n[1].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_set_fext_mimetype(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("http:server:setFileExtAndMimetypeMapping: needs 3 arguments: server, ext, mimetype");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:setFileExtAndMimetypeMapping: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setFileExtAndMimetypeMapping: ext must be a String");
    if (n[2].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setFileExtAndMimetypeMapping: mimetype must be a String");

    n[0].usertype().as<Server>().set_file_extension_and_mimetype_mapping(
        n[1].string().c_str(), n[2].string().c_str()
    );
    return Nil;
}

Value http_server_enable_logger(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() > 1)
        throw std::runtime_error("http:server:enableLogger: needs 0 or 1 argument: [level=1]");

    if (n.size() == 1 && n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:enableLogger: level must be a Number");

    int& level = get_logger_level();
    if (n.size() == 1)
        level = static_cast<int>(n[0].number());
    else
        level = 1;

    return Nil;
}