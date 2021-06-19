#ifndef ARK_MSGPACK_ADAPTOR_HPP
#define ARK_MSGPACK_ADAPTOR_HPP

#include <Ark/Module.hpp>
#include <msgpack.hpp>

namespace msgpack
{
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
    {
        namespace adaptor
        {
            // convert ark value
            template<> struct convert<Value>
            {
                msgpack::object const& operator()(msgpack::object const& o, Value& v) const
                {
                    if(o.type == type::NIL)
                    {
                        v = Ark::Nil;
                    }
                    else if(o.type == type::BOOLEAN)
                    {
                        if(o.via.boolean == true)
                            v = Ark::True;
                        else
                            v = Ark::False;
                    }
                    else if(o.type == type::FLOAT)
                    {
                        v = Value(o.via.f64);
                    }
                    else if(o.type == type::STR)
                    {
                        v = Value(std::string {o.via.str.ptr});
                    }
                    else
                    {
                        Value lst_v  = Ark::Value(ValueType::List);

                        for(unsigned i  = 0; i < o.via.array.size; ++ i)
                        {
                            Value ark_object_v;

                            if(o.via.array.ptr[i].type == type::NIL)
                            {
                                ark_object_v = Ark::Nil;
                            }
                            else if(o.via.array.ptr[i].type == type::BOOLEAN)
                            {
                                if(o.via.array.ptr[i].via.boolean == true)
                                    ark_object_v = Ark::True;
                                else
                                    ark_object_v = Ark::False;
                            }
                            else if(o.via.array.ptr[i].type == type::FLOAT)
                            {
                                ark_object_v = Value(o.via.array.ptr[i].via.f64);
                            }
                            else if(o.via.array.ptr[i].type == type::STR)
                            {
                                ark_object_v = Value(std::string { o.via.array.ptr[i].via.str.ptr });
                            }

                            // TODO : sub list convert
                            lst_v.push_back(ark_object_v);
                        }

                        v = lst_v;
                    }

                    return o;
                }
            };

            // pack ark value
            template<> struct pack<Value>
            {
                template<typename Stream> packer<Stream>& operator()(msgpack::packer<Stream>& o, Value const& v) const
                {

                    if(v.valueType() == ValueType::Nil)
                    {
                        o.pack_nil();
                    }
                    else if(v.valueType() == ValueType::True)
                    {
                        o.pack_true();
                    }
                    else if(v.valueType() == ValueType::False)
                    {
                        o.pack_false();
                    }
                    else if(v.valueType() == ValueType::Number)
                    {
                        o.pack_double(static_cast<Value>(v).number());
                    }
                    else if(v.valueType() == ValueType::String)
                    {
                        std::string str { static_cast<Value>(v).stringRef().toString() };
                        o.pack_str(str.size());
                        o.pack_str_body(str.c_str(), str.size());
                    }
                    else
                    {
                        std::vector<Value> list { static_cast<Value>(v).list() };
                        o.pack_array(list.size());
                        for(unsigned i = 0; i < list.size(); ++ i) { o.pack(list[i]); }
                    }

                    return o;
                }
            };
        }
    }
}

#endif