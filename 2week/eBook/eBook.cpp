#include <iostream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <functional>
#include <numeric>
using namespace std;

class ReadingManager {
public:
	ReadingManager()
		:pages_number_users(MAX_USER_COUNT_ + 1, 0)
	{}

	void Read(int user_id, int page_count) {
		if (user_page_counts.count(user_id) > 0) {
			pages_number_users[user_page_counts[user_id]]--; // удаляем пользователя со старой страницы
		}

		user_page_counts[user_id] = page_count;
		pages_number_users[page_count]++;
	}

	double Cheer(int user_id) const {
		if (user_page_counts.count(user_id) == 0) return 0.;
		if (user_page_counts.size() == 0) return 0.;
		if (user_page_counts.at(user_id) == 0) return 0.;
		if (user_page_counts.size() == 1) return 1.;

		auto user_count = user_page_counts.size();
		auto position = std::accumulate(pages_number_users.begin(), pages_number_users.begin() + user_page_counts.at(user_id), 0);

		return position * 1.0 /static_cast<double>(user_count - 1);
	}

private:
	static const int MAX_USER_COUNT_ = 100'000;

	map<int, int> user_page_counts; // пользватели и страница до которой каждый из них прочитал
	vector<int> pages_number_users; // страницы и сколько пользователей читает их
	
};


int main() {
	// Для ускорения чтения данных отключается синхронизация
	 // cin и cout с stdio,
	 // а также выполняется отвязка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReadingManager manager;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		int user_id;
		cin >> user_id;

		if (query_type == "READ") {
			int page_count;
			cin >> page_count;
			manager.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}

	return 0;
}