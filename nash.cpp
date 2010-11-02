/*
 * =====================================================================================
 *
 *       Filename:  nash.cpp
 *
 *    Description:  Simulation of a cooperative-competitive environemnt of individuals
 *    			using Nash's models
 *
 *        Version:  0.1
 *        Created:  31/10/2010 18:35:14
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
#include	<fstream>
#include	<vector>
#include	"nash.h"

using std::cout;
using std::endl;
using std::vector;
using std::ifstream;

NashEnvironment::NashEnvironment ( int rows, int cols )
{
	this->rows = rows;
	this->cols = cols;

	for ( int i=0; i < rows; i++ )
	{
		vector<nash_strategy> row;

		for ( int j=0; j < cols; j++ )
		{
			row.push_back(coop);
		}

		matrix.push_back(row);
	}
}

NashEnvironment::NashEnvironment ( const char *file )
{
	vector<nash_strategy> row;
	char ch;

	rows = 0;
	cols = 0;

	ifstream in ( file );

	if ( !in )
	{
		throw NashException("Could not open the specified file");
	}

	while ( !in.eof() )
	{
		ch = (char) in.get();

		if ( ch == '\n' )
		{
			if ( cols == 0 )
			{
				cols = row.size();
			} else {
				if ( row.size() != 0 && cols != row.size() )
				{
					throw NashException("The size of the rows does not match");
				}
			}

			if ( row.size() != 0 )
			{
				rows++;
				matrix.push_back( row );
				row.clear();
			}
		} else if ( ch == '.' ) {
			row.push_back( coop );
		} else if ( ch == 'X' ) {
			row.push_back( comp );
		}
	}

	if ( !row.empty() )
	{
		matrix.push_back( row );
		row.clear();
	}

	if ( rows == 0 )
	{
		throw NashException("Invalid or empty strategy file");
	}
}

const int
NashEnvironment::getRows()
{
	return rows;
}

const int
NashEnvironment::getCols()
{
	return cols;
}

void
NashEnvironment::init_costs ( double coop_coop, double coop_comp, double comp_coop, double comp_comp )
{
	costs[coop][coop] = coop_coop;
	costs[comp][comp] = comp_comp;
	costs[comp][coop] = comp_coop;
	costs[coop][comp] = coop_comp;
}		/* -----  end of function nash_init_costs  ----- */

void
NashEnvironment::print ()
{
	for ( int i=0; i < rows; i++ )
	{
		for ( int j=0; j < cols; j++ )
		{
			cout << (( matrix[i][j] == coop ) ? "." : "X" ) << " ";
		}

		cout << endl;
	}
}		/* -----  end of function nash_print_matrix  ----- */

void
NashEnvironment::update_utilities ( vector< vector< double > >& utilities )
{
	for ( int x=0; x < rows; x++ )
	{
		for ( int y=0; y < cols; y++ )
		{
			utilities[x][y] = 0.0;

			for ( int i = (( x == 0 ) ? x : x-1 ); i <= (( x == rows-1 ) ? x : x+1 ); i++ )
			{
				for ( int j = (( y == 0 ) ? y : y-1 ); j <= (( y == cols-1 ) ? y : y+1 ); j++ )
				{
					if ( !( x == i && y == j ))
					{
						utilities[x][y] += costs [matrix[x][y]] [matrix[i][j]];
					}
				}
			}
		}
	}
}

bool
NashEnvironment::refresh_strategies ()
{
	double best_utility = 0.0;
	int best_x = 0, best_y = 0;
	vector< vector<int> > changes;
	vector< vector<double> > utilities;

	for ( int i=0; i < rows; i++ )
	{
		vector<double> row(cols);
		utilities.push_back(row);
	}

	update_utilities ( utilities );

	for ( int x=0; x < rows; x++ )
	{
		for ( int y=0; y < cols; y++ )
		{
			best_x = x;
			best_y = y;
			best_utility = utilities[best_x][best_y];

			for ( int i = (( x == 0 ) ? x : x-1 ); i <= (( x == rows-1 ) ? x : x+1 ); i++ )
			{
				for ( int j = (( y == 0 ) ? y : y-1 ); j <= (( y == cols-1 ) ? y : y+1 ); j++ )
				{
					if ( !( x == i && y == j ))
					{
						if ( utilities[i][j] > best_utility )
						{
							best_x = i;
							best_y = j;
							best_utility = utilities[best_x][best_y];
						}
					}
				}
			}

			if ( matrix[best_x][best_y] != matrix[x][y] )
			{
				vector<int> pair(2);
				pair[0] = x;
				pair[1] = y;
				changes.push_back (pair);
			}
		}
	}

	for ( int i=0; i < changes.size(); i++ )
	{
		matrix[changes[i][0]][changes[i][1]] = ( matrix[changes[i][0]][changes[i][1]] == coop ) ? comp : coop;
	}

	if ( changes.size() > 0 )
	{
		if ( check_loop() )
		{
			throw NashException ("Loop detected - No Nash's equilibrium is possible for this configuration");
		}
	}

	visited_combinations.push_back(matrix);
	return ( changes.size() > 0 );
}		/* -----  end of function nash_refresh_strategies  ----- */


bool
NashEnvironment::check_loop ()
{
	bool found = false;

	for ( int n=0; n < visited_combinations.size(); n++ )
	{
		found = true;

		for ( int i=0; i < rows && found; i++ )
		{
			for ( int j=0; j < cols && found; j++ )
			{
				if ( matrix[i][j] != visited_combinations[n][i][j] )
				{
					found = false;
				}
			}
		}

		if ( found )
		{
			return true;
		}
	}

	return false;
}		/* -----  end of function check_loop  ----- */

