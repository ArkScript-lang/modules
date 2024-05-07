#ifndef ARK_MSGPACK_ADAPTOR_HPP
#define ARK_MSGPACK_ADAPTOR_HPP

#include <Ark/Module.hpp>
#include <msgpack.hpp>

namespace msgpack
{
    using namespace Ark;

    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
    {
        namespace adaptor
        {
            // convert ark value
            template <>
            struct convert<Value>
            {
                void convert_list(const msgpack::object_array o, Value& list_v) const
                {
                    for (std::size_t i = 0; i < o.size; ++i)
                    {
                        Value v;

                        if (o.ptr[i].type == type::NIL)
                            v = Nil;
                        else if (o.ptr[i].type == type::BOOLEAN)
                            if (o.ptr[i].via.boolean == true)
                                v = True;
                            else
                                v = False;
                        else if (o.ptr[i].type == type::FLOAT)
                            v = Value(o.ptr[i].via.f64);
                        else if (o.ptr[i].type == type::STR)
                        {
                            std::size_t s = o.ptr[i].via.str.size;
                            v = Value(std::string(o.ptr[i].via.str.ptr, s));
                        }
                        else if (o.ptr[i].type == type::ARRAY)
                        {
                            v = Value(ValueType::List);
                            convert_list(o.ptr[i].via.array, v);
                        }

                        list_v.push_back(v);
                    }
                }

                msgpack::object const& operator()(msgpack::object const& o, Value& v) const
                {
                    if (o.type == type::NIL)
                        v = Nil;
                    else if (o.type == type::BOOLEAN)
                        if (o.via.boolean == true)
                            v = True;
                        else
                            v = False;
                    else if (o.type == type::FLOAT)
                        v = Value(o.via.f64);
                    else if (o.type == type::STR)
                    {
                        std::size_t s = o.via.str.size;
                        v = Value(std::string(o.via.str.ptr, s));
                    }
                    else
                    {
                        Value list_v = Value(ValueType::List);
                        convert_list(o.via.array, list_v);
                        v = list_v;
                    }

                    return o;
                }
            };

            // pack ark value
            template <>
            struct pack<Value>
            {
                template <typename Stream>
                packer<Stream>& operator()(msgpack::packer<Stream>& o, Value const& v) const
                {

                    if (v.valueType() == ValueType::Nil)
                        o.pack_nil();
                    else if (v.valueType() == ValueType::True)
                        o.pack_true();
                    else if (v.valueType() == ValueType::False)
                        o.pack_false();
                    else if (v.valueType() == ValueType::Number)
                        o.pack_double(static_cast<Value>(v).number());
                    else if (v.valueType() == ValueType::String)
                    {
                        String str = static_cast<Value>(v).string();
                        o.pack_str(str.size());
                        o.pack_str_body(str.c_str(), str.size());
                    }
                    else
                    {
                        std::vector<Value> list = static_cast<Value>(v).list();
                        o.pack_array(list.size());
                        for (std::size_t i = 0, end = list.size(); i < end; ++i)
                            o.pack(list[i]);
                    }

                    return o;
                }
            };
        }
    }
}

#endif