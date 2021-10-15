#include <httplib.hpp>
#include <http_module.hpp>
#include <algorithm>
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

    // TODO make the logger great again
    if (get_logger_level() > 0)
        srv.set_logger([](const auto& req, const auto& res) {
            std::cout << "got request\n";
            std::cout << "method " << req.method << ", path " << req.path << ", body " << req.body << "\n";
            std::cout << "status " << res.status << "\n";
            std::cout << "==================\n\n";
        });

    if (get_error_handler())
        srv.set_error_handler([](const auto& req [[maybe_unused]], const auto& res) {
            std::cout << "ERROR???\n";
            std::cout << "status " << res.status << "\n";
            std::cout << "==================\n\n";
        });

    return srv;
}

UserType::ControlFuncs* get_cfs_client()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "httpClient<0x" << a.data() << ">";
        return os;
    };
    cfs.deleter = [](void* data) {
        std::list<Client>& clients = get_clients();
        for (auto it=clients.begin(), end=clients.end(); it != end; ++it)
        {
            if (&(*it) == data)
            {
                clients.erase(it);
                break;
            }
        }
    };
    return &cfs;
}

UserType::ControlFuncs* get_cfs_header()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "httpHeaders<";
        for (const auto& p : a.as<Headers>())
            os << "\n\t" << p.first << " -> " << p.second;
        os << ">";
        return os;
    };
    cfs.deleter = [](void* data) {
        std::list<Headers>& headers = get_headers();
        for (auto it=headers.begin(), end=headers.end(); it != end; ++it)
        {
            if (&(*it) == data)
            {
                headers.erase(it);
                break;
            }
        }
    };
    return &cfs;
}

UserType::ControlFuncs* get_cfs_param()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "httpParams<";
        for (const auto& p : a.as<Params>())
            os << "\n\t" << p.first << " -> " << p.second;
        os << ">";
        return os;
    };
    cfs.deleter = [](void* data) {
        std::list<Params>& params = get_params();
        for (auto it=params.begin(), end=params.end(); it != end; ++it)
        {
            if (&(*it) == data)
            {
                params.erase(it);
                break;
            }
        }
    };
    return &cfs;
}

UserType::ControlFuncs* get_cfs_server()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "httpServer<0x" << a.data() << ">";
        return os;
    };
    cfs.deleter = [](void* data [[maybe_unused]]) {
        // we don't have anything to do since there is only one server
        // it will be automatically freed
    };
    return &cfs;
}
