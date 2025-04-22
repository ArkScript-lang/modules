#include <http_module.hpp>

Value http_create_headers(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if ((n.size() % 2) == 1)
        throw std::runtime_error("http:headers: needs an even number of arguments: [header -> value]");

    std::vector<std::unique_ptr<Headers>>& h = get_headers();
    h.emplace_back(std::make_unique<Headers>());

    std::string key;
    for (Value& v : n)
    {
        if (v.valueType() != ValueType::String)
            throw TypeError("http:headers: takes only String as argument");

        if (key.empty())
            key = v.stringRef();
        else
        {
            h.back()->insert(std::pair<std::string, std::string>(key, v.stringRef()));
            key = "";
        }
    }

    return Value(UserType(h.back().get(), get_cfs_header()));
}

Value http_create_client(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::String, ValueType::Number))
        throw types::TypeCheckingError(
            "http:client",
            { { types::Contract { { types::Typedef("host", ValueType::String), types::Typedef("port", ValueType::Number) } } } },
            n);

    std::vector<std::unique_ptr<Client>>& c = get_clients();
    c.emplace_back(std::make_unique<Client>(n[0].stringRef(), static_cast<int>(n[1].number())));

    return Value(UserType(c.back().get(), get_cfs_client()));
}

Value http_client_get(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("http:get: needs 2 to 3 arguments: client, route, [headers]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw TypeError("http:get: client must be an httpClient");
    if (n[1].valueType() != ValueType::String)
        throw TypeError("http:get: route must be a String");

    Headers* headers = nullptr;

    if (n.size() == 3)
    {
        if (n[2].valueType() != ValueType::User || !n[2].usertype().is<Headers>())
            throw TypeError("http:get: headers must be httpHeaders");
        else
            headers = &n[2].usertypeRef().as<Headers>();
    }

    auto& c = n[0].usertypeRef().as<Client>();
    std::string route = n[1].stringRef();
    auto res = c.Get(route.c_str(), (headers == nullptr) ? Headers() : *headers);

    if (!res)
        return Nil;

    Value data = Value(ValueType::List);
    data.push_back(Value(res->status));
    data.push_back(Value(res->body));

    return data;
}

enum class HttpVerb
{
    Get,
    Post,
    Patch,
    Put,
    Delete
};

std::string verb_as_string(HttpVerb verb)
{
    switch (verb)
    {
        case HttpVerb::Get:
            return "get";
        case HttpVerb::Post:
            return "post";
        case HttpVerb::Patch:
            return "patch";
        case HttpVerb::Put:
            return "put";
        case HttpVerb::Delete:
            return "delete";
    }
}

Headers* get_headers(std::vector<Value>& n, const std::string& verb)
{
    if (n.size() == 4)
    {
        if (n.back().valueType() == ValueType::User && n.back().usertype().is<Headers>())
            return &n.back().usertypeRef().as<Headers>();
    }
    else if (n.size() == 5)
    {
        if (n.back().valueType() == ValueType::User && n.back().usertype().is<Headers>())
            return &n.back().usertypeRef().as<Headers>();
        throw TypeError(fmt::format("http:{}: headers must be httpHeaders", verb));
    }

    return nullptr;
}

Value handle_http_data_verb(HttpVerb kind, std::vector<Value>& n)
{
    const std::string verb = verb_as_string(kind);

    if (n.size() < 3 || n.size() > 5)
        throw std::runtime_error(fmt::format("http:{}: needs 3 to 5 arguments: client, route, body, [content-type], [headers]", verb));
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Client>())
        throw TypeError(fmt::format("http:{}: client must be an httpClient", verb));
    if (n[1].valueType() != ValueType::String)
        throw TypeError(fmt::format("http:{}: route must be a String", verb));
    if (n[2].valueType() != ValueType::String)
        throw TypeError(fmt::format("http:{}: body must be a String", verb));

    auto& c = n[0].usertypeRef().as<Client>();
    std::string route = n[1].stringRef();
    const auto content_type = n.size() >= 4 ? n[3].string().c_str() : "text/plain";
    Headers* headers = get_headers(n, verb);

    std::optional<Result> maybe_res;

    switch (kind)
    {
        case HttpVerb::Get:
            // Not handled as we can't send data using Get
            break;

        case HttpVerb::Post:
            maybe_res = c.Post(route.c_str(), (headers == nullptr) ? Headers() : *headers, /* body= */ n[2].string(), /* content_type= */ content_type);
            break;
        case HttpVerb::Patch:
            maybe_res = c.Patch(route.c_str(), (headers == nullptr) ? Headers() : *headers, /* body= */ n[2].string(), /* content_type= */ content_type);
            break;
        case HttpVerb::Put:
            maybe_res = c.Put(route.c_str(), (headers == nullptr) ? Headers() : *headers, /* body= */ n[2].string(), /* content_type= */ content_type);
            break;
        case HttpVerb::Delete:
            maybe_res = c.Delete(route.c_str(), (headers == nullptr) ? Headers() : *headers, /* body= */ n[2].string(), /* content_type= */ content_type);
            break;
    }

    if (!maybe_res || !maybe_res.value())
        return Nil;

    Value data = Value(ValueType::List);
    data.push_back(Value(maybe_res.value()->status));
    data.push_back(Value(maybe_res.value()->body));

    return data;
}

Value http_client_post(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    return handle_http_data_verb(HttpVerb::Post, n);
}

Value http_client_put(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    return handle_http_data_verb(HttpVerb::Put, n);
}

Value http_client_delete(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    return handle_http_data_verb(HttpVerb::Delete, n);
}

Value http_client_patch(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    return handle_http_data_verb(HttpVerb::Patch, n);
}

Value http_client_set_follow_location(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if ((!types::check(n, ValueType::User, ValueType::True) && !types::check(n, ValueType::User, ValueType::False)) || !n[0].usertypeRef().is<Client>())
        throw types::TypeCheckingError(
            "http:setFollowLocation",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User), types::Typedef("value", ValueType::True) } },
                types::Contract { { types::Typedef("httpClient", ValueType::User), types::Typedef("value", ValueType::False) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_follow_location(n[1] == True);
    return Nil;
}

Value http_client_set_co_timeout(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::User, ValueType::Number, ValueType::Number) || !n[0].usertype().is<Client>())
        throw types::TypeCheckingError(
            "http:setConnectionTimeout",
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
        throw types::TypeCheckingError(
            "http:setReadTimeout",
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
        throw types::TypeCheckingError(
            "http:setWriteTimeout",
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
        throw types::TypeCheckingError(
            "http:setBasicAuth",
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
        throw types::TypeCheckingError(
            "http:setBearerTokenAuth",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("token", ValueType::String) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_bearer_token_auth(n[1].string().c_str());
    return Nil;
}

Value http_client_set_keep_alive(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if ((!types::check(n, ValueType::User, ValueType::True) && !types::check(n, ValueType::User, ValueType::False)) || !n[0].usertype().is<Client>())
        throw types::TypeCheckingError(
            "http:setKeepAlive",
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
        throw types::TypeCheckingError(
            "http:setProxy",
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
        throw types::TypeCheckingError(
            "http:setProxyBasicAuth",
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
        throw types::TypeCheckingError(
            "http:setProxyBearerTokenAuth",
            { { types::Contract { { types::Typedef("httpClient", ValueType::User),
                                    types::Typedef("token", ValueType::String) } } } },
            n);

    n[0].usertypeRef().as<Client>().set_proxy_bearer_token_auth(n[1].string().c_str());
    return Nil;
}
