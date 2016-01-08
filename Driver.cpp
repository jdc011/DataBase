/*#############################################################################
Author: Jeremy Cruz

Date:   4/3/2015

File:   Driver.cpp
-------------------------------------------------------------------------------
Description: The driver file calls client functions that modify the database.
             Functions are called based on user input of chars that are defined
             in this file.
#############################################################################*/
#include "Client.cpp"
#include<getopt.h>
#include<cstdlib>
#include<cstdio>

/* Prototype function for a separate debug mode setter to be called in main */
void debugSetter(int, char * const *);

/*-----------------------------------------------------------------------------
Name:        main

Description: This is the main method. It calls functions from Client.cpp based
             on user input of chars defined here. These functions will modify
             the database in the file DataFile.txt.

Algorithm:   A while loop is implemented as long as cin is true. Based on input
             of command as a char, certain cases will be called to implement
             the corresponding function from Client.cpp. At the beginning of the
             loop, the command is always reset in order to clear the stdin
             buffer. There are cout << endl statements before each break to
             ensure that 2 lines are written in every instance the loop starts
             again.

Parameters:  arg1: default argument 1 used to set debug mode
             arg2: default argument 2 used to set debug mode

Output:      Default return 0.

Result:      Main function actually runs the program to modify the database.
-----------------------------------------------------------------------------*/
int main(int arg1, char * const * arg2)
{
   int occ,                   /* input occupancy */
       bday;                  /* input birthday */

   string nm,                 /* input name */
          id;                 /* input identification */

   char command;              /* command to call a corresponding function from
                                 Client.cpp */

   ofstream outClientFile;    /* file output object */
   ifstream inClientFile;     /* file input object */
   
   Client client;             /* Client object to call Client functions */
   FileManager fileManager;   /* FileManager object to call FileManager
                                 functions */

   /* Call this function to set up the debug mode based on the command line
      arguments specified by arg1 and arg2 */
   debugSetter(arg1, arg2);

   /* Automatically reset the file if there are no clients; occupancy of 0 */
   if(client.updateOccupancy(false) == 0)
      fileManager.makeFile(outClientFile);

   /* This loop runs the program by constantly calling functions specified by
      user input of commands chars */
   while(cin)
   {
      /* Prompting message */
      cout << "\nDatabase contains " << client.updateOccupancy()
           << " client(s).\n"
           << "Select a command... (i)Insert (l)Lookup (r)Reset (w)Write: ";

      /* Reset command to null */
      command = 0;

      /* Input command */
      cin >> command;

      /* Determines function to be called based on command input */
      switch(command)
      {
         case 'i': /* Inserting/ appending to the database */

            /* Prompt and input for name */
            cout << "Enter client's name: ";
            cin >> nm;

            /* Prompt and input for identification */
            cout << "Enter client's ID number: ";
            cin >> id;

            /* Prompt and input for birthday */
            cout << "Enter client's birthday: ";
            cin >> bday;

            /* Insert input into the database */
            client.insert(occ, nm, id, bday);

            /* Keep stdout consistent */
            cout << endl;
         break;

         case 'l': /* Searching for a client */

            /* Don't do anything if database is empty and exit this case */
            if(client.updateOccupancy(false) == 0)
            {
               /* Prompt issue */
               cout << "The database is empty!" << endl;

               /* Keep stdout consistent */
               cout << endl;
               break;
            }

            /* Prompt and input for client to search */
            cout << "Enter the name of a client to lookup: ";
            cin >> nm;

            /* Found or not */
            if(client.lookup(inClientFile, nm + ' '))
               cout << "Client " << nm << " found!" << endl;
            else
               cout << "Client " << nm << " not found!" << endl;

            /* Keep stout consistent */
            cout << endl;
         break;

         case 'r': /* Clear the database */

            /* Don't do anything if database is already empty and exit this
               case */
            if(client.updateOccupancy() == 0)
            {
               /* Prompt issue */
               cout << "The database is empty!" << endl;

               /* Keep stdout consistent */
               cout << endl;
               break;
            }

            /* Prompt and input verifying deletion of the database */
            cout << "You are about to clear the database! Are you sure you want"
                    " want to do this? (y/n): ";
            cin >> command;

            while(command != 'y' && command != 'n' && cin)
            {
               /* Prompt invalid command and output the previous prompt and
                  input again */
               cout << command << " is not a valid command!\n" << "You are "
                       " about to clear the database! Are you sure you want "
                       " to do this? (y/n): ";
               
               /* Reset the command to null */
               command = 0;

               cin >> command;
            }

            /* Reset the occupancy and clear the database for command 'y' */
            if(command == 'y')
            {
               client.reset();
               fileManager.makeFile(outClientFile);
            }

            /* Exit this case for command 'n' */
            else
            {
               /* Keep stdout consistent */
               cout << endl;
               break;
            }

            /* Keep stdout consistent */
            cout << endl;
         break;

         case 'w': /* Write out the datafile to stdout */
            cout << fileManager.outputFile(inClientFile) << endl;

         /* Invalid command or exit program */
         default:
            /* Keep stdout consistent */
            cout << endl;
      }
   }

   /* Default return for main */
   return 0;
}

/*-----------------------------------------------------------------------------
Name:        debugSetter

Description: Set the debug mode to on or off based on command line argument.

Algorithm:   Set debug off by default and use a while loop to determine if
             commands line argument exists to turn it on. Otherwise, it is
             automatically off.

Parameters:  arg1: default argument 1 from main

             arg2: default argument 2 from main

Output:      void

Result:      Debug is either enabled or disabled during execution.
-----------------------------------------------------------------------------*/
void debugSetter(int arg1, char * const * arg2)
{
   char option; /* determines debug mode */

   /* Set if off by default */
   debugOff();

   /* Loop executes when argument is present and will turn on debug mode */
   while((option = getopt(arg1, arg2, "x")) != EOF)
   {
      switch (option)
      {
         case 'x': /* Turn on if x is found in argument */
            debugOn();
         break;
      }
   }
}
