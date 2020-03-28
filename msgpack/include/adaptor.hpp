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

			template<> struct convert<Value> 
			{
				msgpack::object const& operator()(msgpack::object const& o, Value& v) const 
				{
					if(v.valueType() == ValueType::Number)
					{
						v = Value(o.via.f64);
					}
					else if(v.valueType() == ValueType::String)
					{
						v = Value(std::string {o.via.str.ptr});
					}
					else
					{
						for(unsigned i {0}; i < o.via.array.size; ++ i)
						{
							Value ark_object;
							try
							{
								ark_object = Value(o.via.array.ptr[i].as<double>());
							}
							catch(const std::bad_cast &e)
							{
								try
								{
									ark_object = Value(o.via.array.ptr[i].as<std::string>());
								}
								catch(const std::bad_cast &e) {}
							}
							v.push_back(ark_object);
						}
					}
					
					return o;
				}
			};

			template<> struct pack<Value> 
			{
				template<typename Stream> packer<Stream>& operator()(msgpack::packer<Stream>& o, Value const& v) const 
				{
					if(v.valueType() == ValueType::Number)
					{
						o.pack_double(static_cast<Value>(v).number());
					}
					else if(v.valueType() == ValueType::String)
					{
						std::string str {static_cast<Value>(v).string_ref()};
						o.pack_str(str.size());
						o.pack_str_body(str.c_str(), str.size());
					} 
					else
					{
						std::vector<Value> list {static_cast<Value>(v).list()};
						o.pack_array(list.size());
						for(unsigned i {0}; i < list.size(); ++ i) {o.pack(list[i]);}
					}

					return o;
				}
			};
		}
	}
}

#endif	