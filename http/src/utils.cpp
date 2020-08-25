#include <httplib.hpp>
#include <http_module.hpp>
#include <algorithm>

using namespace httplib;

/*
    ***********************************
                   Misc
    ***********************************
*/

std::vector<std::unique_ptr<Params>>& get_params()
{
    static std::vector<std::unique_ptr<Params>> params;
    return params;
}

std::vector<std::unique_ptr<Headers>>& get_headers()
{
    static std::vector<std::unique_ptr<Headers>> headers;
    return headers;
}

std::vector<std::unique_ptr<Client>>& get_clients()
{
    static std::vector<std::unique_ptr<Client>> clients;
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

    // TODO
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

UserType::ControlFuncs* get_cfs_client()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "httpClient<0x" << a.data() << ">";
        return os;
    };
    cfs.deleter = [](void* data) {
        std::vector<std::unique_ptr<Client>>& clients = get_clients();
        auto it = std::find_if(clients.begin(), clients.end(), [data](const auto& val) -> bool {
            return val.get() == static_cast<Client*>(data);
        });
        if (it != clients.end())
            clients.erase(it);
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
        std::vector<std::unique_ptr<Headers>>& headers = get_headers();
        auto it = std::find_if(headers.begin(), headers.end(), [data](const auto& val) -> bool {
            return val.get() == static_cast<Headers*>(data);
        });
        if (it != headers.end())
            headers.erase(it);
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
        std::vector<std::unique_ptr<Params>>& params = get_params();
        auto it = std::find_if(params.begin(), params.end(), [data](const auto& val) -> bool {
            return val.get() == static_cast<Params*>(data);
        });
        if (it != params.end())
            params.erase(it);
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
    cfs.deleter = [](void* data) {
        // we don't have anything to do since there is only one server
        // it will be automatically freed
    };
    return &cfs;
}