#ifndef QSPRINTERMODEL_H
#define QSPRINTERMODEL_H

#include <QObject>
#include <QMutex>
#include <QThread>

namespace SpriterEngine {
class SpriterModel;
class EntityInstance;
}

class QtSpriterModelWorker;

class QtEntityInstance;

class QtSpriterModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
	Q_PROPERTY(bool debug READ debug WRITE setDebug NOTIFY debugChanged)
	Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
	Q_PROPERTY(bool renderBones READ renderBones WRITE setRenderBones NOTIFY renderBonesChanged)
	Q_PROPERTY(bool renderPoints READ renderPoints WRITE setRenderPoints NOTIFY renderPointsChanged)
	Q_PROPERTY(bool renderBoxes READ renderBoxes WRITE setRenderBoxes NOTIFY renderBoxesChanged)

public:
	explicit QtSpriterModel(QObject *parent = 0);
	~QtSpriterModel();

	static void error(const std::string &errorMessage);

	QString file() const
	{
		return m_file;
	}

	bool debug() const
	{
		return m_debug;
	}

	bool renderBones() const;

	bool renderPoints() const;

	bool renderBoxes() const;

	bool loaded() const
	{
		return m_loaded;
	}

signals:
	void fileChanged(QString file);

	void debugChanged(bool debug);

	void renderBonesChanged(bool renderBones);

	void renderPointsChanged(bool renderPoints);

	void renderBoxesChanged(bool renderBoxes);

	void getNewEntityInstance(QString name, QtEntityInstance* instance);

	void loadedChanged(bool loaded);

public slots:
	void setFile(QString file);

	void setDebug(bool debug);

	void setRenderBones(bool renderBones);

	void setRenderPoints(bool renderPoints);

	void setRenderBoxes(bool renderBoxes);

	void newEntityInstance(QString name, QtEntityInstance *instance, SpriterEngine::EntityInstance *entity);

private slots:

	void setLoaded()
	{
		m_loaded = true;
		emit loadedChanged(true);
	}

private:
	QString m_file;

	bool m_debug;
	QThread m_workerThread;
	bool m_loaded;
};

#endif // QSPRINTERMODEL_H
