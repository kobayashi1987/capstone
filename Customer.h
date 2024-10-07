////
//// Created by jack on 5/10/24.
////
//
//// Customer.h
//#ifndef CUSTOMER_H
//#define CUSTOMER_H
//
//#include <string>
//
//class Customer {
//private:
//    int customerID;
//    std::string firstName;
//    std::string lastName;
//    std::string email;
//    std::string phoneNumber;
//    std::string passportNumber;
//
//public:
//    // Constructor
//    Customer(int id, const std::string& fName, const std::string& lName,
//             const std::string& mail, const std::string& phone, const std::string& passport);
//
//    // Getters
//    int getCustomerID() const;
//    std::string getFirstName() const;
//    std::string getLastName() const;
//    std::string getEmail() const;
//    std::string getPhoneNumber() const;
//    std::string getPassportNumber() const;
//
//    // Setters
//    void setFirstName(const std::string& fName);
//    void setLastName(const std::string& lName);
//    void setEmail(const std::string& mail);
//    void setPhoneNumber(const std::string& phone);
//    void setPassportNumber(const std::string& passport);
//
//    // Display customer information
//    void displayCustomerInfo() const;
//
//    // Serialize customer data to a string (e.g., for saving to a file)
//    std::string serialize() const;
//
//    // Deserialize customer data from a string
//    static Customer deserialize(const std::string& data);
//};
//
//#endif // CUSTOMER_H