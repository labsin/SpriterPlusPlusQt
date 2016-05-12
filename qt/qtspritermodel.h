#ifndef QSPRINTERMODEL_H
#define QSPRINTERMODEL_H

#include <QObject>

namespace SpriterEngine {
class SpriterModel;
class EntityInstance;
}

class QtSpriterModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
	Q_PROPERTY(bool debug READ debug WRITE setDebug NOTIFY debugChanged)
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

	SpriterEngine::EntityInstance *getNewEntityInstance(QString name);

	bool debug() const
	{
		return m_debug;
	}

	bool renderBones() const;

	bool renderPoints() const;

	bool renderBoxes() const;

signals:
	void fileChanged(QString file);

	void debugChanged(bool debug);

	void renderBonesChanged(bool renderBones);

	void renderPointsChanged(bool renderPoints);

	void renderBoxesChanged(bool renderBoxes);

public slots:
	void setFile(QString file);

	void setDebug(bool debug);

	void setRenderBones(bool renderBones);

	void setRenderPoints(bool renderPoints);

	void setRenderBoxes(bool renderBoxes);

private:
	SpriterEngine::SpriterModel *m_model;

	QString m_file;

	bool m_debug;
};

#endif // QSPRINTERMODEL_H
