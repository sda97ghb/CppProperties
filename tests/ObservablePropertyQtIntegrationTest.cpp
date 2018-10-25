#include "UserA.h"
#include "UserB.h"
#include "ObservablePropertyQtIntegrationTest.h"

#include <QDebug>

void testUserA() {
	UserA user;

	QObject::connect(&user, &UserA::nameChanged, [](QString name) {
		qDebug() << "name signal:" << name;
	});
	user.Name.observe([](QString name) {
		qDebug() << "name property observer:" << name;
	});

	QObject::connect(&user, &UserA::ageChanged, [](int age) {
		qDebug() << "age signal:" << age;
	});
	user.Age.observe([](int age) {
		qDebug() << "age property observer:" << age;
	});

	user.setName("John Doe");
	user.Name = "John Smith";

	QString name = user.name();
	qDebug() << name;

	name = user.Name;
	qDebug() << name;

	user.setAge(10);
	user.Age = 20;

	int age = user.age();
	qDebug() << age;

	age = user.Age;
	qDebug() << age;
}

void testUserB() {
	UserB user;

	QObject::connect(&user, &UserB::nameChanged, [](QString name) {
		qDebug() << "name signal:" << name;
	});
	user.Name.observe([](QString name) {
		qDebug() << "name property observer:" << name;
	});

	QObject::connect(&user, &UserB::ageChanged, [](int age) {
		qDebug() << "age signal:" << age;
	});
	user.Age.observe([](int age) {
		qDebug() << "age property observer:" << age;
	});

	user.setName("John Doe");
	user.Name = "John Smith";

	QString name = user.name();
	qDebug() << name;

	name = user.Name;
	qDebug() << name;

	user.setAge(10);
	user.Age = 20;

	int age = user.age();
	qDebug() << age;

	age = user.Age;
	qDebug() << age;
}

void observablePropertyQtIntegrationTest() {
//	testUserA();
	testUserB();
}
