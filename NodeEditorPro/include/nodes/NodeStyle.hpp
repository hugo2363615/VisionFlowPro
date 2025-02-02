﻿#pragma once

#include <QtGui/QColor>

#include "Export.hpp"
#include "Style.hpp"

namespace QtNodes
{

	class   NodeStyle : public Style
	{
	public:

		NodeStyle();

		NodeStyle(QString jsonText);

	public:

		static void setNodeStyle(QString fileName);

	private:

		void loadJsonFile(QString fileName) override;
		void saveJsonFile(QString fileName) override;

	public:

		QColor NormalBoundaryColor;
		QColor SelectedBoundaryColor;
		QColor BackgroundColor;
		QColor TitleColor;
		QColor GradientColor0;
		QColor GradientColor1;
		QColor GradientColor2;
		QColor GradientColor3;
		QColor ShadowColor;
		QColor FontColor;
		QColor FontColorFaded;

		QColor ConnectionPointColor;
		QColor FilledConnectionPointColor;

		QColor WarningColor;
		QColor ErrorColor;

		float PenWidth;
		float HoveredPenWidth;

		float ConnectionPointDiameter;

		float Opacity;
	};
}
