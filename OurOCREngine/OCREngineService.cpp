#include <QtCore/QCoreApplication>

#include "OCREngineService.h"

#include <fstream>
using namespace std;

extern void Start(OurOCREngine &engine);
extern ofstream out;
OCREngineService::OCREngineService(int argc, char **argv)
	: QtService<QCoreApplication>(argc, argv, "OCR Engine Serivce")
{
    setServiceDescription("socialhose.com's OCR Engine using Qt");
}

OCREngineService::~OCREngineService(void)
{
}

void OCREngineService::start()
{
	out << "Start of the service" << endl;
	//logMessage("Starting OCR Engine Service");

	QCoreApplication *app = application();
	if(!m_ocrEngine.Initalize())
	{
		//logMessage("Failed to start OCR Engine Service");
		out << "Failed to start OCR Engine Service" << endl;
		app->quit();
		return;
	}

	m_future = QtConcurrent::run( Start, m_ocrEngine);

	//logMessage("OCR Engine Service started");

	out << "Service started" << endl;
}
/*
void OCREngineService::pause()
{
	m_ocrEngine.Pause();
}
*/
void OCREngineService::stop()
{
	out << "************************ stopping the service ************************" << endl;
	//logMessage("Stopping OCR Engine Service");
	m_ocrEngine.Stop();
	m_future.waitForFinished();
	//logMessage("OCR Engine Service stopped");

	out << "************************ service stopped ****************************" << endl;
}
