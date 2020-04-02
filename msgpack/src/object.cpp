#include <object.hpp>
#include <adaptor.hpp>

CObject get_cobject(const Value &ark_object, ValueType type)
{
	CObject object;

	if(type == ValueType::NFT)
	{
		if(ark_object == Ark::True)
			object = true;
		else if(ark_object == Ark::False)
			object = false;
	}
	else if(type == ValueType::Number)
	{
		object = static_cast<Value>(ark_object).number();
	}
	else if(type == ValueType::String)
	{
		object = static_cast<Value>(ark_object).string_ref();
	}
	else
	{	
		object = static_cast<Value>(ark_object).list();
	}
	return object;
}

namespace ArkMsgpack
{
	// msgpack::sbuffer
	Value msgpack_sbuffer(std::vector<Value> &args)
	{
		if(args.size() != 0)
			throw std::runtime_error("ArgError : No arguments are necessary for this function");
		static msgpack::sbuffer sbuf;

		return Value(UserType(&sbuf));
	}

	// msgpack::object_handle and msgpack::object
	Value msgpack_obj_handle(std::vector<Value> &args)
	{
		if(args.size() != 1)
			throw std::runtime_error("ArgError : This function must have 1 argument");
		if(args[0].valueType() != ValueType::User || args[0].usertype().type_id() != std::type_index(typeid(msgpack::sbuffer)))
			throw Ark::TypeError("The packed buffer must be a msgpack::sbuffer");
		static msgpack::object_handle oh;

		msgpack::sbuffer* sbuf {static_cast<msgpack::sbuffer*>((args[0]).usertype().data())};
		oh = msgpack::unpack(sbuf->data(), sbuf->size());

		return Value(UserType(&oh));
	}

	Value msgpack_obj(std::vector<Value> &args)
	{
		if(args.size() != 1)
			throw std::runtime_error("ArgError : This function must have 1 argument");
		if(args[0].valueType() != ValueType::User || args[0].usertype().type_id() != std::type_index(typeid(msgpack::object_handle)))
			throw Ark::TypeError("The packed buffer must be a msgpack::object_handle");
		static msgpack::object o;

		msgpack::object_handle* oh {static_cast<msgpack::object_handle*>((args[0]).usertype().data())};
		o = oh->get();

		return Value(UserType(&o));
	}

	Value print_msgpack_o(std::vector<Value> &args)
	{
		if(args.size() != 1)
			throw std::runtime_error("ArgError : This function must have 1 argument");
		msgpack::object* o {static_cast<msgpack::object*>(args[0].usertype().data())};

		std::cout << "msgpack::object : " << *o << std::endl;

		return Value(Ark::Nil);
	}
}