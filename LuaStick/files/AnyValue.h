#pragma once

namespace LuaStick
{

class AnyValue
{
public:
	enum class Type
	{
		NIL,
		DOUBLE,
		BOOL,
		CHARP,
	};
public:
	Type type;
	union
	{
		double doubleValue;
		bool boolValue;
		char * charpValue;
	};
	AnyValue () : type(Type::NIL) {}
	AnyValue (const double & value) : type(Type::DOUBLE), doubleValue(value) {}
	AnyValue (const bool & value) : type(Type::BOOL), boolValue(value) {}
	AnyValue (const char * value)
	{
		type = Type::CHARP;
		auto sz = strlen(value) + 1;
		charpValue = new char[sz];
		strcpy_s(charpValue, sz, value);
	}
	AnyValue (const __int16 & value) : type(Type::DOUBLE), doubleValue((double)value) {}
	AnyValue (const unsigned __int16 & value) : type(Type::DOUBLE), doubleValue((double)value) {}
	AnyValue (const __int32 & value) : type(Type::DOUBLE), doubleValue((double)value) {}
	AnyValue (const unsigned __int32 & value) : type(Type::DOUBLE), doubleValue((double)value) {}
	AnyValue (const __int64 & value) : type(Type::DOUBLE), doubleValue((double)value) {}
	AnyValue (const unsigned __int64 & value) : type(Type::DOUBLE), doubleValue((double)value) {}
	AnyValue (const float & value) : type(Type::DOUBLE), doubleValue((double)value) {}
	AnyValue (const wchar_t * value)
	{
		type = Type::CHARP;
		std::string astr;
		wstring_to_string(astr, value);
		charpValue = new char[astr.length() + 1];
		strcpy_s(charpValue, astr.length() + 1, astr.c_str());
	}

	~AnyValue ()
	{
		if (type == Type::CHARP)
			delete[] charpValue;
	}

	AnyValue (const AnyValue & value)
	{
		type = Type::NIL;
		*this = value;
	}

	AnyValue (AnyValue && value)
	{
		type = value.type;
		switch (value.type)
		{
		case AnyValue::Type::NIL:
			break;
		case AnyValue::Type::DOUBLE:
			doubleValue = value.doubleValue;
			break;
		case AnyValue::Type::BOOL:
			boolValue = value.boolValue;
			break;
		case AnyValue::Type::CHARP:
			charpValue = value.charpValue;
			break;
		}
		value.type = Type::NIL;
	}

	AnyValue & operator = (const AnyValue & value)
	{
		if (type == AnyValue::Type::CHARP)
			delete[] charpValue;
		type = value.type;
		switch (value.type)
		{
		case AnyValue::Type::NIL:
			break;
		case AnyValue::Type::DOUBLE:
			doubleValue = value.doubleValue;
			break;
		case AnyValue::Type::BOOL:
			boolValue = value.boolValue;
			break;
		case AnyValue::Type::CHARP:
		{
			auto sz = strlen(value.charpValue) + 1;
			charpValue = new char[sz];
			strcpy_s(charpValue, sz, value.charpValue);
			break;
		}
		default:
			throw std::invalid_argument("The type of argument is not supported");
		}
		return *this;
	}

	double GetDouble() const
	{
		if (type == AnyValue::Type::DOUBLE)
			return doubleValue;
		StickThrowRuntimeError(STICKERR_INCORRECT_ARG_TYPE, (const char *)"Not double");
	}

	bool GetBool() const
	{
		if (type == AnyValue::Type::BOOL)
			return boolValue;
		StickThrowRuntimeError(STICKERR_INCORRECT_ARG_TYPE, (const char *)"Not bool");
	}

	const char * GetCharp() const
	{
		if (type == AnyValue::Type::CHARP)
			return charpValue;
		StickThrowRuntimeError(STICKERR_INCORRECT_ARG_TYPE, (const char *)"Not char *");
	}

	static void wstring_to_string(std::string & u, std::wstring const & t)
	{
		if (!t.empty())
		{
			auto iBuffSz = (t.length() + 1) * sizeof(wchar_t);
			std::vector<char> vBuff(iBuffSz);
			auto writtenLen = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), (int)t.length(), vBuff.data(), (int)vBuff.size(), nullptr, nullptr);
			if (writtenLen == 0)
			{
				//----- The size might become more than twice of the source size -----
				iBuffSz = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), (int)t.length(), nullptr, 0, nullptr, nullptr);
				vBuff.resize(iBuffSz + 1);
				writtenLen = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), (int)t.length(), vBuff.data(), (int)vBuff.size(), nullptr, nullptr);
			}
			vBuff.resize(writtenLen + 1);
			vBuff[writtenLen] = L'\0';
			u = vBuff.data();
		}
		else
		{
			u.clear();
		}
	}

};

}
