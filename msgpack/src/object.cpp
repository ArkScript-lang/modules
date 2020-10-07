#include <object.hpp>
#include <adaptor.hpp>

CObject get_cobject(const Value &ark_object, ValueType type)
{
	CObject object;

	switch(type)
	{
		case ValueType::True:
			object = true;
			break;
		case ValueType::False:
			object = false;
			break;
		case ValueType::Number:
			object = ark_object.number();
			break;
		case ValueType::String:
			object = const_cast<Value&>(ark_object).string_ref().toString();
			break;
		case ValueType::List:
			object = const_cast<Value&>(ark_object).list();
			break;
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
		if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::sbuffer>()))
			throw Ark::TypeError("The packed buffer must be a msgpack::sbuffer");
		static msgpack::object_handle oh;

		msgpack::sbuffer& sbuf = args[0].usertype_ref().as<msgpack::sbuffer>();
		oh = msgpack::unpack(sbuf.data(), sbuf.size());

		return Value(UserType(&oh));
	}

	Value msgpack_obj(std::vector<Value> &args)
	{
		if(args.size() != 1)
			throw std::runtime_error("ArgError : This function must have 1 argument");
		if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::object_handle>()))
			throw Ark::TypeError("The packed buffer must be a msgpack::object_handle");
		static msgpack::object o;

		msgpack::object_handle& oh = args[0].usertype_ref().as<msgpack::object_handle>();
		o = oh.get();

		return Value(UserType(&o));
	}

	/*
	Value print_msgpack_o(std::vector<Value> &args)
	{
		if(args.size() != 1)
			throw std::runtime_error("ArgError : This function must have 1 argument");
		msgpack::object* o {static_cast<msgpack::object*>(args[0].usertype().data())};

		std::cout << "msgpack::object : " << *o << std::endl;

		return Ark::Nil;
	}
	*/
}