// LIB //////////////////////////////////////////

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

	template <typename C>
	static Property<T> of(C* obj, T(C::*getter)()const, void(C::*setter)(const T&)) {
		return Property<T>(std::bind(getter, obj),
						   std::bind(setter, obj, std::placeholders::_1));
	}

	static Property<T> readOnly(Getter getter) {
		return Property<T>(getter, READ_ONLY_SETTER);
	}

	template <typename C>
	static Property<T> readOnlyOf(C* obj, T(C::*getter)()const) {
		return Property<T>(std::bind(getter, obj), READ_ONLY_SETTER);
	}

	static Property<T> readOnlyOf(T constant) {
		return readOnly([=](const T&) {
			return constant;
		});
	}

	static Property<T> readOnlyOf(T(*getter)()) {
		return readOnly([=](const T&) {
			return getter();
		});
	}

	T get() const {
		return m_getter(m_value);
	}

	void set(const T& value) {
		m_setter(value, m_value);
	}

	operator T() {
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

// TEST /////////////////////////////////////////

#include <QCoreApplication>
#include <QtDebug>

class Test {
public:
	Property<int> SimpleIntProp;
	Property<QString> SimpleStrProp;

	Property<int> IntProp;
	Property<QString> StrProp;

	Property<int> ReadOnlyIntProp;
	Property<QString> ReadOnlyStrProp;
	Property<QStringList> ReadOnlyStringsProp;
	Property<QList<int>> ReadOnlyIntsProp;

	Test() :
		m_v(-1) {
		IntProp = Property<int>::of(this, &Test::v, &Test::setV);
		StrProp = Property<QString>::of(this, &Test::str, &Test::setStr);
		ReadOnlyIntProp = Property<int>::readOnly([](const int& field) {
			Q_UNUSED(field)
			return 42;
		});
		ReadOnlyStrProp = Property<QString>::readOnlyOf("Hello World!");
		ReadOnlyStringsProp = Property<QStringList>::readOnlyOf(this, &Test::strings);
		ReadOnlyIntsProp = Property<QList<int>>::readOnlyOf(&Test::ints);
	}

private:
	int v() const {
		qDebug() << "getter";
		return m_v;
	}
	void setV(const int& v) {
		qDebug() << "setter";
		m_v = v;
	}

	QString str() const {
		qDebug() << "getter str";
		return m_str;
	}

	void setStr(const QString& str) {
		qDebug() << "setter str";
		m_str = str;
	}

	QStringList strings() const {
		static QStringList strs = {
			"Hello",
			"World",
			"!"
		};
		return strs;
	}

	static QList<int> ints() {
		static QList<int> intsList = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
		return intsList;
	}

	int m_v;
	QString m_str;
};

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	Test t;

	t.SimpleIntProp = 10;
	t.IntProp = 20;
	t.SimpleStrProp = "Hello";
	t.StrProp = "World!";
//	t.ReadOnlyIntProp = 30; // cases exception
//	t.ReadOnlyStrProp = "Goodbye World!"; // cases exception
//	t.ReadOnlyStringsProp = {"Hello", "World!"}; // cases exception
//	t.ReadOnlyIntsProp = {1, 2, 3}; // cases exception

	int a = t.SimpleIntProp;
	int b = t.IntProp;
	QString c = t.SimpleStrProp;
	QString d = t.StrProp;
	int e = t.ReadOnlyIntProp;
	QString f = t.ReadOnlyStrProp;
	QStringList g = t.ReadOnlyStringsProp;
	QList<int> h = t.ReadOnlyIntsProp;

	qDebug() << a << b << c << d << e << f << g << h;

	return app.exec();
}
