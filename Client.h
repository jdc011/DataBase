/*#############################################################################
Author: Jeremy Cruz

Date:   4/3/2015

File :  Client.h

------------------------------------------------------------------------------
Description: This is a header file containing the function definitions and
             datafields for the class Client.
#############################################################################*/
#ifndef CLIENT_H
#define CLIENT_H

#include<string>

using namespace std;

/*=============================================================================
Class:       Client

Description: This is the object we are inserting into the database.

DataFields:  birthday:       input birthday of client as xxxxxx
             occupancy:      amount of clients present in database; occupant in
                             formof 0000000x
             name:           name of client
             identification: client I.D. in form Axxxxxxxx
             first:          pointer to first client
             next:           pointer to succeeding client

Functions:   Client:            constructor
             ~Client:           destructor
             setName:           setter for name
             setIdentification: setter for identification
             setBirthday:       setter for birthday
             getName:           getter for name
             getIdentification: getter for identification
             getBirthday:       getter for birthday
             updateOccupancy:   update occupancy; increment if a new client has
                                been inserted into the database; occupancy is
                                saved in Occupancy.txt
             insert:            insert a client into the database file
                                DataFile.txt
             reset:             clear the database file DataBase.txt and set the
                                occupancy to 0
             lookup:            look for a client name in the database
=============================================================================*/
class Client
{
   /* Gives access to FileManager to use occupancy in order to determine
      wheather the file DataFile.txt is supposed to be rewritten */
   friend class FileManager;

   /* Datafields */
   private:
      int birthday,
          occupancy;

      string name,
             identification;

      Client * first;
      Client * next;

   /* Functions */
   public:

      /* Constructors and destructor */
      Client();
      Client(int, string, string, int);
      ~Client();

      /* Setters */
      void setName(string);
      void setIdentification(string);
      void setBirthday(int);

      /* Getters */
      string getName(void);
      string getIdentification(void);
      int getBirthday(void);

      /* Various functions for a database */
      int updateOccupancy(bool);
      void insert(int, string, string, int);
      void reset(void);
      bool lookup(ifstream &, string);
};

#endif
