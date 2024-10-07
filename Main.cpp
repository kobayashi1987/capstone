////
//// Created by jack on 5/10/24.
////
//// main.cpp
//#include "Customer.h"
//#include <iostream>
//#include <vector>
//#include <fstream>
//
//void addCustomer(std::vector<Customer>& customers, int& customerIDCounter);
//void saveCustomersToFile(const std::vector<Customer>& customers, const std::string& filename);
//
//int main() {
//    std::vector<Customer> customers;
//    int customerIDCounter = 1; // Unique customer ID counter
//    char choice;
//
//    std::cout << "=== Travel Application Customer Data Entry ===\n";
//
//    do {
//        addCustomer(customers, customerIDCounter);
//
//        std::cout << "Do you want to add another customer? (y/n): ";
//        std::cin >> choice;
//        std::cin.ignore(); // Clear the newline character from the input buffer
//    } while (choice == 'y' || choice == 'Y');
//
//    // Display all customers
//    std::cout << "\n=== Customer List ===\n";
//    for (const auto& customer : customers) {
//        customer.displayCustomerInfo();
//        std::cout << "----------------------\n";
//    }
//
//    // Save customers to file
//    saveCustomersToFile(customers, "customers_data.txt");
//
//    std::cout << "Customer data saved to 'customers_data.txt'\n";
//    return 0;
//}
//
//void addCustomer(std::vector<Customer>& customers, int& customerIDCounter) {
//    std::string firstName, lastName, email, phoneNumber, passportNumber;
//
//    std::cin.ignore(); // Clear the newline character from the previous input
//
//    std::cout << "\nEnter First Name: ";
//    std::getline(std::cin, firstName);
//
//    std::cout << "Enter Last Name: ";
//    std::getline(std::cin, lastName);
//
//    std::cout << "Enter Email: ";
//    std::getline(std::cin, email);
//
//    std::cout << "Enter Phone Number: ";
//    std::getline(std::cin, phoneNumber);
//
//    std::cout << "Enter Passport Number: ";
//    std::getline(std::cin, passportNumber);
//
//    Customer newCustomer(customerIDCounter++, firstName, lastName, email, phoneNumber, passportNumber);
//    customers.push_back(newCustomer);
//
//    std::cout << "Customer added successfully!\n";
//}
//
//void saveCustomersToFile(const std::vector<Customer>& customers, const std::string& filename) {
//    std::ofstream outFile(filename);
//
//    if (!outFile) {
//        std::cerr << "Error: Unable to open file for writing.\n";
//        return;
//    }
//
//    // Write header
//    outFile << "CustomerID,FirstName,LastName,Email,PhoneNumber,PassportNumber\n";
//
//    // Write customer data
//    for (const auto& customer : customers) {
//        outFile << customer.serialize() << "\n";
//    }
//
//    outFile.close();
//}