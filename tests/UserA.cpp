#include "UserA.h"

#include <QDebug>

UserA::UserA(QObject* parent) : QObject(parent) {
	Name = ObservableProperty<QString>::of(this, &UserA::name, &UserA::setName);
	connect(this, &UserA::nameChanged, [&](QString) {
		Name.notifyObservers();
	});

	Age = ObservableProperty<int>::of(this, &UserA::age, &UserA::setAge);
	connect(this, &UserA::ageChanged, [&](int) {
		Age.notifyObservers();
	});
}

QString UserA::name() const {
	qDebug() << "UserA name getter";
	return m_name;
}

int UserA::age() const {
	qDebug() << "UserA age getter";
	return m_age;
}

void UserA::setName(const QString& name) {
	qDebug() << "UserA name setter";

	if (m_name == name)
		return;

	m_name = name;
	emit nameChanged(m_name);
}

void UserA::setAge(const int& age) {
	qDebug() << "UserA age setter";

	if (m_age == age)
		return;

	m_age = age;
	emit ageChanged(m_age);
}
