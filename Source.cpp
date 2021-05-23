#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <Windows.h>
#include <typeinfo>
typedef int Date[3];

using namespace std;
//�������� �� ���� b � ������ �� ���� �
bool smaller(Date a, Date b)
{
	if (a[2] < b[2]) return 1; // ��������� ����
	if (a[2] == b[2] && a[1] < b[1]) return 1; // ��������� �����
	if (a[2] == b[2] && a[1] == b[1] && a[0] < b[0]) return 1; // ��������� ��
	return 0;
}
bool DateBetween(Date first, Date ToCompare, Date last)
{
	return (smaller(first, ToCompare) && smaller(ToCompare, last));
}
///////////////				ROOM			/////////////////

class additionalService
{
public:
	string name;
	double pricePerDay;
	double pricePerHour;
	bool isIncluded;

};

class Client
{
public:
	Client() : name(""), surname("") {};
	string name;
	string surname;
	
};

class Company
{
	string name;
};
class Booking
{
public:
	
	Booking() {};
	bool IsCanceled;
	Date BookedFrom;
	Date BookedTo;
	Client client;

};

class room
{
public:
	

};

//class food_room : public room
//{
//
//};
//
//class entertainment_room: public room
//{
//
//};

class room_in_hotel : public room // ��� �볺���
{
public:
	size_t roomNumber;
	size_t capacity;// ������   
	size_t pricePerDay;
	room_in_hotel();
	room_in_hotel(size_t roomNumber, size_t capacity, size_t pricePerDay);
	vector<Booking*> _bookingHistory;

};
room_in_hotel::room_in_hotel() 
{
	roomNumber = capacity  = pricePerDay  =  0;
}

room_in_hotel::room_in_hotel(size_t roomNumber, size_t capacity, size_t pricePerDay) :
	roomNumber(roomNumber), capacity(capacity), pricePerDay(pricePerDay) {}


////////////////////////////					STOREY				///

class storey
{
public:
	storey() { storeyNumber = 0; }
	storey(size_t corpsNumber, size_t storeyNumber, vector<room_in_hotel*> _rooms) : 
	 storeyNumber(storeyNumber), _rooms(_rooms){}
 	size_t storeyNumber;
	vector<room_in_hotel*> _rooms;
private:


};

/////////////												CORPS							//
class corps 
{
public:
	corps();
	corps(size_t star_rating, size_t corps_number, vector<storey*>& _storeys);
	size_t corps_number;
	size_t star_rating;
	vector<storey*> _storeys;
	vector<additionalService*> _additionalServices;
};

corps::corps()
{
	star_rating = corps_number = 0;
	_storeys = {};
}

corps::corps(size_t star_rating, size_t corps_number, vector<storey*>& _storeys)
{
	this->star_rating = star_rating;
	this->_storeys = _storeys;
	this->corps_number = corps_number;
}

////////////////		HOTEL_COMPLEX						//

class hotel_complex
{
public:
	vector<corps*> _corps;
	hotel_complex()	
	{
		_corps = {};
	}
	hotel_complex(vector<corps*> _corps): _corps(_corps){}
	~hotel_complex() = default;

};

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��� ��������� ����� � ������ �� �������, ����� ���� �������� ������� ����� �� ��, ��� �������
template<typename T>
void addToVector(std::vector<T*>& vc, int positionOfData, T* data)
{
	if (positionOfData > vc.size())
		vc.resize(positionOfData);
	vc[positionOfData - 1] = data;
}

// ����������� ������� �� ��������� �����
void addingNewCorps(const vector<string>& tempVector, hotel_complex* HC, corps* currentCorps, storey* currentStorey, room_in_hotel* currentRoom) // ����� ������
{
	currentCorps->corps_number = stoi(tempVector[0]);
	currentCorps->star_rating = stoi(tempVector[1]);
	currentStorey->storeyNumber = stoi(tempVector[2]);
	currentRoom->roomNumber = stoi(tempVector[3]);
	currentRoom->capacity = stoi(tempVector[4]);
	currentRoom->pricePerDay = stoi(tempVector[5]);

	HC->_corps.push_back(currentCorps);
	currentCorps->_storeys.push_back(currentStorey);
	currentStorey->_rooms.push_back(currentRoom);
	
	/*addToVector(HC->_corps, currentCorps->corps_number, currentCorps);
	addToVector(currentCorps->_storeys, currentStorey->storeyNumber, currentStorey);
	addToVector(currentStorey->_rooms, currentRoom->roomNumber, currentRoom);*/
}

void addingNewStorey(const vector<string>& tempVector, hotel_complex* HC, corps* currentCorps, storey* currentStorey, room_in_hotel* currentRoom) // ����� ������
{
	currentStorey->storeyNumber = stoi(tempVector[2]);	
	currentRoom->roomNumber = stoi(tempVector[3]);
	currentRoom->capacity = stoi(tempVector[4]);
	currentRoom->pricePerDay = stoi(tempVector[5]);

	currentCorps->_storeys.push_back(currentStorey);
	currentStorey->_rooms.push_back(currentRoom);
	/*addToVector(currentCorps->_storeys, currentStorey->storeyNumber, currentStorey);
	addToVector(currentStorey->_rooms, currentRoom->roomNumber, currentRoom);*/
}

void addingNewRoom(const vector<string>& tempVector, hotel_complex* HC, corps* currentCorps, storey* currentStorey, room_in_hotel* currentRoom)// ���� ������
{
	currentRoom->roomNumber = stoi(tempVector[3]);
	currentRoom->capacity = stoi(tempVector[4]);
	currentRoom->pricePerDay = stoi(tempVector[5]);

	currentStorey->_rooms.push_back(currentRoom);
	//addToVector(currentStorey->_rooms, currentRoom->roomNumber, currentRoom);
}


void InitComplexFromFile(hotel_complex* HC, string pathToTheFile) //������������� ����� � ����� � ��������� ����� 
{
	ifstream outData;
	string DataFromFile;
	vector<string> tempVector; // ��� ����� � ����� 
	outData.open(pathToTheFile);
	if (outData.is_open())
	{
		getline(outData, DataFromFile);
		string delim = ("	");
		size_t prev = 0;
		size_t next;
		size_t delta = delim.length();
		while ((next = DataFromFile.find(delim, prev)) != string::npos) {
			tempVector.push_back(DataFromFile.substr(prev, next - prev));
			prev = next + delta;
		}
		tempVector.push_back(DataFromFile.substr(prev));

		corps* currentCorps = new corps; // ������ �� ������ ������ � �����
		storey* currentStorey = new storey;// ������ ������ � �����
		room_in_hotel* currentRoom = new room_in_hotel; // ����� ������ � �����

		addingNewCorps(tempVector, HC, currentCorps, currentStorey, currentRoom);
		tempVector.clear();

		while (!outData.eof())
		{
			getline(outData, DataFromFile);
			string delim = ("	");
			size_t prev = 0;
			size_t next;
			size_t delta = delim.length();
			while ((next = DataFromFile.find(delim, prev)) != string::npos)
			{
				tempVector.push_back(DataFromFile.substr(prev, next - prev));
				prev = next + delta;
			}
			tempVector.push_back(DataFromFile.substr(prev));
			if (tempVector.size() < 6)
				break;



			if (stoi(tempVector[0]) != currentCorps->corps_number)// ����� ������
			{
				currentCorps = new corps;
				currentStorey = new storey;
				currentRoom = new room_in_hotel;
				addingNewCorps(tempVector, HC, currentCorps, currentStorey, currentRoom);
			}
			else
			{
				if (stoi(tempVector[2]) != currentStorey->storeyNumber)//����� ������
				{
					currentStorey = new storey;
					currentRoom = new room_in_hotel;
					addingNewStorey(tempVector, HC, currentCorps, currentStorey, currentRoom);
				}
				else
				{
					currentRoom = new room_in_hotel;
					addingNewRoom(tempVector, HC, currentCorps, currentStorey, currentRoom);// ���� ������
				}
			}
				
			tempVector.clear();
		}
		//����� ����� � 
	}
	else
		cout << "������� �i� ��� �i��������� �����!" << endl;
}

// ������� ���'��  /////
void DeleteHotelComplex(hotel_complex* HC)
{
	if (HC != nullptr)
	{
		for (auto& corpsDel : HC->_corps)
		{
			if (corpsDel != nullptr)
			{
				for (auto& storeyDel : corpsDel->_storeys)
				{
					if (storeyDel != nullptr)
					{
						for (auto& roomDel : storeyDel->_rooms)
						{
							if (roomDel != nullptr)
							{
								for (auto& bookingdel : roomDel->_bookingHistory)
								{
									if (bookingdel != nullptr)
									{
										delete bookingdel;
										bookingdel = nullptr;
									}
								}
								delete roomDel;
								roomDel = nullptr;
							}
						}
						delete storeyDel;
						storeyDel = nullptr;
					}
				}
				delete corpsDel;
				corpsDel = nullptr;
			}
		}
		delete HC;
		HC = nullptr;
	}
}


void InitHistoryOfRoom(hotel_complex* HC, string pathToClientsFile ="", string pathToOrganizationFile="")
{
	//��� ����� � �볺�����
	ifstream outDataClients;
	string DataFromFileClients;
	outDataClients.open(pathToClientsFile);
	if (outDataClients.is_open())
	{
		vector<string> tempVector;
		while (!outDataClients.eof())
		{
			getline(outDataClients, DataFromFileClients);
			
			string delim = ("	");
			size_t prev = 0;
			size_t next;
			size_t delta = delim.length();
			while ((next = DataFromFileClients.find(delim, prev)) != string::npos)
			{
				tempVector.push_back(DataFromFileClients.substr(prev, next - prev));
				prev = next + delta;
			}
			tempVector.push_back(DataFromFileClients.substr(prev));
			if (tempVector.size() < 7)
				break;
		
			for (corps* corps_el : HC->_corps)
			{
				if (corps_el->corps_number == stoi(tempVector[0]))
				{
					for (storey* storey_el : corps_el->_storeys)
					{
						for (room_in_hotel* room_el : storey_el->_rooms)
						{
							if (room_el->roomNumber == stoi(tempVector[1]))
							{
								Booking* bk = new Booking;
								bk->BookedFrom[0] = stoi(tempVector[2].substr(0,2));//���� ����������(��)
								bk->BookedFrom[1] = stoi(tempVector[2].substr(3,2));//����� ����������(��)
								bk->BookedFrom[2] = stoi(tempVector[2].substr(6));//�� ����������(��)
								
								bk->BookedTo[0] = stoi(tempVector[3].substr(0,2));//���� ����������(��)
								bk->BookedTo[1] = stoi(tempVector[3].substr(3,2));//����� ����������(��)
								bk->BookedTo[2] = stoi(tempVector[3].substr(6));//�� ����������(��)
								
								bk->client.surname = tempVector[4];
								bk->client.name = tempVector[5];
								if (tempVector[6] == "yes")
									bk->IsCanceled = true;
								else
									bk->IsCanceled = false;

								room_el->_bookingHistory.push_back(bk);
								goto tobreak;
							}
						}
					}
				}
			}
			tobreak:
			tempVector.clear();
		}
	}
	else
		cout << "������� �� ��� ���������� �����!" << endl;
	



}

void InitExtraService(string path)
{
	fstream f;
	string data;
	f.open(path);
	while (!f.eof())
	{
		getline(f, data);
		cout << data << endl;
	}

}


//				MAIN				///
int main()
{
	setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	hotel_complex* HC = new hotel_complex;
	string path = "C:\\Users\\�����\\Desktop\\�������\\�������(��)\\Project1\\Project1\\For data\\�������.txt";
	string path2 = "C:\\Users\\�����\\Desktop\\�������\\�������(��)\\Project1\\Project1\\For data\\����� ��� �볺���.txt";
	InitComplexFromFile(HC, path);
	InitHistoryOfRoom(HC, path2);
	string path3 = "C:/Users/�����/Desktop/�������/�������(��)/Project1/Project1/For data/������1���������������.txt";

	//vector<>
	DeleteHotelComplex(HC);// ������� ���'��  
	/*string path3 = "C:/Users/�����/Desktop/�������/�������(��)/Project1/Project1/For data/������1���������������.txt";
	extraservice(path3);*/
	/*string sr = typeid(cl).name();
	cout << sr << endl;
	if (sr == "class Client")
	{
		cout << "sdfsd" << endl;
	}*/
	int af;
	SetConsoleCP(866);
	return 0;
	

}
