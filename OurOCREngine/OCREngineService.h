#pragma once
#include "qtservice.h"

#include "OurOCREngine.h"

class OCREngineService :
	public QtService<QCoreApplication>
{
public:
	OCREngineService(int argc, char **argv);
	~OCREngineService(void);

protected:
    void start();
//  void pause();
    void stop();

private:
	OurOCREngine m_ocrEngine;
	QFuture<void> m_future;
};
