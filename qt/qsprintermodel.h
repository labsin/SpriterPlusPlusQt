#ifndef QSPRINTERMODEL_H
#define QSPRINTERMODEL_H

#include <QObject>

namespace SpriterEngine {
class SpriterModel;
class EntityInstance;
}

class QSprinterModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)

public:
	explicit QSprinterModel(QObject *parent = 0);
	~QSprinterModel();

	static void error(const std::string &errorMessage);

	QString file() const
	{
		return m_file;
	}

	SpriterEngine::EntityInstance *getNewEntityInstance(QString name);

signals:
	void fileChanged(QString file);

public slots:
	void setFile(QString file);

private:
	SpriterEngine::SpriterModel *m_model;

	QString m_file;

};

#endif // QSPRINTERMODEL_H
