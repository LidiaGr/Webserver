//
//  dataAndTimeCreator.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/16/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "main.hpp"

struct Date {
	long year;
	long month;
	long day;
	long hour;
	long minute;
	long second;
	long week;
};

std::string getCurrentDate() {
	Date currDate;
	std::map<int, int> daysInMonth;
	daysInMonth[1] = 31;
	daysInMonth[2] = 28;
	daysInMonth[3] = 31;
	daysInMonth[4] = 30;
	daysInMonth[5] = 31;
	daysInMonth[6] = 30;
	daysInMonth[7] = 31;
	daysInMonth[8] = 31;
	daysInMonth[9] = 30;
	daysInMonth[10] = 31;
	daysInMonth[11] = 30;
	daysInMonth[12] = 31;
	
	std::string res;
	struct timeval time;
	time_t t;
	gettimeofday(&time, DST_NONE);
	t = time.tv_sec;
	
	//here +3 GMT
	t = t + (3600 * 3);
	
	currDate.year = t / 31436000 + 1970;
	long leapYears = (currDate.year - 1970) / 4;
	long days = t / 86400 - leapYears;
	days = days % 365;

	std::map<int, int>::iterator it = daysInMonth.begin();
	std::map<int, int>::iterator ite = daysInMonth.end();
	for (; it != ite; it++) {
		if (days <= (*it).second) {
			currDate.day = days;
			currDate.month = (*it).first;
			break;
		}
		days -= (*it).second;
	}
	long seconds =  t - (t / 86400 * 86400);
	currDate.hour = seconds / 3600;
	long minutes = seconds - (currDate.hour * 3600);
	currDate.minute = minutes/60;
	currDate.second = minutes - (currDate.minute * 60);
	long codeMonth = 0;
	if (currDate.month == 1 || currDate.month == 10)
		codeMonth = 1;
	else if (currDate.month == 5)
		codeMonth = 2;
	else if (currDate.month == 8)
		codeMonth = 3;
	else if (currDate.month == 2 || currDate.month == 3 || currDate.month == 11)
		codeMonth = 4;
	else if (currDate.month == 6)
		codeMonth = 5;
	else if (currDate.month == 12 || currDate.month == 9)
		codeMonth = 6;
	long coef = currDate.year % 100;
	long codeYear = (6 + coef + coef / 4 ) % 7;
	currDate.week = (currDate.day + codeMonth + codeYear) % 7;
	std::map<long, std::string> daysWeek;
	daysWeek[0] = "Sat";
	daysWeek[1] = "Sun";
	daysWeek[2] = "Mon";
	daysWeek[3] = "Tue";
	daysWeek[4] = "Wed";
	daysWeek[5] = "Thu";
	daysWeek[6] = "Fri";
	
	std::map<long, std::string> daysMonth;
	daysMonth[1] = "Jan";
	daysMonth[2] = "Feb";
	daysMonth[3] = "Mar";
	daysMonth[4] = "Apr";
	daysMonth[5] = "May";
	daysMonth[6] = "Jun";
	daysMonth[7] = "Jul";
	daysMonth[8] = "Aug";
	daysMonth[9] = "Sep";
	daysMonth[10] = "Oct";
	daysMonth[11] = "Nov";
	daysMonth[12] = "Dec";
	
	res += daysWeek[currDate.week];
	res += ", ";
	if (currDate.day < 10)
		res += "0";
	res += intToString(int(currDate.day));
	res += " ";
	res += daysMonth[currDate.month];
	res += " ";
	res += intToString(int(currDate.year));
	res += " ";
	if (currDate.hour < 10)
		res += "0";
	res += intToString(int(currDate.hour));
	res += ":";
	if (currDate.minute < 10)
		res += "0";
	res += intToString(int(currDate.minute));
	res += ":";
	if (currDate.second < 10)
		res += "0";
	res += intToString(int(currDate.second));
	res += " GMT";
	return res;
}
