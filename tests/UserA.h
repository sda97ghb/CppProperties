#ifndef USERA_H
#define USERA_H

#include "Property/ObservableProperty.h"

#include <QObject>

using property::ObservableProperty;

class UserA : public QObject {
	Q_OBJECT

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)

public:
	ObservableProperty<QString> Name;
	ObservableProperty<int> Age;

	UserA(QObject* parent = nullptr);

	QString name() const;
	int age() const;

public slots:
	void setName(const QString& name);
	void setAge(const int& age);

signals:
	void nameChanged(QString name);
	void ageChanged(int age);

private:
	QString m_name;
	int m_age;
};

#endif // USERA_H
