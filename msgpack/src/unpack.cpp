#include <unpack.hpp>
#include <adaptor.hpp>

namespace ArkMsgpack
{
	Value unpack(std::vector<Value> &args, VM* vm)
	{
		if(args.size() != 1)
			throw std::runtime_error("ArgError : This function must have 1 argument");
		if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::sbuffer>()))
			throw Ark::TypeError("The packed buffer must be msgpack::sbuffer");

		msgpack::sbuffer& sbuf = args[0].usertype_ref().as<msgpack::sbuffer>();
		static msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());

		return Value(UserType(&oh));
	}

	Value convert(std::vector<Value> &args, VM* vm)
	{
		if(args.size() != 1)
			throw std::runtime_error("ArgError : This function must have 1 arguments");
		if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::object>()))
			throw Ark::TypeError("The packed buffer must be msgpack::object");

		msgpack::object& o = args[0].usertype_ref().as<msgpack::object>();
		Value dst;

		o.convert(dst);

		return dst;
	}
}