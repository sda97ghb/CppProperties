// LIB //////////////////////////////////////////

#include <functional>

template <typename T>
class Property {
public:
	using Getter = std::function<T(const T&)>;
	using Setter = std::function<void(const T&, T&)>;

	static Getter DEFAULT_GETTER;
	static Setter DEFAULT_SETTER;

	Property(Getter getter = nullptr, Setter setter = nullptr) {
		m_getter = (getter == nullptr ? DEFAULT_GETTER : getter);
		m_setter = (setter == nullptr ? DEFAULT_SETTER : setter);
	}

	template <typename C>
	static Property<T> of(C* obj, T(C::*getter)()const, void(C::*setter)(const T&)) {
		return Property<T>(std::bind(getter, obj),
						   std::bind(setter, obj, std::placeholders::_1));
	}

	T get() const {
		return m_getter(m_value);
	}

	void set(const T& value) {
		m_setter(value, m_value);
	}

	void operator =(const T& value) {
		set(value);
	}

	operator T() {
		return get();
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

// TEST /////////////////////////////////////////

#include <QCoreApplication>
#include <QtDebug>

class Test {
public:
	Property<int> IntProp;
	Property<QString> StrProp;

	Test() :
		m_v(-1) {
		IntProp = Property<int>::of(this, &Test::v, &Test::setV);
//		IntProp = Property<int>(
//		[&](const int& field) {
//			qDebug() << "getter";
//			return m_v;
//		},
//		[&](const int& value, int& field) {
//			qDebug() << "setter";
//			m_v = value;
//		});
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

	int m_v;
};

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);

	Test t;

	t.IntProp = 10;
	t.StrProp = "Hello!";

	int x = t.IntProp;
	QString str = t.StrProp;

	qDebug() << x << str;

	return a.exec();
}
