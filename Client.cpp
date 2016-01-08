/*#############################################################################
Author: Jeremy Cruz

Date:   4/3/2015

File :  Client.h

------------------------------------------------------------------------------
Description: This file contains all functions that will be used for this
             overall program. Setters and getters for client datafields are
             present. There are also functions for setting the occupancy,
             inserting, searching, and reseting the database. The is another
             class called FileManager that is responsible for starting the
             database file DataFile.txt.
#############################################################################*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include "Client.h"

/* Formats for each data field used to format the datafile */
static const int OCCUPANCY_CHARACTERS = 8;
static const int NAME_CHARACTERS = 15;
static const int IDENTIFICATION_CHARACTERS = 9;
static const int BIRTHDAY_CHARACTERS = 6;
static const char SEPARATOR[] = "\t\t\t";

/* Debug messages */
static const char CREATE_CLIENT[] = "[Client object has been created]\n";
static const char CREATE_FILE[] = "[File object has been created]\n";
static const char DESTROY_CLIENT[] = "[Client object has been deallocated]\n";
static const char DESTROY_FILE[] = "[File object has been deallocated]\n";
static const char INSERT[] = "[Inserting... ";
static const char UPDATE_OCCUPANCY_FALSE[] = "[Reviewing occupancy]\n";
static const char UPDATE_OCCUPANCY_TRUE[] = "[Updating occupancy]\n";
static const char RESET[] = "[Clearing the database]\n";
static const char LOOKUP[] = "[Looking up... ";
static const char MAKE_FILE[] = "[Making the datafile]\n";
static const char WRITE[] = "[Writing the file]\n";

/* Flag variable to set the dubuger */
static bool debug;

/*=============================================================================
Class:       File Manager

Description: This class manages the file that holds the database.

DataFields:  none

Functions:   FileManager:  constructor
             ~FileManager: destructor
             makeFile:     create the datafile with the header and no clients
             outputFile:   write the file to stdout
==============================================================================*/
class FileManager
{
   public:
      /* Constructor and destructor*/
      FileManager();
      ~FileManager();

      /* Various function */
      void makeFile(ofstream &);
      string outputFile(ifstream &);
};

/*-----------------------------------------------------------------------------
Name:        debugOn

Description: Turns on debug mode.

Algorithm:   Assigns debug to true.

Parameters:  none

Output:      void

Result:      Debug is enabled.
------------------------------------------------------------------------------*/
void debugOn(void)
{
   /* Flag assignment */
   debug = true;
}

/*-----------------------------------------------------------------------------
Name:        debugOff

Description: Turns off debug mode.

Algorithm:   Assigns debug to false.

Parameters:  none

Output:      void

Result:      Debug is disabled.
------------------------------------------------------------------------------*/
void debugOff(void)
{
   /* Flag assignment */
   debug = false;
}

/*-----------------------------------------------------------------------------
Name:        Client

Description: Default constructor.

Algorithm:   Outputs prompt of being called if debug is on.

Parameters:  none

Output:      none

Result:      Client object is allocated in the heap.
------------------------------------------------------------------------------*/
Client :: Client()
{
   /* Debug message */
   if(debug)
      cerr << CREATE_CLIENT;
}

/*-----------------------------------------------------------------------------
Name:        Client

Description: Copy constructor that sets the datafields of the client.

Algorithm:   Input parameters are initialized to corresponding datafields. The
             datafields are set using the setters, and initialized using the
             getters. occ is set to occupancy which is set from the
             updateOccupancy function. If this is the first client, the
             occupancy would be 0 and this will equal the first. The next
             pointer always originates to NULL.

Parameters:  occ:  occupancy
             nm:   name
             id:   identification number
             bday: birthdate

Output:      none

Result:      Datafields of Client are all set.
------------------------------------------------------------------------------*/
Client :: Client(int occ, string nm, string id, int bday) :
          occupancy(updateOccupancy(false)), name(nm),
          identification(id), birthday(bday)
{
   /* Set all the datafields */
   setName(nm);
   setIdentification(id);
   setBirthday(bday);

   /* Initialize all the datafields */
   name = getName();
   identification = getIdentification();
   birthday = getBirthday();

   /* Set parameter to occupancy datafield */
   occ = occupancy;

   /* If database is empty originally, this is the first Client */
   if(occupancy == 0)
      first = this;

   /* The following client is always NULL */
   next = NULL;
}

/*-----------------------------------------------------------------------------
Name:        ~Client

Description: Destructor

Algorithm:   Outputs prompt of being called when debug is on.

Parameters:  none

Output:      none

Result:      Client object is deallocated from the heap.
------------------------------------------------------------------------------*/
Client :: ~Client()
{
   /* Debug message */
   if(debug)
      cerr << DESTROY_CLIENT;
}

/*-----------------------------------------------------------------------------
Name:        setName

Description: Setter for name.

Algorithm:   Assigns nm to name.

Parameters:  nm: client's name

Output:      void

Result:      Name is set.
------------------------------------------------------------------------------*/
void Client :: setName(string nm)
{
   /* Assignment */
   nm = name;
}

/*-----------------------------------------------------------------------------
Name:        setIdentification

Description: Setter for identification.

Algorithm:   Assigns id to identification.

Parameters:  id: client's identification

Output:      void

Result:      Identification is set.
------------------------------------------------------------------------------*/
void Client :: setIdentification(string id)
{
   /* Assignment */
   id = identification;
}

/*-----------------------------------------------------------------------------
Name:        setBirthday

Description: Setter for birthday.

Algorithm:   Assigns bday to birthday.

Parameters:  bday: client's birthday

Output:      void

Result:      Birthday is set.
------------------------------------------------------------------------------*/
void Client :: setBirthday(int bday)
{
   /* Assignment */
   bday = birthday;
}

/*-----------------------------------------------------------------------------
Name:        getName

Description: Getter for name.

Algorithm:   Returns name.

Parameters:  none

Output:      name: name of client

Result:      Name is returned.
------------------------------------------------------------------------------*/
string Client :: getName(void)
{
   /* Return value */
   return name;
}

/*-----------------------------------------------------------------------------
Name:        getIdentification

Description: Getter for identification.

Algorithm:   Returns identification.

Parameters:  none

Output:      identification: I.D. of client

Result:      identification is returned.
------------------------------------------------------------------------------*/
string Client :: getIdentification(void)
{
   /* Return value */
   return identification;
}

/*-----------------------------------------------------------------------------
Name:        getBirthday

Description: Getter for birthday.

Algorithm:   Returns birthday.

Parameters:  none

Output:      birthday: birthday of client

Result:      Birthday is returned.
------------------------------------------------------------------------------*/
int Client :: getBirthday(void)
{
   /* Return value */
   return birthday;
}

/*-----------------------------------------------------------------------------
Name:        updateOccupancy

Description: This function will keep track of the occupancy in the database and
             update it when needed.

Algorithm:   Occupancy is read from the Occupancy.txt file. If this is called
             from insert, it will increment the occupancy and overwrite the file
             with it. Lastly, it will read in the updated occupancy from the
             file. If this is not called from insert, incrementing occupancy is
             skipped thus, occupancy will remain the same at the end of this
             function.

Parameters:  fromInsert: determines wheather this is called from insert;
                         defaults to false

Output:      occupancy: amount of clients in the database

Result:      Occupancy is updated and written to the Occupancy.txt file for
             reading in the future.
------------------------------------------------------------------------------*/
int Client :: updateOccupancy(bool fromInsert = false)
{
   /* Debug message */
   if(debug)
      cerr << UPDATE_OCCUPANCY_FALSE;

   ifstream readOccFile;  /* used for reading Occupancy.txt */
   ofstream writeOccFile; /* used for writing Occupancy.txt */
   int updatedOccupancy;  /* temporary variable to hold occupancy if updated and
                             occupancy will be assigned to it */

   /* Read occupancy */
   readOccFile.open("Occupancy.txt");
   readOccFile >> occupancy;
   readOccFile.close();

   /* Increment occupancy if called from insert */
   if(fromInsert)
   {
      if(debug)
         cerr << UPDATE_OCCUPANCY_TRUE;

      writeOccFile.open("Occupancy.txt");
      occupancy++;
      writeOccFile << occupancy;
      writeOccFile.close();
   }

   /* Read updated occupancy(no change if not called from insert) and assign it
      to occupancy */
   readOccFile.open("Occupancy.txt");
   readOccFile >> updatedOccupancy;
   readOccFile.close();
   occupancy = updatedOccupancy;

   /* Return value */
   return occupancy;
}

/*-----------------------------------------------------------------------------
Name:        insert

Description: Insert a client in the database.

Algorithm:   next is assigned to a new Client object. Occupancy represented by
             parameter occ will call updateOccupancy to increment the occupancy.
             The database will be appended with all the corresponding datafields
             for the client inputted by the user. next will be deallocated to
             prevent memory leaks.

Parameters:  occ:  occupant number based on occupancy
             nm:   name of client
             id:   I.D. of client
             bday: birthday of client

Output:      void

Result:      DataFile.txt is appeded with a new client.
------------------------------------------------------------------------------*/
void Client :: insert(int occ, string nm, string id, int bday)
{
   /* Debug message */
   if(debug)
      cerr << INSERT << "Name: " << nm << ", Client I.D.: " << id
           << ", Birthday: " << bday << ", at occupant number: "
           << (occupancy + 1) << "]" << endl;

   const string FILE_NAME = "DataFile.txt"; /* literal name of the database
                                               file */
   /* Call to append to the database */
   ofstream clientFile(FILE_NAME.c_str(), ios :: app);

   /* Insertion begins by assigning next pointer to a new client */
   next = new Client(occ, nm, id, bday);

   /* Update the occupancy */
   occ = updateOccupancy(true);

   /* Append to the database file the client being inserted */
   clientFile << setw(OCCUPANCY_CHARACTERS) << setfill('0') << right << occ
              << SEPARATOR << setw(NAME_CHARACTERS) << setfill (' ') << left
              << nm << SEPARATOR << setw(IDENTIFICATION_CHARACTERS) << left
              << id << SEPARATOR << setw(BIRTHDAY_CHARACTERS) << left << bday
              << SEPARATOR << endl;

   /* Deallocation */
   delete next;

   /* Close the file */
   clientFile.close();
}

/*-----------------------------------------------------------------------------
Name:        reset

Description: Clear the datafile.

Algorithm:   Overwrite the occupancy file with 0 to empty the database. When
             occupancy is read as 0, the driver will clear the datafile.

Parameters:  none

Output:      none

Result:      Occupancy is reset to 0.
------------------------------------------------------------------------------*/
void Client :: reset(void)
{
   /* Debug message */
   if(debug)
      cerr << RESET;

   ofstream occFile; /* write the occupancy file */

   /* Overwrite the occupancy file with 0 */
   occFile.open("Occupancy.txt");
   occFile << 0;
   occFile.close();
}

/*-----------------------------------------------------------------------------
Name:        lookup

Description: Search for a client based on name entry.

Algorithm:   Open the datafile. Search the file for the name to search while the
             search has not reached EOF. Once the name is found in the file, the
             flag isFound to determine wheather client exists, will be set to
             true and break from the search loop. Once EOF is reached and name
             has not been found, the isFound flag will remain false.

Parameters:  clientFile: the file that holds the clients to be looked for
             nm:         name of client to search

Output:      isFound: status of wheather the desired client has been found

Result:      Returns either true or false depending on wheather the client has
             been found in the database.
------------------------------------------------------------------------------*/
bool Client :: lookup(ifstream &clientFile, string nm)
{
   /* Debug message */
   if(debug)
      cerr << LOOKUP << "Name: " << nm << "]" << endl;

   bool isFound = false; /* flag determining if client is found defaults to
                            false */
   string line;          /* string representing the items searched in the
                            file */
   int offset;           /* represent the area of the file searched */

   /* Perform linear search on the file for the client until EOF is reached */
   clientFile.open("DataFile.txt");
   while(!clientFile.eof())
   {
      getline(clientFile, line);

      /* Flag becomes true if found and search stops */
      if((offset = line.find(nm, 0)) != string :: npos)
      {
         isFound = true;
         break;
      }
   }

   /* Close the file */
   clientFile.close();

   /* Return value */
   return isFound;
}

/*-----------------------------------------------------------------------------
Name:        FileManager

Description: Default constructor.

Algorithm:   Outputs prompt of being called when debug is on.

Parameters:  none

Output:      none

Result:      FileManager object is allocated in the heap.
------------------------------------------------------------------------------*/
FileManager :: FileManager()
{
   /* Debug message */
   if(debug)
      cerr << CREATE_FILE;
}

/*-----------------------------------------------------------------------------
Name:        ~FileManager

Description: Destructor.

Algorithm:   Outputs prompt of being called when debug is on.

Parameters:  none

Output:      none

Result:      FileManager object is deallocated from the heap.
------------------------------------------------------------------------------*/
FileManager :: ~FileManager()
{
   /* Debug message */
   if(debug)
      cerr << DESTROY_FILE;
}

/*-----------------------------------------------------------------------------
Name:        makeFile

Description: Create a new database file.

Algorithm:   Open the datafile and overwrite it with the file header. File will
             be void of clients.

Parameters:  clientFile: the new datafile to write

Output:      none

Result:      A new datafile now exists.
------------------------------------------------------------------------------*/
void FileManager :: makeFile(ofstream &clientFile)
{
   /* Debug message */
   if(debug)
      cerr << MAKE_FILE;

   const char HEADER_CHAR = '-';       /* Character the separates the header */
   const int AMOUNT_HEADER_CHARS = 75; /* Amount of times to print the header
                                          separating character */

   /* Open the datafile */
   clientFile.open("DataFile.txt");

   /* Overwrite the datafile with the header */
   clientFile << setw(OCCUPANCY_CHARACTERS) << setfill('0') << left
              << "Occupant" << SEPARATOR << setw(NAME_CHARACTERS)
              << setfill (' ') << left << "Client Name" << SEPARATOR
              << setw(IDENTIFICATION_CHARACTERS) << left << "Client I.D."
              << SEPARATOR << setw(BIRTHDAY_CHARACTERS) << left << "Birthday"
              << SEPARATOR << endl;

   /* Print the header separator character 75 times and terminate with a new
      line */
   for(int amount = 1; amount <= AMOUNT_HEADER_CHARS; amount++)
      clientFile << HEADER_CHAR;
   clientFile << '\n';

   /* Close the file */
   clientFile.close();
}

/*-----------------------------------------------------------------------------
Name:        outputFile

Description: Write out the file to stdout.

Algorithm:   File will be read up to the end. The string fileContent will
             constantly be appended with the file contents of DataFile.txt.

Parameters:  clientFile: the file to write to stdout

Output:      fileContent: the contents of the datafile

Result:      The datafile is printed to stdout.
-----------------------------------------------------------------------------*/
string FileManager :: outputFile(ifstream &clientFile)
{
   /* Debug message */
   if(debug)
      cerr << WRITE;

   string fileContent;  /* contents of the datafile */
   string clipFromFile; /* contents read from file to append to the file
                           contents */

   /* Read the whole file and constantly append to the string holding its
      contents */
   clientFile.open("DataFile.txt");
   while(!clientFile.eof())
   {
      clientFile >> clipFromFile;
      fileContent += clipFromFile;
   }

   /* Close the file */
   clientFile.close();

   /* Return value */
   return fileContent;
}
