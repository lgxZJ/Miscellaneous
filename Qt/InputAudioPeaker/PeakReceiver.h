#ifndef A_H
#define A_H

#include <QObject>
#include <iostream>

class PeakReceiver : public QObject
{
	Q_OBJECT
public slots:
	void peak(qint16 value){
		received = true;
	}

	void peakShow(qint16 value) {
		std::cout << value << " ";
	}

	void deviceUnplugged() {
		std::cout << "~~unplugged~~";
	}

public:
	bool received = false;
};

#endif // A_H
