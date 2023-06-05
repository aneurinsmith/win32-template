/*
	\brief		The main header file for a simple Logger class and global namespace methods.
	\file		logger.h
	\author		Aneurin F. Smith
*/

#pragma once
#include "include.h"
#include "sinks/console_sink.h"

#pragma push_macro("ERROR");
#pragma push_macro("DEBUG");
#undef ERROR
#undef DEBUG

enum Level {
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL,
	NONE
};

/*
	\brief		Namespace used to make all global Logger methods unique.
*/
namespace LOG {

	/*
		\brief		Converts the argument parameters into a single string.
		\param		lvl – The level of the message (inserted into string).
		\param		msgs – Variadic number of messages to be concatenated.
		\see		https://en.cppreference.com/w/cpp/string/basic_string
	*/
	template<typename... Args>
	inline string convert(Level lvl, Args... msgs) {

		string msg;
		stringstream stream;

		switch (lvl) {
		case TRACE: stream << "[trace] "; break;
		case DEBUG: stream << "[debug] "; break;
		case INFO: stream << "[info] "; break;
		case WARN: stream << "[warn] "; break;
		case ERROR: stream << "[error] "; break;
		case FATAL: stream << "[fatal] "; break;
		}

		using List = int[];
		(void)List {
			0, ((void)(stream << msgs), 0)...
		};

		return stream.str();
	}

	/*
		\brief		The strucutre template of each Logger.
	*/
	class Logger {
	public:

		/*
			\brief		Create a static instance of the Logger class.
			\return		A static Logger to be used when referencing every sink.
		*/
		static Logger& instance() {
			static Logger s_logger;
			return s_logger;
		}

		/*
			\brief		Set the minimum level required to be displayed for each sink associated with the Logger.
			\param		lvl – The level each sink will be set to.
		*/
		void set_level(Level lvl) {
			for (auto& sink : sinks) {
				sink->set_level(lvl);
			}
		}

		/*
			\brief		The method which notifies all associated sinks to display the provided message(s).
			\param		lvl – The level of the message. This is inserted into the string and also used to determine
						if the message should be shown.
			\param		msgs – Variadic number of messages to be printed for each associated sink.
		*/
		template<typename... Args>
		void print(Level lvl, Args... msgs) {

			string msg = convert(lvl, msgs...);

			for (auto& sink : sinks) {
				sink->print(lvl, msg);
			}
		}

		/*
			\brief		Adds a sink to the Logger.
			\param		sink – Pointer to a sink to be added to the Logger.
		*/
		void add_sink(shared_ptr<basesink> sink) {
			sinks.push_back(sink);
		}

	protected:
		vector<shared_ptr<basesink>> sinks;
	};

	/*
		\brief		Global method to create a Logger with ConsoleSink.
		\param		name – The name of the Logger, shown as the title of the console window. Can be left blank.
		\param		ringbuffer – The number of items that can be added before they need to start getting removed; Default 200; Max 32,766.
		\return		
	*/
	inline Logger console_logger(wstring name = L"Console", int ringbuffer = 200) {
		auto sink = make_shared<ConsoleSink>(name, ringbuffer);
		auto logger = Logger();
		logger.add_sink(sink);
		Logger::instance().add_sink(sink);
		return logger;
	}

	/*
		\brief		Global method to set the minimum level of every sink.
		\param		lvl – The level each sink will be set to.
	*/
	inline void set_level(Level lvl) {
		Logger::instance().set_level(lvl);
	}

	/*
		\brief		Global method to output to every sink.
		\param		lvl – The level of the message. This is inserted into the string and also used to determine
					if the message should be shown.
		\param		msgs – Variadic number of messages to be printed for every sink.
	*/
	template<typename... Args>
	inline void print(Level lvl, Args... msgs) {
		Logger::instance().print(lvl, msgs...);
	}

	/*
		\brief		Global method to output a trace message to every sink
		\param		msgs – Variadic number of messages to be printed for every sink.
	*/
	template<typename... Args>
	inline void trace(Args... msgs) {
		Logger::instance().print(TRACE, msgs...);
	}

	/*
		\brief		Global method to output a debug message to every sink
		\param		msgs – Variadic number of messages to be printed for every sink.
	*/
	template<typename... Args>
	inline void debug(Args... msgs) {
		Logger::instance().print(DEBUG, msgs...);
	}

	/*
		\brief		Global method to output a info message to every sink
		\param		msgs – Variadic number of messages to be printed for every sink.
	*/
	template<typename... Args>
	inline void info(Args... msgs) {
		Logger::instance().print(INFO, msgs...);
	}

	/*
		\brief		Global method to output a warn message to every sink
		\param		msgs – Variadic number of messages to be printed for every sink.
	*/
	template<typename... Args>
	inline void warn(Args... msgs) {
		Logger::instance().print(WARN, msgs...);
	}

	/*
		\brief		Global method to output a error message to every sink
		\param		msgs – Variadic number of messages to be printed for every sink.
	*/
	template<typename... Args>
	inline void error(Args... msgs) {
		Logger::instance().print(ERROR, msgs...);
	}

	/*
		\brief		Global method to output a fatal message to every sink
		\param		msgs – Variadic number of messages to be printed for every sink.
	*/
	template<typename... Args>
	inline void fatal(Args... msgs) {
		Logger::instance().print(FATAL, msgs...);
	}
}

#pragma pop_macro("ERROR");
#pragma pop_macro("DEBUG");