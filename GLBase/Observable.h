#pragma once

#include <map>
#include <functional>

template <typename MSG, typename ... ARGS>
class Observable {
private:
	std::map<MSG, std::function<void(ARGS...)>> callbacks;
public:
	void observe(MSG msg, std::function<void(ARGS...)> callback) {
		callbacks[msg] = callback;
	}

	void invoke(MSG msg, ARGS... args) {
		if (callbacks.find(msg) != callbacks.end()) {
			callbacks[msg](std::forward<ARGS>(args)...);
		}
	}
};