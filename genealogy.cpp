// Project 5 - CS315 with Dr.Raphael Finkel - Genealogy
// Author: Tanner Palin
// Sources: Looked into an old BFS implementation I used for CS216 with professor Pike a couple years ago.
// Discussion: Discussed techniques for BFS and the verification of data with Will Shapiro and Gave Rivera.
// Purpose: This program takes in a formatted text file to generate a bipartite graph containing people and families.
// Each person has two families, one they are born into, and one they are married into. Using the families we can
// check our data for integrity/consistency by ensuring a person is doubly linked to their families. In a similar way
// we can also determine how two people are related to each other by performing a breadth first search through the graph.
// Each family or person is recognized by an integer ID instead of a surname.

#include <iostream>
#include <string>
#include <queue>
// I decided to use the queue library since one was needed for the BFS.
// We demonstrated that we could manually implement a queue in the first project and is besides the point of this project.
// Using the queue library also cuts down on unnecessary code for the grader to have to look through.

using namespace std;

// Depending on the size of the text file and number of children, you may want to change these constants.
const int MAXNUM = 100;
const int MAXCHILDREN = 10;

// Struct that will act as in individual in genealogy.
struct person {
    int personId;
    struct family *birth;
    struct family *marriage;
    bool visited;           // has it been visited yet in BFS?
};

// Struct that will hold immediately related individuals in genealogy.
struct family {
    int familyId;
    struct person *husband;
    struct person *wife;
    struct person *children[MAXCHILDREN];
};

// Creates a new person from our text file.
void insertPerson(struct person *people[], struct family *families[]) {
    int numIndex, numSave;
    string cursor;
    cin >> numIndex;
    numSave = numIndex;
    cin >> cursor; // Just moves cursor past the word "Parent".
    cin >> numIndex;
    cin >> cursor; // Jumping cursor over another word.
    people[numSave]->birth = families[numIndex];
    cin >> numIndex;
    people[numSave]->marriage = families[numIndex];
    cout << "Person " << people[numSave]->personId << " was born into family " << people[numSave]->birth->familyId;
    cout << " and married into family " << people[numSave]->marriage->familyId << "." << endl;
    return;
}

// Creates a new family from our text file.
string createFamily(struct person *people[], struct family *families[]) {
    int numIndex, numSave;
    string cursor;
    cin >> numIndex;
    numSave = numIndex;
    cin >> cursor; // Skipping over husband.
    
    cin >> numIndex; // Number for husband
    cin >> cursor; // Skip
    
    families[numSave]->husband = people[numIndex];
    cin >> numIndex; // Number for wife
   
    families[numSave]->wife = people[numIndex];
    // Time to add children to the family (make babies).
    cin >> cursor;
    for(int j=0; j < MAXCHILDREN; j++) {
        if(cursor == "Child") {
            cin >> numIndex;
            families[numSave]->children[j] = people[numIndex];
        }
        else if(cursor != "Child") {
            break;
        }
        cin >> cursor;
    }
    cout << "Family " << families[numSave]->familyId << " has huband " << families[numSave]->husband->personId;
    cout << ", wife " << families[numSave]->wife->personId;
    if(families[numSave]->children[0] == NULL) {
        cout << " and no Children.\n";
    }
    else{
        cout << " and Children";
        for(int k=0; k < MAXCHILDREN; k++) {
        if(families[numSave]->children[k] != NULL) {
            cout << " " << families[numSave]->children[k]->personId;
        }
        else{ 
        cout << "." << endl;
        break ;
        }
    }
    }
    return cursor;
}

// Initializes our people and families to have certain values.
void initializeLists(struct person *peeps[], struct family *fams[]) {
    for(int k=0; k < MAXNUM;k++) {
        peeps[k]->personId = k;
        fams[k]->familyId = k;
        peeps[k]->birth = NULL;
        peeps[k]->marriage = NULL;
        fams[k]->husband = NULL;
        fams[k]->wife = NULL;
        for(int j = 0; j < MAXCHILDREN; j++) {
           fams[k]->children[j] = NULL;
        }
    }
    return;
}

// Verifies our genealogy data by making sure everything is doubly linked between people and families.
void verifyData(struct person *people[], struct family *families[]) {
    bool isErr = false;
    bool isDoubly = false;
    for(int q = 0; q < MAXNUM; q++) {
        // Checking birth family of person.
        if(people[q]->birth != NULL) {
            // Make sure they are a child of birth family.
            for(int j = 0; j < MAXCHILDREN; j++) {
                if(people[q]->birth->children[j] == people[q]) {
                    isDoubly = true;
                }
            }
            if((people[q]->birth->familyId != 0) && (isDoubly == false)) {
                cout << "Person " << people[q]->personId << " points to birth family ";
                cout << people[q]->birth->familyId << " but there is no backpointer.\n";
                isErr = true;
            }
        }
        if((people[q]->marriage != NULL) && people[q]->marriage->familyId != 0) {
            if((people[q]->marriage->husband != people[q]) && (people[q]->marriage->wife != people[q])) {
                cout << "Person " << people[q]->personId << " points to marriage family ";
                cout << people[q]->marriage->familyId << " but there is no backpointer.\n";
                isErr = true;
            }
        }
        for(int t=0; t < MAXCHILDREN; t++) {
            if(families[q]->children[t] != NULL){
                if(families[q] != families[q]->children[t]->birth) {
                    cout << "Family " << families[q]->familyId << " points to Child " << families[q]->children[t]->personId;
                    cout << " but there is no backpointer.\n";
                    isErr = true;
                }
            }
        }
        if(families[q]->wife != NULL) {
            if(families[q]->wife->marriage != families[q] && families[q]->wife->personId != 0){
                cout << "Family " << families[q]->familyId << " points to wife " << families[q]->wife->personId;
                cout << " but there is no backpointer.\n";
                isErr = true;
            }
            if(families[q]->husband->marriage != families[q] && families[q]->husband->personId != 0) {
                cout << "Family " << families[q]->familyId << " points to husband " << families[q]->husband->personId;
                cout << " but there is no backpointer.\n";
                isErr = true;
            }
        }
    }
    if(isErr == false) {
       cout << "The data is consistent.\n";
   }
    return;
}

// Determines the appropriate path to print from a BFS between two people.
void printRelation(int startPoint, int endPoint, int path[], struct person *people[]) {
    int nextPoint = endPoint;
    int nextFamily = 0;
    int relation[MAXNUM];
    int next = 0;
    relation[next] = nextPoint;
    next = next + 1;
    while(nextPoint != startPoint) {
        if(people[nextPoint]->birth == people[path[nextPoint]]->birth) {
            nextFamily = people[nextPoint]->birth->familyId;
        }
        else if(people[nextPoint]->marriage == people[path[nextPoint]]->birth) {
            nextFamily = people[nextPoint]->marriage->familyId;
        }
        else if(people[nextPoint]->birth == people[path[nextPoint]]->marriage) {
            nextFamily = people[nextPoint]->birth->familyId;
        }
        else if(people[nextPoint]->marriage == people[path[nextPoint]]->marriage) {
            nextFamily = people[nextPoint]->marriage->familyId;
        }
        relation[next] = nextFamily;
        next = next + 1;
        relation[next] = path[nextPoint];
        next = next+1;
        nextPoint = path[nextPoint];
    }
    next = next - 1;
    for(int j = next; j>=0; j--) {
         cout << "Person " << relation[j];
         if(j != 0) {
            cout << " -> Family " << relation[j-1] << " -> ";
            j--;
         }
    }
    cout << "\n";
}

// Resets the visited boolean used in our BFS.
void resetPaths(struct person *people[]) {
    for(int i=0; i < MAXNUM; i++) {
        people[i]->visited = false;
    }
    return;
}

// Performs a BFS and finds shortest path of relation between two people.
bool relateTwo(struct person *people[], struct family *families[]){
    int fromHere, toHere;
    int shortestPath[MAXNUM];
    queue<person*> peopleList; // Queue to hold people for BFS.
    person *nextPerson = new struct person; // Person currently being checked in BFS.
    cin >> fromHere;
    cin >> toHere;
    people[fromHere]->visited = true;
    peopleList.push(people[fromHere]);
    while(peopleList.empty() == false) { // While the queue is not empty
        nextPerson = peopleList.front();
        peopleList.pop();
        if(nextPerson == people[toHere]) { // If this is the person/droid we are looking for.
            printRelation(fromHere, toHere, shortestPath, people);
            resetPaths(people);
            return true;
        }
        if(nextPerson->marriage != NULL) {
            // If person is married into a family, add members of that family to BFS queue.
            if(nextPerson->marriage->husband != NULL) {
            if((nextPerson->marriage->husband->visited == false)) {
                nextPerson->marriage->husband->visited = true;
                peopleList.push(nextPerson->marriage->husband);
                shortestPath[nextPerson->marriage->husband->personId] = nextPerson->personId;
            }
            }
            if(nextPerson->marriage->wife != NULL) {
            if((nextPerson->marriage->wife->visited == false)) {
                nextPerson->marriage->wife->visited = true;
                peopleList.push(nextPerson->marriage->wife);
                shortestPath[nextPerson->marriage->wife->personId] = nextPerson->personId;
            }
            }
            // Adding children to queue.
            for(int p=0; p < MAXCHILDREN; p++) {
                if(nextPerson->marriage->children[p] != NULL) {
                if(nextPerson->marriage->children[p]->visited == false){
                    nextPerson->marriage->children[p]->visited = true;
                    peopleList.push(nextPerson->marriage->children[p]);
                    shortestPath[nextPerson->marriage->children[p]->personId] = nextPerson->personId;
                }
                }
            }
        }
        if(nextPerson->birth != NULL) {
            // If person is born into a family, add members of that family to BFS queue.
            if(nextPerson->birth->husband != NULL) {
            if((nextPerson->birth->husband->visited == false)) {
                nextPerson->birth->husband->visited = true;
                peopleList.push(nextPerson->birth->husband);
                shortestPath[nextPerson->birth->husband->personId] = nextPerson->personId;
            }
            }
            if(nextPerson->birth->wife != NULL) {
            if((nextPerson->birth->wife->visited == false)) {
                nextPerson->birth->wife->visited = true;
                peopleList.push(nextPerson->birth->wife);
                shortestPath[nextPerson->birth->wife->personId] = nextPerson->personId;
            }
            }
             for(int j=0; j < MAXCHILDREN; j++) {
                if(nextPerson->birth->children[j] != NULL) {
                if((nextPerson->birth->children[j]->visited == false)) {
                    nextPerson->birth->children[j]->visited = true; 
                    peopleList.push(nextPerson->birth->children[j]);
                    shortestPath[nextPerson->birth->children[j]->personId] = nextPerson->personId;
                }
            }
            }
        }
    }
    resetPaths(people);
    return false;
}

int main(int argc, char *argv[]) {
    if(argc != 1) {
        cout << "Usage: ./wordCount < fileName\n(Redirect standard input to text file, thank you.)\n";
        return 0;
    }

    struct person *people[MAXNUM];    
    struct family *families[MAXNUM];

    for(int p=0; p < MAXNUM; p++) { // Allocating memory.
        people[p] = new person;
		families[p] = new family;
    }
    initializeLists(people, families); // Set array structs to control values.
    string command;
    while(cin >> command) { // While there are commands in the file left to be executed.
        if(command == "Family") {
            command = createFamily(people, families);
            while (command == "Family")
            {
                command = createFamily(people, families);
            }   // This while loop accounts for accidental intake using cin.
        }
        if(command == "Person") {
            insertPerson(people, families);
        }
        if(command == "Verify") {
            verifyData(people, families); 
        }
        if(command == "Relate") {
            if(relateTwo(people, families) == false) {
                cout << "Could not relate the two people.\n";
            }
        }
    }
    return 0;
}