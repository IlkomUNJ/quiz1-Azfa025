#ifndef STORE_QUERIES_H
#define STORE_QUERIES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include "store.h"
#include <cstdio>

// Helper: parse ISO date YYYY-MM-DD to time_t (start of day)
static time_t isoToTime(const std::string &iso) {
    std::tm tm{};
    if (iso.size() < 10) return 0;
    int y=0, m=0, d=0;
    if (sscanf(iso.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return 0;
    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d;
    tm.tm_hour = 0; tm.tm_min = 0; tm.tm_sec = 0; tm.tm_isdst = -1;
    return mktime(&tm);
}

// Helper: get ISO date string for daysAgo
static std::string isoDaysAgo(int days) {
    time_t now = time(nullptr);
    now -= (time_t)days * 24 * 60 * 60;
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return std::string(buf);
}

// 1) List transactions in last k days
inline void listTransactionsLastKDays() {
    std::cout << "Enter k (days): "; int k; std::cin >> k;
    std::string cutoff = isoDaysAgo(k);
    time_t tcut = isoToTime(cutoff);
    std::cout << "Transactions since " << cutoff << ":\n";
    for (const auto &inv : g_invoices) {
        if (isoToTime(inv.date) >= tcut) {
            std::cout << "Invoice " << inv.id << " | Date: " << inv.date << " | Buyer: " << inv.buyerId << " | Seller: " << inv.sellerId << " | Total: " << inv.totalAmount << " | Status: ";
            if (inv.status==InvoiceStatus::PAID) std::cout<<"PAID";
            else if (inv.status==InvoiceStatus::COMPLETE) std::cout<<"COMPLETE";
            else if (inv.status==InvoiceStatus::CANCELLED) std::cout<<"CANCELLED";
            else std::cout<<"PENDING";
            std::cout << std::endl;
        }
    }
}

// 2) List paid but not completed
inline void listPaidNotCompleted() {
    std::cout << "Paid but not completed orders:\n";
    for (const auto &inv : g_invoices) {
        if (inv.status == InvoiceStatus::PAID) {
            std::cout << "Invoice " << inv.id << " | Date: " << inv.date << " | Buyer: " << inv.buyerId << " | Seller: " << inv.sellerId << " | Total: " << inv.totalAmount << std::endl;
        }
    }
}

// 3) Most frequent item transactions (top m)
inline void listMostFrequentItems() {
    std::cout << "Enter m (top items): "; int m; std::cin >> m;
    std::map<int,int> counts; // itemId -> count
    std::map<int,std::string> names;
    for (const auto &inv : g_invoices) {
        for (const auto &it : inv.items) {
            counts[it.itemId] += it.quantity;
            names[it.itemId] = it.itemName;
        }
    }
    std::vector<std::pair<int,int>> vec;
    for (auto &p : counts) vec.push_back(p);
    std::sort(vec.begin(), vec.end(), [](auto &a, auto &b){ return a.second > b.second; });
    std::cout << "Top " << m << " items by quantity sold:\n";
    for (int i=0;i<std::min<int>(m, (int)vec.size()); ++i) {
        std::cout << i+1 << ". " << names[vec[i].first] << " (ID " << vec[i].first << ") - " << vec[i].second << " units" << std::endl;
    }
}

// 4) Most active buyers per day (in last k days)
inline void listMostActiveBuyersPerDay() {
    std::cout << "Enter days window (k): "; int k; std::cin >> k;
    std::string cutoff = isoDaysAgo(k);
    time_t tcut = isoToTime(cutoff);
    std::map<int,int> counts; // buyerId -> invoice count
    for (const auto &inv : g_invoices) {
        if (isoToTime(inv.date) >= tcut) counts[inv.buyerId]++;
    }
    std::vector<std::pair<int,int>> vec(counts.begin(), counts.end());
    std::sort(vec.begin(), vec.end(), [](auto &a, auto &b){ return a.second > b.second; });
    std::cout << "Most active buyers in last "<<k<<" days (by number of transactions):\n";
    for (size_t i=0;i<vec.size();++i) std::cout << i+1 << ". Buyer " << vec[i].first << " - " << vec[i].second << " transactions\n";
}

// 5) Most active sellers per day (in last k days)
inline void listMostActiveSellersPerDay() {
    std::cout << "Enter days window (k): "; int k; std::cin >> k;
    std::string cutoff = isoDaysAgo(k);
    time_t tcut = isoToTime(cutoff);
    std::map<int,int> counts; // sellerId -> invoice count
    for (const auto &inv : g_invoices) {
        if (isoToTime(inv.date) >= tcut) counts[inv.sellerId]++;
    }
    std::vector<std::pair<int,int>> vec(counts.begin(), counts.end());
    std::sort(vec.begin(), vec.end(), [](auto &a, auto &b){ return a.second > b.second; });
    std::cout << "Most active sellers in last "<<k<<" days (by number of transactions):\n";
    for (size_t i=0;i<vec.size();++i) std::cout << i+1 << ". Seller " << vec[i].first << " - " << vec[i].second << " transactions\n";
}

// 6) Most popular items per month (ask for YYYY-MM)
inline void listMostPopularItemsPerMonth() {
    std::cout << "Enter month (YYYY-MM): "; std::string month; std::cin >> month;
    std::map<int,int> counts; std::map<int,std::string> names;
    for (const auto &inv : g_invoices) {
        if (inv.date.rfind(month,0) == 0) {
            for (const auto &it : inv.items) { counts[it.itemId] += it.quantity; names[it.itemId] = it.itemName; }
        }
    }
    std::vector<std::pair<int,int>> vec;
    for (auto &p : counts) vec.push_back(p);
    std::sort(vec.begin(), vec.end(), [](auto &a, auto &b){ return a.second > b.second; });
    std::cout << "Most popular items in " << month << ":\n";
    for (size_t i=0;i<vec.size(); ++i) std::cout << i+1 << ". " << names[vec[i].first] << " (ID " << vec[i].first << ") - " << vec[i].second << " units\n";
}

// 7) Loyal customers per month (buyer with most transactions in month)
inline void listLoyalCustomersPerMonth() {
    std::cout << "Enter month (YYYY-MM): "; std::string month; std::cin >> month;
    std::map<int,int> counts;
    for (const auto &inv : g_invoices) {
        if (inv.date.rfind(month,0) == 0) counts[inv.buyerId]++;
    }
    if (counts.empty()) { std::cout << "No transactions in that month." << std::endl; return; }
    std::vector<std::pair<int,int>> vec(counts.begin(), counts.end());
    std::sort(vec.begin(), vec.end(), [](auto &a, auto &b){ return a.second > b.second; });
    std::cout << "Top loyal customers in " << month << ":\n";
    for (size_t i=0;i<std::min<size_t>(5, vec.size()); ++i) std::cout << i+1 << ". Buyer " << vec[i].first << " - " << vec[i].second << " transactions\n";
}

#endif // STORE_QUERIES_H
