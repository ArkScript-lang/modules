#include <http_module.hpp>
#include <iostream>

// we need [status-code, content, type]
#define CHECK_FUNC_RETURN_VAL_FOR_REQ(val)                           \
    (val.valueType() == ValueType::List && val.list().size() == 3 && \
     val.list()[0].valueType() == ValueType::Number &&               \
     val.list()[1].valueType() == ValueType::String &&               \
     val.list()[2].valueType() == ValueType::String)

Value http_create_server(std::vector<Value>& n [[maybe_unused]], VM* vm [[maybe_unused]])
{
    create_server();
    return Nil;
}

using HttpMethod_t = Server& (Server::*)(const std::string&, Server::Handler);

Value handle_request_for(std::string_view funcname, HttpMethod_t handler, std::vector<Value>& n, VM* vm)
{
    typecheck(funcname, n);
    Server& srv = create_server();

    (srv.*handler)(n[0].string().c_str(), [funcname, n, vm](const Request& req, Response& res) {
        // todo: find a way to have 1 execution context per thread. Should we use a lock?
        //       Pre-create as many execution contexts as threads inside cpp-httplib pool?
        //       Track contexts that are being used to find the first available one
        std::string content;
        std::string type;
        Value r;

        // checking for req.matches
        if (req.matches.size() > 1)
        {
            Value matches(ValueType::List);
            for (std::size_t i = 1, end = req.matches.size(); i < end; ++i)
                matches.push_back(Value(req.matches[i]));

            if (req.params.size() == 0)
                r = Nil;  // fixme: vm->resolve(&n[1], matches, req.body, Nil);
            else
            {
                // craft params
                std::list<Params>& p = get_params();
                p.emplace_back(req.params);

                Value params = Value(UserType(&p.back(), get_cfs_param()));

                r = Nil;  // fixme: vm->resolve(&n[1], matches, req.body, params);
            }
        }
        // no matches, maybe params
        else if (req.params.size() != 0)
        {
            // craft params
            std::list<Params>& p = get_params();
            p.emplace_back(req.params);

            Value params = Value(UserType(&p.back(), get_cfs_param()));

            r = Nil;  // fixme: vm->resolve(&n[1], Nil, req.body, params);
        }
        // no matches, no params
        else
            r = Nil;  // fixme: vm->resolve(&n[1], Nil, req.body, Nil);

        if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
        {
            res.status = static_cast<int>(r.list()[0].number());
            content = r.list()[1].stringRef();
            type = r.list()[2].stringRef();
        }
        else
        {
            // internal server error because we couldn't match the request
            res.status = 500;
            content = "Couldn't parse the return value for the route '" + n[0].string() + "' (" + std::string(funcname) + ")";
        }

        res.set_content(content, type.c_str());
        return res.status;
    });

    return Nil;
}

Value http_server_get(std::vector<Value>& n, VM* vm)
{
    return handle_request_for("http:server:get", &Server::Get, n, vm);
}

Value http_server_post(std::vector<Value>& n, VM* vm)
{
    return handle_request_for("http:server:post", &Server::Post, n, vm);
}

Value http_server_put(std::vector<Value>& n, VM* vm)
{
    return handle_request_for("http:server:put", &Server::Put, n, vm);
}

Value http_server_delete(std::vector<Value>& n, VM* vm)
{
    return handle_request_for("http:server:delete", &Server::Delete, n, vm);
}

Value http_server_stop(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    create_server().stop();
    return Nil;
}

Value http_server_listen(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::String) && !types::check(n, ValueType::String, ValueType::Number))
        types::generateError(
            "http:server:listen",
            { { types::Contract { { types::Typedef("host", ValueType::String) } },
                types::Contract { { types::Typedef("host", ValueType::String), types::Typedef("port", ValueType::Number) } } } },
            n);

    Server& srv = create_server();

    if (n[0].string() == "0.0.0.0")
    {
        srv.bind_to_any_port("0.0.0.0");
        srv.listen_after_bind();
    }
    else
        srv.listen(n[0].string().c_str(), static_cast<int>(n[1].number()));

    return Nil;
}

Value http_server_set_mount_point(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::String, ValueType::String))
        types::generateError(
            "http:server:setMountPoint",
            { { types::Contract { { types::Typedef("folder", ValueType::String),
                                    types::Typedef("destination", ValueType::String) } } } },
            n);

    auto ret = create_server().set_mount_point(n[0].string().c_str(), n[1].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_remove_mount_point(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::String))
        types::generateError(
            "http:server:rmMountPoint",
            { { types::Contract { { types::Typedef("folder", ValueType::String) } } } },
            n);

    auto ret = create_server().remove_mount_point(n[0].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_set_fext_mimetype(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::String, ValueType::String))
        types::generateError(
            "http:server:setFileExtAndMimetypeMapping",
            { { types::Contract { { types::Typedef("ext", ValueType::String),
                                    types::Typedef("mimetype", ValueType::String) } } } },
            n);

    create_server().set_file_extension_and_mimetype_mapping(
        n[0].string().c_str(), n[1].string().c_str());
    return Nil;
}

Value http_server_enable_logger(std::vector<Value>& n, VM* vm [[maybe_unused]])
{
    if (!types::check(n, ValueType::Number))
        types::generateError(
            "http:server:enableLogger",
            { { types::Contract { { types::Typedef("logLevel", ValueType::Number) } } } },
            n);

    int& level = get_logger_level();
    level = static_cast<int>(n[0].number());
    return Nil;
}
