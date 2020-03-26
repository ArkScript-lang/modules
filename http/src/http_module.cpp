#include <Ark/Module.hpp>
#include <httplib.hpp>
#include <list>
#include <mutex>

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

std::mutex& get_mutex()
{
    static std::mutex m;
    return m;
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

std::list<Headers>& get_headers()
{
    static std::list<Headers> headers;
    return headers;
}

std::list<Params>& get_params()
{
    static std::list<Params> params;
    return params;
}

std::list<Client>& get_clients()
{
    static std::list<Client> clients;
    return clients;
}

/*
    ***********************************
                   Server
    ***********************************
*/

Value http_create_server(std::vector<Value>& n)
{
    Value server = Ark::Value(Ark::UserType(&create_server()));
    server.usertype_ref().setOStream([](std::ostream& os, const UserType& A) -> std::ostream& {
        os << "httpServer<0x" << A.data() << ">";
        return os;
    });
    return server;
}

Value http_server_get(std::vector<Value>& n)
{
    if (n.size() < 3 || n.size() > 4)
        throw std::runtime_error("httpServerGet needs 3 to 4 arguments: server, route, content, [type=text/plain]");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerGet: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerGet: route must be a String");
    if (n[2].valueType() != ValueType::String && !n[2].isFunction())
        throw Ark::TypeError("httpServerGet: content must be a String or a Function");

    std::string type = "text/plain";
    if (n.size() == 4)
    {
        if (n[3].valueType() != ValueType::String)
            throw Ark::TypeError("httpServGet: type must be a String");
        else
            type = n[3].string();
    }

    Server *srv = static_cast<Server*>(n[0].usertype().data());
    srv->Get(n[1].string().c_str(), [n, type](const Request& req, Response& res) {
        // TODO allow use of req.matches
        // TODO allow use of external functions (eg, httpServerStop when going to /stop)
        get_mutex().lock();

        std::string content = (n[2].valueType() == ValueType::String) ? n[2].string() : "";
        if (n[2].isFunction())
        {
            Value r = n[2].resolve("test");
            if (r.valueType() == ValueType::String)
                content = r.string();
        }

        res.set_content(content, type.c_str());
        get_mutex().unlock();
    });

    return Nil;
}

Value http_server_stop(std::vector<Value>& n)
{
    if (n.size() != 1)
        throw std::runtime_error("httpServerStop: needs a single argument: server");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerGet: server must be an httpServer");
    
    static_cast<Server*>(n[0].usertype().data())->stop();
    
    return Nil;
}

Value http_server_listen(std::vector<Value>& n)
{
    if (n.size() != 3)
        throw std::runtime_error("httpServerListen: needs 3 arguments: server, host, port");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerListen: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerListen: host must be a String");
    if (n[2].valueType() != ValueType::Number)
        throw Ark::TypeError("httpServerListen: port must be a Number");

    static_cast<Server*>(n[0].usertype().data())->listen(n[1].string().c_str(), static_cast<int>(n[2].number()));
    
    return Nil;
}

Value http_server_bind_to_any_port(std::vector<Value>& n)
{
    if (n.size() != 2)
        throw std::runtime_error("httpServerBindToAnyPort: needs 2 arguments: server, host");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerBindToAnyPort: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerBindToAnyPort: host must be a String");
    
    return Value(static_cast<Server*>(n[0].usertype().data())->bind_to_any_port(n[1].string().c_str()));
}

Value http_server_listen_after_bind(std::vector<Value>& n)
{
    if (n.size() != 1)
        throw std::runtime_error("httpServerListenAfterBind: needs a single argument: server");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerListenAfterBind: server must be an httpServer");
    
    static_cast<Server*>(n[0].usertype().data())->listen_after_bind();

    return Nil;
}

Value http_server_set_mount_point(std::vector<Value>& n)
{
    if (n.size() != 3)
        throw std::runtime_error("httpServerSetMountPoint: needs 3 arguments: server, folder, destination");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerSetMountPoint: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerSetMountPoint: folder must be a String");
    if (n[2].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerSetMountPoint: destination must be a String");
    
    auto ret = static_cast<Server*>(n[0].usertype().data())->set_mount_point(n[1].string().c_str(), n[2].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_remove_mount_point(std::vector<Value>& n)
{
    if (n.size() != 2)
        throw std::runtime_error("httpServerRmMountPoint: needs 2 arguments: server, folder");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerRmMountPoint: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerRmMountPoint: folder must be a String");
    
    auto ret = static_cast<Server*>(n[0].usertype().data())->remove_mount_point(n[1].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_set_fext_mimetype(std::vector<Value>& n)
{
    if (n.size() != 3)
        throw std::runtime_error("httpServerSetFileExtAndMimetypeMapping: needs 3 arguments: server, ext, mimetype");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Server))
        throw Ark::TypeError("httpServerSetFileExtAndMimetypeMapping: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerSetFileExtAndMimetypeMapping: ext must be a String");
    if (n[2].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerSetFileExtAndMimetypeMapping: mimetype must be a String");
    
    static_cast<Server*>(n[0].usertype().data())->set_file_extension_and_mimetype_mapping(
        n[1].string().c_str(), n[2].string().c_str()
    );
    return Nil;
}

Value http_server_enable_logger(std::vector<Value>& n)
{
    if (n.size() > 1)
        throw std::runtime_error("httpServerEnableLogger: needs 0 or 1 argument: [level=1]");
    
    if (n.size() == 1 && n[0].valueType() != ValueType::String)
        throw Ark::TypeError("httpServerEnableLogger: level must be a Number");

    int& level = get_logger_level();
    if (n.size() == 1)
        level = static_cast<int>(n[0].number());
    else
        level = 1;

    return Nil;
}

/*
    ***********************************
                   Client
    ***********************************
*/

Value http_create_headers(std::vector<Value>& n)
{
    std::list<Headers>& h = get_headers();
    h.emplace_back();
    h.back().insert(std::pair<std::string, std::string>());

    if ((n.size() % 2) == 1)
        throw std::runtime_error("httpCreateHeaders: needs an even number of arguments: [header -> value]");
    
    std::string key = "";
    for (const Value& v : n)
    {
        if (v.valueType() != ValueType::String)
            throw Ark::TypeError("httpCreateHeaders: takes only String as argument");
        
        if (key == "")
            key = v.string();
        else
        {
            h.back().insert(std::pair<std::string, std::string>(key, v.string()));
            key = "";
        }
    }

    Value headers = Ark::Value(Ark::UserType(&h.back()));
    headers.usertype_ref().setOStream([](std::ostream& os, const UserType& A) -> std::ostream& {
        os << "httpHeaders<";
        for (auto& p : *static_cast<Headers*>(A.data()))
            std::cout << "\n\t" << p.first << " -> " << p.second;
        os << ">";
        return os;
    });
    return headers;
}

Value http_create_client(std::vector<Value>& n)
{
    if (n.size() != 2)
        throw std::runtime_error("httpCreateClient: needs 2 arguments: host and port");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("httpCreateClient: host must be a String");
    if (n[1].valueType() != ValueType::Number)
        throw Ark::TypeError("httpCreateClient: port must be a Number");

    std::list<Client>& c = get_clients();
    c.emplace_back(n[0].string().c_str(), static_cast<int>(n[1].number()));

    Value client = Ark::Value(Ark::UserType(&c.back()));
    client.usertype_ref().setOStream([](std::ostream& os, const UserType& A) -> std::ostream& {
        os << "httpClient<0x" << A.data() << ">";
        return os;
    });
    return client;
}

Value http_client_get(std::vector<Value>& n)
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("httpClientGet: needs 2 arguments: client, route, [headers]");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Client))
        throw Ark::TypeError("httpClientGet: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpClientGet: route must be a String");
    
    Headers* h = nullptr;

    if (n.size() == 3)
    {
        if (n[2].valueType() != ValueType::User || n[2].usertype().type_id() != typeid(Headers))
            throw Ark::TypeError("httpClientGet: headers must be httpHeaders");
        else
            h = static_cast<Headers*>(n[2].usertype().data());
    }

    Client* c = static_cast<Client*>(n[0].usertype().data());
    std::string route = n[1].string();
    auto res = (h == nullptr) ? c->Get(route.c_str()) : c->Get(route.c_str(), *h);

    if (!res)
        return Nil;
    
    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_create_params(std::vector<Value>& n)
{
    std::list<Params>& p = get_params();
    p.emplace_back();
    p.back().insert(std::pair<std::string, std::string>());

    if ((n.size() % 2) == 1)
        throw std::runtime_error("httpCreateParams: needs an even number of arguments: [key -> value]");
    
    std::string key = "";
    for (const Value& v : n)
    {
        if (v.valueType() != ValueType::String)
            throw Ark::TypeError("httpCreateParams: takes only String as arguments");
        
        if (key == "")
            key = v.string();
        else
        {
            p.back().insert(std::pair<std::string, std::string>(key, v.string()));
            key = "";
        }
    }

    Value params = Ark::Value(Ark::UserType(&p.back()));
    params.usertype_ref().setOStream([](std::ostream& os, const UserType& A) -> std::ostream& {
        os << "httpParams<";
        for (auto& p : *static_cast<Params*>(A.data()))
            std::cout << "\n\t" << p.first << " -> " << p.second;
        os << ">";
        return os;
    });
    return params;
}

Value http_client_post(std::vector<Value>& n)
{
    if (n.size() != 3)
        throw std::runtime_error("httpClientPost: needs 3 arguments: client, route, parameters");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Client))
        throw Ark::TypeError("httpClientPost: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpClientPost: route must be a String");
    if (n[2].valueType() != ValueType::String ||
            (n[2].valueType() != ValueType::User || n[2].usertype().type_id() != typeid(Params)))
        throw Ark::TypeError("httpClientPost: parameters must be a String or httpParams");

    Client* c = static_cast<Client*>(n[0].usertype().data());
    std::string route = n[1].string();
    auto res = (n[2].valueType() == ValueType::String) ?
        c->Post(route.c_str(), n[2].string().c_str(), "text/plain")
        : c->Post(route.c_str(), *static_cast<Params*>(n[2].usertype().data()));

    if (!res)
        return Nil;
    
    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_client_put(std::vector<Value>& n)
{
    if (n.size() != 3)
        throw std::runtime_error("httpClientPut: needs 3 arguments: client, route, parameters");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Client))
        throw Ark::TypeError("httpClientPut: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpClientPut: route must be a String");
    if (n[2].valueType() != ValueType::String ||
            (n[2].valueType() != ValueType::User || n[2].usertype().type_id() != typeid(Params)))
        throw Ark::TypeError("httpClientPut: parameters must be a String or httpParams");

    Client* c = static_cast<Client*>(n[0].usertype().data());
    std::string route = n[1].string();
    auto res = (n[2].valueType() == ValueType::String) ?
        c->Put(route.c_str(), n[2].string().c_str(), "text/plain")
        : c->Put(route.c_str(), *static_cast<Params*>(n[2].usertype().data()));

    if (!res)
        return Nil;
    
    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_client_delete(std::vector<Value>& n)
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("httpClientDelete: needs 2 arguments: client, route, [data]");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Client))
        throw Ark::TypeError("httpClientDelete: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("httpClientDelete: route must be a String");
    
    std::string content = "";

    if (n.size() == 3)
    {
        if (n[2].valueType() != ValueType::String)
            throw Ark::TypeError("httpClientDelete: data must be a String");
        else
            content = n[2].string();
    }

    Client* c = static_cast<Client*>(n[0].usertype().data());
    std::string route = n[1].string();
    auto res = (content.empty()) ? c->Delete(route.c_str()) : c->Delete(route.c_str(), content.c_str(), "text/plain");

    if (!res)
        return Nil;
    
    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_client_set_follow_location(std::vector<Value>& n)
{
    if (n.size() != 2)
        throw std::runtime_error("httpClientSetFollowLocation: needs 2 arguments: client, value");
    if (n[0].valueType() != ValueType::User || n[0].usertype().type_id() != typeid(Client))
        throw Ark::TypeError("httpClientSetFollowLocation: client must be an httpClient");
    if (n[1] != Ark::True || n[1] != Ark::False)
        throw Ark::TypeError("httpClientSetFollowLocation: value must be a Boolean");
    
    static_cast<Client*>(n[0].usertype().data())->set_follow_location(n[1] == True);

    return Nil;
}