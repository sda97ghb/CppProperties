# CppProperties
This library provides C++ properties support similar to properties from C# or Kotlin.

This library consist only of header files. All you need to start use the library is
to copy *include/Propery* directory to your project.

## Library classes

There are two property class templates: **Property\<T\>** and **ObservableProperty\<T\>**.

Template argument **T** can be of any type compatible with C++:
- primitive type such as **int** or **double**
- C++ class such as **std::string** or **std::list\<std::string\>**
- Qt class such as **QString** or **QVariantList**

**Property\<T\>** holds a value of type T and has mechanism of assigning user defined getter and setter. 
If getter and setter are not provided by user default getter and setter will be used.

**ObservableProperty\<T\>** can do all work of **Property\<T\>** and also add an ability to assign an 
observer which will get property change notifications.

Each property overloads assignment and type cast operators, so you can write code like this:
```c++
class C {
public:
    Property<int> val;
}

void f() {
    C c;
    c.val = 10;
    int val = c.val;
}
```

Also you can use get and set methods of the propery.

# Property\<T\>

This type of property holds a value of type T and has mechanism of assigning user defined getter and setter. 
If getter and setter are not provided by user default getter and setter will be used.

First of all you need to include the header and use the class from namespace (if you wish).
```c++
#include "Property/Property.h"
using property::Property;
```
Now you can use properties. To define a property in your class you need a code like following:
```c++
class User {
public:
    Property<QString> Name;
    Property<int> Age;
}
```

## Custom getters and setters

For each property custom getter and setter can be assigned.

A property contains its value. This value is passed in getter and setter as **field** parameter.

Getter signature is `T(const T& field)` or `[](const T& field) -> T`.

Setter signature is `void(const T& value, T& field)` or `(const T& value, T& field) -> void`.

Default implementations are:
```c++
DEFAULT_GETTER = [](const T& field) -> T {
    return field;
};

DEFAULT_SETTER = [](const T& value, T& field) -> void {
    field = value;
};
```

There are few ways of assigning of custom getters and setters.

## Getter and setter from lambda
Custom getters and setters can be set from lambda functions.

Default getter and setter are defined as Property<T>::DEFAULT_GETTER and Property<T>::DEFAULT_SETTER respectively and can be used as parameters here.
```c++
User::User() {
    // Set getter and setter from lambda.
    // Default getter and setter is defined as Property<T>::DEFAULT_GETTER and Property<T>::DEFAULT_SETTER.
    Name = Property<QString>::of(
        [](const QString& field) {
            qDebug() << "getter";
            return field;
        },
        [](const QString& value, QString& field) {
            qDebug() << "setter";
            field = value;
        }
    );
    
    // Set default getter and custom setter.
    Name = Property<QString>::of(
        Property<T>::DEFAULT_GETTER,
        [](const QString& value, QString& field) {
            qDebug() << "setter";
            field = value;
        }
    );
}
```

## Getter and setter from methods of the class
You can assign getter and setter from methods of the class.

**Notice:** setter **must** have `const T&` argument not `T` or `T&` or `T*` or anything else.

```c++
User::User() {
    // Set getter and setter from class methods.
    Name = Property<T>::of(this, &User::getName, &User::setName);
}

QString User::getName() const {
    return m_name;
}

void User::setName(const QString& name) {
    m_name = name;
}
```

## Read-only properties

Property can be set as read-only. There are few ways of doing this:
```c++
// Set special read-only setter
Name = Property<QString>::of(getter, Property<QString>::READ_ONLY_SETTER);

// Set from lambda
Name = Property<QString>::readOnlyOf([](const QString& field){ return "John Doe"; });

// Set from getter method
Name = Property<QString>::readOnlyOf(this, &User::getName);

QString User::getName() const {
    return "John Doe";
}

// Set from static method or any non-method function
Name = Property<QString>::readOnlyOf(&globalUserName);

QString globalUserName() {
    return "John Doe";
}

// Set from constant
Name = Property<QString>::readOnlyOf("John Doe");
```

If setter of read-only property is called **Property\<T\>::CallSetterOfReadOnlyPropertyException** will be thrown.

# ObservableProperty\<T\>
Observable properties are similar to usual properties but also can be observed. Each observable property can have any number of observers.

## Observing mechanism
```c++
class User {
public:
    ObservableProperty<QString> name;
}
void f(User& user) {
    user.Name.observe([](const QString& name){
        qDebug() << name;
    });
    
    // After this line observer will be called.
    user.Name = "John Doe";
}
```

## Notification mechanism

As soon as observable properties need a mechanism to notify their subscribers about changes happend in setters
we need a new special syntax of sending notifications. Because of this we've got new parameter in setters:

```c++
class User {
public:
    ObservableProperty<QString> Name;
    
    User() {
        Name = ObservableProperty<QString>::of(
            ObservableProperty<QString>::DEFAULT_GETTER,
            [&](const QString& value, QString& field,
                ObservableProperty<QString>::ObserverNotifier notifyChanged) {
                field = value;
                notifyChanged(field);
            }
        );
    }
}
```

There is second way of sending notifications based on use of notifyObservers() method of the property.
This way is esspecially useful when we need to set setter from method of the property holder's class.

```c++
class User {
public:
    ObservableProperty<QString> Name;
    
    User() {
        Name = ObservableProperty<QString>::of(this, &User::getName, &User::setName);
    }
    
    QString getName() const { 
        return m_name;
    }
    
    void setName(const QString& name) {
        m_name = name;
        Name.notifyObservers();
    }
    
private:
    QString m_name;
}
```

# Qt compatibility

**ObservableProperty\<T\>** is fully compatible with **Q_PROPERTY.**

Two examples of usage **Q_PROPERTY** and **ObservableProperty\<T\>** in the same class
can be found in `test/UserA.*` and `test/UserB.*` classes.

**UserA** is an example of possible migration from **Q_PROPERTY** class to **ObservableProperty\<T\>** class.

**UserB** is an example of possible migration from **ObservableProperty\<T\>** to **Q_PROPERTY** class.
