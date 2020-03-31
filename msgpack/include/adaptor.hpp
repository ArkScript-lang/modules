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
						Value lst {ValueType::List};

						for(unsigned i {0}; i < o.via.array.size; ++ i)
						{
							Value ark_object;
							try
							{
								ark_object = Value(o.via.array.ptr[i].via.f64);
							}
							catch(const std::bad_cast &e)
							{

								try
								{
									ark_object = Value(std::string {o.via.array.ptr[i].via.str.ptr});
								}
								catch(const std::bad_cast &e) 
								{
									/*
									try
									{
										for(unsigned j = 0; j < o.via.array.ptr[i].via.array.size; ++ j)
										{

										}
									}static_cast<Value>(v).list()
									catch(const std::bad_cast &e) {}*/
								} // sub list convert
							}

							lst.push_back(ark_object);
						}
						v = lst;
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

/*
			// ark value msgpack object implementation
			template <> struct object<Value> 
			{
				void operator()(msgpack::object& o, Value const& v) const 
				{
					if(v.valueType() == ValueType::NFT)
					{
						o.type = type::BOOLEAN; 
						o.via.boolean = static_cast<Value>(v).number();
					}
					else if(v.valueType() == ValueType::Number)
					{
						o.type = type::FLOAT64;
						o.via.f64 = static_cast<Value>(v).number();
					}
					else
					{
						std::vector<Value> lst {static_cast<Value>(v).list()};

						o.type = type::ARRAY;
						o.via.array.size = lst.size();
						o.via.array.ptr = static_cast<msgpack::object*>(std::malloc(sizeof(msgpack::object) * o.via.array.size));
						
						for(unsigned i {0}; i < o.via.array.size; ++ i) 
						{
							if(lst[i].valueType() == ValueType::NFT)
							{
								o.via.array.ptr[i].type = type::BOOLEAN;
								o.via.array.ptr[i].via.f64 = static_cast<Value>(lst[i]).number();
							}
							else if(lst[i].valueType() == ValueType::Number)
							{
								o.via.array.ptr[i].type = type::FLOAT64;
								o.via.array.ptr[i].via.f64 = static_cast<Value>(lst[i]).number();								
							}
							else
							{
								o.via.array.ptr[i].type = type::STR;
								o.via.array.ptr[i].via.str.ptr = static_cast<Value>(lst[i]).string_ref().c_str();							
							}
						}
					}
				}
			};
*/	
		}
	}
}

#endif	