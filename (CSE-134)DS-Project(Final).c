#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define max_seat 52

// Define a structure for Driver
typedef struct driver
{
    char name[20];               // Driver name
    char contact[15];            // Driver contact number
    struct driver *next;         // Pointer to the next driver
} driver;

// Define a structure for Bus
typedef struct bus
{
    int bus_no;                 // Bus number
    driver *d;                  // Pointer to the driver assigned
} bus;

// Define a structure for Transport Destination and Price
typedef struct destination
{
    char name[20];
    int base_price;
} destination;

typedef struct Ticket
{
    int ticketNumber;
    int seatNumber;
} Ticket;

typedef struct Node
{
    Ticket data;
    struct Node* next;
} Node;



//Store Destinations and Prices
destination destinations[] = {{"Mirpur", 30}, {"Dhanmondi", 40}, {"Narayanganj", 50}, {"Uttara", 20}};
int num_destinations = sizeof(destinations) / sizeof(destination);


// Define a structure for transport
typedef struct transport
{
    char time[10];                // Time of departure
    char place[20];               // Destination place
    int seats;                    // Number of seats required
    bus b;                        // Bus and driver assigned
    int seat_no[10];           	  // Seat numbers assigned
    int payment;                  // Payment amount in Taka
    char method[10];              // Payment method
    char mobile_number[20];       // Mobile number field
    char transaction_number[20];  // Added transaction number field
    int status;                   // Payment status (0 for not done, 1 for done)
} transport;


// Function to display destinations and prices
void displayDestinations()
{
    printf("\nDestinations and Prices:\n");
    for (int i = 0; i < num_destinations; i++)
    {
        printf("0%d. %-15s %d Taka\n", i + 1, destinations[i].name, destinations[i].base_price);
    }
}

// Calculate the price based on the destination and number of seats
int calculatePrice(int destinationIndex, int seats)
{
    int base_price = destinations[destinationIndex].base_price;
    return base_price * seats;
}


//Driver Details
void display_driver(driver *d)
{
    printf("| Driver name: %s              |\n", d->name);
    printf("| Driver contact number: %s    |\n", d->contact);
}
Node* ticketList = NULL;
void displayTotalTicketsSold()
{
    int totalTickets = 0;
    Node *current = ticketList;

    while (current != NULL)
    {
        totalTickets++;  // Increment for each sold ticket
        current = current->next;
    }

    printf("Total Tickets Sold: %d\n", totalTickets);
}

//Create New driver node
driver *create_driver(char *name, char *contact)
{
    driver *new_driver = (driver *)malloc(sizeof(driver));

    strcpy(new_driver->name, name);
    strcpy(new_driver->contact, contact);
    new_driver->next = NULL;

    return new_driver;
}


//Add a driver node at the end of the list
void insert_driver(driver **head, char *name, char *contact)
{
    driver *new_driver = create_driver(name, contact);

    // If the list is empty, make the new node as the head
    if (*head == NULL)
    {
        *head = new_driver;
    }
    else
    {
        driver *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_driver;
    }
}
//Delete a Driver
void delete_driver(driver **head, char *name)
{
    if (*head == NULL)
    {
        return;
    }
    //If the head node matches the name, delete the head node
    if (strcmp((*head)->name, name) == 0)
    {
        driver *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    //Otherwise,traverse the list until the node to be deleted is found and delete it
    else
    {
        driver *prev = *head;
        driver *temp= (*head)->next;
        while (temp != NULL && strcmp(temp->name, name) != 0)
        {
            prev = temp;
            temp = temp->next;
        }
        // If the node is found, delete it and link the previous and next nodes
        if (temp != NULL)
        {
            prev->next = temp->next;
            free(temp);
        }
    }
}

void addTicket(int seatNumber)
{
    Node* newTicket = (Node*)malloc(sizeof(Node));

    newTicket->data.seatNumber = seatNumber;
    newTicket->next = ticketList;
    ticketList = newTicket;
}


// Function to display available seats for a given bus
void displayAvailableSeats(bus *b, int bookedSeats[])
{
    printf("\nAvailable Seats for Bus %d:\n", b->bus_no);

    for (int i = 0; i < max_seat; i++)
    {
        // Check if the seat is booked
        if (bookedSeats[i] == 0)
        {
            printf("%d ", i + 1);
            if ((i + 1) % 4 == 0)
            {
                // Print a newline after every four seats
                printf("\n");
            }
            else if ((i + 1) % 2 == 0)
            {
                // Print extra spaces between pairs of seats
                printf("   ");
            }
        }
        else
        {
            // Print 'X' for booked seats
            printf("X ");
            if ((i + 1) % 4 == 0)
            {
                printf("\n");
            }
        }
    }
    printf("\n");
}

// Function to book transport
void bookTransport(driver *drivers, bus *buses, int bookedSeats[])
{
    transport t;
    int destinationIndex;
    int seats;

    printf("\nEnter the time of departure (HH:MM): ");
    scanf("%s",&t.time);

    displayDestinations();

    printf("Enter the destination number (1-%d): ",num_destinations);
    scanf("%d",&destinationIndex);

    // Validate destination choice
    if (destinationIndex < 1 || destinationIndex > num_destinations)
    {
        printf("Invalid destination choice.\n");
        return;
    }

    printf("How Many Seats You Need? ");
    scanf("%d", &seats);

    // Validate the number of seats
    if (seats <= 0)
    {
        printf("Invalid Number Of Seats.\n");
        return;
    }

    // Display selected destination and calculate the price
    printf("Selected Destination: %s\n", destinations[destinationIndex - 1].name);
    int price = calculatePrice(destinationIndex - 1, seats);
    printf("Price for %d seat(s): %d taka\n", seats, price);

    // Choose a bus for booking
    static int currentBusIndex = 0;
    int busIndex = currentBusIndex % 52 + 1; // Sequential index from 1 to 52
    currentBusIndex++;

    bus selectedBus = buses[busIndex - 1];
    printf("Selected Bus: %d\n", selectedBus.bus_no);
    printf("___________________________________________________\n");
    // Display available seats for the selected bus
    displayAvailableSeats(&selectedBus, bookedSeats);

    int seatNumber;
    // Choose seats for booking
    printf("To Choose Seat Numbers (1-52) Separated By Spaces: ");
    for (int i = 0; i < seats; i++)
    {
        scanf("%d", &t.seat_no[i]);
        seatNumber=t.seat_no[i];

        // Validate seat numbers
        if (t.seat_no[i] < 1 || t.seat_no[i] > 52 || bookedSeats[t.seat_no[i] - 1] == 1)
        {
            printf("Invalid seat number or seat already booked. Please choose a different number.\n");
            return;
        }
        else
        {
            // Mark the seat as booked
            bookedSeats[t.seat_no[i] - 1] = 1;
        }
    }

    // Display booking details
    printf("___________________________________________________\n");
    printf("\nBooking Details:\n");
    printf("Time: %s\n", t.time);
    printf("Destination: %s\n", destinations[destinationIndex - 1].name);
    printf("Seats booked: ");
    for (int i = 0; i < seats; i++)
    {
        printf("%d ", t.seat_no[i]);
    }
    printf("\n__________________________________________________\n");
    printf("\n");

    // Display payment status
    printf("Select payment method (bkash/nagad/rocket): ");
    scanf("%s",&t.method);

    // Get user input for mobile number
    printf("Enter mobile number: ");
    scanf("%s",&t.mobile_number);

    // Get user input for transaction number
    printf("Enter transaction number: ");
    scanf("%s",&t.transaction_number);

    printf("___________________________________________________\n");
    printf("Is the payment done? (yes/no): ");
    char choice[10];
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0)
    {
        t.status = 1;
        printf("Payment status: Success\n");
        printf("___________________________________________________\n");
    }
    else
    {
        t.status = 0;
        printf("Payment status: Make payment complete first.\n");
    }
    // int ticketNumber = rand() % 1000 + 1; // Generating a random ticket number for simplicity
    addTicket(seatNumber);
    printf("\nTransport booked successfully!\n");
}


//Display All Driver Information
void displayDrivers(driver *head)
{
    printf(" _________________________________________\n");
    printf("|           List of Drivers:              |\n");
    driver *temp = head;
    while (temp != NULL)
    {
        printf("|_________________________________________|\n");
        display_driver(temp);
        printf("|_________________________________________|\n");
        temp = temp->next;
    }
}


//Admin panel
void adminPanel(driver *drivers, bus *buses)
{
    printf(" _______________________________\n");
    printf("|          ADMIN PANNEL         |\n");
    printf("|_______________________________|\n");
    printf("| 1. Display List of Drivers    |\n");
    printf("| 2. Add a Driver               |\n");
    printf("| 3. Remove a Driver            |\n");
    printf("| 4. Display Total Tickets Sold |\n");
    printf("| 5. Exit Admin Panel           |\n");
    printf("|_______________________________|\n");
    printf("Enter your choice: ");

    int adminChoice;
    scanf("%d", &adminChoice);

    switch(adminChoice)
    {
    case 1:
        displayDrivers(drivers);
        break;
    case 2:
    {
        char name[20];
        char contact[15];
        printf("Enter the Name of the new driver: ");
        scanf("%s",&name);
        printf("Enter the Contact No. of the new driver: ");
        scanf("%s",&contact);
        insert_driver(&drivers, name, contact);
        printf("Driver Added successfully!\n");
        break;
    }
    case 3:
    {
        char name[20];
        printf("Enter the name of the driver to be removed: ");
        scanf("%s", name);
        delete_driver(&drivers, name);
        printf("Driver removed successfully!\n");
        break;
    }
    case 4:
        displayTotalTicketsSold();
        break;
    case 5:
        printf("Exit Admin Panel.\n");
        break;

    default:
        printf("Invalid choice. Please choose a valid option.\n");
        break;
    }
}

//Main Function
int main()
{
    int bookedSeats[max_seat] = {0};
    driver *drivers = NULL;

    insert_driver(&drivers, "Karim", "+880123456789");
    insert_driver(&drivers, "Ripon", "+880987654321");
    insert_driver(&drivers, "Siraj", "+880456789123");
    insert_driver(&drivers, "Jalil", "+880321654987");

    // Create an array of 50 buses
    bus buses[52];
    for (int i = 0; i < 52; i++)
    {
        buses[i].bus_no = i + 1;
        driver *temp = drivers;
        while (temp != NULL)
        {
            temp = temp->next;
        }
        buses[i].d = temp;
    }
    while (1)
    {
        printf(" ____________________________\n");
        printf("|          MAIN MENU         |\n");
        printf("|____________________________|\n");
        printf("| 1.Book Transport           |\n");
        printf("| 2.Admin Panel              |\n");
        printf("| 3.Display Available Seats  |\n");
        printf("| 4.Exit                     |\n");
        printf("|____________________________|\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            bookTransport(drivers, buses, bookedSeats);
            break;
        case 2:
            adminPanel(drivers, buses);
            break;
        case 3:
            displayAvailableSeats(buses,bookedSeats);
            break;
        case 4:
            printf("Exiting the program. Thank you!\n");
            exit(0);
        default:
            printf("Invalid choice. Please choose a valid option.\n");
            break;
        }
    }

    return 0;
}

