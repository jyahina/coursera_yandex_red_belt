#include "airline_ticket.h"
#include <sstream>
#include "test_runner.h"

using namespace std;

#define UPDATE_FIELD(ticket, field, values)  {auto it = values.find(#field); \
if (it != values.end()) { istringstream is(it->second); is >> ticket.field; }}

ostream& operator << (ostream& os, const Date& d)
{
	//"2018-2-28"
	os << d.year << "-" << d.month << "-" << d.day << endl;
	return os;
}

ostream& operator << (ostream& os, const Time& t)
{
	//"20:33"
	os << t.hours << ":" << t.minutes << endl;
	return os;
}

istream& operator>>(istream& is, Date& d) {
	int year;
	int month;
	int day;
	is >> year;
	is.ignore(1);
	is >> month;
	is.ignore(1);
	is >> day;
	d.year = year;
	d.month = month;
	d.day = day;
	return is;
}

istream& operator>>(istream& is, Time& t) {
	int hours;
	int minutes;
	is >> hours;
	is.ignore(1);
	is >> minutes;
	t.hours = hours;
	t.minutes = minutes;
	return is;
}

bool operator<(const Date& date1, const Date& date2)
{
	return (date1.year < date2.year
		|| (date1.year == date2.year && date1.month < date2.month)
		|| (date1.year == date2.year && date1.month == date2.month && date1.day < date2.day));
}

bool operator>(const Date& date1, const Date& date2)
{
	return !(date1 < date2);
}

bool operator==(const Date& date1, const Date& date2)
{
	return date1.year == date2.year && date1.month == date2.month && date1.day == date2.day;
}

bool operator==(const Date& date1, const string& date2)
{
	return date1.year == std::stoi(date2.substr(0, 4))
		&& date1.month == std::stoi(date2.substr(5, 6))
		&& date1.day == std::stoi(date2.substr(7, 9));
}

bool operator!=(const Date& date1, const string& date2)
{
	return !(date1 == date2);
}


bool operator!=(const Date& date1, const Date& date2)
{
	return !(date1 == date2);
}

bool operator<(const Time& time1, const Time& time2)
{
	return (time1.hours < time2.hours) || (time1.hours == time2.hours && time1.minutes < time2.minutes);
}

bool operator>(const Time& time1, const Time& time2)
{
	return !(time1 < time2);
}

bool operator==(const Time& time1, const Time& time2)
{
	return time1.hours == time2.hours && time1.minutes == time2.minutes;
}

bool operator!=(const Time& time1, const Time& time2)
{
	return !(time1 == time2);
}

bool operator==(const Time& time1, const string& time2)
{
	return time1.hours == std::stoi(time2.substr(0, 2)) && time1.minutes == std::stoi(time2.substr(3, 6));
}

bool operator!=(const Time& time1, const string& time2)
{
	return time1.hours != std::stoi(time2.substr(0, 2)) || time1.minutes != std::stoi(time2.substr(3, 6));
}

void TestUpdate() {
	AirlineTicket t;
	t.price = 0;

	const map<string, string> updates1 = {
	  {"departure_date", "2018-2-28"},
	  {"departure_time", "17:40"},
	};
	UPDATE_FIELD(t, departure_date, updates1);
	UPDATE_FIELD(t, departure_time, updates1);
	UPDATE_FIELD(t, price, updates1);

	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 0);

	const map<string, string> updates2 = {
	  {"price", "12550"},
	  {"arrival_time", "20:33"},
	};
	UPDATE_FIELD(t, departure_date, updates2);
	UPDATE_FIELD(t, departure_time, updates2);
	UPDATE_FIELD(t, arrival_time, updates2);
	UPDATE_FIELD(t, price, updates2);

	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 12550);
	ASSERT_EQUAL(t.arrival_time, (Time{ 20, 33 }));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestUpdate);
}