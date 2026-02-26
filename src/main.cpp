#include "deezer/deezerclient.hpp"
#include "qml/loginpage.hpp"
#include "qml/playerpage.hpp"
#include "qml/settings.hpp"

#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#define registerType(t) qmlRegisterType<t>(#t,1,0,#t)

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

		registerType(Settings);

		qmlRegisterType<LoginPage>("Pages.Login", 1, 0, "Login");
		qmlRegisterType<PlayerPage>("Pages.Player", 1, 0, "Player");
	}
}

auto main(int argc, char *argv[]) -> int
{
	QCoreApplication::setApplicationName(QStringLiteral(APP_NAME));
	QCoreApplication::setApplicationVersion(QStringLiteral(APP_VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(ORG_NAME));
	QCoreApplication::setOrganizationDomain(QStringLiteral(ORG_DOMAIN));

	// For now at least
	QGuiApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

	const QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	defineTypes(engine);

	QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
		&app, [](const QUrl &url) -> void
		{
			qCritical() << "Failed to load:" << url.toString();
			QCoreApplication::exit(-1);
		}, Qt::QueuedConnection);

	DeezerClient::createInstance(&engine);

	engine.load(QStringLiteral(":/qml/Main.qml"));

	return app.exec();
}
