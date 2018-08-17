#include "taichigame.h"
#include "registerwidget.h"
#include <windows.h>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	RegisterWidget registerWidget;
	TaichiGame w;

	std::array<int, 4> cpui;    //#include <array>
	// Calling __cpuid with 0x0 as the function_id argument  
	// gets the number of the highest valid function ID.  ʲô��function ID��
	__cpuid(cpui.data(), 0x0);    //cpui[0] = "funcition_id�����ֵ"
	int nIds_ = cpui[0];
	std::vector<std::array<int, 4>> data_;  //���������������cpui��ֵ    
	for (int i = 0; i <= nIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		data_.push_back(cpui);
	}
	//reinterpret_cast<int*>(vendor) //*reinterpret_cast<int*>(vendor)
	//����0 0+4 0+8��ֵ������CPUоƬ������
	char vendor[0x20] = { 0 };
	*reinterpret_cast<int*>(vendor) = data_[0][1];
	*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
	*reinterpret_cast<int*>(vendor + 8) = data_[0][2];  // vendor="GenuineIntel"    
	std::string vendor_ = vendor;
	bool isIntel_ = false;
	bool isAMD = false;
	if ("GenuineIntel" == vendor_)
	{
		isIntel_ = true;    //����ΪINTEL
	}
	else if ("AuthenticAMD" == vendor_)
	{
		isAMD = true;       //����ΪAMD
	}
	char vendor_serialnumber[0x14] = { 0 };
	sprintf_s(vendor_serialnumber, sizeof(vendor_serialnumber), "%08X%08X", data_[1][3], data_[1][0]);
	//�á�wmic cpu get processorid����ȡ�Ľ�����бȶԣ����Ӧ����һ�µġ�    //"BFEBFBFF000906E9"	

	QByteArray mima = QCryptographicHash::hash(vendor_serialnumber, QCryptographicHash::Md5);
	QString hashCode = QString(mima.toHex()).toUpper();
	QString password = hashCode.left(20);

	QFile file("./Licence/Licence.txt");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray line = file.readLine();
	QString str(line);
	file.close();
	if (str == password)
		w.show();
	else
		registerWidget.show();

	QObject::connect(&registerWidget, SIGNAL(nextButtonClicked()), &w, SLOT(show()));

	return a.exec();
}