#include "StartingWindow.h"

#include "IClientSDK.h"

#include <QtWidgets/QApplication>

#include <memory>

std::string SERVER_ADDRESS = "localhost";//"192.168.2.96";
unsigned short PORT = 53000;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if (argc == 3)
	{
		SERVER_ADDRESS = argv[1];
		PORT = static_cast<unsigned short>(std::stoi(argv[2]));
	}

	IClientSDKPtr sdk = IClientSDK::Create(SERVER_ADDRESS, PORT);

	auto startingWindow = std::make_shared<StartingWindow>(sdk);

	startingWindow->show();

    return a.exec();
}
