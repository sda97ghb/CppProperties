#include "PropertyTest.h"
#include "ObservablePropertyTest.h"

#include <QCoreApplication>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

//	propertyTest();
	observablePropertyTest();

	return app.exec();
}
