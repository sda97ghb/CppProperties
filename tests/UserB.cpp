#include "UserB.h"

UserB::UserB(QObject* parent) : QObject(parent) {
	Name.observe([=](QString name) {
		emit nameChanged(name);
	});
	Age.observe([=](int age) {
		emit ageChanged(age);
	});
}

QString UserB::name() const {
	return Name;
}

int UserB::age() const {
	return Age;
}

void UserB::setName(QString name) {
	Name = name;
}

void UserB::setAge(int age) {
	Age = age;
}
