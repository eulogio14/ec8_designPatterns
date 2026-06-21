#include <iostream>
#include <cassert>
#include "../include/expenses.h"

void test_strategy_sort_by_amount_descending() {
    ExpenseList expenses = {{"","","", 10.0}, {"","","",50.0}};
    sort_with(expenses,by_amount_descending);
    assert(expenses.front().amount == 50.0);
}

void test_strategy_sort_by_category_then_date() {
    ExpenseList expenses ={
        {"2026-06-02", "food", "", 10.0},
        {"2026-06-01", "food", "", 10.0},
        {"2026-06-01", "books", "", 10.0}
    };
    sort_with(expenses, by_category_then_date);
    assert(expenses.front().category == "books");
    assert(expenses[1].date == "2026-06-01");
}

void test_factory_create_csv_exporter() {
    auto exporter = make_exporter<CsvExporter>();
    ExpenseList expenses = {{"2026-06-01", "test", "test", 10.0}};
    std::string out = exporter(expenses);
    assert(out.find("date, category, detail, amount") != std::string::npos);
}

void test_factory_create_json_exporter() {
    auto exporter = make_exporter<JsonExporter>();
    ExpenseList expenses = {{"2026-06-01", "test", "test", 10.0}};
    std::string out = exporter(expenses);
    assert(out.find("{ \"date\"") != std::string::npos);
}

void test_decorator_audit_preserves_inner_export() {
    auto exporter = AuditedExporter{TextExporter{}};
    ExpenseList expenses = {{"2026-06-01", "test", "test", 10.0}};
    std::string out = exporter.export_expenses(expenses);
    assert(out.find("audit: exported 1 items") != std::string::npos);
    assert(out.find("test | test | 10") != std::string::npos); // Preserva el formato interno
}

void test_decorator_summary_adds_total() {
    auto exporter = SummaryExporter{TextExporter{}};
    ExpenseList expenses = {{"", "", "", 10.0}, {"", "", "", 20.0}};
    std::string out = exporter.export_expenses(expenses);
    assert(out.find("[total: 30.00]") != std::string::npos);
}

void test_integration_export_sorted_expenses() {
    ExpenseList expenses = {
        {"2026-06-01", "food", "Chifa", 45.00},
        {"2026-06-02", "transport", "Bus", 5.00}
    };
    sort_with(expenses, by_amount_descending);

    auto exporter = AuditedExporter{ SummaryExporter{ CsvExporter{} } };
    auto output = exporter.export_expenses(expenses);

    assert(expenses.front().amount == 45.00);
    assert(output.find("date, category, detail, amount") != std::string::npos);
    assert(output.find("total") != std::string::npos);
    assert(output.find("audit") != std::string::npos);
}

int main() {
    test_strategy_sort_by_amount_descending();
    test_strategy_sort_by_category_then_date();
    test_factory_create_csv_exporter();
    test_factory_create_json_exporter();
    test_decorator_audit_preserves_inner_export();
    test_decorator_summary_adds_total();
    test_integration_export_sorted_expenses();

    std::cout << "Todos los tests pasaron exitosamente.\n";
    return 0;
}