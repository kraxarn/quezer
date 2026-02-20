#include "deezer/deezerclient.hpp"
#include "deezer/enums/mediaformat.hpp"
#include "deezer/objects/album.hpp"
#include "deezer/objects/mediaurl.hpp"
#include "deezer/objects/page.hpp"
#include "deezer/objects/searchalbum.hpp"
#include "deezer/objects/songdata.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>

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

	void testStuff(DeezerClient &client)
	{
		qDebug() << "Logging in to Deezer...";
		if (!client.login(qEnvironmentVariable("ARL")))
		{
			qCritical() << "Failed to login!";
		}

		UserData userData = UserData::fromJson({});
		SongData songData = SongData::fromJson({});
		MediaUrl mediaUrl = MediaUrl::fromJson({});

		{
			ApiResponse *response = client.gw().userData();
			QObject::connect(response, &ApiResponse::finished, [&client, &userData, response]() -> void
			{
				userData = response->value<UserData>();
				response->deleteLater();

				qDebug().nospace() << "Welcome " << userData.blogName() << "!";
			});
		}
		{
			ApiResponse *response = client.api().search(SearchMediaType::Album,
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
			ApiResponse *response = client.api().album(547868882);
			QObject::connect(response, &ApiResponse::finished, [response]() -> void
			{
				const auto album = response->value<Album>();
				response->deleteLater();

				qDebug() << "Album:" << album.id() << album.title();
			});
		}
		{
			while (userData.checkForm().isEmpty())
			{
				QCoreApplication::processEvents();
			}

			ApiResponse *response = client.gw().songData(userData, 2662655242);
			QObject::connect(response, &ApiResponse::finished, [&songData, response]() -> void
			{
				if (!response->isValid())
				{
					qCritical() << "Error:" << response->errorString();
					response->deleteLater();
					return;
				}

				songData = response->value<SongData>();
				response->deleteLater();
			});
		}
		{
			while (songData.trackToken().isEmpty() || userData.licenseToken().isEmpty())
			{
				QCoreApplication::processEvents();
			}

			ApiResponse *response = client.media().url(userData, songData, MediaFormat::LowQuality);
			QObject::connect(response, &ApiResponse::finished, [&mediaUrl, response]() -> void
			{
				mediaUrl = response->value<MediaUrl>();
				response->deleteLater();
			});
		}
		{
			while (mediaUrl.sources().isEmpty())
			{
				QCoreApplication::processEvents();
			}

			ApiResponse *response = client.get(mediaUrl.sources().at(0).url());
			QObject::connect(response, &ApiResponse::finished, [&mediaUrl, response]() -> void
			{
				const QByteArray &data = response->data();

				if (QFile file(QStringLiteral("file.mp3")); file.open(QIODevice::WriteOnly))
				{
					file.write(data);
					file.close();
				}

				response->deleteLater();
				qInfo() << "File saved!";
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

	DeezerClient client(nullptr);
	testStuff(client);

	return app.exec();
}
