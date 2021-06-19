#include <pack.hpp>
#include <adaptor.hpp>


Value msgpack_pack(std::vector<Value> &args, VM* vm)
{
	if(args.size() != 2)
		throw std::runtime_error("ArgError : This function must have 2 arguments");
	if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::sbuffer>()))
		throw Ark::TypeError("msgpack:pack The packed buffer must be a msgpack::sbuffer");

	msgpack::sbuffer& sbuf = args[0].usertypeRef().as<msgpack::sbuffer>();

	msgpack::pack(sbuf, args[1]);

	return Ark::Nil;
}