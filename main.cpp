#include "PropertyTest.h"
#include "ObservablePropertyTest.h"
#include "ObservablePropertyQtIntegrationTest.h"

#include <QCoreApplication>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

//	propertyTest();
//	observablePropertyTest();
	observablePropertyQtIntegrationTest();

	return app.exec();
}
