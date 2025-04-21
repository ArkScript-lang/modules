#include <httplib.h>

#include <http_module.hpp>

#include <algorithm>
#include <list>
#include <ctime>

using namespace httplib;

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

UserType::ControlFuncs* get_cfs_client()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "httpClient<0x" << a.data() << ">";
        return os;
    };
    cfs.deleter = [](void* data) {
        std::vector<std::unique_ptr<Client>>& clients = get_clients();
        for (auto it = clients.begin(), end = clients.end(); it != end; ++it)
        {
            if (it->get() == data)
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
        std::vector<std::unique_ptr<Headers>>& headers = get_headers();
        for (auto it = headers.begin(), end = headers.end(); it != end; ++it)
        {
            if (it->get() == data)
            {
                headers.erase(it);
                break;
            }
        }
    };
    return &cfs;
}
