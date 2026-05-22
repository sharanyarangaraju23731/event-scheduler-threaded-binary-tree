#include <iostream>
#include <string>
using namespace std;

struct Event
{
    string name;
    int day, month, year, hour, minute;
    Event* left;
    Event* right;
    bool isThreaded;
};

struct TBT 
{
    Event* root;
};

void initializeTBT(TBT &tree);
Event* createEvent();
void insert(TBT &tree);
void inorder(TBT &tree);
void deleteOld(TBT &tree);
Event* findNextUpcomingEvent(TBT &tree);
Event* searchEvent(TBT &tree,string eventName);
void deleteEventByName(TBT &tree, string eventName);
void updateEvent(TBT &tree, string eventName);

void initializeTBT(TBT &tree)
{
    tree.root=nullptr;    
}

Event* createEvent()
{
    Event* newevent=new Event;
    cout << "Enter the event name: ";
    cin.ignore();
    getline(cin,newevent->name);
    cout << "Enter the event date(DD MM YYYY): ";
    cin >> newevent->day >> newevent->month >> newevent->year;
    cout << "Enter the event time(HH MM): ";
    cin >> newevent-> hour >> newevent -> minute;
    
    newevent->left=newevent->right=nullptr;
    newevent->isThreaded=false;
    
    return newevent;
}

void insert(TBT &tree)
{
    Event* newNode=createEvent();
    
    if(tree.root==nullptr)
    {
        tree.root=newNode;
        return;
    }
    
    Event* curr=tree.root;
    Event* parent=nullptr;
    
    while(curr)
    {
        parent=curr;
        
        if(newNode->year < curr->year || newNode->year==curr->year && newNode->month < curr->month ||
           newNode->year==curr->year && newNode->month==curr->month && newNode->day < curr->day ||
           newNode->year==curr->year && newNode->month==curr->month && newNode->day==curr->day && newNode->hour < curr->hour ||
           newNode->year==curr->year && newNode->month==curr->month && newNode->day==curr->day && newNode->hour==curr->hour && newNode->minute < curr->minute)
        {
            if(!curr->left)
            {
                curr->left=newNode;
                newNode->right=curr;
                newNode->isThreaded=true;
                return;
            }
            curr=curr->left;
        }
            
        else
        {
            if (!curr->right || curr->isThreaded) 
            {
                newNode->right = curr->right;
                newNode->isThreaded = true;
                curr->right = newNode;
                curr->isThreaded = false;
                return;
            }
            curr = curr->right;
        }
    }
}

void inorder(TBT &tree)
{
    if(tree.root==nullptr)
    {
        cout<<"Tree is empty"<<endl;
        return;
    }
    
    Event *curr=tree.root;
    while(curr->left)
    {
        curr=curr->left;
    }
    
    while (curr)
    {
        cout << curr->name << " on " << curr->day << "/" << curr->month << "/" << curr->year 
        << " at " << curr->hour << ":" << curr->minute << endl;

        if (curr->isThreaded)
        curr = curr->right;
        else
        {
            curr = curr->right;
            while (curr && curr->left)
                curr = curr->left;
        }
    
    }
}

void deleteOld(TBT &tree)
{
    if(tree.root==nullptr)
    {
        cout<<"No event to delete"<<endl;
        return;
    }
    
    Event* parent=nullptr;
    Event* curr=tree.root;
    
    while(curr->left)
    {
        parent=curr;
        curr=curr->left;
    }
    
    cout << "Deleting oldest event: " << curr->name << " on " 
         << curr->day << "/" << curr->month << "/" << curr->year 
         << " at " << curr->hour << ":" << curr->minute << endl;

    if (curr->isThreaded) 
    {
        if (parent) 
        {
            parent->left = nullptr;
        } 
        else 
        {
            tree.root = curr->right;
        }
    } 
    else 
    {
        if (parent) 
        {
            parent->left = curr->right;
        } 
        else 
        {
            tree.root = curr->right;
        }
    }

    delete curr;
}

Event* findNextUpcomingEvent(TBT &tree) 
{
    if (tree.root == nullptr) 
    {
        cout << "No events scheduled." << endl;
        return nullptr;
    }
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currDay = ltm->tm_mday;
    int currMonth = ltm->tm_mon + 1;
    int currYear = ltm->tm_year + 1900;
    int currHour = ltm->tm_hour;
    int currMinute = ltm->tm_min;
    
    Event* curr = tree.root;
    Event* nextEvent = nullptr;
    
    while (curr) 
    {
        if (curr->year > currYear ||
            (curr->year == currYear && curr->month > currMonth) ||
            (curr->year == currYear && curr->month == currMonth && curr->day > currDay) ||
            (curr->year == currYear && curr->month == currMonth && curr->day == currDay && curr->hour > currHour) ||
            (curr->year == currYear && curr->month == currMonth && curr->day == currDay && curr->hour == currHour && curr->minute >= currMinute)) 
        {
            nextEvent = curr;
            curr = curr->left;
        } 
        
        else 
        {
            curr = curr->right;
        }
    
    }
    
    if (nextEvent) 
    {
        cout << "Next Upcoming Event: " << nextEvent->name << " on " 
             << nextEvent->day << "/" << nextEvent->month << "/" << nextEvent->year
             << " at " << nextEvent->hour << ":" << nextEvent->minute << endl;
    } 
    else 
    {
        cout << "No upcoming events." << endl;
    }

    return nextEvent;
}

Event* searchEvent(TBT &tree, string eventName)
{
    if (tree.root == nullptr)
    {
        cout << "Tree is empty." << endl;
        return nullptr;
    }

    Event* curr = tree.root;
    while (curr->left)
        curr = curr->left;

    while (curr)
    {
        if (curr->name == eventName)
        {
            cout << "Event found: " << curr->name << " on " 
                 << curr->day << "/" << curr->month << "/" << curr->year 
                 << " at " << curr->hour << ":" << curr->minute << endl;
            return curr;
        }

        if (curr->isThreaded)
            curr = curr->right;
        else
        {
            curr = curr->right;
            while (curr && curr->left)
                curr = curr->left;
        }
    }

    cout << "Event not found." << endl;
    return nullptr;
}

void deleteEventByName(TBT &tree, string eventName) 
{
    Event* curr = tree.root;
    Event* parent = nullptr;
    bool found = false;

    while (curr != nullptr) 
    {
        if (curr->name == eventName) 
        {
            found = true;
            break;
        }

        parent = curr;

        if (eventName < curr->name) 
        {
            curr = curr->left;
        } 
        else 
        {
            if (curr->isThreaded) 
            {
                break;
            }
            curr = curr->right;
        }
    }

    if (!found) 
    {
        cout << "Event not found." << endl;
        return;
    }

    if (curr->left == nullptr && (curr->right == nullptr || curr->isThreaded)) 
    {
        Event* child = nullptr;
        if (curr->left != nullptr) 
        {
            child = curr->left;
        } 
        else if (!curr->isThreaded) 
        {
            child = curr->right;
        }

        if (parent == nullptr) 
        {
            tree.root = child;
        } 
        else if (parent->left == curr) 
        {
            parent->left = child;
        } 
        else 
        {
            if (!parent->isThreaded) 
            {
                parent->right = child;
            }
        }

        if (curr->isThreaded && child != nullptr) 
        {
            child->right = curr->right;
        }

        delete curr;
    } 
    else 
    {
        Event* successorParent = curr;
        Event* successor = curr->right;

        while (successor != nullptr && successor->left != nullptr) 
        {
            successorParent = successor;
            successor = successor->left;
        }

        curr->name = successor->name;
        curr->day = successor->day;
        curr->month = successor->month;
        curr->year = successor->year;
        curr->hour = successor->hour;
        curr->minute = successor->minute;

        if (successorParent->left == successor) 
        {
            if (successor->isThreaded) 
            {
                successorParent->left = nullptr;
            } 
            else 
            {
                successorParent->left = successor->right;
            }
        } 
        else 
        {
            if (successor->isThreaded) 
            {
                successorParent->right = nullptr;
            } 
            else 
            {
                successorParent->right = successor->right;
            }
        }

        delete successor;
    }

    cout << "Event deleted." << endl;
}

void updateEvent(TBT &tree, string eventName) 
{
    Event* event = searchEvent(tree, eventName);

    if (event == nullptr) 
    {
        cout << "Event not found." << endl;
        return;
    }

    string newName;
    int newDay, newMonth, newYear, newHour, newMinute;

    cout << "Enter new event name: ";
    cin.ignore();
    getline(cin, newName);
    cout << "Enter new date (DD MM YYYY): ";
    cin >> newDay >> newMonth >> newYear;
    cout << "Enter new time (HH MM): ";
    cin >> newHour >> newMinute;

    deleteEventByName(tree, eventName);

    Event* updatedEvent = new Event;
    updatedEvent->name = newName;
    updatedEvent->day = newDay;
    updatedEvent->month = newMonth;
    updatedEvent->year = newYear;
    updatedEvent->hour = newHour;
    updatedEvent->minute = newMinute;
    updatedEvent->left = nullptr;
    updatedEvent->right = nullptr;
    updatedEvent->isThreaded = false;

    Event* curr = tree.root;
    Event* parent = nullptr;

    while (curr != nullptr) 
    {
        parent = curr;

        if (updatedEvent->year < curr->year ||
            (updatedEvent->year == curr->year && updatedEvent->month < curr->month) ||
            (updatedEvent->year == curr->year && updatedEvent->month == curr->month && updatedEvent->day < curr->day) ||
            (updatedEvent->year == curr->year && updatedEvent->month == curr->month && updatedEvent->day == curr->day && updatedEvent->hour < curr->hour) ||
            (updatedEvent->year == curr->year && updatedEvent->month == curr->month && updatedEvent->day == curr->day && updatedEvent->hour == curr->hour && updatedEvent->minute < curr->minute)) 
            {
            
            if (curr->left == nullptr) 
            {
                curr->left = updatedEvent;
                updatedEvent->right = curr;
                updatedEvent->isThreaded = true;
                return;
            }
            curr = curr->left;
        } 
        else 
        {
            if (curr->right == nullptr || curr->isThreaded) 
            {
                updatedEvent->right = curr->right;
                updatedEvent->isThreaded = true;
                curr->right = updatedEvent;
                curr->isThreaded = false;
                return;
            }
            curr = curr->right;
        }
    }

    tree.root = updatedEvent;
}

int main() 
{
    TBT tree;
    initializeTBT(tree);
    int choice;
    string name;

    do 
    {
        cout << "\nEvent Scheduler Menu:\n";
        cout << "1. Insert Event\n";
        cout << "2. Display All Events (Inorder)\n";
        cout << "3. Delete Oldest Event\n";
        cout << "4. Find Next Upcoming Event\n";
        cout << "5. Search Event by Name\n";
        cout << "6. Delete Event by Name\n";
        cout << "7. Update Event\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) 
        {
            case 1:
                insert(tree);
                break;
            case 2:
                inorder(tree);
                break;
            case 3:
                deleteOld(tree);
                break;
            case 4:
                findNextUpcomingEvent(tree);
                break;
            case 5:
                cout << "Enter event name to search: ";
                cin.ignore();
                getline(cin, name);
                if (searchEvent(tree, name)) 
                {
                    cout << "Event found.\n";
                } 
                else 
                {
                    cout << "Event not found.\n";
                }
                break;
            case 6:
                cout << "Enter event name to delete: ";
                cin.ignore();
                getline(cin, name);
                deleteEventByName(tree, name);
                break;
            case 7:
                cout << "Enter event name to update: ";
                cin.ignore();
                getline(cin, name);
                updateEvent(tree, name);
                break;
            case 8:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);

    return 0;
}