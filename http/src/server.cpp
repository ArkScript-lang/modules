#include <httplib.hpp>
#include <http_module.hpp>
#include <list>

using namespace httplib;

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
    Value server = Ark::Value(Ark::UserType(&create_server()));
    server.usertype_ref().setControlFuncs(get_cfs_server());
    return server;
}

Value http_server_get(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() < 3 || n.size() > 4)
        throw std::runtime_error("http:server:get needs 3: server, route, content, [mimetype if content is a String]");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:get: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:get: route must be a String");
    if (n[2].valueType() != ValueType::String && !n[2].isFunction())
        throw Ark::TypeError("http:server:get: content must be a String or a Function");

    Server& srv = n[0].usertype().as<Server>();
    srv.Get(n[1].string().c_str(), [n, vm](const Request& req, Response& res) {
        std::string content = (n[2].valueType() == ValueType::String) ? n[2].string().toString() : "";
        std::string type = (n.size() == 4 && n[3].valueType() == ValueType::String) n[3].string().toString() ? "text/plain";

        if (n[2].isFunction())
        {
            // don't get me wrong here, I didn't follow DRY for optimization purposes
            // so, don't DRY but WET

            // checking for req.matches
            if (req.matches.size() != 0)
            {
                Value matches(ValueType::List);
                for (std::size_t i=0, end=req.matches.size(); i < end; ++i)
                    // matches[i] is a string, we have implicit conversion to Value(ValueType::String here)
                    matches.push_back(req.matches[i]);

                if (req.params.size() == 0)
                {
                    Value r = vm->resolve(&n[2], matches, Nil);
                    if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                    {
                        res.status = static_cast<int>(r.list()[0].number());
                        content = r.list()[1].string_ref().toString();
                        type = r.list()[2].string_ref().toString();
                    }
                }
                else
                {
                    // craft params
                    std::vector<std::unique_ptr<Params>>& p = get_params();
                    p.emplace_back(std::make_unique<Params>(req.params));

                    Value params = Ark::Value(Ark::UserType(p.back().get()));
                    params.usertype_ref().setControlFuncs(get_cfs_param());

                    Value r = vm->resolve(&n[2], matches, params);
                    if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                    {
                        res.status = static_cast<int>(r.list()[0].number());
                        content = r.list()[1].string_ref().toString();
                        type = r.list()[2].string_ref().toString();
                    }
                }
            }
            // no matches, maybe params
            else if (req.params.size() != 0)
            {
                // craft params
                std::vector<std::unique_ptr<Params>>& p = get_params();
                p.emplace_back(std::make_unique<Params>(req.params));

                Value params = Ark::Value(Ark::UserType(p.back().get()));
                params.usertype_ref().setControlFuncs(get_cfs_param());

                Value r = vm->resolve(&n[2], params);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
            // no matches, no params
            else
            {
                Value r = vm->resolve(&n[2], Nil);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
        }

        res.set_content(content, type.c_str());
        return res.status;
    });

    return Nil;
}

Value http_server_post(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("http:server:post needs 3 arguments: server, route, content");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:post: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:post: route must be a String");
    if (!n[2].isFunction())
        throw Ark::TypeError("http:server:post: content must be a Function");

    Server& srv = n[0].usertype().as<Server>();
    srv.Post(n[1].string().c_str(), [n, vm](const Request& req, Response& res) {
        std::string content;
        std::string type;

        // don't get me wrong here, I didn't follow DRY for optimization purposes
        // so, don't DRY but WET

        // checking for req.matches
        if (req.matches.size() != 0)
        {
            Value matches(ValueType::List);
            for (std::size_t i=0, end=req.matches.size(); i < end; ++i)
                // matches[i] is a string, we have implicit conversion to Value(ValueType::String here)
                matches.push_back(req.matches[i]);

            if (req.params.size() == 0)
            {
                Value r = vm->resolve(&n[2], matches, req.body, Nil);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
            else
            {
                // craft params
                std::vector<std::unique_ptr<Params>>& p = get_params();
                p.emplace_back(std::make_unique<Params>(req.params));

                Value params = Ark::Value(Ark::UserType(p.back().get()));
                params.usertype_ref().setControlFuncs(get_cfs_param());

                Value r = vm->resolve(&n[2], matches, req.body, params);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
        }
        // no matches, maybe params
        else if (req.params.size() != 0)
        {
            // craft params
            std::vector<std::unique_ptr<Params>>& p = get_params();
            p.emplace_back(std::make_unique<Params>(req.params));

            Value params = Ark::Value(Ark::UserType(p.back().get()));
            params.usertype_ref().setControlFuncs(get_cfs_param());

            Value r = vm->resolve(&n[2], req.body, params);
            if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
            {
                res.status = static_cast<int>(r.list()[0].number());
                content = r.list()[1].string_ref().toString();
                type = r.list()[2].string_ref().toString();
            }
        }
        // no matches, no params
        else
        {
            Value r = vm->resolve(&n[2], req.body, Nil);
            if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
            {
                res.status = static_cast<int>(r.list()[0].number());
                content = r.list()[1].string_ref().toString();
                type = r.list()[2].string_ref().toString();
            }
        }

        res.set_content(content, type.c_str());
        return res.status;
    });

    return Nil;
}

Value http_server_put(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("http:server:put needs 3 arguments: server, route, content");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:put: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:put: route must be a String");
    if (!n[2].isFunction())
        throw Ark::TypeError("http:server:put: content must be a Function");

    Server& srv = n[0].usertype().as<Server>();
    srv.Put(n[1].string().c_str(), [n, vm](const Request& req, Response& res) {
        std::string content;
        std::string type;

        // don't get me wrong here, I didn't follow DRY for optimization purposes
        // so, don't DRY but WET

        // checking for req.matches
        if (req.matches.size() != 0)
        {
            Value matches(ValueType::List);
            for (std::size_t i=0, end=req.matches.size(); i < end; ++i)
                // matches[i] is a string, we have implicit conversion to Value(ValueType::String here)
                matches.push_back(req.matches[i]);

            if (req.params.size() == 0)
            {
                Value r = vm->resolve(&n[2], matches, req.body, Nil);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
            else
            {
                // craft params
                std::vector<std::unique_ptr<Params>>& p = get_params();
                p.emplace_back(std::make_unique<Params>(req.params));

                Value params = Ark::Value(Ark::UserType(p.back().get()));
                params.usertype_ref().setControlFuncs(get_cfs_param());

                Value r = vm->resolve(&n[2], matches, req.body, params);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
        }
        // no matches, maybe params
        else if (req.params.size() != 0)
        {
            // craft params
            std::vector<std::unique_ptr<Params>>& p = get_params();
            p.emplace_back(std::make_unique<Params>(req.params));

            Value params = Ark::Value(Ark::UserType(p.back().get()));
            params.usertype_ref().setControlFuncs(get_cfs_param());

            Value r = vm->resolve(&n[2], req.body, params);
            if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
            {
                res.status = static_cast<int>(r.list()[0].number());
                content = r.list()[1].string_ref().toString();
                type = r.list()[2].string_ref().toString();
            }
        }
        // no matches, no params
        else
        {
            Value r = vm->resolve(&n[2], req.body, Nil);
            if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
            {
                res.status = static_cast<int>(r.list()[0].number());
                content = r.list()[1].string_ref().toString();
                type = r.list()[2].string_ref().toString();
            }
        }

        res.set_content(content, type.c_str());
        return res.status;
    });

    return Nil;
}

Value http_server_delete(std::vector<Value>& n, Ark::VM* vm)
{
    if (n.size() != 3)
        throw std::runtime_error("http:server:delete needs 3 arguments: server, route, content");
    if (n[0].valueType() != ValueType::User || !n[0].usertype().is<Server>())
        throw Ark::TypeError("http:server:delete: server must be an httpServer");
    if (n[1].valueType() != ValueType::String)
        throw Ark::TypeError("http:server:delete: route must be a String");
    if (!n[2].isFunction())
        throw Ark::TypeError("http:server:delete: content must be a Function");

    Server& srv = n[0].usertype().as<Server>();
    srv.Delete(n[1].string().c_str(), [n, vm](const Request& req, Response& res) {
        std::string content;
        std::string type;

        // don't get me wrong here, I didn't follow DRY for optimization purposes
        // so, don't DRY but WET

        // checking for req.matches
        if (req.matches.size() != 0)
        {
            Value matches(ValueType::List);
            for (std::size_t i=0, end=req.matches.size(); i < end; ++i)
                // matches[i] is a string, we have implicit conversion to Value(ValueType::String here)
                matches.push_back(req.matches[i]);

            if (req.params.size() == 0)
            {
                Value r = vm->resolve(&n[2], matches, req.body, Nil);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
            else
            {
                // craft params
                std::vector<std::unique_ptr<Params>>& p = get_params();
                p.emplace_back(std::make_unique<Params>(req.params));

                Value params = Ark::Value(Ark::UserType(p.back().get()));
                params.usertype_ref().setControlFuncs(get_cfs_param());

                Value r = vm->resolve(&n[2], matches, req.body, params);
                if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
                {
                    res.status = static_cast<int>(r.list()[0].number());
                    content = r.list()[1].string_ref().toString();
                    type = r.list()[2].string_ref().toString();
                }
            }
        }
        // no matches, maybe params
        else if (req.params.size() != 0)
        {
            // craft params
            std::vector<std::unique_ptr<Params>>& p = get_params();
            p.emplace_back(std::make_unique<Params>(req.params));

            Value params = Ark::Value(Ark::UserType(p.back().get()));
            params.usertype_ref().setControlFuncs(get_cfs_param());

            Value r = vm->resolve(&n[2], req.body, params);
            if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
            {
                res.status = static_cast<int>(r.list()[0].number());
                content = r.list()[1].string_ref().toString();
                type = r.list()[2].string_ref().toString();
            }
        }
        // no matches, no params
        else
        {
            Value r = vm->resolve(&n[2], req.body, Nil);
            if (CHECK_FUNC_RETURN_VAL_FOR_REQ(r))
            {
                res.status = static_cast<int>(r.list()[0].number());
                content = r.list()[1].string_ref().toString();
                type = r.list()[2].string_ref().toString();
            }
        }

        res.set_content(content, type.c_str());
        return res.status;
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