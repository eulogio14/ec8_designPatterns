//
// Created by rodri on 21/06/2026.
//

#ifndef EC8_DESIGNPATTERNS_EXPENSES_H
#define EC8_DESIGNPATTERNS_EXPENSES_H

#include <string>
#include <vector>
#include <functional>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <sstream>
#include <iomanip>

struct Expense {
    std::string date;
    std::string category;
    std::string detail;
    double amount{};

};
using ExpenseList = std::vector<Expense>;

template <class T>
concept ExpenseExporter = requires(T exporter, const ExpenseList& expenses){
    {exporter.export_expenses(expenses)} -> std::same_as<std::string>;
};

using ExportFunction = std::function<std::string(const ExpenseList&)>;

void sort_with(auto &expenses, auto criterio) {
    std::ranges::sort(expenses, criterio);
}

inline  auto by_amount_descending = [](const Expense& a, const Expense& b) {
    return a.amount > b.amount;
};

inline auto by_category_then_date = [](const Expense& a, const Expense& b) {
    if (a.category == b.category) return a.date < b.date;
    return a.category < b.category;
};

struct CsvExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream out;
        out<<"date, category, detail, amount\n";
        for (const auto& e: expenses) {
            out<<e.date<<", "<<e.category<<", "<<e.detail<<", "<<e.amount<<"\n";
        }
        return out.str();
    }
};

struct JsonExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream out;
        out<<"[\n";
        for (size_t i = 0; i<expenses.size(); ++i) {
            out<<" { \"date\": \"" <<expenses[i].date<<"\", "
                << "\"category\": \""<<expenses[i].category<<"\", "
            << "\"detail\": \""<<expenses[i].detail<<"\", "
            << "\"amount\": \""<<expenses[i].amount<<"\"};\n";
            if (i + 1 < expenses.size()) out << ",";
            out <<"\n";
        }
        out<<"]\n";
        return out.str();
    }
};

struct TextExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream out;
        for (const auto& e : expenses) {
            // Formato: date | category | detail | amount
            out << e.date << " | " << e.category << " | " << e.detail << " | " << e.amount << "\n";
        }
        return out.str();
    }
};

//factory method
template<ExpenseExporter E, class... Args>
ExportFunction make_exporter(Args&&... args) {
    return [exporter = E{std::forward<Args>(args)...}](const ExpenseList& expenses) {
        return exporter.export_expenses(expenses);
    };
}

//patron decorator

template<ExpenseExporter Inner>
struct  SummaryExporter {
    Inner inner;
    std::string export_expenses(const ExpenseList& expenses) const {
        double total = 0;
        for (const auto& e: expenses) total += e.amount;

        std::ostringstream out;
        out << inner.export_expenses(expenses);
        out << "\n[total: "<< std::fixed<<std::setprecision(2) << total << "]\n";
        return out.str();
    }
};

template<ExpenseExporter Inner>
struct AuditedExporter {
    Inner inner;

    std::string export_expenses(const ExpenseList& expenses) const {
        return inner.export_expenses(expenses) +
                "\n[audit: exported " + std::to_string(expenses.size()) + " items]\n";
    }
};
#endif //EC8_DESIGNPATTERNS_EXPENSES_H