#include "Property/ObservableProperty.h"
#include "ObservablePropertyTest.h"

#include <QtDebug>

using property::ObservableProperty;

class ReadOnlyTest {
public:
	// Auto generate getters and setters
	ObservableProperty<int> SimpleIntProp;
	ObservableProperty<QString> SimpleStrProp;

	// Getters and setters will be set in the constructor
	ObservableProperty<int> IntProp;
	ObservableProperty<QString> StrProp;

	// Read-only properties, getters will be set in the constructor
	ObservableProperty<int> ReadOnlyIntProp;
	ObservableProperty<QString> ReadOnlyStrProp;
	ObservableProperty<QStringList> ReadOnlyStringsProp;
	ObservableProperty<QList<int>> ReadOnlyIntsProp;

	ReadOnlyTest() :
		m_v(-1) {
		// Retranslate getter and setters calls to methods of this object
		IntProp = ObservableProperty<int>::of(this, &ReadOnlyTest::v, &ReadOnlyTest::setV);
		StrProp = ObservableProperty<QString>::of(this, &ReadOnlyTest::str, &ReadOnlyTest::setStr);

		// Set const getter from lambda
		ReadOnlyIntProp = ObservableProperty<int>::readOnlyOf([](const int& field) {
			Q_UNUSED(field)
			return 42;
		});

		// Set const getter from value
		ReadOnlyStrProp = ObservableProperty<QString>::readOnlyOf("Hello World!");

		// Retranslate getter call to method of this object, deny setter
		ReadOnlyStringsProp = ObservableProperty<QStringList>::readOnlyOf(this, &ReadOnlyTest::strings);

		// Retranslate getter call to static method, deny setter
		ReadOnlyIntsProp = ObservableProperty<QList<int>>::readOnlyOf(&ReadOnlyTest::ints);
	}

	void testNotifyReadOnly() {
		ReadOnlyIntProp.notifyObservers();
		ReadOnlyIntsProp.notifyObservers();
		ReadOnlyStrProp.notifyObservers();
		ReadOnlyStringsProp.notifyObservers();
	}

private:
	int v() const {
		qDebug() << "getter";
		return m_v;
	}
	void setV(const int& v) {
		qDebug() << "setter";
		m_v = v;
		IntProp.notifyObservers();
	}

	QString str() const {
		qDebug() << "getter str";
		return m_str;
	}

	void setStr(const QString& str) {
		qDebug() << "setter str";
		m_str = str;
		StrProp.notifyObservers();
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


void observablePropertyTest() {
	ReadOnlyTest t;

	t.SimpleIntProp.observe([](const int& value) {
		qDebug() << "Observer:" << value;
	});
	t.SimpleStrProp.observe([](const QString& value) {
		qDebug() << "Observer:" << value;
	});

	t.IntProp.observe([](const int& value) {
		qDebug() << "Observer:" << value;
	});
	t.StrProp.observe([](const QString& value) {
		qDebug() << "Observer:" << value;
	});

	t.ReadOnlyIntProp.observe([](const int& value) {
		qDebug() << "Observer:" << value;
	});
	t.ReadOnlyStrProp.observe([](const QString& value) {
		qDebug() << "Observer:" << value;
	});
	t.ReadOnlyIntsProp.observe([](const QList<int>& value) {
		qDebug() << "Observer:" << value;
	});
	t.ReadOnlyStringsProp.observe([](const QStringList& value) {
		qDebug() << "Observer:" << value;
	});

	t.SimpleIntProp = 10;
	qDebug() << "";
	t.SimpleStrProp = "Hello";
	qDebug() << "";
	t.IntProp = 20;
	qDebug() << "";
	t.StrProp = "World!";
	qDebug() << "";
//	t.ReadOnlyIntProp = 30; // cases exception
//	t.ReadOnlyStrProp = "Goodbye World!"; // cases exception
//	t.ReadOnlyStringsProp = {"Hello", "World!"}; // cases exception
//	t.ReadOnlyIntsProp = {1, 2, 3}; // cases exception
	t.testNotifyReadOnly();

	int a = t.SimpleIntProp;
	qDebug() << "";
	QString b = t.SimpleStrProp;
	qDebug() << "";
	int c = t.IntProp;
	qDebug() << "";
	QString d = t.StrProp;
	qDebug() << "";
	int e = t.ReadOnlyIntProp;
	QString f = t.ReadOnlyStrProp;
	QStringList g = t.ReadOnlyStringsProp;
	QList<int> h = t.ReadOnlyIntsProp;

	qDebug() << a;
	qDebug() << b;
	qDebug() << c;
	qDebug() << d;
	qDebug() << e;
	qDebug() << f;
	qDebug() << g;
	qDebug() << h;
}
