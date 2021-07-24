#include <http_module.hpp>
#include <iostream>

// we need [status-code, content, type]
#define CHECK_FUNC_RETURN_VAL_FOR_REQ(val) (val.valueType() == ValueType::List && val.list().size() == 3 && \
                                            val.list()[0].valueType() == ValueType::Number && \
                                            val.list()[1].valueType() == ValueType::String && \
                                            val.list()[2].valueType() == ValueType::String)

/*
    ***********************************
                   Server
    ***********************************
*/

Value http_create_server(std::vector<Value>& n, Ark::VM* vm)
{
    Server* srv = &create_server();
    // TODO make the logger great again
    if (get_logger_level() > 0)
        srv->set_logger([](const auto& req, const auto& res) {
            std::cout << "got request\n";
            std::cout << "method " << req.method << ", path " << req.path << ", body " << req.body << "\n";
            std::cout << "status " << res.status << "\n";
            std::cout << "==================\n\n";
        });

    if (get_error_handler())
        srv->set_error_handler([](const auto& req, const auto& res) {
            std::cout << "ERROR???\n";
            std::cout << "status " << res.status << "\n";
            std::cout << "==================\n\n";
        });

    Value server = Ark::Value(Ark::UserType(srv));
    server.usertypeRef().setControlFuncs(get_cfs_server());
    return server;
}

void http_server_generic(void Server::*proxy(const char*, Handler), std::vector<Value>& n, Ark::VM* vm)
{
    Server& srv = create_server();
    String route = n[0].string();

    srv.*proxy(route.c_str(), [n, vm](const Request& req, Response& res) {
        std::string content = (n[1].valueType() == ValueType::String) ? n[1].string().c_str() : "";
        std::string type = (n.size() == 3 && n[2].valueType() == ValueType::String) ? n[2].string().toString() : "text/plain";

        if (n[1].isFunction())
        {
            // don't get me wrong here, I didn't follow DRY for optimization purposes
            // so, don't DRY but WET

            // checking for req.matches
            if (req.matches.size() > 1)
            {
                Value matches(ValueType::List);
                for (std::size_t i=1, end=req.matches.size(); i < end; ++i)
                    matches.push_back(Value(req.matches[i]));

                if (req.params.size() == 0)
                {
                    Value r = vm->resolve(&n[1], matches, Nil);
                    if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                    {
                        res.status = static_cast<int>(r.list()[0].number());
                        content = r.list()[1].stringRef().toString();
                        type = r.list()[2].stringRef().toString();
                    }
                }
                else
                {
                    // craft params
                    std::list<Params>& p = get_params();
                    p.emplace_back(req.params);

                    Value params = Ark::Value(Ark::UserType(&p.back()));
                    params.usertypeRef().setControlFuncs(get_cfs_param());

                    Value r = vm->resolve(&n[1], matches, params);
                    if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                    {
                        res.status = static_cast<int>(r.list()[0].number());
                        content = r.list()[1].stringRef().toString();
                        type = r.list()[2].stringRef().toString();
                    }
                }
            }
            // no matches, maybe params
            else if (req.params.size() != 0)
            {
                // craft params
                std::list<Params>& p = get_params();
                p.emplace_back(req.params);

                Value params = Ark::Value(Ark::UserType(&p.back()));
                params.usertypeRef().setControlFuncs(get_cfs_param());

                Value r = vm->resolve(&n[1], params);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].stringRef().toString();
                    type = r.list()[2].stringRef().toString();
                }
            }
            // no matches, no params
            else
            {
                Value r = vm->resolve(&n[1], Nil);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].stringRef().toString();
                    type = r.list()[2].stringRef().toString();
                }
            }
        }

        res.set_content(content, type.c_str());
    });
}

Value http_server_get(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() < 2 || n.size() > 3)
        throw std::runtime_error("http:server:get needs between 2 and 3 arguments: route, content, [mimetype if content is a String]");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:get: route must be a String");
    if (n[1].valueType() != ValueType::String && !n[1].isFunction())
        throw Ark::TypeError("http:server:get: content must be a String or a Function");

    http_server_generic(&Server::Get, n, vm);
    return Nil;
}

Value http_server_post(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:server:post needs 2 arguments: route, content");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:post: route must be a String");
    if (!n[1].isFunction())
        throw Ark::TypeError("http:server:post: content must be a Function");

    http_server_generic(&Server::Post, n, vm);
    return Nil;
}

Value http_server_put(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:server:put needs 2 arguments: route, content");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:put: route must be a String");
    if (!n[1].isFunction())
        throw Ark::TypeError("http:server:put: content must be a Function");

    http_server_generic(&Server::Put, n, vm);
    return Nil;
}

Value http_server_delete(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:server:delete needs 2 arguments: route, content");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:delete: route must be a String");
    if (!n[1].isFunction())
        throw Ark::TypeError("http:server:delete: content must be a Function");

    http_server_generic(&Server::Delete, n, vm);
    return Nil;
}

Value http_server_stop(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 0)
        throw std::runtime_error("http:server:stop: don't take arguments");

    create_server().stop();
    return Nil;
}

Value http_server_listen(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() < 1 || n.size() > 2)
        throw std::runtime_error("http:server:listen: needs between 1 and 2 arguments: host, [port]");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:listen: host must be a String");
    if (n.size() == 2 && n[0].string() == "0.0.0.0" && n[1].valueType() != ValueType::Number)
        throw Ark::TypeError("http:server:listen: port must be a Number, and is a mandatory argument when host is 0.0.0.0");

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

Value http_server_set_mount_point(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:server:setMountPoint: needs 2 arguments: folder, destination");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setMountPoint: folder must be a String");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setMountPoint: destination must be a String");

    auto ret = create_server().set_mount_point(n[0].string().c_str(), n[1].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_remove_mount_point(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 1)
        throw std::runtime_error("http:server:rmMountPoint: needs 1 argument: folder");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:rmMountPoint: folder must be a String");

    auto ret = create_server().remove_mount_point(n[0].string().c_str());
    if (!ret)
        return False;  // directory doesn't exist
    return True;
}

Value http_server_set_fext_mimetype(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 2)
        throw std::runtime_error("http:server:setFileExtAndMimetypeMapping: needs 2 arguments: ext, mimetype");
    if (n[0].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setFileExtAndMimetypeMapping: ext must be a String");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:setFileExtAndMimetypeMapping: mimetype must be a String");

    create_server().set_file_extension_and_mimetype_mapping(
        n[0].string().c_str(), n[1].string().c_str()
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
