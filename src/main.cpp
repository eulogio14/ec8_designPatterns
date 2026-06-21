#include <iostream>
#include "../include/expenses.h"

int main() {
    ExpenseList expenses = {
        {"2026-06-03", "transport", "Uber a UTEC", 25.50},
        {"2026-06-01", "food", "Chifa", 45.00},
        {"2026-06-04", "food", "Menu universitario", 15.00},
        {"2026-06-02", "books", "Libro de C++", 120.00}
    };

    sort_with(expenses, by_amount_descending);

    auto exporter = AuditedExporter{
        SummaryExporter{
            CsvExporter{}
        }
    };

    std::string output = exporter.export_expenses(expenses);

    std::cout << "=== EXPORTACION DE GASTOS ===\n";
    std::cout<<output<<std::endl;
    return 0;
}