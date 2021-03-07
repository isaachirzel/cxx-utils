#define HIRZEL_VAR_IMPLEMENTATION
#include <hirzel/var.h>
#include <cassert>
#include <iostream>

using hirzel::var;

#define compare(v, t, n, i, u, d, c, b, s)\
	assert(v.type() == t);\
	assert(v.size() == n);\
	assert(v.to_int() == i);\
	assert(v.to_uint() == u);\
	assert(v.to_double() == d);\
	assert(v.to_char() == c);\
	assert(v.to_bool() == b);\
	assert(v.to_string() == s);

void test_null()
{
	var v;
	compare(v, var::NULL_TYPE, 0, 0, 0u, 0.0, (char)0, false, "null");
}

void test_int()
{
	var v;
	assert(v.type() != var::INT_TYPE);
	v = 62;
	compare(v, var::INT_TYPE, sizeof(intmax_t), 62, 62u, 62.0, (char)62, true, "62");
	v = -1023;
	compare(v, var::INT_TYPE, sizeof(intmax_t), -1023, (uintmax_t)(-1023), -1023.0, (char)-1023, true, "-1023");
}

void test_uint()
{
	var v;
	assert(v.type() != var::UINT_TYPE);
	v = 45u;
	compare(v, var::UINT_TYPE, sizeof(uintmax_t), 45, 45u, 45.0, (char)45, true, "45");
}

void test_float()
{
	var v;
	assert(v.type() != var::FLOAT_TYPE);
	v = 12.4;
	compare(v, var::FLOAT_TYPE, sizeof(double), 12, 12u, 12.4, (char)12, true, std::to_string(12.4));
}

void test_char()
{
	var v;
	assert(v.type() != var::CHAR_TYPE);
	v = ' ';
	compare(v, var::CHAR_TYPE, sizeof(char), 32, 32u, 32.0, ' ', true, " ");
}

void test_bool()
{
	var v;
	assert(v.type() != var::BOOL_TYPE);
	v = true;
	compare(v, var::BOOL_TYPE, sizeof(bool), 1, 1u, 1.0, 1, true, "true");
	v = false;
	compare(v, var::BOOL_TYPE, sizeof(bool), 0, 0u, 0.0, 0, false, "false");
}

void test_string()
{
	var v;
	assert(v.type() != var::STR_TYPE);
	v = "";
	compare(v, var::STR_TYPE, 0, 0, 0u, 0.0, (char)0, false, "");
	v = "HELLO";
	compare(v, var::STR_TYPE, 5, 5, 5u, 5.0, (char)5, true, "HELLO");
}

void test_array()
{
	var j = { 1, "HELLO", true };
	assert(j.size() == 3);
	assert(j.type() == var::ARRAY_TYPE);

	var& v0 = j[0];
	assert(v0.type() == var::INT_TYPE);
	assert(v0.to_int() == 1);
	assert(v0.size() == sizeof(intmax_t));

	var& v1 = j[1];
	assert(v1.type() == var::STR_TYPE);
	assert(v1.to_string() == "HELLO");
	assert(v1.size() == 5);

	var& v2 = j[2];
	assert(v2.type() == var::BOOL_TYPE);
	assert(v2.to_bool() == true);
	assert(v2.to_string() == "true");
	assert(v2.size() == sizeof(bool));
}

void test_map()
{
	var m;
	m["val"] = 2;
	assert(m["val"].to_int() == 2);
	assert(m.size() == 1);
	//std::cout << m["val"] << std::endl;
}

typedef void(*Func)();
#define TEST(name) std::cout << "Testing " #name "...\n"; test_##name(); std::cout << "\t\tAll tests passed\n";

int main()
{
	
	TEST(null);
	TEST(int);
	TEST(uint);
	TEST(float);
	TEST(char);
	TEST(bool);
	TEST(string);
	TEST(array);
	TEST(map);
	return 0;
}