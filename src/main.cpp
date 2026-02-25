#include "deezer/cypher.hpp"
#include "deezer/deezerclient.hpp"
#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/album.hpp"
#include "deezer/objects/mediaurl.hpp"
#include "deezer/objects/options.hpp"
#include "deezer/objects/page.hpp"
#include "deezer/objects/searchalbum.hpp"
#include "deezer/objects/songdata.hpp"
#include "qml/loginpage.hpp"

#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace
{
	void defineTypes(const QQmlApplicationEngine &engine)
	{
		engine.rootContext()->setContextProperty(QStringLiteral("AppName"),
			QCoreApplication::applicationName());

		engine.rootContext()->setContextProperty(QStringLiteral("AppVersion"),
			QCoreApplication::applicationVersion());

		engine.rootContext()->setContextProperty(QStringLiteral("QtVersion"),
			QStringLiteral(QT_VERSION_STR));

		engine.rootContext()->setContextProperty(QStringLiteral("BuildDate"),
			QStringLiteral(__DATE__));

		qmlRegisterType<LoginPage>("LoginPage", 1, 0, "LoginPage");
	}
}

auto main(int argc, char *argv[]) -> int
{
	QCoreApplication::setApplicationName(QStringLiteral(APP_NAME));
	QCoreApplication::setApplicationVersion(QStringLiteral(APP_VERSION));

	// For now at least
	QGuiApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

	const QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	defineTypes(engine);

	engine.load(QStringLiteral(":/qml/Main.qml"));

	return app.exec();
}
