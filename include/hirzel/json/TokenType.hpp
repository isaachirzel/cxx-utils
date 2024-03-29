#ifndef HIRZEL_JSON_TOKEN_TYPE_HPP
#define HIRZEL_JSON_TOKEN_TYPE_HPP

namespace hirzel::json
{
	enum class TokenType
	{
		LeftBrace,
		RightBrace,
		LeftBracket,
		RightBracket,
		Comma,
		Colon,
		String,
		Number,
		True,
		False,
		Null,
		EndOfFile
	};
}

#endif
