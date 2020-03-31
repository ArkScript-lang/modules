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
					if(o.type == type::FLOAT64 || o.type == type::BOOLEAN)
					{
						v = Value(o.via.f64);
					}
					else if(o.type == type::STR)
					{
						v = Value(std::string {o.via.str.ptr});
					}
					else
					{
						Value lst_v {ValueType::List};

						for(unsigned i {0}; i < o.via.array.size; ++ i)
						{
							Value ark_object_v;

							// number
							if(o.via.array.ptr[i].type == type::FLOAT64 || o.via.array.ptr[i].type == type::BOOLEAN)
								ark_object_v = Value(o.via.array.ptr[i].via.f64);
							// string
							else if(o.via.array.ptr[i].type == type::STR)
								ark_object_v = Value(std::string {o.via.array.ptr[i].via.str.ptr});

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
					if(v.valueType() == ValueType::NFT)
					{
						if(v == Ark::True)
							o.pack_true();
						else
							o.pack_false();
					}
					else if(v.valueType() == ValueType::Number)
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