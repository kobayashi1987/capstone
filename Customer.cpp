//// Customer.cpp
//#include "Customer.h"
//#include <iostream>
//#include <sstream>
//
//// Constructor
//Customer::Customer(int id, const std::string& fName, const std::string& lName,
//                   const std::string& mail, const std::string& phone, const std::string& passport)
//        : customerID(id), firstName(fName), lastName(lName), email(mail),
//          phoneNumber(phone), passportNumber(passport) {}
//
//// Getters
//int Customer::getCustomerID() const { return customerID; }
//std::string Customer::getFirstName() const { return firstName; }
//std::string Customer::getLastName() const { return lastName; }
//std::string Customer::getEmail() const { return email; }
//std::string Customer::getPhoneNumber() const { return phoneNumber; }
//std::string Customer::getPassportNumber() const { return passportNumber; }
//
//// Setters
//void Customer::setFirstName(const std::string& fName) { firstName = fName; }
//void Customer::setLastName(const std::string& lName) { lastName = lName; }
//void Customer::setEmail(const std::string& mail) { email = mail; }
//void Customer::setPhoneNumber(const std::string& phone) { phoneNumber = phone; }
//void Customer::setPassportNumber(const std::string& passport) { passportNumber = passport; }
//
//// Display customer information
//void Customer::displayCustomerInfo() const {
//    std::cout << "Customer ID: " << customerID << std::endl;
//    std::cout << "Name: " << firstName << " " << lastName << std::endl;
//    std::cout << "Email: " << email << std::endl;
//    std::cout << "Phone Number: " << phoneNumber << std::endl;
//    std::cout << "Passport Number: " << passportNumber << std::endl;
//}
//
//// Serialize customer data
//std::string Customer::serialize() const {
//    std::ostringstream oss;
//    oss << customerID << "," << firstName << "," << lastName << ","
//        << email << "," << phoneNumber << "," << passportNumber;
//    return oss.str();
//}
//
//// Deserialize customer data
//Customer Customer::deserialize(const std::string& data) {
//    std::istringstream iss(data);
//    std::string token;
//    int id;
//    std::string fName, lName, mail, phone, passport;
//
//    std::getline(iss, token, ',');
//    id = std::stoi(token);
//    std::getline(iss, fName, ',');
//    std::getline(iss, lName, ',');
//    std::getline(iss, mail, ',');
//    std::getline(iss, phone, ',');
//    std::getline(iss, passport, ',');
//
//    return Customer(id, fName, lName, mail, phone, passport);
//}