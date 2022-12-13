//#include "OCREngineService.h"
#include <QtCore/QCoreApplication>

#include "OurOCREngine.h"


int main(int argc, char *argv[])
{
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8") );
	QCoreApplication a(argc, argv);

	//OCREngineService service(argc, argv);
	OurOCREngine m_ocrEngine;

	m_ocrEngine.Initalize();
	Start(m_ocrEngine);

    return a.exec();
}
