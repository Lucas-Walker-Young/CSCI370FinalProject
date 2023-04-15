// -------------------------------------------------------------------------------------------------
// --------------------------- CSCI370 Final Project :: Skywalker Young ----------------------------
// -------------------------------------------------------------------------------------------------
#include "../include/DestinationSelect.hpp"
#include "../include/StockpileControl.hpp"
#include "../include/StockpileSelect.hpp"
#include "../include/AsteroidControl.hpp"
#include "../include/RefineryControl.hpp"
#include "../include/AsteroidSelect.hpp"
#include "../include/RefinerySelect.hpp"
#include "../include/TimeController.hpp"
#include "../include/DroneControl.hpp"
#include "../include/DroneSelect.hpp"
#include "../include/DroneHaul.hpp"
#include "../include/DroneMove.hpp"


#include "../include/IO.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// -------------------------------- CONTROLLER :: Main Application ---------------------------------
// -------------------------------------------------------------------------------------------------
struct Application {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    const std::string database = "database.csci.viu.ca";
    const std::string username;
    const std::string password;


    // -----------------------------------------------------------------------------------------
    // ------------------------------ CONSTRUCTOR :: Constructor -------------------------------
    // -----------------------------------------------------------------------------------------
    Application(const std::string& username, const std::string& password): username( username ),
									   password( password )

	{}


    // -----------------------------------------------------------------------------------------
    // -------------------------- METHOD :: Start and Run Application --------------------------
    // -----------------------------------------------------------------------------------------
    void run() {

        Environment* environment = Environment::createEnvironment();
        Connection*  connection  = environment->createConnection(username, password, database);

        DestinationSelect::configure(connection);
        StockpileControl::configure(connection);
        StockpileSelect::configure(connection);
        AsteroidControl::configure(connection);
        RefineryControl::configure(connection);
        AsteroidSelect::configure(connection);
        RefinerySelect::configure(connection);
        TimeController::configure(connection);
        DroneControl::configure(connection);
        DroneSelect::configure(connection);
        DroneHaul::configure(connection);
        DroneMove::configure(connection);

        control();

        DroneMove::terminate(connection);
        DroneHaul::terminate(connection);
        DroneSelect::terminate(connection);
        DroneControl::terminate(connection);
        TimeController::terminate(connection);
        RefinerySelect::terminate(connection);
        AsteroidSelect::terminate(connection);
        RefineryControl::terminate(connection);
        AsteroidControl::terminate(connection);
        StockpileSelect::terminate(connection);
        StockpileControl::terminate(connection);
        DestinationSelect::terminate(connection);

        environment->terminateConnection(connection);
        Environment::terminateEnvironment(environment);

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options() {

        std::stringstream output;
        output << std::endl;

        output << std::setw(7)  << std::left << "Command"   << "   ";
        output << std::setw(19) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(7)  << "" << "   ";
        output << std::setw(19) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(7)  << "A"                   << "   ";
        output << std::setw(19) << "control an asteroid" << "   ";

        output << std::endl;
        output << std::setw(7)  << "D"                   << "   ";
        output << std::setw(19) << "control a drone"     << "   ";

        output << std::endl;
        output << std::setw(7)  << "R"                   << "   ";
        output << std::setw(19) << "control a refinery"  << "   ";

        output << std::endl;
        output << std::setw(7)  << "S"                   << "   ";
        output << std::setw(19) << "control a stockpile" << "   ";

        output << std::endl;
        output << std::setw(7)  << "Z"                   << "   ";
        output << std::setw(19) << "advance the program" << "   ";

        output << std::endl;
        output << std::setw(7)  << "O"                   << "   ";
        output << std::setw(19) << "print this menu"     << "   ";

        output << std::endl;
        output << std::setw(7)  << "Q"                   << "   ";
        output << std::setw(19) << "quit program"        << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control() {

        static StockpileSelect stockpile_select;
        static AsteroidSelect  asteroid_select;
        static RefinerySelect  refinery_select;
        static DroneSelect     drone_select;
        static TimeController  time_controller;

        while ( true ) {

            auto command = IO::read("[MAIN] >>> enter command (O for Options): ");

            if ( std::regex_match(command, std::regex("A|a")) ) {
                asteroid_select.control(); continue;
            }

            if ( std::regex_match(command, std::regex("D|d")) ) {
                drone_select.control(); continue;
            }

            if ( std::regex_match(command, std::regex("R|r")) ) {
                refinery_select.control(); continue;
            }

            if ( std::regex_match(command, std::regex("S|s")) ) {
                stockpile_select.control(); continue;
            }

            if ( std::regex_match(command, std::regex("Z|z")) ) {
                time_controller.control(); continue;
            }

            if ( std::regex_match(command, std::regex("O|o")) ) {
                options(); continue;
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


// -------------------------------------------------------------------------------------------------
// ----------------------------------------- MAIN ROUTINE ------------------------------------------
// -------------------------------------------------------------------------------------------------
int main() {

    std::string username;
    std::string password;

    IO io;

    io.read(">>> enter username: ", username);

    if ( !username.empty() ) {

	io.hide();
	io.read(">>> enter password: ", password);
 	io.show();
	io.line();

	if ( !password.empty() ) {

    	    Application application(username, password);
    	    application.run();

	}

    }

}
