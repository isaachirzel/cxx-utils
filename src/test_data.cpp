#define HIRZEL_IMPLEMENT

#include <hirzel/json.hpp>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace hirzel;
using namespace hirzel::json;

void assert_cast_values(const JsonValue& obj, i64 intValue, f64 floatValue, bool boolValue, const std::string&)
{
	assert(obj.asI64() == intValue);
	assert(obj.asF64() == floatValue);
	assert(obj.asBool() == boolValue);
	// TODO: Trim zeroes and spaces to make this comparison work
	//assert(obj.asString() == stringValue);
}

void assert_primitive(const JsonValue& obj, ValueType type)
{
	// TODO: Check value?
	assert(obj.type() == type);
	//assert(obj.is##funcname());
	assert(obj.length() == 0);
}

void assert_not_primitive(const JsonValue& obj, ValueType type)
{
	assert(obj.type() != type);
	//assert(!obj.is##funcname())
}

void assert_not_null(const JsonValue& obj)
{
	assert(obj.type() != ValueType::Null);
	assert(!obj.isNull());
}

void assert_not_bool(const JsonValue& obj)
{
	assert(obj.type() != ValueType::Boolean);
	assert(!obj.isBoolean());
}

void assert_not_int(const JsonValue& obj)
{
	assert(obj.type() != ValueType::Integer);
	assert(!obj.isInteger());
}

void assert_not_float(const JsonValue& obj)
{
	assert(obj.type() != ValueType::Float);
	assert(!obj.isFloat());
}

void assert_not_string(const JsonValue& obj)
{
	assert(obj.type() != ValueType::String);
	assert(!obj.isString());
}

void assert_not_array(const JsonValue& obj)
{
	assert(obj.type() != ValueType::Array);
	assert(!obj.isArray());
}

void assert_not_table(const JsonValue& obj)
{
	assert(obj.type() != ValueType::Object);
	assert(!obj.isObject());
}

void assert_equals(const JsonValue& a, const JsonValue& b)
{
	assert(a == b);
	assert(b == a);
	assert(!(a != b));
	assert(!(b != a));
}

void assert_not_equals(const JsonValue& a, const JsonValue& b)
{
	assert(!(a == b));
	assert(!(b == a));
	assert(a != b);
	assert(b != a);
}

void assert_null(const JsonValue& obj)
{
	assert(obj.type() == ValueType::Null);
	assert(obj.isNull());
	assert(obj.length() == 0);
	assert_cast_values(obj, 0, 0.0, false, "null");
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_table(obj);
	assert_equals(obj, JsonValue());
	assert_equals(obj, JsonValue(ValueType::Null));
}

template <typename V>
void assert_boolean(const JsonValue& obj, const V& value)
{
	assert_primitive(obj, ValueType::Boolean);
	assert_cast_values(obj, (int)value, (double)value, value, (value ? "true" : "false"));
	assert_not_null(obj); 
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_table(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_integer(const JsonValue& obj, i64 value)
{
	assert_primitive(obj, ValueType::Integer);
	assert(obj.integer() == value);
	assert(obj.isNumber());
	assert_cast_values(obj, value, (double)value, (bool)value, std::to_string(value));
	assert_not_null(obj);
	assert_not_bool(obj);
	assert_not_float(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_table(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_decimal(const JsonValue& obj, f64 value)
{
	assert_primitive(obj, ValueType::Float);
	assert(obj.decimal() == value);
	assert_cast_values(obj, (int)value, (double)value, (bool)value, std::to_string(value));
	assert(obj.isNumber());
	assert_not_null(obj);
	assert_not_int(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_not_table(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_string(const JsonValue& obj, i64 intValue, f64 floatValue, bool boolValue, const char* stringValue)
{
	assert(obj.type() == ValueType::String);
	assert(std::string(obj.typeName()) == "string");
	assert(obj.isString());
	assert(obj.length() == obj.asString().length());
	assert_cast_values(obj, intValue, floatValue, boolValue, stringValue);
	assert_not_null(obj);
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_bool(obj);
	assert_not_array(obj);
	assert_not_table(obj);
	assert_equals(obj, JsonValue(stringValue));
}

void assert_array(const JsonValue& obj, const JsonArray& value)
{
	assert(obj.type() == ValueType::Array);
	assert(obj.isArray());
	assert_not_null(obj);
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_bool(obj);
	assert_not_string(obj);
	assert_not_table(obj);
	assert_equals(obj, JsonValue(value));
}

void assert_object(const JsonValue& obj, const JsonObject& value)
{
	assert(obj.type() == ValueType::Object);
	assert(obj.isObject());
	assert_not_null(obj);
	assert_not_bool(obj);
	assert_not_int(obj);
	assert_not_float(obj);
	assert_not_string(obj);
	assert_not_array(obj);
	assert_equals(obj, JsonValue(value));
}

void test_null()
{
	auto init = JsonValue();
	assert_null(init);

	auto copy = JsonValue(init);
	assert_null(copy);
	assert_equals(copy, init);
	copy = init;
	assert_null(copy);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_null(move);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_null(move);
	assert_equals(move, clone);

	assert_null(JsonValue());
	assert_null(JsonValue(ValueType::Null));

	assert_not_equals(move, JsonValue(143));
	assert_not_equals(move, JsonValue(234.82));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(true));
	assert_not_equals(move, JsonValue(JsonArray()));
	assert_not_equals(move, JsonValue(JsonObject()));
}

void test_integer()
{
	JsonValue init = 234;
	assert_integer(init, 234);

	JsonValue copy(init);
	assert_integer(copy, 234);
	assert_equals(copy, init);
	copy = init;
	assert_integer(copy, 234);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_integer(move, 234);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_integer(move, 234);
	assert_equals(move, clone);

	assert_integer(JsonValue(0), 0);
	assert_integer(JsonValue(-0), -0);
	assert_integer(JsonValue(1), 1);
	assert_integer(JsonValue(-1), -1);
	assert_integer(JsonValue(4L), 4L);
	assert_integer(JsonValue(4U), 4U);
	assert_integer(JsonValue(-1023), -1023);
	assert_integer(JsonValue(62), 62);

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(ValueType::Array));
	assert_not_equals(move, JsonValue(ValueType::Object));
}

void test_float()
{
	JsonValue init = 2.34;
	assert_decimal(init, 2.34);

	JsonValue copy(init);
	assert_decimal(copy, 2.34);
	assert_equals(copy, init);
	copy = init;
	assert_decimal(copy, 2.34);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_decimal(move, 2.34);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_decimal(move, 2.34);
	assert_equals(move, clone);

	assert_decimal(JsonValue(ValueType::Float), 0.0);
	assert_decimal(JsonValue(1.34), 1.34);
	assert_decimal(JsonValue(0.0f), 0.0f);
	assert_decimal(JsonValue(-0.0), -0.0);
	assert_decimal(JsonValue(-123592.235), -123592.235);
	assert_decimal(JsonValue(2.0f), 2.0f);

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(ValueType::Array));
	assert_not_equals(move, JsonValue(ValueType::Object));
}

void test_boolean()
{
	JsonValue init = true;
	assert_boolean(init, true);
	
	JsonValue copy(init);
	assert_boolean(copy, true);
	assert_equals(copy, init);
	copy = init;
	assert_boolean(copy, true);
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_boolean(move, true);
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_boolean(move, true);
	assert_equals(move, clone);

	assert_boolean(JsonValue(ValueType::Boolean), false);
	assert_boolean(JsonValue(true), true);
	assert_boolean(JsonValue(false), false);

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(ValueType::Array));
	assert_not_equals(move, JsonValue(ValueType::Object));
}

void test_string()
{
	JsonValue init = "this is obj string";
	assert_string(init, 0, 0.0, true, "this is obj string");

	JsonValue copy(init);
	assert_string(copy, 0, 0.0, true, "this is obj string");
	assert_equals(copy, init);
	copy = init;
	assert_string(copy, 0, 0.0, true, "this is obj string");
	assert_equals(copy, init);

	JsonValue move(std::move(init));
	assert_string(move, 0, 0.0, true, "this is obj string");
	assert_equals(move, copy);
	JsonValue clone = copy;
	move = std::move(copy);
	assert_string(move, 0, 0.0, true, "this is obj string");
	assert_equals(move, clone);

	assert_string(JsonValue(ValueType::String), 0, 0.0, false, "");
	assert_string(JsonValue(""), 0, 0.0, false, "");
	assert_string(JsonValue("hello"), 0, 0.0, true, "hello");
	assert_string(JsonValue("1.2"), 1, 1.2, true, "1.2");
	assert_string(JsonValue("123"), 123, 123.0, true, "123");
	assert_string(JsonValue("0"), 0, 0.0, true, "0");
	assert_string(JsonValue("0000"), 0, 0.0, true, "0000");

	assert_not_equals(move, JsonValue());
	assert_not_equals(move, JsonValue(130));
	assert_not_equals(move, JsonValue(-130.52));
	assert_not_equals(move, JsonValue("hello"));
	assert_not_equals(move, JsonValue(false));
	assert_not_equals(move, JsonValue(ValueType::Array));
	assert_not_equals(move, JsonValue(ValueType::Object));
}

void test_array()
{
	using Array = json::JsonArray;

	{
		JsonValue empty = Array {};
		assert_array(empty, Array());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		JsonValue copy(empty);
		assert_array(copy, Array());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_array(copy, Array());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		JsonValue move(std::move(empty));
		assert_array(move, Array());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_array(move, Array());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_equals(move, JsonValue(ValueType::Array));
		assert_not_equals(move, JsonValue(JsonArray { 1, 2, 3}));
		assert_not_equals(move, JsonValue(ValueType::Object));
	}

	{
		JsonValue init = Array { "hello", 1, true, 0.4 };
		assert_array(init, Array({ "hello", 1, true, 0.4 }));
		assert(init.length() == 4);
		assert(!init.isEmpty());
		assert_string(init[0], 0, 0.0, true, "hello");
		assert_integer(init[1], 1);
		assert_boolean(init[2], true);
		assert_decimal(init[3], 0.4);

		JsonValue copy(init);
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);
		copy = init;
		assert_array(copy, Array({ "hello", 1, true, 0.4 }));
		assert(copy.length() == 4);
		assert(!copy.isEmpty());
		assert_string(copy[0], 0, 0.0, true, "hello");
		assert_integer(copy[1], 1);
		assert_boolean(copy[2], true);
		assert_decimal(copy[3], 0.4);
		assert_equals(copy, init);

		JsonValue move(std::move(init));
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_integer(move[1], 1);
		assert_boolean(move[2], true);
		assert_decimal(move[3], 0.4);
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_array(move, Array({ "hello", 1, true, 0.4 }));
		assert(move.length() == 4);
		assert(!move.isEmpty());
		assert_string(move[0], 0, 0.0, true, "hello");
		assert_integer(move[1], 1);
		assert_boolean(move[2], true);
		assert_decimal(move[3], 0.4);
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_not_equals(move, JsonValue(ValueType::Array));
		assert_not_equals(move, JsonValue(ValueType::Object));
	}

	JsonValue array = Array { 1, 2.0f, "3", false, JsonValue() };
	assert_array(array, Array({ 1, 2.0f, "3", false, JsonValue() }));
	assert(array.length() == 5);
	assert(!array.isEmpty());
	assert_integer(array[0], 1);
	assert_decimal(array[1], 2.0f);
	assert_string(array[2], 3, 3.0, true, "3");
	assert_boolean(array[3], false);
	assert_null(array[4]);

	std::vector<std::string> array_of_strings = { "this", "is", "obj", "sentence" };

	array = JsonValue::from(array_of_strings);

	for (size_t i = 0; i < array_of_strings.size(); ++i)
		assert(array[i] == array_of_strings[i] && "got unexpected element value");

	std::vector<int> array_of_ints = { 6, 6345, 26, 34, -1231 };

	array = JsonValue::from(array_of_ints);

	for (size_t i = 0; i < array_of_ints.size(); ++i)
		assert(array[i] == array_of_ints[i] && "got unexpected element value");
}

void test_table()
{
	using Table = JsonObject;

	{
		JsonValue empty = Table {};
		assert_object(empty, Table());
		assert(empty.length() == 0);
		assert(empty.isEmpty());

		JsonValue copy(empty);
		assert_object(copy, Table());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);
		copy = empty;
		assert_object(copy, Table());
		assert(copy.length() == 0);
		assert(copy.isEmpty());
		assert_equals(copy, empty);

		JsonValue move(std::move(empty));
		assert_object(move, Table());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_object(move, Table());
		assert(move.length() == 0);
		assert(move.isEmpty());
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_not_equals(move, JsonValue(ValueType::Array));
		assert_equals(move, JsonValue(ValueType::Object));
		assert_not_equals(move, JsonValue(Table { { "key", "value" } }));
	}
	{
		auto table = Table {
			{ "boolean", true },
			{ "integer", 1362 },
			{ "decimal", 235.125 },
			{ "string", "this is text" },
			{ "null", JsonValue() }
		}; 
		JsonValue init = table;
		assert_object(init, table);
		assert(init.length() == 5);
		assert(!init.isEmpty());
		assert_boolean(init["boolean"], true);
		assert_integer(init["integer"], 1362);
		assert_decimal(init["decimal"], 235.125);
		assert_string(init["string"], 0, 0.0, true, "this is text");
		assert_null(init["null"]);

		JsonValue copy(init);
		assert_object(copy, table);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);
		copy = init;
		assert_object(copy, table);
		assert(copy.length() == 5);
		assert(!(copy.isEmpty()));
		assert_equals(copy, init);

		JsonValue move(std::move(init));
		assert_object(move, table);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, copy);
		JsonValue clone = copy;
		move = std::move(copy);
		assert_object(move, table);
		assert(move.length() == 5);
		assert(!(move.isEmpty()));
		assert_equals(move, clone);

		assert_not_equals(move, JsonValue());
		assert_not_equals(move, JsonValue(130));
		assert_not_equals(move, JsonValue(-130));
		assert_not_equals(move, JsonValue(-130.52));
		assert_not_equals(move, JsonValue("hello"));
		assert_not_equals(move, JsonValue(false));
		assert_not_equals(move, JsonValue(ValueType::Array));
		assert_not_equals(move, JsonValue(ValueType::Object));
	}

	std::unordered_map<std::string, std::string> table_of_strings = {
		{ "abc", "def" },
		{ "hij", "klm" },
		{ "nop", "qrs" },
		{ "tuv", "wxyz" }
	};

	JsonValue table = JsonValue::from(table_of_strings);

	for (const auto& pair : table_of_strings)
		assert(table[pair.first] == pair.second && "got unexpected table value");

	std::unordered_map<std::string, double> table_of_doubles = {
		{ "123", 123.0 },
		{ "194.65", 194.65 },
		{ "987.125", 987.125 }
	};

	table = JsonValue::from(table_of_doubles);
	
	for (const auto& pair : table_of_doubles)
		assert(table[pair.first] == pair.second && "did not get expected table value");
}

int main()
{
	test_null();
	test_integer();
	test_float();
	test_boolean();
	test_string();
	test_array();
	test_table();

	return 0;
}