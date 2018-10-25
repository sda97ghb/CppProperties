#ifndef PROPERTY_H
#define PROPERTY_H

#include <functional>

template <typename T>
class Property {
public:
	class CallSetterOfReadOnlyPropertyException : public std::logic_error {
	public:
		CallSetterOfReadOnlyPropertyException() :
			std::logic_error("Attempt to set value for read only property") {}
	};

	using Getter = std::function<T(const T&)>;
	using Setter = std::function<void(const T&, T&)>;

	static Getter DEFAULT_GETTER;
	static Setter DEFAULT_SETTER;
	static Setter READ_ONLY_SETTER;

	Property(Getter getter = nullptr, Setter setter = nullptr) {
		m_getter = (getter == nullptr ? DEFAULT_GETTER : getter);
		m_setter = (setter == nullptr ? DEFAULT_SETTER : setter);
	}

	static Property<T> of(Getter getter, Setter setter) {
		return Property<T>(getter, setter);
	}

	template <typename C>
	static Property<T> of(C* obj, T(C::*getter)()const, void(C::*setter)(const T&)) {
		return Property<T>(std::bind(getter, obj),
						   std::bind(setter, obj, std::placeholders::_1));
	}

	static Property<T> readOnlyOf(Getter getter) {
		return Property<T>(getter, READ_ONLY_SETTER);
	}

	template <typename C>
	static Property<T> readOnlyOf(C* obj, T(C::*getter)()const) {
		return Property<T>(std::bind(getter, obj), READ_ONLY_SETTER);
	}

	static Property<T> readOnlyOf(T constant) {
		return readOnlyOf([=](const T&) {
			return constant;
		});
	}

	static Property<T> readOnlyOf(T(*getter)()) {
		return readOnlyOf([=](const T&) {
			return getter();
		});
	}

	T get() const {
		return m_getter(m_value);
	}

	void set(const T& value) {
		m_setter(value, m_value);
	}

	operator T() const {
		return m_getter(m_value);
	}

	void operator =(const T& value) {
		m_setter(value, m_value);
	}

private:
	Getter m_getter;
	Setter m_setter;
	T m_value;
};

template <typename T>
typename Property<T>::Getter Property<T>::DEFAULT_GETTER = [](const T& field) -> T {
	return field;
};

template <typename T>
typename Property<T>::Setter Property<T>::DEFAULT_SETTER = [](const T& value, T& field) -> void {
	field = value;
};

template <typename T>
typename Property<T>::Setter Property<T>::READ_ONLY_SETTER = [](const T& value, T& field) -> void {
	(void)value;
	(void)field;
	throw CallSetterOfReadOnlyPropertyException();
};

#endif // PROPERTY_H
