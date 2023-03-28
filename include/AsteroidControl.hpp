// -------------------------------------------------------------------------------------------------
// --------------------------------- HEADER :: Asteroid Controller ---------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_ASTEROIDCONTROL_HPP
#define CSCI370FINALPROJECT_ASTEROIDCONTROL_HPP

#include "Asteroid.hpp"
#include "IO.hpp"
#include "OreType.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// --------------------------------- STRUCT :: Asteroid Controller ---------------------------------
// -------------------------------------------------------------------------------------------------
struct AsteroidControl {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* selector;
    static Statement* fallback;
    static Statement* getter;
    static Statement* setter;
    static Statement* deller;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        fallback = connection->createStatement(
            "SELECT * FROM OreTypes WHERE tid = :1                  "
        );

        selector = connection->createStatement(
            "SELECT T.tid, T.name, T.value, CAST (NULL AS CHAR(9))  "
            "  FROM CanCreate X                                     "
            "  JOIN OreTypes  T                                     "
            "    ON X.tid     =  T.tid                              "
            " WHERE X.fid     = :1                                  "
            "   AND X.tid     = :2                                  "
        );

        getter = connection->createStatement(
            "SELECT T.tid, T.name, T.value, CAST (NULL AS CHAR(9))  "
            "  FROM CanCreate X                                     "
            "  JOIN OreTypes  T                                     "
            "    ON X.tid     =  T.tid                              "
            " WHERE X.fid     = :1                                  "
        );

        setter = connection->createStatement(
            "UPDATE Asteroids                                       "
            "   SET miningFor = :1                                  "
            " WHERE fid       = :2                                  "
        );

        deller = connection->createStatement(
            "UPDATE Asteroids                                       "
            "   SET miningFor = NULL                                "
            " WHERE fid       = :1                                  "
        );

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Terminate Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void terminate(Connection* connection) {
        connection->terminateStatement(selector);
        connection->terminateStatement(fallback);
        connection->terminateStatement(getter);
        connection->terminateStatement(setter);
        connection->terminateStatement(deller);
    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: Abort Current Operation ---------------------------
    // -----------------------------------------------------------------------------------------
    void abort(Asteroid* asteroid) {

        if ( !asteroid->miningFor.empty() ) {

            deller->setString(1, asteroid->fid);
            deller->executeUpdate();

            asteroid->miningFor = "";

            IO::print("asteroid operation aborted");

        } else { IO::print("cannot abort nonexistent asteroid operation"); }

    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: Print Current Asteroid ----------------------------
    // -----------------------------------------------------------------------------------------
    void print(Asteroid* asteroid) {
        IO::print(asteroid->stringify());
    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ METHOD :: View all Refinable Ore-Types -------------------------
    // -----------------------------------------------------------------------------------------
    void view(Asteroid* asteroid) {

        getter->setString(1, asteroid->fid);
        ResultSet* oretypes = getter->executeQuery();

        if ( oretypes->next() ) {

            std::stringstream output;
            OreType::stringify_head(output);

            do { OreType(oretypes).stringify_body(output); }
            while ( oretypes->next() );

            IO::print(output.str());

        }

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ METHOD :: (Attempt to) Assign Operation ------------------------
    // -----------------------------------------------------------------------------------------
    void assign(Asteroid* asteroid, const std::string& command) {

        if ( asteroid->miningFor != command ) {

            if ( !asteroid->miningFor.empty() ) {
                IO::print("previous asteroid operation aborted");
            }

            setter->setString(1, command);
            setter->setString(2, asteroid->fid);
            setter->executeUpdate();

            asteroid->miningFor = command;

            IO::print("asteroid operation assigned");

        }

        else { IO::print("operation is already underway"); }

    }


    // -----------------------------------------------------------------------------------------
    // -------------- METHOD :: (Attempt to) Select Ore-Type and Assign Operation --------------
    // -----------------------------------------------------------------------------------------
    void select(Asteroid* asteroid, const std::string& command) {

        selector->setString(1, asteroid->fid);
        selector->setString(2, command);
        fallback->setString(1, command);

        ResultSet* oretype = selector->executeQuery();

        if ( oretype->next() ) {
            assign(asteroid, command);
            return;
        }

        oretype = fallback->executeQuery();

        if ( !oretype->next() ) { IO::print("unrecognized ore-type"); }
        else { IO::print("the selected ore-type cannot be mined at this facility"); }

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options(Asteroid* asteroid) {

        std::stringstream output;
        output << std::endl;

        output << std::setw(9)  << std::left << "Command"   << "   ";
        output << std::setw(26) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(26) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"                   << "   ";
        output << std::setw(26) << "select an ore-type to mine"  << "   ";

        if ( !asteroid->miningFor.empty() ) {
            output << std::endl;
            output << std::setw(9)  << "A"                       << "   ";
            output << std::setw(26) << "abort current operation" << "   ";
        }

        output << std::endl;
        output << std::setw(9)  << "P"                           << "   ";
        output << std::setw(26) << "print current asteroid"      << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                           << "   ";
        output << std::setw(26) << "view all ore-types"          << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                           << "   ";
        output << std::setw(26) << "print this menu"             << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                           << "   ";
        output << std::setw(26) << "quit to asteroid select"     << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control(Asteroid* asteroid) {

        while ( true ) {

            auto command = IO::read("[ASTEROID CONTROL] >>> enter type-id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {
                select(asteroid, command); continue;
            }

            if ( std::regex_match(command, std::regex("A|a")) ) {
                abort(asteroid); continue;
            }

            if ( std::regex_match(command, std::regex("P|p")) ) {
                print(asteroid); continue;
            }

            if ( std::regex_match(command, std::regex("V|v")) ) {
                view(asteroid); continue;
            }

            if ( std::regex_match(command, std::regex("O|o")) ) {
                options(asteroid); continue;
            }

            if ( std::regex_match(command, std::regex("Q|q")) ) {
                return;
            }

            if (command.empty() ) {
                continue;
            }

            IO::print("unrecognized command");

        }

    }

};


#endif // CSCI370FINALPROJECT_ASTEROIDCONTROL_HPP
