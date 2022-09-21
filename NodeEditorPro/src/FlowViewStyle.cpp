﻿#include "FlowViewStyle.hpp"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>
#include <QtCore/QJsonArray>

#include <QDebug>

#include "StyleCollection.hpp"
#include "QJsonPhaser.hpp"

using QtNodes::FlowViewStyle;

inline void initResources() { Q_INIT_RESOURCE(resources); }

FlowViewStyle::
FlowViewStyle() :
	BackgroundColor(53, 53, 53, 255),
	FineGridColor(60, 60, 60, 255),
	CoarseGridColor(25, 25, 25, 255)
{
	loadJsonFile("DefaultStyle.json");
}


FlowViewStyle::
FlowViewStyle(QString jsonText)
{
	loadJsonFile(jsonText);
}


void
FlowViewStyle::
setStyle(QString jsonText)
{
	FlowViewStyle style(jsonText);

	StyleCollection::setFlowViewStyle(style);
}

void
FlowViewStyle::
loadJsonFile(QString styleFile)
{
	QJsonObject obj = QJsonPhaser::readJsonObj(styleFile);
	QJsonObject styleObj = obj["FlowViewStyle"].toObject();
	if (styleObj.isEmpty())
	{
		saveJsonFile(styleFile);
	}
	else
	{
		QJsonPhaser::convertFromJson(obj["FlowViewStyle"].toObject(), *this);
	}
}

void QtNodes::FlowViewStyle::saveJsonFile(QString fileName)
{
	QJsonObject obj = QJsonPhaser::readJsonObj(fileName);
	obj.insert("FlowViewStyle", QJsonPhaser::convertToJson(*this));
	QJsonPhaser::writeJsonObj(fileName, obj);
}

