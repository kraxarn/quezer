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

	engine.loadFromModule(
		QCoreApplication::applicationName(),
		QStringLiteral("Main")
	);

	return app.exec();
}
