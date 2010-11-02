/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main file for the simulation of a Nash's environment
 *
 *        Version:  0.1
 *        Created:  01/11/2010 13:23:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Licence:  GNU GPL v.3
 *        Company:  DO WHAT YOU WANT CAUSE A PIRATE IS FREE, YOU ARE A PIRATE!
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<cstdlib>
#include	"nash.h"

using std::cout;
using std::cerr;
using std::endl;

/**
 * \brief Main function for the program
 */

int
main ( int argc, char *argv[] )
{
	NashEnvironment *nash = NULL;

	if ( argc < 6 )
	{
		cerr << "Usage: " << argv[0] << " <environment_file> <coop_coop_gain> <coop_comp_gain> <comp_coop_gain> <comp_comp_gain>" << endl;
		return 1;
	}

	nash = new NashEnvironment ( argv[1] );

	nash->init_costs (
		strtod ( argv[2], NULL ),
		strtod ( argv[3], NULL ),
		strtod ( argv[4], NULL ),
		strtod ( argv[5], NULL )
	);

	nash->print();

	try
	{
		while ( nash->refresh_strategies() )
		{
			cout << endl;

			for ( int i=0; i < nash->getCols(); i++ )
				cout << "==";

			cout << endl;
			nash->print();
		}
	}

	catch ( NashException e )
	{
		cerr << endl << e.what() << endl;
		delete nash;
		return 1;
	}

	cout << endl << "Nash's equilibrium configuration found" << endl;
	delete nash;
	return 0;
}				/* ----------  end of function main  ---------- */

