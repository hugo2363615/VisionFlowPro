﻿#include "HImageFolderModel.hpp"
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>

HImageFolderModel::HImageFolderModel()
{
	m_image_view = new HImageViewWidget();
	m_image_view->resize(200, 200);
	m_paraWidget = new QWidget();
	m_paraWidget->resize(200, 250);
	m_host = new QVBoxLayout();
	m_h_host = new QHBoxLayout();
	m_paraWidget->setLayout(m_host);
	btn_selectFolder = new QPushButton(u8"选择目录");
	btn_last = new QPushButton(u8"上一张");
	btn_next = new QPushButton(u8"下一张");

	btn_selectFolder->setFixedHeight(25);
	btn_last->setFixedHeight(25);
	btn_next->setFixedHeight(25);
	m_h_host->addWidget(btn_last);
	m_h_host->addWidget(btn_next);

	m_host->addWidget(btn_selectFolder);
	m_host->addLayout(m_h_host);
	m_host->addWidget(m_image_view);

	btn_last->installEventFilter(this);
	btn_next->installEventFilter(this);
	btn_selectFolder->installEventFilter(this);

	m_hImageData = std::make_shared<HImageData>();
}

unsigned int HImageFolderModel::nPorts(PortType portType) const
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

bool HImageFolderModel::eventFilter(QObject* object, QEvent* event)
{
	if (object == btn_selectFolder)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			folderPath =
				QFileDialog::getExistingDirectory(nullptr,
					tr("Select Folder"),
					QDir::homePath());
			if (folderPath == "")
			{
				return false;
			}
			HalconCpp::ListFiles(folderPath.toStdString().c_str(), "files", &fileListStr);
			TupleRegexpSelect(fileListStr, "\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima|hobj)$", &imgListStr);
			imageCounst = imgListStr.Length();
			if (imageCounst == 0)
			{
				return false;
			}
			curIndex = 0;
			tmpImg.ReadImage(imgListStr[curIndex].ToTuple());
			m_hImageData->setHImage(tmpImg);
			m_image_view->showImage(*m_hImageData->hImage());
			Q_EMIT dataUpdated(0);

			return true;
		}
		else if (event->type() == QEvent::Resize)
		{

		}
	}
	else if (object == btn_last)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (curIndex - 1 >= 0)
			{
				curIndex--;
				tmpImg.ReadImage(imgListStr[curIndex].ToTuple());
				m_hImageData->setHImage(tmpImg);
				m_image_view->showImage(*m_hImageData->hImage());
				Q_EMIT dataUpdated(0);
			}
			else
			{
				curIndex = imageCounst - 1;
			}
		}
	}
	else if (object == btn_next)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (curIndex + 1 < imageCounst)
			{
				curIndex++;
				tmpImg.ReadImage(imgListStr[curIndex].ToTuple());
				m_hImageData->setHImage(tmpImg);
				m_image_view->showImage(*m_hImageData->hImage());
				Q_EMIT dataUpdated(0);
			}
			else
			{
				curIndex = 0;
			}
		}
	}
	return false;
}

NodeDataType
HImageFolderModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}
std::shared_ptr<NodeData>
HImageFolderModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<HImageData>(m_hImageData);
}
