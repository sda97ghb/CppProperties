/****************************************************************************
**
** MIT License
**
** Copyright (c) 2018 Dmitry Smirnov
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** ** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
****************************************************************************/

#ifndef OBSERVABLEPROPERTY_H
#define OBSERVABLEPROPERTY_H

#include <functional>
#include <list>

namespace property {

template <typename T>
class ObservableProperty {
public:
	class CallSetterOfReadOnlyPropertyException : public std::logic_error {
	public:
		CallSetterOfReadOnlyPropertyException() :
			std::logic_error("Attempt to set value for read only property") {}
	};

	using ObserverNotifier = std::function<void(const T&)>;
	using Observer = std::function<void(const T&)>;
	using Getter = std::function<T(const T&)>;
	using Setter = std::function<void(const T&, T&, ObserverNotifier)>;

	static Getter DEFAULT_GETTER;
	static Setter DEFAULT_SETTER;
	static Setter READ_ONLY_SETTER;

	ObservableProperty(Getter getter = nullptr, Setter setter = nullptr) {
		m_getter = (getter == nullptr ? DEFAULT_GETTER : getter);
		m_setter = (setter == nullptr ? DEFAULT_SETTER : setter);
	}

	static ObservableProperty<T> of(Getter getter, Setter setter) {
		return ObservableProperty<T>(getter, setter);
	}

	template <typename C>
	static ObservableProperty<T> of(C* obj, T(C::*getter)()const, void(C::*setter)(const T&)) {
		return ObservableProperty<T>(
		[=](const T& field) -> T {
			(void)field;
			return (obj->*getter)();
		},
		[=](const T& value, T& field, ObserverNotifier notifyChanged) -> void {
			(void)field;
			(void)notifyChanged;
			(obj->*setter)(value);
		});
	}

	static ObservableProperty<T> readOnlyOf(Getter getter) {
		return ObservableProperty<T>(getter, READ_ONLY_SETTER);
	}

	template <typename C>
	static ObservableProperty<T> readOnlyOf(C* obj, T(C::*getter)()const) {
		return ObservableProperty<T>(std::bind(getter, obj), READ_ONLY_SETTER);
	}

	static ObservableProperty<T> readOnlyOf(T constant) {
		return readOnlyOf([=](const T&) {
			return constant;
		});
	}

	static ObservableProperty<T> readOnlyOf(T(*getter)()) {
		return readOnlyOf([=](const T&) {
			return getter();
		});
	}

	T get() const {
		return m_getter(m_value);
	}

	void set(const T& value) {
		m_setter(value, m_value, notifier());
	}

	operator T() const {
		return m_getter(m_value);
	}

	void operator =(const T& value) {
		m_setter(value, m_value, notifier());
	}

	void observe(Observer observer) {
		m_observers.push_back(observer);
	}

	void notifyObservers() {
		for (Observer observer : m_observers)
			observer(get());
	}

private:
	ObserverNotifier notifier() {
		return [&](const T& value) {
			for (Observer observer : m_observers)
				observer(value);
		};
	}

	Getter m_getter;
	Setter m_setter;
	T m_value;
	std::list<Observer> m_observers;
};

template <typename T>
typename ObservableProperty<T>::Getter ObservableProperty<T>::DEFAULT_GETTER =
[](const T& field) -> T {
	return field;
};

template <typename T>
typename ObservableProperty<T>::Setter ObservableProperty<T>::DEFAULT_SETTER =
[](const T& value, T& field, ObservableProperty<T>::ObserverNotifier notifyChanged) -> void {
	field = value;
	notifyChanged(field);
};

template <typename T>
typename ObservableProperty<T>::Setter ObservableProperty<T>::READ_ONLY_SETTER =
[](const T& value, T& field, ObservableProperty<T>::ObserverNotifier notifyChanged) -> void {
	(void)value;
	(void)field;
	(void)notifyChanged;
	throw CallSetterOfReadOnlyPropertyException();
};

} // property namespace

#endif // OBSERVABLEPROPERTY_H
