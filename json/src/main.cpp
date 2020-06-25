#include <Ark/Module.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

namespace json{

	std::list<nlohmann::json>& get_json_object(){static std::list<nlohmann::json> jsonObject; return jsonObject;}


	Value open(std::vector<Value>& args, Ark::VM* vm){
		/*
		args[0] -> filename to open
		*/
	
		if (args.size() != 1){
			throw std::runtime_error("ArgError : This function must have 1 argument");
		}
		if(args[0].valueType() != ValueType::String){
			throw std::runtime_error("ArgError : This function take a String argument"); 
		}

		//open and read the file
		const char* filename = args[0].string_ref().c_str();
		std::ifstream file(filename);
		std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

		//parsing
		get_json_object().push_back(nlohmann::json::parse(content));
		Ark::Value v = Ark::Value(Ark::UserType(&get_json_object().back()));

		v.usertype_ref().setOStream([](std::ostream& os, const Ark::UserType& A) -> std::ostream& {
			os << *static_cast<nlohmann::json*>(A.data());;
			return os;
		});

		return v;
	}


	Value fromString(std::vector<Value>& args, Ark::VM* vm){
		/*
		args[0] -> json to deserialize
		*/

		if (args.size() != 1){
			throw std::runtime_error("ArgError : This function must have 1 argument");
		}
		if(args[0].valueType() != ValueType::String){
			throw std::runtime_error("ArgError : This function take a String argument"); 
		}

		//parsing
		get_json_object().push_back(nlohmann::json::parse(args[0].string_ref().c_str()));
		Ark::Value v = Ark::Value(Ark::UserType(&get_json_object().back()));

		v.usertype_ref().setOStream([](std::ostream& os, const Ark::UserType& A) -> std::ostream& {
			os << *static_cast<nlohmann::json*>(A.data());
			return os;
		});

		return v;
	}


	Value get(std::vector<Value>& args, Ark::VM* vm){
		/*
		args[0] -> json_object
		args[1] -> key
		*/

		if (args.size() != 2){
			throw std::runtime_error("ArgError : This function must have 2 arguments");
		}
		if(args[0].valueType() != ValueType::User && args[1].valueType() != ValueType::String){
			throw std::runtime_error("ArgError : This function take jsonobject and a String arguments"); 
		}

		nlohmann::json json_object = *static_cast<nlohmann::json*>(args[0].usertype_ref().data()); //conversion to json object to be able to get the result
		get_json_object().push_back(json_object[args[1].string_ref().c_str()]);
		Ark::Value v = Ark::Value(Ark::UserType(&get_json_object().back()));

		v.usertype_ref().setOStream([](std::ostream& os, const Ark::UserType& A) -> std::ostream& {
			os << *static_cast<nlohmann::json*>(A.data());
			return os;
		});

		return v;
	}


	Value toString(std::vector<Value>& args, Ark::VM* vm){
		/*
		args[0] -> json_object
		*/
		if(args.size() != 1){
			throw std::runtime_error("ArgError : This function must have 1 arguments");
		}
		if(args[0].valueType() != ValueType::User){
			throw std::runtime_error("ArgError : This function take a json object argument"); 
		}

		nlohmann::json json_object = *static_cast<nlohmann::json*>(args[0].usertype_ref().data());
		return json_object.dump();
	}


	nlohmann::json listToJson(std::vector<Value>& arkList){
		nlohmann::json json_list;

		for(auto& el : arkList){
			switch(el.valueType()){
				case ValueType::String:
					json_list.emplace_back(el.string_ref().c_str());
					break;
				case ValueType::Number:
					json_list.emplace_back(el.number());
					break;
				case ValueType::True:
				case ValueType::False:
				{
					bool value = (el == Ark::True);
					json_list.emplace_back(value);
					break;
				}
				case ValueType::List:
					json_list.emplace_back(listToJson(el.list()));
					break;
				case ValueType::User:
				{
					nlohmann::json& json_object = *static_cast<nlohmann::json*>(el.usertype_ref().data());
					json_list.emplace_back(json_object);
					break;
				}
				default:
					throw std::runtime_error("Cannot parse your json");
			}
		}
		return json_list;
	}


	Value jset(std::vector<Value>& args, Ark::VM* vm){
		/*
		args[0] -> variable to edit (userType)
		args[1] -> key (String)
		args[2] -> object in value (userType, String, Number, Bool, List)
		*/

		if (args.size() != 3){
			throw std::runtime_error("ArgError : This function must have 3 arguments");
		}
		
		if(args[0].valueType() != ValueType::User && args[1].valueType() != ValueType::String){
			throw std::runtime_error("ArgError : the function take a json object, a String and a Value argument");
		}

		nlohmann::json& json_object = *static_cast<nlohmann::json*>(args[0].usertype_ref().data());

		//definition of the value according to the object type
		switch(args[2].valueType()){
			case ValueType::String:
			{
				json_object[args[1].string_ref().c_str()] = args[2].string_ref().c_str();
				break;
			}
			case ValueType::Number:
			{
				json_object[args[1].string_ref().c_str()] = args[2].number();
				break;
			}
			case ValueType::False:
			case ValueType::True:
			{
				json_object[args[1].string_ref().c_str()] = (args[2] == Ark::True);
				break;
			}
			case ValueType::List:
			{
				json_object[args[1].string_ref().c_str()] = listToJson(args[2].list());
				break;
			}
			case ValueType::User:
			{
				nlohmann::json& json_to_push = *static_cast<nlohmann::json*>(args[2].usertype_ref().data());
				json_object[args[1].string_ref().c_str()] = json_to_push;
				break;
			}
			default:
			{
				throw std::runtime_error("ArgError, cannot parse the value");
			}
		}

		return Nil;
	}


	Value write(std::vector<Value>& args, Ark::VM* vm){
		/*
		args[0] -> json_object
		args[1] -> filename to write
		*/

		std::ofstream file {args[1].string_ref().c_str()};
		nlohmann::json& json_object = *static_cast<nlohmann::json*>(args[0].usertype_ref().data());

		file << json_object.dump(4);
		file.close();
		return Nil;
	}


	Value fromList(std::vector<Value>& args, Ark::VM* vm){
		/*
		args[0] -> list
		*/

		std::vector<Value>& ark_list = args[0].list();
		get_json_object().push_back(nlohmann::json());
		auto& json_object = get_json_object().back();

		Ark::Value v = Ark::Value(Ark::UserType(&get_json_object().back()));

		if(ark_list.size()%2!=0){
			throw std::runtime_error("ArgError : An key doesn't have a value");
		}
		for(int i=0;i<ark_list.size();i+=2){
			if(ark_list[i].valueType() != ValueType::String){
				throw std::runtime_error("ArgError : something is not a correct type for a key");
			}
			switch(ark_list[i+1].valueType()){
				case ValueType::String:
					json_object[ark_list[i].string_ref().c_str()] = ark_list[i+1].string_ref().c_str();
					break;
				case ValueType::Number:
					json_object[ark_list[i].string_ref().c_str()] = ark_list[i+1].number();
					break;
				case ValueType::True:
				case ValueType::False:
					json_object[ark_list[i].string_ref().c_str()] = (ark_list[i+1] == Ark::True);
					break;
				case ValueType::List:
					json_object[ark_list[i].string_ref().c_str()] = listToJson(ark_list[i+1].list());
					break;
				case ValueType::User:
					json_object[ark_list[i].string_ref().c_str()] = *static_cast<nlohmann::json*>(ark_list[i+1].usertype_ref().data());
					break;
			}
		}

		v.usertype_ref().setOStream([](std::ostream& os, const Ark::UserType& A) -> std::ostream& {
			os << *static_cast<nlohmann::json*>(A.data());
			return os;
		});

		return v;
	}
}

ARK_API_EXPORT Mapping_t getFunctionsMapping(){
    Mapping_t map;

	map["open"] = json::open;
	map["get"] = json::get;
	map["toString"] = json::toString;
	map["fromString"] = json::fromString;
	map["jset"] = json::jset;
	map["write"] = json::write;
	map["fromList"] = json::fromList;
    
	return map;
}

