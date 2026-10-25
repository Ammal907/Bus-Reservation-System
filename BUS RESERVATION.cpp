#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <cctype>

using namespace std;

const int SEATS_PER_BUS = 50;
const int BUS_COUNT = 8;
const int MAX_BOOKINGS = BUS_COUNT * SEATS_PER_BUS;
const string FILE_NAME = "booking.txt";

// =====================================================
// HELPER FUNCTIONS
// =====================================================

string trim(string s)
{
    size_t first = s.find_first_not_of(" \t\r\n");

    if (first == string::npos)
        return "";

    size_t last = s.find_last_not_of(" \t\r\n");

    return s.substr(first, last - first + 1);
}

string normalize(string s)
{
    s = trim(s);

    for (size_t i = 0; i < s.length(); i++)
        s[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(s[i]))
        );

    return s;
}

string readLine(string prompt)
{
    string value;

    while (true)
    {
        cout << prompt;
        getline(cin >> ws, value);

        value = trim(value);

        if (value.empty())
        {
            cout << "Input cannot be empty.\n";
            continue;
        }

        // Pipe is used as the file separator.
        if (value.find('|') != string::npos)
        {
            cout << "Character | is not allowed.\n";
            continue;
        }

        return value;
    }
}

bool parseInteger(string text, int& value)
{
    stringstream ss(text);
    char extra;

    if (!(ss >> value))
        return false;

    if (ss >> extra)
        return false;

    return true;
}

// =====================================================
// BASE CLASS: PERSON
// =====================================================

class Person
{
protected:
    string name;

public:
    Person() : name("") {}

    Person(string n) : name(n) {}

    virtual void input()
    {
        name = readLine("Enter Name: ");
    }

    virtual void display() const
    {
        cout << "Name: " << name << endl;
    }

    virtual ~Person() {}
};

// Forward declaration
class Passenger;
class Admin;

void showPassengerInfo(const Passenger& p);

// =====================================================
// PASSENGER CLASS
// =====================================================

class Passenger : public Person
{
protected:
    int seatNo;
    string departure;
    string destination;
    int busId;

public:
    static int passengerCount;

    Passenger()
        : Person(), seatNo(-1),
          departure(""), destination(""), busId(-1)
    {
    }

    Passenger(string n, string dep, string dest,
              int seat, int id)
        : Person(n), seatNo(seat),
          departure(dep), destination(dest), busId(id)
    {
    }

    // Copy constructor
    Passenger(const Passenger& p)
        : Person(p.name),
          seatNo(p.seatNo),
          departure(p.departure),
          destination(p.destination),
          busId(p.busId)
    {
    }

    // Assignment operator
    Passenger& operator=(const Passenger& p)
    {
        if (this != &p)
        {
            name = p.name;
            seatNo = p.seatNo;
            departure = p.departure;
            destination = p.destination;
            busId = p.busId;
        }

        return *this;
    }

    void input() override
    {
        name = readLine("Enter Passenger Name: ");
    }

    void display() const override
    {
        cout << "Passenger: " << name
             << " | Bus ID: " << busId
             << " | Seat: " << seatNo
             << " | Departure: " << departure
             << " | Destination: " << destination
             << endl;
    }

    void assignSeat(int s)
    {
        seatNo = s;
    }

    void setRoute(string dep, string dest)
    {
        departure = dep;
        destination = dest;
    }

    void setBusId(int id)
    {
        busId = id;
    }

    int getSeat() const
    {
        return seatNo;
    }

    int getBusId() const
    {
        return busId;
    }

    string getName() const
    {
        return name;
    }

    string getDeparture() const
    {
        return departure;
    }

    string getDestination() const
    {
        return destination;
    }

    static void incrementCount()
    {
        passengerCount++;
    }

    static int getTotal()
    {
        return passengerCount;
    }

    // Compare passenger name and route
    bool operator==(const Passenger& p) const
    {
        return normalize(name) == normalize(p.name)
            && normalize(departure) == normalize(p.departure)
            && normalize(destination) == normalize(p.destination);
    }

    // Demonstration of + operator
    Passenger operator+(const Passenger& p) const
    {
        Passenger temp(*this);

        temp.name = name + " & " + p.name;

        return temp;
    }

    // Decrease seat number for operator demonstration.
    // This does not change the actual booking.
    Passenger operator-(int amount) const
    {
        Passenger temp(*this);

        if (amount > 0 && temp.seatNo > 1)
        {
            temp.seatNo -= amount;

            if (temp.seatNo < 1)
                temp.seatNo = 1;
        }

        return temp;
    }

    // Prefix increment
    Passenger& operator++()
    {
        if (seatNo >= 1 && seatNo < SEATS_PER_BUS)
            seatNo++;

        return *this;
    }

    // Postfix increment
    Passenger operator++(int)
    {
        Passenger temp(*this);

        if (seatNo >= 1 && seatNo < SEATS_PER_BUS)
            seatNo++;

        return temp;
    }

    // Stream output operator
    friend ostream& operator<<(
        ostream& out, const Passenger& p)
    {
        out << "Passenger: " << p.name
            << " | Seat: " << p.seatNo
            << " | Bus ID: " << p.busId
            << " | Route: " << p.departure
            << " -> " << p.destination;

        return out;
    }

    // Stream input operator
    friend istream& operator>>(
        istream& in, Passenger& p)
    {
        cout << "Enter name: ";
        getline(in >> ws, p.name);

        cout << "Enter departure city: ";
        getline(in >> ws, p.departure);

        cout << "Enter destination city: ";
        getline(in >> ws, p.destination);

        p.seatNo = -1;
        p.busId = -1;

        return in;
    }

    friend void showPassengerInfo(const Passenger& p);
    friend class Admin;
};

int Passenger::passengerCount = 0;

// =====================================================
// FRIEND FUNCTION
// =====================================================

void showPassengerInfo(const Passenger& p)
{
    cout << "\n--- Friend Function Output ---\n";

    cout << "Name: " << p.name << endl;
    cout << "Seat: " << p.seatNo << endl;
    cout << "Bus ID: " << p.busId << endl;
    cout << "Departure: " << p.departure << endl;
    cout << "Destination: " << p.destination << endl;
}

// =====================================================
// VIP FEATURES
// =====================================================

class VIPFeatures
{
protected:
    int vipLevel;

public:
    VIPFeatures() : vipLevel(1) {}

    void increaseLevel()
    {
        vipLevel++;
    }
};

// =====================================================
// VIP PASSENGER
// =====================================================

class VIPPassenger : public Passenger
{
public:
    VIPPassenger() : Passenger() {}

    void display() const override
    {
        cout << "VIP Passenger: " << name
             << " | Seat: " << seatNo
             << " | Bus ID: " << busId
             << endl;
    }
};

// Multiple inheritance
class SpecialVIPPassenger
    : public VIPPassenger, public VIPFeatures
{
public:
    SpecialVIPPassenger()
        : VIPPassenger(), VIPFeatures()
    {
    }

    void display() const override
    {
        cout << "Special VIP: " << name
             << " | Seat: " << seatNo
             << " | VIP Level: " << vipLevel
             << endl;
    }
};

// =====================================================
// SEAT CLASS
// =====================================================

class Seat
{
private:
    int num;
    bool booked;

public:
    Seat() : num(-1), booked(false) {}

    void bookSeat(int n)
    {
        num = n;
        booked = true;
    }

    void release()
    {
        num = -1;
        booked = false;
    }

    bool isBooked() const
    {
        return booked;
    }

    int getNumber() const
    {
        return num;
    }
};

// =====================================================
// ROUTE CLASS
// =====================================================

class Route
{
private:
    string start;
    string dest;

public:
    Route() : start(""), dest("") {}

    Route(string s, string d)
        : start(s), dest(d)
    {
    }

    string getStart() const
    {
        return start;
    }

    string getDestination() const
    {
        return dest;
    }

    bool matches(string dep, string destination) const
    {
        return normalize(start) == normalize(dep)
            && normalize(dest) == normalize(destination);
    }

    void display() const
    {
        cout << start << " -> " << dest;
    }
};

// =====================================================
// BUS CLASS
// =====================================================

class Bus
{
private:
    int busId;
    Seat seats[SEATS_PER_BUS];
    Route route;

public:
    Bus(int id, string start, string destination)
        : busId(id), route(start, destination)
    {
    }

    int getBusId() const
    {
        return busId;
    }

    string getDeparture() const
    {
        return route.getStart();
    }

    string getDestination() const
    {
        return route.getDestination();
    }

    bool matchesRoute(string dep, string dest) const
    {
        return route.matches(dep, dest);
    }

    int availableSeats() const
    {
        int available = 0;

        for (int i = 0; i < SEATS_PER_BUS; i++)
        {
            if (!seats[i].isBooked())
                available++;
        }

        return available;
    }

    bool hasAvailableSeat() const
    {
        return availableSeats() > 0;
    }

    int allocateSeat()
    {
        for (int i = 0; i < SEATS_PER_BUS; i++)
        {
            if (!seats[i].isBooked())
            {
                seats[i].bookSeat(i + 1);
                return i + 1;
            }
        }

        return -1;
    }

    bool markSeatBooked(int s)
    {
        if (s < 1 || s > SEATS_PER_BUS)
            return false;

        if (seats[s - 1].isBooked())
            return false;

        seats[s - 1].bookSeat(s);

        return true;
    }

    void releaseSeat(int s)
    {
        if (s >= 1 && s <= SEATS_PER_BUS)
            seats[s - 1].release();
    }

    void resetSeats()
    {
        for (int i = 0; i < SEATS_PER_BUS; i++)
            seats[i].release();
    }

    void showRoute() const
    {
        cout << "Bus ID: " << busId << " | Route: ";

        route.display();

        cout << " | Available Seats: "
             << availableSeats()
             << "/" << SEATS_PER_BUS
             << endl;
    }
};

// =====================================================
// ADMIN CLASS
// =====================================================

class Admin
{
public:
    void showSeat(const Passenger& p) const
    {
        // Direct access is allowed because Admin is a friend.
        cout << "Admin Access Seat: " << p.seatNo << endl;
        cout << "Admin Access Bus ID: " << p.busId << endl;
    }
};

// =====================================================
// BOOKING CLASS
// =====================================================

class Booking
{
private:
    Passenger passengers[MAX_BOOKINGS];
    int count;

    int findBusById(Bus buses[], int busCount, int id)
    {
        for (int i = 0; i < busCount; i++)
        {
            if (buses[i].getBusId() == id)
                return i;
        }

        return -1;
    }

    bool save(const Passenger& p)
    {
        ofstream file(FILE_NAME.c_str(), ios::app);

        if (!file)
        {
            cout << "Error: Could not open booking file.\n";
            return false;
        }

        file << p.getBusId() << "|"
             << p.getSeat() << "|"
             << p.getName() << "|"
             << p.getDeparture() << "|"
             << p.getDestination() << "\n";

        if (!file)
        {
            cout << "Error: Could not save booking.\n";
            return false;
        }

        return true;
    }

public:
    Booking() : count(0) {}

    void showAvailableBuses(Bus buses[], int busCount) const
    {
        cout << "\n--- Available Bus Routes ---\n";

        for (int i = 0; i < busCount; i++)
            buses[i].showRoute();
    }

    void book(Bus buses[], int busCount)
    {
        if (count >= MAX_BOOKINGS)
        {
            cout << "Booking limit reached.\n";
            return;
        }

        string dep = readLine("Enter Departure City: ");
        string dest = readLine("Enter Destination City: ");

        if (normalize(dep) == normalize(dest))
        {
            cout << "Departure and destination cannot be same.\n";
            return;
        }

        int selectedBus = -1;
        bool routeExists = false;

        // Find a bus with matching route and free seats.
        for (int i = 0; i < busCount; i++)
        {
            if (buses[i].matchesRoute(dep, dest))
            {
                routeExists = true;

                if (buses[i].hasAvailableSeat())
                {
                    selectedBus = i;
                    break;
                }
            }
        }

        if (selectedBus == -1)
        {
            if (routeExists)
            {
                cout << "All buses on this route are full.\n";
            }
            else
            {
                cout << "No bus available for this route.\n";
                showAvailableBuses(buses, busCount);
            }

            return;
        }

        cout << "\nBus Found!\n";

        buses[selectedBus].showRoute();

        Passenger p;

        p.input();

        int seat = buses[selectedBus].allocateSeat();

        if (seat == -1)
        {
            cout << "No seat available.\n";
            return;
        }

        p.assignSeat(seat);
        p.setBusId(buses[selectedBus].getBusId());

        // Save the canonical route stored in the bus.
        p.setRoute(
            buses[selectedBus].getDeparture(),
            buses[selectedBus].getDestination()
        );

        // Save first. If saving fails, release the seat.
        if (!save(p))
        {
            buses[selectedBus].releaseSeat(seat);
            cout << "Booking was not completed.\n";
            return;
        }

        passengers[count] = p;
        count++;

        Passenger::incrementCount();

        cout << "\nBooking Successful!\n";
        p.display();
    }

    void load(Bus buses[], int busCount)
    {
        ifstream file(FILE_NAME.c_str());

        if (!file)
        {
            cout << "No existing booking file found.\n";
            cout << "The system will start with empty bookings.\n";
            return;
        }

        // Reset in-memory booking data before reloading.
        for (int i = 0; i < busCount; i++)
            buses[i].resetSeats();

        count = 0;
        Passenger::passengerCount = 0;

        string line;

        while (getline(file, line))
        {
            if (count >= MAX_BOOKINGS)
            {
                cout << "Maximum booking limit reached while loading.\n";
                break;
            }

            stringstream ss(line);

            string busStr, seatStr, name, dep, dest, extra;

            if (!getline(ss, busStr, '|') ||
                !getline(ss, seatStr, '|') ||
                !getline(ss, name, '|') ||
                !getline(ss, dep, '|') ||
                !getline(ss, dest, '|') ||
                getline(ss, extra, '|'))
            {
                cout << "Skipping invalid record.\n";
                continue;
            }

            int id, seat;

            if (!parseInteger(busStr, id) ||
                !parseInteger(seatStr, seat))
            {
                cout << "Skipping record with invalid numbers.\n";
                continue;
            }

            name = trim(name);

            if (name.empty())
            {
                cout << "Skipping record with empty name.\n";
                continue;
            }

            int busIndex = findBusById(buses, busCount, id);

            if (busIndex == -1)
            {
                cout << "Skipping record with unknown bus ID.\n";
                continue;
            }

            Bus& bus = buses[busIndex];

            if (seat < 1 || seat > SEATS_PER_BUS)
            {
                cout << "Skipping record with invalid seat number.\n";
                continue;
            }

            // A saved booking must match its bus's fixed route.
            if (!bus.matchesRoute(dep, dest))
            {
                cout << "Skipping record with incorrect route.\n";
                continue;
            }

            // Reject duplicate seat bookings.
            if (!bus.markSeatBooked(seat))
            {
                cout << "Skipping duplicate seat booking.\n";
                continue;
            }

            Passenger p(
                name,
                bus.getDeparture(),
                bus.getDestination(),
                seat,
                id
            );

            passengers[count] = p;
            count++;

            Passenger::incrementCount();
        }

        file.close();

        cout << "Data Loaded Successfully.\n";
        cout << "Valid bookings loaded: " << count << endl;
    }

    void showAll() const
    {
        if (count == 0)
        {
            cout << "No passengers booked.\n";
            return;
        }

        cout << "\n--- All Passengers ---\n";

        for (int i = 0; i < count; i++)
            passengers[i].display();
    }

    bool getPassenger(Passenger& p) const
    {
        if (count == 0)
            return false;

        p = passengers[0];

        return true;
    }
};

// =====================================================
// MAIN
// =====================================================

int main()
{
    // Fixed fleet: each bus has a fixed route.
    // Two buses are available for Karachi -> Lahore
    // and two for Karachi -> Islamabad.

    Bus buses[BUS_COUNT] =
    {
        Bus(101, "Karachi", "Lahore"),
        Bus(102, "Karachi", "Lahore"),
        Bus(103, "Karachi", "Islamabad"),
        Bus(104, "Karachi", "Islamabad"),
        Bus(105, "Lahore", "Karachi"),
        Bus(106, "Lahore", "Islamabad"),
        Bus(107, "Islamabad", "Karachi"),
        Bus(108, "Islamabad", "Lahore")
    };

    Booking booking;
    Admin admin;

    // Load saved bookings and reserve their seats.
    booking.load(buses, BUS_COUNT);

    int choice = 0;

    do
    {
        cout << "\n========== BUS RESERVATION SYSTEM ==========\n";
        cout << "1. Book Seat\n";
        cout << "2. Show All Passengers\n";
        cout << "3. Show Bus Routes and Available Seats\n";
        cout << "4. Total Passengers\n";
        cout << "5. Polymorphism Demonstration\n";
        cout << "6. Operator Overloading Demonstration\n";
        cout << "7. Friend Function and Admin Access\n";
        cout << "8. Reload Booking File\n";
        cout << "9. Exit\n";
        cout << "Enter Choice: ";

        if (!(cin >> choice))
        {
            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );

            cout << "Invalid input. Enter a number from 1 to 9.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            booking.book(buses, BUS_COUNT);
            break;

        case 2:
            booking.showAll();
            break;

        case 3:
            booking.showAvailableBuses(buses, BUS_COUNT);
            break;

        case 4:
            cout << "Total Passengers: "
                 << Passenger::getTotal()
                 << endl;
            break;

        case 5:
        {
            Person* ptr;

            SpecialVIPPassenger vip;

            ptr = &vip;

            ptr->input();

            vip.increaseLevel();

            // Runtime polymorphism.
            ptr->display();

            break;
        }

        case 6:
        {
            Passenger p1;

            if (!booking.getPassenger(p1))
            {
                cout << "Book a passenger first.\n";
                break;
            }

            Passenger p2;

            cin >> p2;

            // Copy constructor
            Passenger copy = p1;

            cout << "\nCopied Passenger:\n";
            cout << copy << endl;

            cout << "\nAddition Operator:\n";
            cout << (p1 + p2) << endl;

            cout << "\nSubtraction Operator:\n";
            cout << (p1 - 1) << endl;

            if (p1 == p2)
                cout << "Passengers match by name and route.\n";
            else
                cout << "Passengers do not match.\n";

            ++p1;

            cout << "After Prefix ++: " << p1.getSeat() << endl;

            Passenger old = p1++;

            cout << "Postfix returned seat: "
                 << old.getSeat() << endl;

            cout << "Seat in local copy after postfix: "
                 << p1.getSeat() << endl;

            break;
        }

        case 7:
        {
            Passenger p;

            if (!booking.getPassenger(p))
            {
                cout << "No passengers available.\n";
                break;
            }

            admin.showSeat(p);

            showPassengerInfo(p);

            break;
        }

        case 8:
            booking.load(buses, BUS_COUNT);
            break;

        case 9:
            cout << "Program Exited.\n";
            break;

        default:
            cout << "Invalid Choice.\n";
        }

    } while (choice != 9);

    return 0;
}