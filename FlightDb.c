
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "FlightDb.h"
#include "AVLTree.h"

struct flightDb {
	// TODO: Add to this struct
	Tree byFlightNumber;
	Tree byDepartureDate;
    Tree byDepartureAirport;
};

// Comparison functions /////////////////////////////////////////////////////
 
// compare function that first compares by flight number and then by day and
// time, lastly by flight duration
int compareByFlightNumber(Record r1, Record r2) {
    if (strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2)) != 0) {
        return strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
    } if (RecordGetDepartureDay(r1) != RecordGetDepartureDay(r2)) {
        return RecordGetDepartureDay(r1) - RecordGetDepartureDay(r2);
    } else if (RecordGetDepartureHour(r1) != RecordGetDepartureHour(r2)) {
        return RecordGetDepartureHour(r1) - RecordGetDepartureHour(r2);
    } else {
		return RecordGetDepartureMinute(r1) - RecordGetDepartureMinute(r2);
    }
}

// compare function that first compares by day and time and then by flight
// number
int compareByDepartureDate(Record r1, Record r2) {
    if (RecordGetDepartureDay(r1) != RecordGetDepartureDay(r2)) {
        return RecordGetDepartureDay(r1) - RecordGetDepartureDay(r2);
    } else if (RecordGetDepartureHour(r1) != RecordGetDepartureHour(r2)) {
        return RecordGetDepartureHour(r1) - RecordGetDepartureHour(r2);
    } else if (RecordGetDepartureMinute(r1) != RecordGetDepartureMinute(r2)) {
		return RecordGetDepartureMinute(r1) - RecordGetDepartureMinute(r2);
	} else {
		return strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
    }
}

// specific compare function for DbFindByDepartureAirportDay function that
// first compares by departure airport and then by day and time
int compareByDepartureAirport(Record r1, Record r2) {
    if (strcmp(RecordGetDepartureAirport(r1), 
               RecordGetDepartureAirport(r2)) != 0) {
        return strcmp(RecordGetDepartureAirport(r1), 
               RecordGetDepartureAirport(r2));
    } else if (RecordGetDepartureDay(r1) != RecordGetDepartureDay(r2)) {
        return RecordGetDepartureDay(r1) - RecordGetDepartureDay(r2);
    } else if (RecordGetDepartureHour(r1) != RecordGetDepartureHour(r2)) {
        return RecordGetDepartureHour(r1) - RecordGetDepartureHour(r2);
    } else if (RecordGetDepartureMinute(r1) != RecordGetDepartureMinute(r2)) {
		return RecordGetDepartureMinute(r1) - RecordGetDepartureMinute(r2);
	} else {
		return strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
    }
}

/**
 * Creates a new flight DB. 
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
FlightDb DbNew(void) {
	// TODO: Complete this function
	// allocate memory for the FlightDB
	FlightDb db = malloc(sizeof(*db));
    if (db == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
	// create both trees by using the comp functions
	db->byFlightNumber = TreeNew(compareByFlightNumber);
    db->byDepartureDate = TreeNew(compareByDepartureDate);
    db->byDepartureAirport = TreeNew(compareByDepartureAirport);
    return db;
}

/**
 * Frees all memory allocated to the given flight DB
 */
void     DbFree(FlightDb db) {
	// TODO: Complete this function
    TreeFree(db->byDepartureAirport, false);
    TreeFree(db->byDepartureDate, false);
    TreeFree(db->byFlightNumber, true);
    free(db);
}

/**
 * Inserts  a  flight  record  into the given DB if there is not already
 * record with the same flight number, departure airport, day, hour  and
 * minute.
 * If  inserted successfully, this function takes ownership of the given 
 * record (so the caller should not modify or free it). 
 * Returns true if the record was successfully inserted,  and  false  if
 * the  DB  already  contained  a  record  with  the same flight number,
 * departure airport, day, hour and minute.
 * The time complexity of this function must be O(log n).
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
bool     DbInsertRecord(FlightDb db, Record r) {
	// TODO: Complete this function
	if (TreeInsert(db->byDepartureDate, r) &&
        TreeInsert(db->byFlightNumber, r) &&
        TreeInsert(db->byDepartureAirport, r)) {
        return true;
    } else {
        return false;
    } 
}

/**
 * Searches  for  all  records with the given flight number, and returns
 * them all in a list in increasing order of  (day, hour, min).  Returns
 * an empty list if there are no such records. 
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List     DbFindByFlightNumber(FlightDb db, char *flightNumber) {
	// TODO: Complete this function
    Record lower_dummy = RecordNew(flightNumber, "AAA", "AAA", 0, 00, 0, 1);
    Record upper_dummy = RecordNew(flightNumber, "AAA", "AAA", 6, 23, 59, 1);
    List l = TreeSearchBetween(db->byFlightNumber, lower_dummy, upper_dummy);
    RecordFree(lower_dummy);
    RecordFree(upper_dummy);
	return l;
}

/**
 * Searches  for all records with the given departure airport and day of
 * week (0 to 6), and returns them all in a list in increasing order  of
 * (hour, min, flight number).
 * Returns an empty list if there are no such records.
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List     DbFindByDepartureAirportDay(FlightDb db, char *departureAirport,
                                     int day) {
	// TODO: Complete this function
    Record lower_dummy = RecordNew("0", departureAirport,
                                   "AAA", day, 00, 0, 1);
    Record upper_dummy = RecordNew("zzzzz", departureAirport,
                                   "AAA", day, 23, 59, 1);
    List l = TreeSearchBetween(db->byDepartureAirport,
                               lower_dummy, upper_dummy);
    RecordFree(lower_dummy);
    RecordFree(upper_dummy);
	return l;
}


/**
 * Searches  for  all  records  between  (day1, hour1, min1)  and (day2,
 * hour2, min2), and returns them all in a list in increasing  order  of
 * (day, hour, min, flight number).
 * Returns an empty list if there are no such records.
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List     DbFindBetweenTimes(FlightDb db, 
                            int day1, int hour1, int min1, 
                            int day2, int hour2, int min2) {
	// TODO: Complete this function
    Record lower_dummy = RecordNew("0", "AAA",
                                   "AAA", day1, hour1, min1, 1);
    Record upper_dummy = RecordNew("zzzzz", "zzz",
                                   "AAA", day2, hour2, min2, 1);
    List l = TreeSearchBetween(db->byDepartureDate,
                               lower_dummy, upper_dummy);
    RecordFree(lower_dummy);
    RecordFree(upper_dummy);
	return l;
}

/**
 * Searches  for  and  returns  the  earliest next flight from the given
 * departure airport, on or after the given (day, hour, min).
 * The returned record must not be freed or modified. 
 * The time complexity of this function must be O(log n).
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
Record   DbFindNextFlight(FlightDb db, char *departureAirport, 
                          int day, int hour, int min) {
	// TODO: Complete this function
    Record dummy = RecordNew("0", departureAirport, "AAA", day, hour, min, 1);
    Record searched_rec = TreeNext(db->byDepartureAirport, dummy);
    RecordFree(dummy);
    // if the next flight is in the new week, we create a dummy that searches
    if (searched_rec == NULL) {
        Record overweekend = RecordNew("0", departureAirport, "AAA",
                                 0, 00, 0, 1);
        searched_rec = TreeNext(db->byDepartureAirport, overweekend);
        RecordFree(overweekend);
    }
	return searched_rec;
}

