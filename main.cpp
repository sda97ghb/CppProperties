#include "Property.h"

#include <QCoreApplication>
#include <QtDebug>

class Test {
public:
	// Auto generate getters and setters
	Property<int> SimpleIntProp;
	Property<QString> SimpleStrProp;

	// Getters and setters will be set in the constructor
	Property<int> IntProp;
	Property<QString> StrProp;

	// Read-only properties, getters will be set in the constructor
	Property<int> ReadOnlyIntProp;
	Property<QString> ReadOnlyStrProp;
	Property<QStringList> ReadOnlyStringsProp;
	Property<QList<int>> ReadOnlyIntsProp;

	Test() :
		m_v(-1) {
		// Retranslate getter and setters calls to methods of this object
		IntProp = Property<int>::of(this, &Test::v, &Test::setV);
		StrProp = Property<QString>::of(this, &Test::str, &Test::setStr);

		// Set const getter from lambda
		ReadOnlyIntProp = Property<int>::readOnlyOf([](const int& field) {
			Q_UNUSED(field)
			return 42;
		});

		// Set const getter from value
		ReadOnlyStrProp = Property<QString>::readOnlyOf("Hello World!");

		// Retranslate getter call to method of this object, deny setter
		ReadOnlyStringsProp = Property<QStringList>::readOnlyOf(this, &Test::strings);

		// Retranslate getter call to static method, deny setter
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
