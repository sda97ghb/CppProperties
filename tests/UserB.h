#ifndef USERB_H
#define USERB_H

#include "Property/ObservableProperty.h"

#include <QObject>

using property::ObservableProperty;

class UserB : public QObject {
	Q_OBJECT

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)

public:
	ObservableProperty<QString> Name;
	ObservableProperty<int> Age;

	UserB(QObject* parent = nullptr);

	QString name() const;
	int age() const;

public slots:
	void setName(QString name);
	void setAge(int age);

signals:
	void nameChanged(QString name);
	void ageChanged(int age);
};

#endif // USERB_H
