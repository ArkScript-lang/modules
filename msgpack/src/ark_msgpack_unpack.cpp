# include <ark_msgpack_unpack.hpp>

namespace ArkMsgpack
{
	namespace Minimal
	{
		Value unpack(std::vector<Value> &args)
		{
			//unpack(Value packed(string or list)) and return an object unpacked 
			if(args.size() != 1)
				throw std::runtime_error("ArgError : This function must have 1 arguments");
			if(args[0].valueType() != ValueType::String && args[0].valueType() != ValueType::List)
				throw Ark::TypeError("The packed buffer must be a string or a list");    
			Value dst;
			bool ark_bool;
			double ark_number;
			std::string ark_string;
			msgpack::object deserialized;
			auto type_test = [&](void) {
				std::string packed {static_cast<Value>(args[0]).string_ref()};
				deserialized = msgpack::unpack(packed.data(), packed.size()).get();
				try
				{
					deserialized.convert(ark_bool);
					dst = Value(ark_bool);
				}
				catch(const std::bad_cast &e)
				{
					try
					{
						deserialized.convert(ark_number);
						dst = Value(ark_number);
					}
					catch(const std::bad_cast &e) 
					{
						try
						{
							deserialized.convert(ark_string);
							dst = Value(ark_string);
						}
						catch(const std::exception &e) {}
					}
				}
			};

			if(args[0].valueType() == ValueType::List)
			{
				std::vector<Value> packed {static_cast<Value>(args[0]).list()};
				dst = list_unpacking(packed);
			}
			else
			{
				type_test();
			}

			return dst;
		}

		Value object_str(std::vector<Value> &args)
		{
			if(args.size() != 1)
				throw std::runtime_error("ArgError : This function must have 1 argument");
			if(args[0].valueType() != ValueType::String && args[0].valueType() != ValueType::List)
				throw Ark::TypeError("The packed buffer must be a string or a list");
			Value msg_object_str;
			std::ostringstream str_buffer; 
			msgpack::object deserialized;

			if(args[0].valueType() == ValueType::List)
			{
				list_unpacked_str(static_cast<Value>(args[0]).list(), str_buffer);
				msg_object_str = Value(str_buffer.str());
			}
			else
			{
				std::string packed {static_cast<Value>(args[0]).string_ref()};
				deserialized =  msgpack::unpack(packed.data(), packed.size()).get();
				str_buffer << deserialized;
				msg_object_str = Value(str_buffer.str());
			}
		
			return msg_object_str;
		}

		void list_unpacked_str(std::vector<Value> &buffer_list, std::ostringstream &stream)
		{
			msgpack::object deserialized;
			stream << '[';
 
			for(unsigned i {0}; i < buffer_list.size(); ++ i)
			{
				std::string buffer {static_cast<Value>(buffer_list[i]).string_ref()};
				deserialized = msgpack::unpack(buffer.data(), buffer.size()).get();
				if(i > 0)
					stream << ' ';
				stream << deserialized;
			}
		
			stream << ']';
		}

		Value list_unpacking(std::vector<Value> &buffer_list)
		{
			std::vector<Value> list;
			bool ark_bool;
			double ark_number;
			std::string ark_string;
			msgpack::object deserialized;
			auto each_to_value = [&](void) {
				try
				{
					deserialized.convert(ark_bool);
					list.push_back(Value(ark_bool));
				}
				catch(const std::bad_cast &e)
				{
					try
					{
						deserialized.convert(ark_number);
						list.push_back(Value(ark_number));
					}
					catch(const std::bad_cast &e) 
					{
						try
						{
							deserialized.convert(ark_string);
							list.push_back(Value(ark_string));                  
						}
						catch(std::exception &e) {}
					}
				}
			};
 
			for(unsigned i {0}; i < buffer_list.size(); ++ i)
			{
				std::string buffer {static_cast<Value>(buffer_list[i]).string_ref()};
				deserialized = msgpack::unpack(buffer.data(), buffer.size()).get();
				each_to_value();
			}

			return list;
		}
	}
}