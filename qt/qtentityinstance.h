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

class QtEntityInstance : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QtSpriterModel* model READ model WRITE setModel NOTIFY modelChanged)
	Q_PROPERTY(QString animation READ animation WRITE setAnimation NOTIFY animationChanged)
	Q_PROPERTY(QPointF scale READ scale WRITE setScale NOTIFY scaleChanged)
	Q_PROPERTY(float speedRatio READ speedRatio WRITE setSpeedRatio NOTIFY speedRatioChanged)

public:
	QtEntityInstance(QQuickItem *parent = 0);
	~QtEntityInstance();

	QString name() const
	{
		return m_name;
	}

	QtSpriterModel* model() const
	{
		return m_model;
	}

	QString animation() const
	{
		return m_animation;
	}

	QPointF scale() const
	{
		return m_scale;
	}

	float speedRatio() const
	{
		return m_speedRatio;
	}

signals:
	void nameChanged(QString name);

	void modelChanged(QtSpriterModel* model);

	void animationChanged(QString animation);

	void scaleChanged(QPointF scale);

	void speedRatioChanged(float speedRatio);

public slots:
	void setName(QString name);

	void setModel(QtSpriterModel* model);

	void updateQQuickWindow(QQuickWindow* window);
	void updateIfLoaded();

	void setAnimation(QString animation);

	void setScale(QPointF scale);

	void setSpeedRatio(float speedRatio);
	void updateInterface();

private:
	void load();
	void unload();

	QSGSpriterBase *getQSGSpriterNode(SpriterEngine::UniversalObjectInterface* interface);

	QString m_name;
	bool m_zOrderChanged;
	bool m_loaded;

	QtSpriterModel* m_model;
	SpriterEngine::EntityInstance* m_entity;

	SpriterEngine::ObjectInterfaceVector* m_zOrder;
	QHash<SpriterEngine::UniversalObjectInterface*, QSGSpriterBase*> m_nodeMap;


	// QQuickItem interface
	QString m_animation;

	QPointF m_scale;

	float m_speedRatio;

	QTime m_time;

	QMutex m_nodeMapMutex;

protected:
	QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};



#endif // QENTITYINSTANCE_H
