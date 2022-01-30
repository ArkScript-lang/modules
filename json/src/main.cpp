#include <Ark/Module.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>

namespace json
{

    std::vector<std::unique_ptr<nlohmann::json>>& get_json_object()
    {
        static std::vector<std::unique_ptr<nlohmann::json>> jsonObject;
        return jsonObject;
    }

    UserType::ControlFuncs* get_cfs()
    {
        static UserType::ControlFuncs cfs;
        cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
            os << a.as<nlohmann::json>();
            return os;
        };
        cfs.deleter = [](void* data) {
            std::vector<std::unique_ptr<nlohmann::json>>& jsons = get_json_object();
            auto it = std::find_if(jsons.begin(), jsons.end(), [data](const auto& val) -> bool {
                return val.get() == static_cast<nlohmann::json*>(data);
            });
            if (it != jsons.end())
                jsons.erase(it);
        };
        return &cfs;
    }

    Value open(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::String))
            types::generateError(
                "json:open",
                { { types::Contract { { types::Typedef("filename", ValueType::String) } } } },
                args);

        // open and read the file
        const char* filename = args[0].stringRef().c_str();
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

        // parsing
        nlohmann::json* ptr = get_json_object().emplace_back(std::make_unique<nlohmann::json>(nlohmann::json::parse(content))).get();
        Value v = Ark::Value(Ark::UserType(ptr, get_cfs()));

        return v;
    }

    Value fromString(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::String))
            types::generateError(
                "json:fromString",
                { { types::Contract { { types::Typedef("jsonObject", ValueType::String) } } } },
                args);

        // parsing
        nlohmann::json* ptr = get_json_object()
                                  .emplace_back(
                                      std::make_unique<nlohmann::json>(nlohmann::json::parse(args[0].stringRef().c_str())))
                                  .get();

        Value v = Ark::Value(Ark::UserType(ptr, get_cfs()));
        return v;
    }

    Value jsonToArk(nlohmann::json& obj)
    {
        if (obj.is_null())
            return Nil;
        else if (obj.is_boolean())
            return obj.get<bool>() ? True : False;
        else if (obj.is_number())
            return Value(obj.get<double>());
        else if (obj.is_array())
        {
            Value v(ValueType::List);
            for (auto it = obj.begin(), end = obj.end(); it != end; ++it)
                v.push_back(jsonToArk(*it));
            return v;
        }
        else if (obj.is_string())
            return Value(obj.get<std::string>());
        else  // is_object() == true
        {
            nlohmann::json* ptr = get_json_object().emplace_back(std::make_unique<nlohmann::json>(obj)).get();
            Value v = Ark::Value(Ark::UserType(ptr, get_cfs()));
            return v;
        }
    }

    Value get(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (args.size() < 2 || !args[0].usertype().is<nlohmann::json>())
            types::generateError(
                "json:get",
                { { types::Contract { { types::Typedef("json", ValueType::User), types::Typedef("key", ValueType::Number) } },
                    types::Contract { { types::Typedef("json", ValueType::User), types::Typedef("key", ValueType::String) } } } },
                args);

        if (types::check(args, ValueType::User, ValueType::Number))
        {
            if (args[0].usertypeRef().as<nlohmann::json>().is_array())
            {
                nlohmann::json& json_object = args[0].usertypeRef().as<nlohmann::json>();  // conversion to json object to be able to get the result
                nlohmann::json& obj = json_object[static_cast<unsigned>(args[1].number())];

                return jsonToArk(obj);
            }
            return Nil;
        }
        else if (types::check(args, ValueType::User, ValueType::String))
        {
            if (args[0].usertypeRef().as<nlohmann::json>().is_object())
            {
                nlohmann::json& json_object = args[0].usertypeRef().as<nlohmann::json>();  // conversion to json object to be able to get the result
                nlohmann::json& obj = json_object[args[1].stringRef().c_str()];

                return jsonToArk(obj);
            }
            return Nil;
        }

        types::generateError(
            "json:get",
            { { types::Contract { { types::Typedef("json", ValueType::User), types::Typedef("key", ValueType::Number) } },
                types::Contract { { types::Typedef("json", ValueType::User), types::Typedef("key", ValueType::String) } } } },
            args);
        return Nil;
    }

    Value toString(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::User))
            types::generateError(
                "json:toString",
                { { types::Contract { { types::Typedef("jsonObject", ValueType::User) } } } },
                args);

        nlohmann::json& json_object = args[0].usertypeRef().as<nlohmann::json>();
        return Value(json_object.dump());
    }

    nlohmann::json listToJson(std::vector<Value>& arkList)
    {
        nlohmann::json json_list;

        for (auto& el : arkList)
        {
            switch (el.valueType())
            {
                case ValueType::String:
                    json_list.emplace_back(el.stringRef().c_str());
                    break;

                case ValueType::Number:
                    json_list.emplace_back(el.number());
                    break;

                case ValueType::True:
                case ValueType::False:
                {
                    json_list.emplace_back(el == Ark::True);
                    break;
                }

                case ValueType::List:
                    json_list.emplace_back(listToJson(el.list()));
                    break;

                case ValueType::User:
                {
                    nlohmann::json& json_object = el.usertypeRef().as<nlohmann::json>();
                    json_list.emplace_back(json_object);
                    break;
                }

                default:
                    throw Ark::TypeError("json module: Couldn't parse the provided json");
            }
        }
        return json_list;
    }

    Value jset(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::User, ValueType::String, ValueType::Any) || !args[0].usertype().is<nlohmann::json>())
            types::generateError(
                "json:set",
                { { types::Contract { { types::Typedef("jsonObject", ValueType::User),
                                        types::Typedef("key", ValueType::String),
                                        types::Typedef("value", ValueType::Any) } } } },
                args);

        nlohmann::json& json_object = args[0].usertypeRef().as<nlohmann::json>();

        // definition of the value according to the object type
        switch (args[2].valueType())
        {
            case ValueType::String:
            {
                json_object[args[1].stringRef().c_str()] = args[2].stringRef().c_str();
                break;
            }

            case ValueType::Number:
            {
                json_object[args[1].stringRef().c_str()] = args[2].number();
                break;
            }

            case ValueType::False:
            case ValueType::True:
            {
                json_object[args[1].stringRef().c_str()] = (args[2] == Ark::True);
                break;
            }

            case ValueType::List:
            {
                json_object[args[1].stringRef().c_str()] = listToJson(args[2].list());
                break;
            }

            case ValueType::User:
            {
                if (!args[2].usertypeRef().is<nlohmann::json>())
                    throw Ark::TypeError("json:set: couldn't parse value (UserType)");

                nlohmann::json& json_to_push = args[2].usertypeRef().as<nlohmann::json>();
                json_object[args[1].stringRef().c_str()] = json_to_push;
                break;
            }

            default:
                break;
        }

        return Nil;
    }

    Value write(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::User, ValueType::String) || !args[0].usertype().is<nlohmann::json>())
            types::generateError(
                "json:write",
                { { types::Contract { { types::Typedef("jsonObject", ValueType::User),
                                        types::Typedef("filename", ValueType::String) } } } },
                args);

        std::ofstream file(args[1].stringRef().c_str());
        nlohmann::json& json_object = args[0].usertypeRef().as<nlohmann::json>();

        file << json_object.dump(4);
        file.close();
        return Nil;
    }

    Value fromList(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::List))
            types::generateError(
                "json:fromList",
                { { types::Contract { { types::Typedef("jsonAsList", ValueType::List) } } } },
                args);

        std::vector<Value>& ark_list = args[0].list();
        if (ark_list.size() % 2 != 0)
            throw std::runtime_error("json:fromList: the list must contain an even number of elements: even ones are the keys and odd ones are the values");

        nlohmann::json* ptr = get_json_object().emplace_back(std::make_unique<nlohmann::json>()).get();
        Value v = Ark::Value(Ark::UserType(ptr, get_cfs()));

        for (std::size_t i = 0, end = ark_list.size(); i < end; i += 2)
        {
            if (ark_list[i].valueType() != ValueType::String)
                throw Ark::TypeError("json:fromList: keys need to be of type String");

            switch (ark_list[i + 1].valueType())
            {
                case ValueType::String:
                    (*ptr)[ark_list[i].stringRef().c_str()] = ark_list[i + 1].stringRef().c_str();
                    break;

                case ValueType::Number:
                    (*ptr)[ark_list[i].stringRef().c_str()] = ark_list[i + 1].number();
                    break;

                case ValueType::True:
                case ValueType::False:
                    (*ptr)[ark_list[i].stringRef().c_str()] = (ark_list[i + 1] == Ark::True);
                    break;

                case ValueType::List:
                    (*ptr)[ark_list[i].stringRef().c_str()] = listToJson(ark_list[i + 1].list());
                    break;

                case ValueType::User:
                    if (!ark_list[i + 1].usertype().is<nlohmann::json>())
                        throw Ark::TypeError("json:fromList: value for key '" + ark_list[i].stringRef().toString() + "' isn't handled");

                    (*ptr)[ark_list[i].stringRef().c_str()] = ark_list[i + 1].usertypeRef().as<nlohmann::json>();
                    break;

                default:
                    throw Ark::TypeError("json:fromList: value for key '" + ark_list[i].stringRef().toString() + "' isn't handled");
            }
        }

        return v;
    }

    Value len(std::vector<Value>& args, Ark::VM* vm [[maybe_unused]])
    {
        if (!types::check(args, ValueType::User) || !args[0].usertype().is<nlohmann::json>())
            types::generateError(
                "json:len",
                { { types::Contract { { types::Typedef("jsonObject", ValueType::User) } } } },
                args);

        return Value(static_cast<double>(args[0].usertypeRef().as<nlohmann::json>().size()));
    }
}

ARK_API mapping* getFunctionsMapping()
{
    mapping* map = mapping_create(8);

    mapping_add(map[0], "json:open", json::open);
    mapping_add(map[1], "json:get", json::get);
    mapping_add(map[2], "json:toString", json::toString);
    mapping_add(map[3], "json:fromString", json::fromString);
    mapping_add(map[4], "json:set", json::jset);
    mapping_add(map[5], "json:write", json::write);
    mapping_add(map[6], "json:fromList", json::fromList);
    mapping_add(map[7], "json:len", json::len);

    return map;
}
