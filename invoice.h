#ifndef INVOICE_H
#define INVOICE_H

#include <vector>
#include <string>
#include "cart.h"

enum class InvoiceStatus { PENDING, PAID, CANCELLED };

struct InvoiceItem {
    int itemId;
    std::string itemName;
    int quantity;
    double unitPrice;
    double totalPrice() const { return unitPrice * quantity; }
};

struct Invoice {
    int id;
    int buyerId;
    int sellerId;
    std::vector<InvoiceItem> items;
    double totalAmount;
    InvoiceStatus status;
};

#endif
