#include <http_module.hpp>
#include <httplib.hpp>
#include <list>

using namespace httplib;

/*
    ***********************************
                   Misc
    ***********************************
*/

std::list<Params>& get_params()
{
    static std::list<Params> params;
    return params;
}

std::list<Headers>& get_headers()
{
    static std::list<Headers> headers;
    return headers;
}

std::list<Client>& get_clients()
{
    static std::list<Client> clients;
    return clients;
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