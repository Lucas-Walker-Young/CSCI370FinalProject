// -------------------------------------------------------------------------------------------------
// ----------------------------------- HEADER :: Time-Controller -----------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_TIMECONTROLLER_HPP
#define CSCI370FINALPROJECT_TIMECONTROLLER_HPP

#include "IO.hpp"

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ----------------------------------- STRUCT :: Time-Controller -----------------------------------
// -------------------------------------------------------------------------------------------------
struct TimeController {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* createRefinedOre;
    static Statement* updateRefinedOre;
    static Statement* updateRefineries;

    static Statement* createOre;
    static Statement* updateAsteroids;

    static Statement* updateOre;
    static Statement* updateDrones;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        createRefinedOre = connection->createStatement(
            "INSERT INTO RefinedOre                             "
            "     SELECT refining, fid, SYSDATE                 "
            "       FROM Refineries                             "
            "      WHERE refining IS NOT NULL                   "
        );

        updateRefinedOre = connection->createStatement(
            "UPDATE Ore O                                       "
            "   SET tid = (                                     "
            "              SELECT DISTINCT   X.tidOut           "
            "                FROM CanBecome  X                  "
            "                JOIN CanRefine  Y                  "
            "                  ON X.tidOut   = Y.tid            "
            "                JOIN Refineries R                  "
            "                  ON Y.fid      = R.fid            "
            "               WHERE X.tidIn    = O.tid            "
            "                 AND R.refining = O.oid            "
            "              )"
            "WHERE EXISTS ("
            "              SELECT DISTINCT   X.tidOut           "
            "                FROM CanBecome  X                  "
            "                JOIN CanRefine  Y                  "
            "                  ON X.tidOut   = Y.tid            "
            "                JOIN Refineries R                  "
            "                  ON Y.fid      = R.fid            "
            "               WHERE X.tidIn    = O.tid            "
            "                 AND R.refining = O.oid            "
            "              )                                    "

        );

        updateRefineries = connection->createStatement(
            "UPDATE Refineries SET refining = NULL              "
        );

        createOre = connection->createStatement(
            "INSERT INTO Ore                                                        "
            "     SELECT (SELECT TO_CHAR(COUNT(*) + 1, 'fm000000000') FROM Ore),    "
            "            miningFor,                                                 "
            "            fid,                                                       "
            "            fid,                                                       "
            "            SYSDATE                                                    "
            "       FROM Asteroids                                                  "
            "      WHERE miningFor IS NOT NULL                                      "
        );

        updateAsteroids = connection->createStatement(
            "UPDATE Asteroids SET miningFor = NULL              "
        );

        updateOre = connection->createStatement(
            "UPDATE Ore O                                       "
            "   SET O.location = (                              "
            "                     SELECT D.destination          "
            "                       FROM Drones D               "
            "                      WHERE D.haul = O.oid         "
            "                    )                              "
            " WHERE EXISTS (                                    "
            "                SELECT D.destination               "
            "                  FROM Drones D                    "
            "                 WHERE D.haul = O.oid              "
            "              )                                    "
        );

        updateDrones = connection->createStatement(
            "UPDATE Drones                                      "
            "       SET location = destination,                 "
            "          destination = NULL,                      "
            "           haul = NULL                             "
            " WHERE destination IS NOT NULL                     "
        );

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Terminate Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void terminate(Connection* connection) {
        connection->terminateStatement(createRefinedOre);
        connection->terminateStatement(updateRefinedOre);
        connection->terminateStatement(updateRefineries);
        connection->terminateStatement(createOre);
        connection->terminateStatement(updateAsteroids);
        connection->terminateStatement(updateOre);
        connection->terminateStatement(updateDrones);
    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control() {

        createRefinedOre->executeUpdate();
        updateRefinedOre->executeUpdate();
        updateRefineries->executeUpdate();

        createOre->executeUpdate();
        updateAsteroids->executeUpdate();

        updateOre->executeUpdate();
        updateDrones->executeUpdate();

    }

};


#endif // CSCI370FINALPROJECT_TIMECONTROLLER_HPP
