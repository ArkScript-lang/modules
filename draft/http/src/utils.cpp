#include <httplib.h>

#include <http_module.hpp>

#include <algorithm>
#include <list>
#include <ctime>

using namespace httplib;

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

    srv.set_logger([](const auto& req, const auto& res) {
        if (auto level = get_logger_level(); level > 0)
        {
            switch (level)
            {
                case 1:
                {
                    std::time_t t = std::time(nullptr);
                    std::cout << "[" << t << "]" << req.method << "\t" << req.path << "(" << res.status << ")";
                    break;
                }

                case 2:
                    std::cout << " -- " << req.body;
                    break;

                default:
                    break;
            }
            std::cout << "\n";
        }
    });


    if (get_error_handler())
        srv.set_error_handler([](const auto& /*req*/, auto& res) {
            std::string fmt = "<p>Error Status: <span style='color:red;'>" + std::to_string(res.status) + "</span></p>";
            res.set_content(fmt, "text/html");
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
        for (auto it = clients.begin(), end = clients.end(); it != end; ++it)
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
        for (auto it = headers.begin(), end = headers.end(); it != end; ++it)
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
        for (auto it = params.begin(), end = params.end(); it != end; ++it)
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

void typecheck(std::string_view funcname, const std::vector<Value>& n)
{
    if (n.size() != 2 || n[0].valueType() != ValueType::String || !n[1].isFunction())
        types::generateError(
            funcname,
            { { types::Contract { { types::Typedef("route", ValueType::String), types::Typedef("content", ValueType::PageAddr) } },
                types::Contract { { types::Typedef("route", ValueType::String), types::Typedef("content", ValueType::Closure) } },
                types::Contract { { types::Typedef("route", ValueType::String), types::Typedef("content", ValueType::CProc) } } } },
            n);
}
