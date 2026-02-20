#include "deezer/deezerclient.hpp"
#include "deezer/objects/album.hpp"
#include "deezer/objects/page.hpp"
#include "deezer/objects/searchalbum.hpp"
#include "deezer/objects/songdata.hpp"

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

	void testStuff(const DeezerClient &client)
	{
		qDebug() << "Logging in to Deezer...";
		if (!client.login(qEnvironmentVariable("ARL")))
		{
			qCritical() << "Failed to login!";
		}

		{
			ApiResponse *response = client.userData();
			QObject::connect(response, &ApiResponse::finished, [&client, response]() -> void
			{
				const UserData &userData = response->value<UserData>();
				response->deleteLater();

				qDebug().nospace() << "Welcome " << userData.blogName() << "!";
				client.gw().setCheckForm(userData.checkForm());
			});
		}
		{
			ApiResponse *response = client.search(SearchMediaType::Album,
				QStringLiteral("Penny's Big Breakaway"));

			QObject::connect(response, &ApiResponse::finished, [response]() -> void
			{
				const auto page = response->value<Page<SearchAlbum>>();
				response->deleteLater();

				qDebug()
					<< "Results:" << page.total()
					<< "- Has next:" << page.next().isValid();
			});
		}
		{
			ApiResponse *response = client.album(547868882);
			QObject::connect(response, &ApiResponse::finished, [response]() -> void
			{
				const auto album = response->value<Album>();
				response->deleteLater();

				qDebug() << "Album:" << album.id() << album.title();
			});
		}
		{
			while (client.gw().checkForm().isEmpty())
			{
				QCoreApplication::processEvents();
			}

			ApiResponse *response = client.gw().songData(2662655242);
			QObject::connect(response, &ApiResponse::finished, [response]() -> void
			{
				if (!response->isValid())
				{
					qCritical() << "Error:" << response->errorString();
					response->deleteLater();
					return;
				}

				const auto songData = response->value<SongData>();
				response->deleteLater();
			});
		}
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
		QStringLiteral("kraxarn.%1")
		.arg(QCoreApplication::applicationName()),
		QStringLiteral("Main")
	);

	const DeezerClient client(nullptr);
	testStuff(client);

	return app.exec();
}
