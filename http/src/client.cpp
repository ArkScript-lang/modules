#include <http_module.hpp>

Value http_create_headers(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if ((n.size() % 2) == 1)
        throw std::runtime_error("http:headers:create: needs an even number of arguments: [header -> value]");

    std::list<Headers>& h = get_headers();
    h.emplace_back();

    std::string key = "";
    for (Value& v : n)
    {
        if (v.valueType() != ValueType::String)
            throw TypeError("http:headers:create: takes only String as argument");

        if (key == "")
            key = v.stringRef();
        else
        {
            h.back().insert(std::pair<std::string, std::string>(key, v.stringRef()));
            key = "";
        }
    }

    Value headers = Value(UserType(&h.back(), get_cfs_header()));
    return headers;
}

Value http_create_client(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::String, ValueType::Number))
        types::generateError(
            "http:client:create",
            { { types::Contract { { types::Typedef("host", ValueType::String), types::Typedef("port", ValueType::Number) } } } },
            n);

    std::list<Client>& c = get_clients();
    c.emplace_back(n[0].stringRef(), static_cast<int>(n[1].number()));

    return Value(UserType(&c.back(), get_cfs_client()));
}

Value http_client_get(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("http:client:get: needs 2 to 3 arguments: client, route, [headers]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw TypeError("http:client:get: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw TypeError("http:client:get: route must be a String");

    Headers* headers = nullptr;

    if (n.size() == 3)
    {
        if (n[2].valueType() != ValueType::User || !n[2].usertype().is<Headers>())
            throw TypeError("http:client:get: headers must be httpHeaders");
        else
            headers = &n[2].usertypeRef().as<Headers>();
    }

    Client& c = n[0].usertypeRef().as<Client>();
    std::string route = n[1].stringRef();
    auto res = (headers == nullptr) ? c.Get(route.c_str()) : c.Get(route.c_str(), *headers);

    if (!res)
        return Nil;

    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

Value http_create_params(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if ((n.size() % 2) == 1)
        throw std::runtime_error("http:params:create: needs an even number of arguments: [key -> value]");

    std::list<Params>& p = get_params();
    p.emplace_back();

    std::string key = "";
    for (Value& v : n)
    {
        if (v.valueType() != ValueType::String)
            throw TypeError("http:params:create: takes only String as arguments");

        if (key == "")
            key = v.stringRef();
        else
        {
            p.back().insert(std::pair<std::string, std::string>(key, v.stringRef()));
            key = "";
        }
    }

    Value params = Value(UserType(&p.back(), get_cfs_param()));
    return params;
}

Value http_params_tolist(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User) || !n[0].usertypeRef().is<Params>())
        types::generateError(
            "http:params:toList",
            { { types::Contract { { types::Typedef("params", ValueType::User) } } } },
            n);

    Value lst(ValueType::List);
    for (auto& s : n[0].usertypeRef().as<Params>())
    {
        Value inner(ValueType::List);
        inner.push_back(Value(s.first));
        inner.push_back(Value(s.second));
        lst.push_back(inner);
    }

    return lst;
}

Value http_client_post(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (n.size() < 3 || n.size() > 5)
        throw std::runtime_error("http:client:post: needs 3 to 5 arguments: client, route, parameters, [content-type], [headers]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw TypeError("http:client:post: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw TypeError("http:client:post: route must be a String");
    if (n[2].valueType() != ValueType::String ||
        (n[2].valueType() != ValueType::User || !n[2].usertype().is<Params>()))
        throw TypeError("http:client:post: parameters must be a String or httpParams");

    Client& c = n[0].usertypeRef().as<Client>();
    std::string route = n[1].stringRef();

    Headers* headers = nullptr;

    // if params is a string, headers come last
    if (n.size() == 5 && n[2].valueType() == ValueType::String)
    {
        if (n[4].valueType() != ValueType::User || !n[4].usertype().is<Headers>())
            throw TypeError("http:client:post: headers must be httpHeaders");
        else
            headers = &n[4].usertypeRef().as<Headers>();
    }
    else if (n[2].valueType() != ValueType::String && n.size() == 4)
    {
        if (n[3].valueType() != ValueType::User || !n[3].usertype().is<Headers>())
            throw TypeError("http:client:post: headers must be httpHeaders");
        else
            headers = &n[3].usertypeRef().as<Headers>();
    }

    if (n[2].valueType() == ValueType::String && n.size() >= 4 && n[3].valueType() == ValueType::String)
    {
        auto res = headers != nullptr ? c.Post(route.c_str(), *headers, /* body */ n[2].string().c_str(), /* content type */ n[3].string().c_str())
                                      : c.Post(route.c_str(), /* body */ n[2].string().c_str(), /* content type */ n[3].string().c_str());
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
    else if (n[2].valueType() == ValueType::String && n.size() == 3)
    {
        auto res = headers != nullptr ? c.Post(route.c_str(), *headers, /* body */ n[2].string().c_str(), /* content type */ "text/plain")
                                      : c.Post(route.c_str(), /* body */ n[2].string().c_str(), /* content type */ "text/plain");
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
    else
    {
        auto res = headers != nullptr ? c.Post(route.c_str(), *headers, n[2].usertype().as<Params>())
                                      : c.Post(route.c_str(), n[2].usertype().as<Params>());
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
}

Value http_client_put(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (n.size() < 3 || n.size() > 5)
        throw std::runtime_error("http:client:put: needs 3 to 5 arguments: client, route, parameters, [content-type], [headers]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw TypeError("http:client:put: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw TypeError("http:client:put: route must be a String");
    if (n[2].valueType() != ValueType::String ||
        (n[2].valueType() != ValueType::User || !n[2].usertype().is<Params>()))
        throw TypeError("http:client:put: parameters must be a String or httpParams");

    Client& c = n[0].usertypeRef().as<Client>();
    std::string route = n[1].stringRef();

    Headers* headers = nullptr;

    // if params is a string, headers come last
    if (n.size() == 5 && n[2].valueType() == ValueType::String)
    {
        if (n[4].valueType() != ValueType::User || !n[4].usertype().is<Headers>())
            throw TypeError("http:client:put: headers must be httpHeaders");
        else
            headers = &n[4].usertypeRef().as<Headers>();
    }
    else if (n[2].valueType() != ValueType::String && n.size() == 4)
    {
        if (n[3].valueType() != ValueType::User || !n[3].usertype().is<Headers>())
            throw TypeError("http:client:put: headers must be httpHeaders");
        else
            headers = &n[3].usertypeRef().as<Headers>();
    }

    if (n[2].valueType() == ValueType::String && n.size() >= 4 && n[3].valueType() == ValueType::String)
    {
        auto res = headers != nullptr ? c.Put(route.c_str(), *headers, /* body */ n[2].string().c_str(), /* content type */ n[3].string().c_str())
                                      : c.Put(route.c_str(), /* body */ n[2].string().c_str(), /* content type */ n[3].string().c_str());
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
    else if (n[2].valueType() == ValueType::String && n.size() == 3)
    {
        auto res = headers != nullptr ? c.Put(route.c_str(), *headers, /* body */ n[2].string().c_str(), /* content type */ "text/plain")
                                      : c.Put(route.c_str(), /* body */ n[2].string().c_str(), /* content type */ "text/plain");
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
    else
    {
        auto res = headers != nullptr ? c.Put(route.c_str(), *headers, n[2].usertype().as<Params>())
                                      : c.Put(route.c_str(), n[2].usertype().as<Params>());
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
}

Value http_client_delete(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (n.size() < 3 || n.size() > 5)
        throw std::runtime_error("http:client:delete: needs 3 to 5 arguments: client, route, body, [content-type], [headers]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw TypeError("http:client:delete: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw TypeError("http:client:delete: route must be a String");
    if (n[2].valueType() != ValueType::String)
        throw TypeError("http:client:delete: body must be a String");

    Client& c = n[0].usertypeRef().as<Client>();
    std::string route = n[1].stringRef();

    Headers* headers = nullptr;

    // if params is a string, headers come last
    if (n.size() == 5)
    {
        if (n[4].valueType() != ValueType::User || !n[4].usertype().is<Headers>())
            throw TypeError("http:client:delete: headers must be httpHeaders");
        else
            headers = &n[4].usertypeRef().as<Headers>();
    }

    if (n.size() >= 4 && n[3].valueType() == ValueType::String)
    {
        auto res = headers != nullptr ? c.Delete(route.c_str(), *headers, /* body */ n[2].string().c_str(), /* content type */ n[3].string().c_str())
                                      : c.Delete(route.c_str(), /* body */ n[2].string().c_str(), /* content type */ n[3].string().c_str());
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
    else
    {
        auto res = headers != nullptr ? c.Delete(route.c_str(), *headers, /* body */ n[2].string().c_str(), /* content type */ "text/plain")
                                      : c.Delete(route.c_str(), /* body */ n[2].string().c_str(), /* content type */ "text/plain");
        if (!res)
            return Nil;

        Value data = Value(ValueType::List);
        data.push_back(Value(res->status));
        data.push_back(Value(res->body));

        return data;
    }
}

Value http_client_set_follow_location(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if ((!types::check(n, ValueType::User, ValueType::True) && !types::check(n, ValueType::User, ValueType::False)) || !n[0].usertypeRef().is<Client>())
        types::generateError(
            "http:client:setFollowLocation",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User), types::Typedef("value", ValueType::True) } },
                types::Contract { { types::Typedef("httpClient", ValueType::User), types::Typedef("value", ValueType::False) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_follow_location(n[1] == True);
    return Nil;
}

Value http_client_set_co_timeout(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::Number, ValueType::Number) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setConnectionTimeout",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("seconds", ValueType::Number),
                                    types::Typedef("microseconds", ValueType::Number) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_connection_timeout(
        static_cast<int>(n[1].number()),
        static_cast<int>(n[2].number()));

    return Nil;
}

Value http_client_set_read_timeout(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::Number, ValueType::Number) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setReadTimeout",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("seconds", ValueType::Number),
                                    types::Typedef("microseconds", ValueType::Number) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_read_timeout(
        static_cast<int>(n[1].number()),
        static_cast<int>(n[2].number()));

    return Nil;
}

Value http_client_set_write_timeout(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::Number, ValueType::Number) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setWriteTimeout",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("seconds", ValueType::Number),
                                    types::Typedef("microseconds", ValueType::Number) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_write_timeout(
        static_cast<int>(n[1].number()),
        static_cast<int>(n[2].number()));

    return Nil;
}

Value http_client_set_basic_auth(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::String, ValueType::String) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setBasicAuth",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("username", ValueType::String),
                                    types::Typedef("password", ValueType::String) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_basic_auth(
        n[1].string().c_str(),
        n[2].string().c_str());
    return Nil;
}

Value http_client_set_bearer_token_auth(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::String) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setBearerTokenAuth",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("token", ValueType::String) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_bearer_token_auth(n[1].string().c_str());
    return Nil;
}

Value http_client_set_keep_alive(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if ((!types::check(n, ValueType::User, ValueType::True) && !types::check(n, ValueType::User, ValueType::False)) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setKeepAlive",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("toggle", ValueType::True) } },
                types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("toggle", ValueType::False) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_keep_alive(n[1] == True);
    return Nil;
}

Value http_client_set_proxy(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::String, ValueType::Number) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setProxy",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("host", ValueType::String),
                                    types::Typedef("port", ValueType::Number) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_proxy(n[1].string().c_str(), static_cast<int>(n[2].number()));
    return Nil;
}

Value http_client_set_proxy_basic_auth(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::String, ValueType::String) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setProxyBasicAuth",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("username", ValueType::String),
                                    types::Typedef("password", ValueType::String) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_proxy_basic_auth(
        n[1].string().c_str(),
        n[2].string().c_str());

    return Nil;
}

Value http_client_set_proxy_bearer_token_auth(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::String) || !n[0].usertype().is<Client>())
        types::generateError(
            "http:client:setProxyBearerTokenAuth",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("token", ValueType::String) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_proxy_bearer_token_auth(n[1].string().c_str());
    return Nil;
}
