#pragma once

enum StickErr : int
{
	STICKERR_SYSTEM = 1000,
	STICKERR_INCORRECT_ARG_TYPE = 1001,
};

constexpr const char* STICKERR_PREFIX = "stick_error:";
constexpr size_t STICKERR_PREFIX_LENGTH = sizeof("stick_error:") - 1;
constexpr const char* STICKERR_SYSTEM_MSG = "System error";
constexpr const char* STICKERR_INCORRECT_ARG_TYPE_MSG = "Incorrect argument type";

#define StickThrowScriptError(msg) throw std::runtime_error(msg)
#define StickThrowRuntimeError(id, ...) throw std::runtime_error(LuaStick::Err::make_error_message(id, id ## _MSG, __VA_ARGS__))
#define StickThrowSystemError(...) throw std::runtime_error(LuaStick::Err::make_error_message(STICKERR_SYSTEM, STICKERR_SYSTEM_MSG, (const char*)__FILE__, __LINE__, __VA_ARGS__))
#define StickSystemErrorMessage(...) LuaStick::Err::make_error_message(STICKERR_SYSTEM, STICKERR_SYSTEM_MSG, (const char*)__FILE__, __LINE__, __VA_ARGS__)

namespace LuaStick
{

class Err
{
private:
	static void join_string(std::string& message, const std::string& separator) {}

	template<typename ... Args>
	static void join_string(std::string& message, const std::string& separator, const char *&& head, Args&& ... args)
	{
		message += separator + head;
		join_string(message, ":", std::move(args)...);
	}

	template<typename T, typename ... Args>
	static void join_string(std::string& message, const std::string& separator, T&& head, Args&& ... args)
	{
		message += separator + std::to_string(head);
		join_string(message, ":", std::move(args)...);
	}

public:
	template<typename ... Args>
	static std::string make_error_message(int errorCode, const char * errorMessage, Args&& ... args)
	{
		std::string message = std::string(STICKERR_PREFIX) + std::to_string(errorCode) + ":" + errorMessage;
		join_string(message, ":", std::move(args)...);
		return message;
	}
};

}
