#ifndef HIRZEL_JSON_JSON_VALUE_TYPE_HPP
#define HIRZEL_JSON_JSON_VALUE_TYPE_HPP

namespace hirzel::json
{
	enum class JsonValueType
	{
		Null,
		Integer,
		Decimal,
		Boolean,
		String,
		Array,
		Object
	};
}

#endif
