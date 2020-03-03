#include <iostream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <functional>
#include <numeric>
#include <queue>
using namespace std;

class HotelBookingManager {
public:
	HotelBookingManager() {}

	void Book(int64_t time, string hotel_name, int client_id, int room_count)
	{
		hotels_reservations[hotel_name].push(Reservation{ time, hotel_name, client_id, room_count });
		client_booking_counts[hotel_name][client_id] += room_count;
		removeResevationByCurrentTime(time);
	}

	int Clients(string hotel_name)
	{
		return client_booking_counts[hotel_name].size();
	}

	int Rooms(string hotel_name)
	{
		int rooms = 0;
		for (auto info : client_booking_counts[hotel_name])
		{
			rooms += info.second;
		}

		return rooms;
	}

private:
	static const int MAX_CLIENTS_COUNT_ = 1'000'000'000;
	static const int MAX_ROOMS_COUNT_ = 1'000;
	static const int TIME_WINDOW_SIZE = 86'400;
	struct Reservation
	{
		int64_t time;
		string hotel_name;
		int client_id;
		int room_count;
	};
	map<string, map<int, int>> client_booking_counts;
	map<string, queue<Reservation>> hotels_reservations;

	void removeResevationByCurrentTime(int64_t current_time)
	{
		for (auto& hotel : hotels_reservations)
		{
			while (!hotel.second.empty() && current_time - hotel.second.front().time >= TIME_WINDOW_SIZE)
			{
				auto info = hotel.second.front();
				client_booking_counts[info.hotel_name][info.client_id] -= info.room_count;
				hotel.second.pop();
			}
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