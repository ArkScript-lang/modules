#include <ark_msgpack_pack.hpp>

namespace ArkMsgpack
{
	namespace Minimal
	{
		Value pack(std::vector<Value> &args)
    	{
        	// pack(Value src) and return a buffer's string packed 
        	if(args.size() != 1)
            	throw std::runtime_error("ArgError : This function must have 1 arguments");
        	ValueType type {args[0].valueType()};
        	std::stringstream buffer;
        	CObject value_src {get_cobject(args[0], type)};
        	Value packed;
        
        	buffer.seekg(0);
        	if(type == ValueType::NFT)
        	{
            	bool src {std::get<bool>(value_src)};
            	msgpack::pack(buffer, src);
            	packed = Value(buffer.str());
        	}
        	else if(type == ValueType::Number)
        	{
            	double src {std::get<double>(value_src)};
            	msgpack::pack(buffer, src);
            	packed = Value(buffer.str());
        	}
        	else if(type == ValueType::String)
        	{
            	std::string src {std::get<std::string>(value_src)};
            	msgpack::pack(buffer, src);
            	packed = Value(buffer.str());
        	}
        	else
        	{
            	std::vector<Value> src {std::get<std::vector<Value>>(value_src)};
            	packed = list_packing(src);
        	}

        	return packed;
    	}
    	
    	Value list_packing(std::vector<Value> &src_list)
    	{
        	std::vector<Value> list;
        	std::stringstream buffer;
        	CObject each;

        	for(unsigned i {0}; i < src_list.size(); ++ i)
        	{
            	ValueType type {src_list[i].valueType()};
            	each = get_cobject(src_list[i], type);
            	if(type == ValueType::NFT)
            	{
                	bool src = std::get<bool>(each);
                	msgpack::pack(buffer, src);
                	list.push_back(Value(buffer.str()));
                	buffer.str("");
            	}
            	else if(type == ValueType::Number)
            	{
                	double src = std::get<double>(each);
                	msgpack::pack(buffer, src);
                	list.push_back(Value(buffer.str()));
                	buffer.str("");
            	}
            	else if(type == ValueType::String)
            	{
                	std::string src = std::get<std::string>(each);
                	msgpack::pack(buffer, src);
                	list.push_back(Value(buffer.str()));
                	buffer.str("");
            	}
        	}

        	return list;
    	}
	}
}