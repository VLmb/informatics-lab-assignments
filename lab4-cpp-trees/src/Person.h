#include <string>
#include <ctime>
#include <sstream>

enum class Role { Student, Teacher };

struct PersonID {
    int series;
    int number;

    std::string toString() const {
        return std::to_string(series) + " " + std::to_string(number);
    }

    bool operator<(const PersonID& other) const {
        if (series < other.series) return true;
        if (number < other.number) return true;
        return false;
    }

    bool operator==(const PersonID& other) const {
        return series == other.series && number == other.number;
    }
};

class Person {
private:
    PersonID id;
    std::string firstName;
    std::string middleName;
    std::string lastName;
    time_t birthDate;
    Role role;

public:
    Person() : id({0,0}), birthDate(0), role(Role::Student) {}
    
    Person(PersonID id, const std::string& first, const std::string& middle,
           const std::string& last, time_t birth, Role role)
        : id(id), firstName(first), middleName(middle), lastName(last), birthDate(birth), role(role) {}

    PersonID GetID() const { return id; }
    const std::string& GetFirstName() const { return firstName; }
    const std::string& GetMiddleName() const { return middleName; }
    const std::string& GetLastName() const { return lastName; }
    time_t GetBirthDate() const { return birthDate; }

    std::string GetFullName() const {
        return firstName + " " + middleName + " " + lastName;
    }

    bool operator<(const Person& other) const {
        if (lastName < other.lastName) return true;
        if (firstName < other.firstName) return true;
        if (middleName < other.middleName) return true;
        return false;
    }

    bool operator>(const Person& other) const {
        return other < *this;
    }

    bool operator<=(const Person& other) const {
        return !(other < *this);
    }

    bool operator>=(const Person& other) const {
        return !(*this < other);
    }

    bool operator==(const Person& other) const {
        return lastName == other.lastName && firstName == other.firstName &&
        middleName == other.middleName;
    }

    bool operator!=(const Person& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << "Person: [ID: " << p.id.toString()
           << ", Name: " << p.GetFullName();

        char buf[80];
        std::tm bt = *std::localtime(&p.birthDate); 
        if (p.birthDate != static_cast<time_t>(-1) && p.birthDate != 0) {
            if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &bt) > 0) {
                 os << ", Birth Date: " << buf;
            } else {
                os << ", Birth Date: (invalid format)";
            }
        } else {
            os << ", Birth Date: (not set or invalid)";
        }


        os << ", Role: " << (p.role == Role::Student ? "Student" : "Teacher")
           << "]";
        return os;
    }
};
