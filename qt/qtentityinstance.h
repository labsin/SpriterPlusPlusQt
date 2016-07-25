#ifndef QENTITYINSTANCE_H
#define QENTITYINSTANCE_H

#include "qtspritermodel.h"

#include <QQuickItem>
#include <QTime>
#include <QPointF>
#include <QList>
#include <QHash>
#include <QMutex>

namespace SpriterEngine {
class UniversalObjectInterface;
typedef std::vector<UniversalObjectInterface*> ObjectInterfaceVector;
}

class QSGSpriterBase;

struct QtEntityInstanceData {
	QtEntityInstanceData(QString _entityName = QString(""), QString _animationName = QString(""),
						 QPointF _scale = QPointF(1.0, 1.0), float _playbackSpeed = 1.0) :
		entityName(_entityName), animationName(_animationName), scale(_scale), playbackSpeed(_playbackSpeed)
	{
	}
	QString entityName;
	QString animationName;
	QPointF scale;
	float playbackSpeed;
};

class QtEntityInstanceWorker : public QObject {
	Q_OBJECT

public:
	QtEntityInstanceWorker(QtSpriterModel *model);
	~QtEntityInstanceWorker();

public slots:
	void startResume();
	void pause();

	void setCurrentAnimation(const QString animation);
	void setScale(const QPointF scale);
	void setPlaybackSpeedRatio(float speed);

	void load(QtEntityInstanceData data);

	void update();

signals:
	void error(QString error);
	void loaded(bool);
	void updated(bool zOrderChanged, SpriterEngine::ObjectInterfaceVector*);

	void finished();
	void looped();
	void running(bool);

private:
	void unload();

	SpriterEngine::EntityInstance* m_currentEntity;
	QHash<QString, SpriterEngine::EntityInstance*> m_entityMap;

	SpriterEngine::ObjectInterfaceVector* m_zOrder;

	QtSpriterModelWorker* m_modelWorker;

	QTime m_time;
};

class QtEntityInstance : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QtSpriterModel* model READ model WRITE setModel NOTIFY modelChanged)
	Q_PROPERTY(QString animation READ animation WRITE setAnimation NOTIFY animationChanged)
	Q_PROPERTY(QPointF scale READ scale WRITE setScale NOTIFY scaleChanged)
	Q_PROPERTY(float speedRatio READ speedRatio WRITE setSpeedRatio NOTIFY speedRatioChanged)
	Q_PROPERTY(QString error READ errorString NOTIFY errorStringChanged)
	Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
	Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
	QtEntityInstance(QQuickItem *parent = 0);
	~QtEntityInstance();

	QString name() const
	{
		return data.entityName;
	}

	QtSpriterModel* model() const
	{
		return m_model;
	}

	QString animation() const
	{
		return data.animationName;
	}

	QPointF scale() const
	{
		return data.scale;
	}

	float speedRatio() const
	{
		return data.playbackSpeed;
	}

	QString errorString() const
	{
		return m_errorString;
	}

	bool loaded() const
	{
		return m_loaded;
	}

	bool running() const
	{
		return m_running;
	}

signals:
	void nameChanged(QString name);

	void modelChanged(QtSpriterModel* model);

	void animationChanged(QString animation);

	void scaleChanged(QPointF scale);

	void speedRatioChanged(float speedRatio);

	void looped();

	void finished();

	void errorStringChanged(QString error);

	void loadedChanged(bool loaded);

	void runningChanged(bool running);

	void workerSetCurrentAnimation(QString animation);
	void workerSetScale(QPointF scale);
	void workerSetPlaybackSpeedRatio(float speed);
	void workerLoad(QtEntityInstanceData data);
	void workerUpdate();
	void workerDelete();

	void startResume();
	void pause();

public slots:
	void setName(QString name);

	void setModel(QtSpriterModel* model);

	void setNewEntityInstance(QString name, SpriterEngine::EntityInstance* entity);

	void setAnimation(QString animation);

	void setScale(QPointF scale);

	void setSpeedRatio(float speedRatio);

private slots:
	void updateQQuickWindow(QQuickWindow* window);
	void setZOrder(bool zOrderChanged, SpriterEngine::ObjectInterfaceVector* zOrder);
	void setRunning(bool running) {
		if(m_running == running) {
			return;
		}

		m_running = running;
		emit runningChanged(running);
	}

private:
	void load();
	void unload();
	void updateInterface();

	void setErrorString(QString error);

	void setLoaded(bool loaded);

	QSGSpriterBase *getQSGSpriterNode(SpriterEngine::UniversalObjectInterface* interface);

	QtSpriterModel* m_model;

	QMutex m_zOrderMutex;
	SpriterEngine::ObjectInterfaceVector* m_zOrder;
	bool m_zOrderChanged;
	QHash<SpriterEngine::UniversalObjectInterface*, QSGSpriterBase*> m_nodeMap;

	QtEntityInstanceData data;

	// QQuickItem interface
	QString m_animation;

	QPointF m_scale;

	float m_speedRatio;

	QString m_errorString;

	bool m_loaded;
	bool m_loading;

	bool m_running;

protected:
	QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};

Q_DECLARE_METATYPE(QtEntityInstanceData)

#endif // QENTITYINSTANCE_H
