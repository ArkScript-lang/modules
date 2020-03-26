#include <ark_msgpack_obj.hpp>

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