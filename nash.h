/*
 * =====================================================================================
 *
 *       Filename:  nash.h
 *
 *    Description:  Header file for the simulation of Nash's model
 *
 *        Version:  0.1
 *        Created:  01/11/2010 13:20:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Licence:  GNU GPL v.3
 *        Company:  DO WHAT YOU WANT CAUSE A PIRATE IS FREE, YOU ARE A PIRATE!
 *
 * =====================================================================================
 */

#include	<vector>
#include	<exception>
typedef enum  { coop, comp } nash_strategy;

class NashException : public std::exception
{
	const char *message;

public:
	NashException ( const char *m )
	{
		message = m;
	}

	virtual const char* what()
	{
		return message;
	}
};

class NashEnvironment
{
	int rows;
	int cols;
	double costs[2][2];
	std::vector< std::vector< nash_strategy > > matrix;
	std::vector< std::vector< std::vector< nash_strategy > > > visited_combinations;

	/**
	 * \brief  Verify if a certain scenario has already been visited, for avoiding loops in environments without Nash's balance
	 * \return True if the current scenario was already analyzed, false otherwise
	 */
	bool check_loop ();

	/**
	 * \brief  Update the utility costs on the matrix
	 * \param  utilities 	Reference to the utilities matrix to be updated
	 */
	void update_utilities ( std::vector< std::vector<double> >& utilities );
	
public:

	/**
	 * \brief   Constructor for the class
	 * \param   rows 	Rows of the Nash environment's matrix
	 * \param   cols 	Cols of the Nash environment's matrix
	 */
	NashEnvironment ( int rows, int cols );

	/**
	 * \brief   Constructor for the class
	 */
	NashEnvironment ( const char *file );

	/**
	 * \brief   Number of rows in the matrix
	 */
	const int getRows();

	/**
	 * \brief   Number of cols in the matrix
	 */
	const int getCols();

	/**
	 * \brief  Initialize the costs of the matrix of strategy costs
	 */
	void init_costs ( double coop_coop, double coop_comp, double comp_coop, double comp_comp );

	/**
	 * \brief  Print the current status of the matrix of strategies
	 */
	void print ();

	/**
	 * \brief  Refresh the matrix of the utility values
	 * \return True if there were strategy changes, false otherwise
	 */
	bool refresh_strategies ();
};

