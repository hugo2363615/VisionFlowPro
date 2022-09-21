﻿#include "HImageLoaderModel.hpp"
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>

HImageLoaderModel::HImageLoaderModel()
{
	m_image_view = new HImageViewWidget();
	m_image_view->installEventFilter(this);
	m_image_view->resize(200, 200);
	m_hImageData = std::make_shared<HImageData>();
}

unsigned int HImageLoaderModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
	case PortType::In:
		result = 0;
		break;

	case PortType::Out:
		result = 1;

	default:
		break;
	}

	return result;
}

QJsonObject HImageLoaderModel::save() const
{
	QJsonObject modelJson = NodeDataModel::save();
	if (m_hImageData)
	{
		HTuple* R, * G, * B;
		HTuple width, height;
		int chanels = m_hImageData->hImage()->CountChannels();
		m_hImageData->hImage()->GetImageSize(&width, &height);
	}

	return modelJson;
}

void HImageLoaderModel::restore(QJsonObject const& p)
{
	QJsonValue val_img = p["imagedata"];
	QJsonValue val_width = p["width"];
	QJsonValue val_height = p["chanel"];

}

bool HImageLoaderModel::eventFilter(QObject* object, QEvent* event)
{
	if (object == m_image_view)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			imageName =
				QFileDialog::getOpenFileName(nullptr,
					tr("Open Image"),
					QDir::homePath(),
					tr("Image Files (*.png *.jpg *.bmp)"));
			if (imageName == "")
			{
				return false;
			}

			HImage tmpImg;
			tmpImg.ReadImage(imageName.toStdString().c_str());
			m_hImageData->setHImage(tmpImg);
			m_image_view->showImage(*m_hImageData->hImage());

			Q_EMIT dataUpdated(0);

			return true;
		}
		else if (event->type() == QEvent::Resize)
		{

		}
	}

	return false;
}

NodeDataType
HImageLoaderModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}
std::shared_ptr<NodeData>
HImageLoaderModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<HImageData>(m_hImageData);
}
