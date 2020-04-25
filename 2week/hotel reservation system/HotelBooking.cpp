#include <iostream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <set>
#include <functional>
#include <numeric>
#include <queue>
using namespace std;

class HotelBookingManager {
public:
	HotelBookingManager() = default;

	void Book(int64_t time, const string& hotel_name, int client_id, int room_count)
	{
		auto& hotel = hotel_booking[hotel_name];
		hotel.reservation.push(Reservation{ time, client_id, room_count });
		hotel.clients_rooms[client_id] += room_count;
		hotel.bookedRooms += room_count;

		current_time = time;
	}

	int Clients(const string& hotel_name)
	{
		removeResevation(hotel_name);

		return hotel_booking[hotel_name].clients_rooms.size();
	}

	int Rooms(const string& hotel_name)
	{
		removeResevation(hotel_name);

		return hotel_booking[hotel_name].bookedRooms;
	}

private:

	const int TIME_WINDOW_SIZE = 86'400;

	struct Reservation
	{
		int64_t time;
		int client_id;
		int room_count;
	};

	struct infoHotel
	{
		queue<Reservation> reservation;
		map<int, int> clients_rooms;
		int bookedRooms = 0;
	};

	map<string, infoHotel> hotel_booking;
	int64_t current_time;

	void removeResevation(const string& hotel_name)
	{
		auto& info = hotel_booking[hotel_name];

		while (!info.reservation.empty() && current_time - info.reservation.front().time >= TIME_WINDOW_SIZE)
		{
			auto& reservation = info.reservation.front();
			auto& client_room = info.clients_rooms[reservation.client_id];
			client_room -= reservation.room_count;

			if (client_room <= 0) info.clients_rooms.erase(reservation.client_id);

			info.bookedRooms -= reservation.room_count;
			info.reservation.pop();

		}

	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	HotelBookingManager manager;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;

		cin >> query_type;

		if (query_type == "BOOK") {
			int64_t time; int client_id, room_count; string hotel_name;
			cin >> time >> hotel_name >> client_id >> room_count;

			manager.Book(time, hotel_name, client_id, room_count);
		}
		else if (query_type == "CLIENTS") {
			string hotel_name;	cin >> hotel_name;

			cout << manager.Clients(hotel_name) << "\n";
		}
		else if (query_type == "ROOMS") {
			string hotel_name;	cin >> hotel_name;

			cout << manager.Rooms(hotel_name) << "\n";
		}
	}

	return 0;
}