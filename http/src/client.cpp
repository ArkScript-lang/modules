#include <httplib.hpp>
#include <http_module.hpp>

using namespace httplib;

/*
    ***********************************
                   Client
    ***********************************
*/

Value http_create_headers(std::vector<Value>& n, Ark::VM* vm)
{
    if ((n.size() % 2) == 1)
        throw std::runtime_error("http:headers:create: needs an even number of arguments: [header -> value]");

    std::vector<std::unique_ptr<Headers>>& h = get_headers();
    h.emplace_back(std::make_unique<std::pair<std::string, std::string>>());

    std::string key = "";
    for (Value& v : n)
    {
        if (v.valueType() != ValueType::String)
            throw Ark::TypeError("http:headers:create: takes only String as argument");

        if (key == "")
            key = v.string_ref().toString();
        else
        {
            h.back()->insert(std::pair<std::string, std::string>(key, v.string_ref().toString()));
            key = "";
        }
    }

    Value headers = Ark::Value(Ark::UserType(h.back().get()));
    headers.usertype_ref().setControlFuncs(get_cfs_header());
    return headers;
}

Value http_create_client(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:client:create: needs 2 arguments: host and port");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:client:create: host must be a String");
    if (n[1].valueType() != ValueType::Number)
        throw Ark::TypeError("http:client:create: port must be a Number");

    std::vector<std::unique_ptr<Client>>& c = get_clients();
    c.emplace_back(std::make_unique<Client>(n[0].string_ref().toString(), static_cast<int>(n[1].number())));

    Value client = Ark::Value(Ark::UserType(c.back().get()));
    client.usertype_ref().setControlFuncs(get_cfs_client());
    return client;
}

Value http_client_get(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("http:client:get: needs 2 arguments: client, route, [headers]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw Ark::TypeError("http:client:get: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:client:get: route must be a String");

    Headers* h = nullptr;

    if (n.size() == 3)
    {
        if (n[2].valueType() != ValueType::User || !n[2].usertype().is<Headers>())
            throw Ark::TypeError("http:client:get: headers must be httpHeaders");
        else
            h = &n[2].usertype().as<Headers>();
    }

    Client& c = n[0].usertype().as<Client>();
    std::string route = n[1].string_ref().toString();
    auto res = (h == nullptr) ? c.Get(route.c_str()) : c.Get(route.c_str(), *h);

    if (!res)
        return Nil;

    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_create_params(std::vector<Value>& n, Ark::VM* vm)
{
    if ((n.size() % 2) == 1)
        throw std::runtime_error("http:params:create: needs an even number of arguments: [key -> value]");

    std::vector<std::unique_ptr<Params>>& p = get_params();
    p.emplace_back(std::make_unique<Params>());

    std::string key = "";
    for (Value& v : n)
    {
        if (v.valueType() != ValueType::String)
            throw Ark::TypeError("http:params:create: takes only String as arguments");

        if (key == "")
            key = v.string_ref().toString();
        else
        {
            p.back()->insert(std::pair<std::string, std::string>(key, v.string_ref().toString()));
            key = "";
        }
    }

    Value params = Ark::Value(Ark::UserType(p.back().get()));
    params.usertype_ref().setControlFuncs(get_cfs_param());
    return params;
}

Value http_client_post(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("http:client:post: needs 3 arguments: client, route, parameters");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw Ark::TypeError("http:client:post: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:client:post: route must be a String");
    if (n[2].valueType() != ValueType::String ||
            (n[2].valueType() != ValueType::User || !n[2].usertype().is<Params>()))
        throw Ark::TypeError("http:client:post: parameters must be a String or httpParams");

    Client& c = n[0].usertype().as<Client>();
    std::string route = n[1].string_ref().toString();
    auto res = (n[2].valueType() == ValueType::String) ?
        c.Post(route.c_str(), n[2].string().c_str(), "text/plain")
        : c.Post(route.c_str(), n[2].usertype().as<Params>());

    if (!res)
        return Nil;

    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_client_put(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("http:client:put: needs 3 arguments: client, route, parameters");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw Ark::TypeError("http:client:put: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:client:put: route must be a String");
    if (n[2].valueType() != ValueType::String ||
            (n[2].valueType() != ValueType::User || !n[2].usertype().is<Params>()))
        throw Ark::TypeError("http:client:put: parameters must be a String or httpParams");

    Client& c = n[0].usertype().as<Client>();
    std::string route = n[1].string_ref().toString();
    auto res = (n[2].valueType() == ValueType::String) ?
        c.Put(route.c_str(), n[2].string().c_str(), "text/plain")
        : c.Put(route.c_str(), n[2].usertype().as<Params>());

    if (!res)
        return Nil;

    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_client_delete(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("http:client:delete: needs 2 arguments: client, route, [data]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw Ark::TypeError("http:client:delete: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:client:delete: route must be a String");

    std::string content = "";

    if (n.size() == 3)
    {
        if (n[2].valueType() != ValueType::String)
            throw Ark::TypeError("http:client:delete: data must be a String");
        else
            content = n[2].string_ref().toString();
    }

    Client& c = n[0].usertype().as<Client>();
    std::string route = n[1].string_ref().toString();
    auto res = (content.empty()) ? c.Delete(route.c_str()) : c.Delete(route.c_str(), content.c_str(), "text/plain");

    if (!res)
        return Nil;

    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_client_set_follow_location(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:client:setFollowLocation: needs 2 arguments: client, value");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw Ark::TypeError("http:client:setFollowLocation: client must be an httpClient");
    if (n[1] != Ark::True && n[1] != Ark::False)
        throw Ark::TypeError("http:client:setFollowLocation: value must be a Boolean");

    n[0].usertype().as<Client>().set_follow_location(n[1] == True);

    return Nil;
}